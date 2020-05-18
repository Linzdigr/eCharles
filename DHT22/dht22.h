#include <wiringPi.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>

#define MAXTIMINGS 85

	using namespace std;

class dht22{
	private:
		int dht22_dat[5] = {};
		static uint8_t sizecvt(const int);
		float hygrometry = 0;
		float temperature = 0;
		int dht22_pin = 0;
	public:
		dht22(int);
		int refresh();
		float getTemperature();
		float getHygrometry();
		~dht22();
};
