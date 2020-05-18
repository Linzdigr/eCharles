using namespace std;

#include "dht22.h"

dht22::dht22(int pin)
:temperature(0), hygrometry(0),dht22_pin(pin), dht22_dat{0,0,0,0,0} {

}

uint8_t dht22::sizecvt(const int read){
  /* digitalRead() and friends from wiringpi are defined as returning a value
  < 256. However, they are returned as int() types. This is a safety function */

	if(read > 255 || read < 0){
		printf("Invalid data from wiringPi library\n");
		exit(EXIT_FAILURE);
	}

	return (uint8_t)read;
}

int dht22::refresh(){
	for(int i=0; i<=150; i++){
		uint8_t laststate = HIGH;
		uint8_t counter = 0;
		uint8_t j = 0;

		dht22_dat[0] = dht22_dat[1] = dht22_dat[2] = dht22_dat[3] = dht22_dat[4] = 0;
	
		// pull pin down for 18 milliseconds
		pinMode(this->dht22_pin, OUTPUT);
		digitalWrite(this->dht22_pin, HIGH);
		delay(10);
	  digitalWrite(this->dht22_pin, LOW);
	  delay(18);
	  // then pull it up for 40 microseconds
	  digitalWrite(this->dht22_pin, HIGH);
	  delayMicroseconds(40); 
	  // prepare to read the pin
	  pinMode(this->dht22_pin, INPUT);
	
	  // detect change and read data
	  for (uint8_t i=0; i< MAXTIMINGS; i++) {
	    counter = 0;
	    while (sizecvt(digitalRead(this->dht22_pin)) == laststate) {
	      counter++;
	      delayMicroseconds(1);
	      if (counter == 255) {
	        break;
	      }
	    }
	    laststate = sizecvt(digitalRead(this->dht22_pin));
	
	    if (counter == 255) break;
	
	    // ignore first 3 transitions
	    if ((i >= 4) && (i%2 == 0)) {
	      // shove each bit into the storage bytes
	      dht22_dat[j/8] <<= 1;
	      if (counter > 16)
	        dht22_dat[j/8] |= 1;
	      j++;
	    }
	  }
	
	  // check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
	  // print it out if data is good
	  if ((j >= 40) && 
	      (dht22_dat[4] == ((dht22_dat[0] + dht22_dat[1] + dht22_dat[2] + dht22_dat[3]) & 0xFF)) ) {
	        float t, h;
	        h = (float)dht22_dat[0] * 256 + (float)dht22_dat[1];
	        h /= 10;
	        t = (float)(dht22_dat[2] & 0x7F)* 256 + (float)dht22_dat[3];
	        t /= 10.0;
	        if ((dht22_dat[2] & 0x80) != 0)  t *= -1;
	
	
		    this->hygrometry = h;
		    this->temperature = t;
			
		pinMode(this->dht22_pin, OUTPUT);
		digitalWrite(this->dht22_pin, HIGH);
	    return 1;
	  }
	}
	pinMode(this->dht22_pin, OUTPUT);
	digitalWrite(this->dht22_pin, HIGH);
	return -1;
}

// int main(){
	// float h = 0;
	// float t = 0;
	
	// wiringPiSetup();
		// if(read_dht22_dat(2, t, h) == -1)
			// printf("FAILED");

		// printf("%.2f | %.2f", t, h);
	
	// return 0;
// }

float dht22::getTemperature(){
	return this->temperature;
}

float dht22::getHygrometry(){
	return this->hygrometry;
}

dht22::~dht22(){

}
