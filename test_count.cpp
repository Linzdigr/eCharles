/**
 * This is the main file containing all calls to measure.
 * @author	Yvan FEREZ (Radiation-Survey-Lab.fr)
 * @license	GPL
 * @version 0.25.5b
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
	#define INTR_PIN 				1
#endif
#define STATUS_LED_PIN 				0
#define ENABLE_UV_MODULE_PIN		22
#define DHT22PIN					2
#define INIT_FAILURE				-1
#define REFRESH_SENSOR_DELAY_SEC	5
#define CNF_FILE 					"/etc/otal.cnf"

#define UV_ANALOG_CHANNEL           2
#define DUST_ANALOG_CHANNEL         1
#define CO2_ANALOG_CHANNEL          0

#define MAXTIMINGS 85

	using namespace std;

/* BEGIN OF MEASURE VARIABLES */
unsigned int count = 0;
/* END OF MEASURE VARIABLES */

/*() << " GMT] " << lvl << ": " << message;
	if(nl)
		errorLogFile << endl;

	errorLogFile.close();

	return true;
}

int getAnalogChannelVal(int channel){
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
*/

void falling_state(){
	cout << count++ << endl;
}

/*

const std::string getHeaders(){
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

PI_THREAD(activeLed){
	/* BLINK! 
	while(1){
		usleep(1770);
		digitalWrite(STATUS_LED_PIN, 0);
		usleep(1770);
		digitalWrite(STATUS_LED_PIN, 1);
	}
}

bool record(ofstream &logFile, ofstream &errorLogFile){
	string logFilePath = "/var/www/data_" + today_str() + ".log";

	logFile.open(logFilePath.c_str(), fstream::app);
	if(!logFile.is_open()){
		log("ERROR", "Log file can't be opened: check permissions or if file really exists");
		return false;
	}
	if(logFile.tellp() == 0)
		logFile << getHeaders() << '\n';
	logFile << count << ';' << currentPressure << ';' << currentTemperature << ';' << currentHygrometry << ';' <<  uvLevel << ';' << currentDateTime() << '\n';

	logFile.close();
	
	return true;
}
*/
int main(void){
	int wPiReturn = 0;
	//time_t timer = time(0);
	
	//log("INFO", "Starting init process Daemon...");

	if((wPiReturn = wiringPiSetup()) == INIT_FAILURE){
//		log("ERROR", "wiringPiSetup() have failed! Aborting.");
		cout << 'no' << endl;
		return INIT_FAILURE;
	}
/*
	log("INFO", "GPIO utilities loaded.");
	*/
	pinMode(INTR_PIN, INPUT);
	pullUpDnControl(INTR_PIN, PUD_UP);
	/* pinMode(STATUS_LED_PIN, OUTPUT);
	pinMode(ENABLE_UV_MODULE_PIN, OUTPUT);
	
	log("INFO", "PIN states set.");
	
	BMP085 *bcm = new BMP085(BMP085::OSS_ULTRAHIGH);
	// For rev. 1 Model B pis:
	// BMP085 *bcm = new BMP085(oss, "/dev/i2c-0");
	if(!bcm->ok) {
		cerr << bcm->err << endl;
		return 1;
	}
	bcm->hiRes = true;
	log("INFO", "External modules set.");
*/
	wiringPiISR(INTR_PIN, INT_EDGE_FALLING, &falling_state);	// For geiger counter
/*
	if((piThreadCreate(activeLed)) != 0)		// Starting led thread
		log("WARNING", "LED thread not started.");

	log("INFO", "Starting routine sensor.");
	int takes = 0;
*/
	while(1){
		;
	}

	return true;
}
