/**
 * This is the main file containing calls to get all measures from the station.
 * They are then logged into a daily file.
 * @author	Yvan FEREZ (Radiation-Survey-Lab.fr)
 * @license	GPL
 * @version 0.5.0b
 * @lastUpdateOn 2016-05-11
*/
#include <wiringPi.h>
#include <iostream>
#include <stdio.h>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <chrono>
#include <time.h>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <utility>


#ifndef INTR_PIN
  #define INTR_PIN                  1
#endif
#define STATUS_LED_PIN              0
#define ENABLE_UV_MODULE_PIN        22
#define DHT22PIN                    2
#define WATERING_PIN                12
#define INIT_FAILURE                -1
#define REFRESH_SENSOR_DELAY_SEC    5
#define CNF_FILE                    "/etc/otal.cnf"

#define SOIL_MOISTURE_CHANNEL       3
#define UV_ANALOG_CHANNEL           2
#define DUST_ANALOG_CHANNEL         1

// Extrem known values: 870 - 370
#define SOIL_DRY_LIMIT              750
#define SOIL_WET_LIMIT              600

  using namespace std;

#include "BMP085/BMP085.h"
#include "MCP3008/mcp3008Spi.h"
#include "DHT22/dht22.h"

/* BEGIN OF SENSOR VARIABLES */
volatile unsigned int gcount = 0, currentPressureSeaLevel = 0;
volatile double currentPressure = 0, particuleLevel = 0, co2Level = 0;
volatile float currentTemperature = 0, currentTemperature22 = 0, currentHygrometry = 0, uvLevel = 0, currentSoilMoisture = 0;
bool firstSensingCycle = true;
ofstream errorLogFile;
ofstream logFile;
dht22 dht(DHT22PIN);
BMP085 *bcm;
/* END OF MEASURE VARIABLES */

const std::string nowStr(bool with_time = false) {
  time_t now = time(0);
  struct tm tstruct;
  char buf[80];
  tstruct = *localtime(&now);

  if(with_time) {
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
  } else {
    strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct)
  }

  return buf;
}

bool log(string lvl, string message, bool nl = true) {
  const string errorLogFilePath = "/var/log/otal.log";

  errorLogFile.open(errorLogFilePath.c_str(), fstream::app);

  if(!errorLogFile.is_open()){
    cerr << "Error: Error log file can't be opened!" << endl;
    return INIT_FAILURE;
  }
  errorLogFile << '[' << nowStr(true) << " GMT] " << lvl << ": " << message;
  if(nl)
    errorLogFile << endl;

  errorLogFile.close();

  return true;
}

int getAnalogChannelVal(int channel) {
  mcp3008Spi a2d("/dev/spidev0.0", SPI_MODE_0, 1000000, 8);
  int a2dVal = 0;
  int a2dChannel = channel;
  unsigned char data[3];
  data[0] = 1;  //  first byte transmitted -> start bit
  data[1] = 0b10000000 |( ((a2dChannel & 7) << 4)); // second byte transmitted -> (SGL/DIF = 1, D2=D1=D0=0)
  data[2] = 0; // third byte transmitted....don't care

  a2d.spiWriteRead(data, sizeof(data) );
  
  a2dVal = 0;
  a2dVal = (data[1]<< 8) & 0b1100000000; //merge data[1] & data[2] to get result
  a2dVal |=  (data[2] & 0xff);
    
  return a2dVal;
}

//bmp180 pressureSensor; // For .cpp bmp180 version only

std::vector<std::string> explode(std::string const & s, char delim){
  std::vector<std::string> result;
  std::istringstream iss(s);

  for (std::string token; std::getline(iss, token, delim); ){
      result.push_back(std::move(token));
  }
  return result;
}

void falling_state(){
  gcount++;
}

const std::string getHeaders() {
  log("INFO", "Getting headers in config file... ", false);
  ifstream cnfFile;
  string line;

  cnfFile.open(CNF_FILE, fstream::in);

  while(getline(cnfFile, line)){
    auto v = explode(line, '=');
    if(v[0] == "CSVHEADER")
      return v[1];
  }
  log("INFO", "[Done]");

  return NULL;
}

PI_THREAD(activeLed) {
  /* BLINK STATUS ! */
  while(1){
    usleep(1000000);
    digitalWrite(STATUS_LED_PIN, 0);
    usleep(500000);
    digitalWrite(STATUS_LED_PIN, 1);
  }
}

PI_THREAD(wateringProcess) {
  /* Watering */
  if(currentSoilMoisture > SOIL_DRY_LIMIT) {
    digitalWrite(WATERING_PIN, 0);
  } else if(currentSoilMoisture < SOIL_WET_LIMIT) {
    digitalWrite(WATERING_PIN, 1);
  }
}

