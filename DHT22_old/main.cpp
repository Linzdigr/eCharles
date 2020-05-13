#include <stdio.h>
#include <iostream>

	using namespace std;

#include "pi_dht_read.h"

int main(){
	float temperature = 0, humidity = 0;
	pi_dht_read(DHT22, 2, &humidity, &temperature);
	
	cout << humidity << endl << temperature;
	
	return 0;
}