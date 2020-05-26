#include <iostream>
#include <stdio.h>
#include "dht22.h"

#define DHT22PIN    2

dht22 dht(DHT22PIN);

int main() {
  if(wiringPiSetup() == -1){
    cout << "ERROR: wiringPiSetup() have failed! Aborting." << endl;
    return -1;
  }

  dht.refresh();

  cout << dht.getTemperature() << endl;
  cout << dht.getHygrometry() << endl;

  return true;
}