bool record(ofstream &logFile, ofstream &errorLogFile){
  string logFilePath = "/var/www/data_" + nowStr() + ".log";

  logFile.open(logFilePath.c_str(), fstream::app);
  if(!logFile.is_open()){
    log("ERROR", "Log file can't be opened: check permissions or if file really exists");
    return false;
  }
  if(logFile.tellp() == 0)
    logFile << getHeaders() << '\n';
  logFile << gcount << ';' << currentPressure << ';' << currentTemperature << ';' << currentHygrometry << ';' <<  uvLevel << ';' << currentSoilMoisture << ';' << nowStr(true) << '\n';

  logFile.close();
  
  return true;
}

void refreshSensorValues() {

  BMP085::reading data;

  if(bcm != NULL)
    data = bcm->getBoth();
  currentPressure += (data.kPa)*1000;	// Converting into Pascal
  currentTemperature += data.celcius;
  
  digitalWrite(ENABLE_UV_MODULE_PIN, HIGH); // Set the uv module to active mode.
  usleep(5);	// Module WakeUp time.
  uvLevel += getAnalogChannelVal(UV_ANALOG_CHANNEL);
  currentSoilMoisture += getAnalogChannelVal(SOIL_MOISTURE_CHANNEL);
  digitalWrite(ENABLE_UV_MODULE_PIN, LOW);	// Sleep mode

  cout << "Soil moisture: " << currentSoilMoisture << endl;

  dht.refresh();
  currentHygrometry += dht.getHygrometry();

  if(!firstSensingCycle) {
    currentPressure /= 2;
    currentTemperature /= 2;
    uvLevel /= 2;
    currentHygrometry /= 2;
    currentSoilMoisture /= 2;
  }

  firstSensingCycle = false;

  return;
}

int init() {
  int wPiReturn = 0;
  
  log("INFO", "Starting init process Daemon...");

  if((wPiReturn = wiringPiSetup()) == INIT_FAILURE){
    log("ERROR", "wiringPiSetup() have failed! Aborting.");
    return INIT_FAILURE;
  }

  log("INFO", "GPIO utilities loaded.");

  pinMode(INTR_PIN, INPUT);
  pullUpDnControl(INTR_PIN, PUD_UP);
  pinMode(STATUS_LED_PIN, OUTPUT);
  pinMode(ENABLE_UV_MODULE_PIN, OUTPUT);
  pinMode(WATERING_PIN, OUTPUT);

  digitalWrite(WATERING_PIN, 1); // HIGH to inactive: normaly closed.
  
  log("INFO", "PIN states set.");

  try{
    bcm = new BMP085(BMP085::OSS_ULTRAHIGH);
    if(!bcm->ok) {
      cerr << bcm->err << endl;
      log("ERROR", bcm->err);
      bcm = NULL;	
    }
  }
  catch(const std::exception &e){
    cerr << "No BMP085 module detected! Will not be used." << endl;
    log("ERROR", "No BMP085 module detected! Will not be used.");
    bcm = NULL;
  }

  // For rev. 1 Model B pis:
  // BMP085 *bcm = new BMP085(oss, "/dev/i2c-0");
  if(bcm != NULL) {
    bcm->hiRes = true;
  }

  log("INFO", "External modules set.");

  refreshSensorValues();

  wiringPiISR(INTR_PIN, INT_EDGE_FALLING, &falling_state);	// For geiger gcounter

  if((piThreadCreate(activeLed)) != 0) {		// Starting led thread
    log("WARNING", "LED thread not started.");
  }

  if((piThreadCreate(wateringProcess)) != 0) {		// Starting watering thread
    log("WARNING", "Watering thread not started.");
  }
}

int main(void){
  time_t timer = time(0);
  init();

  log("INFO", "Starting routine sensor.");

  while(1){
    unsigned int delta_time = difftime(time(0), timer);
    if(delta_time % REFRESH_SENSOR_DELAY_SEC == 0){ // Accumulate datas every REFRESH_SENSOR_DELAY_SEC !
      refreshSensorValues();
    }

    if(delta_time >= 60){	// Check if the current minute is over

      /* Logging computed datas */
      record(logFile, errorLogFile);

      /* Reset vars */
      currentPressure = currentTemperature = currentTemperature22 = currentSoilMoisture = uvLevel = 0;
      firstSensingCycle = true;
      gcount = 0;
      timer = time(0);	// Reset timer
    }
    usleep(150);	// Don't take the load of the CPU.
  }

  return true;
}