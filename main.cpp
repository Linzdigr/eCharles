/**
 * This is the main file containing all calls to measure and handle class / methods.
 * @author	Yvan FEREZ (Radiation-Survey-Lab.fr)
 * @license	GPL 3.0
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
	#define INTR_PIN 				7
#endif
#define STATUS_LED_PIN 				0
#define INIT_FAILURE				-1
#define REFRESH_SENSOR_DELAY_SEC	5
#define CNF_FILE 					"/etc/otal.cnf"

	using namespace std;

#include "BMP085.hpp"

/* BEGIN OF MEASURE VARIABLES */
unsigned int count, currentPressureSeaLevel;
double currentPressure, particuleLevel, co2Level;
float currentTemperature, uvLevel;
ofstream errorLogFile;
ofstream logFile;
/* END OF MEASURE VARIABLES */


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
	count++;
}

const std::string today_str(){
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];
	tstruct = *localtime(&now);

	strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);

	return buf;
}

const std::string currentDateTime(){
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];
	tstruct = *localtime(&now);

	strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);

	return buf;
}

const std::string getHeaders(){
	ifstream cnfFile;
	string line;

	cnfFile.open(CNF_FILE, fstream::in);

	if(!errorLogFile.is_open()){
                cout << "Error: config file can't be opened!" << endl;
                return NULL;
	}

	while(getline(cnfFile, line)){
		auto v = explode(line, '=');
		if(v[0] == "CSVHEADER")
			return v[1];
	}

	return NULL;
}

bool record(ofstream &logFile, ofstream &errorLogFile){
	string logFilePath = "/var/www/data_" + today_str() + ".log";

	logFile.open(logFilePath.c_str(), fstream::app);
	if(!logFile.is_open()){
		errorLogFile << '[' << currentDateTime() << "] Error: Log file can't be opened: check permissions or if file really exists\n";
		return false;
	}
	if(logFile.tellp() == 0)
		logFile << getHeaders() << '\n';
	logFile << count << ';' << currentPressure << ';' << currentTemperature << ';' << currentDateTime() << '\n';

	logFile.close();
	
	return true;
}

int main(void){
	const string errorLogFilePath = "/var/log/geigerError.log";
	time_t timer = time(0);
	errorLogFile.open(errorLogFilePath.c_str());
	
	wiringPiSetup();

	pinMode(INTR_PIN, INPUT);
	pinMode(STATUS_LED_PIN, OUTPUT);

	if(!errorLogFile.is_open()){
		cerr << "Error: Error log file can't be opened!" << endl;
		return INIT_FAILURE;
	}
	
	BMP085 *bcp;
	try{
		bcm = new BMP085(BMP085::OSS_ULTRAHIGH);

		if(!bcm->ok) {
                	cerr << bcm->err << endl;
                	return 1;
        	}
		bcm->hiRes = true;
	}
	catch(BMP085::smbusIOException e){
		cerr << "No module BMP085 detected" << endl;
	}
	// For rev. 1 Model B pis:
	// BMP085 *bcm = new BMP085(oss, "/dev/i2c-0");

	count = 0;

	wiringPiISR(INTR_PIN, INT_EDGE_FALLING, &falling_state);	// For geiger counter
	while(1){
		unsigned int delta_time = difftime(time(0), timer);

		if(delta_time % REFRESH_SENSOR_DELAY_SEC == 0){
			/* Pressure + Temperature Sensor */
			
			BMP085::reading data = bcm->getBoth();
			currentPressure += (data.kPa)*1000;	// Converting into Pascal
			currentTemperature += data.celcius;
			currentPressure /= 2;
			currentTemperature /= 2;

			/* UV Sensor */
			
			/* Particle/Dust Sensor */
			
			/* CO2 Sensor */
			
			/* Temperature (2) + Hygrometry Sensor */
			
		}

		if(delta_time >= 60){	// Check if the current minute is done
			/* Logging computed datas */
			timer = time(0);	// Reset timer
			record(logFile, errorLogFile);
			count = 0;
		}
		/* BLINK! */
		if(delta_time % 2 == 0)
			digitalWrite(STATUS_LED_PIN, 0);
		else if (delta_time % 2 == 1)
			digitalWrite(STATUS_LED_PIN, 1);
		usleep(1770);
	}
	return true;
}
