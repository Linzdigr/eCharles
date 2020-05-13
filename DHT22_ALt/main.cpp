#include <iostream>

#include <wiringPi.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>

	using namespace std;
	
#include "dht.h"

dht DHT;
	
int main(){
	wiringPiSetup();
	int chk = DHT.read22(2);
	
	cout << chk << endl;
	cout << DHT.humidity << endl;
	cout << DHT.temperature << endl;
	
	return 0;
}

