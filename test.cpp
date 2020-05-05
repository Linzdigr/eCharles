#include "bmp180dev3.h"

	void main(){
	   int fd;
	   double temperature, pressure;
	   double PAlt;
	   fd=i2c_Open(I2CBus);
		printf ("\nCalibration:%i (0= worked)\n",bmp_Calibration(fd));
		printf ("Return:%i\tTemperature\t:%.1f C\n",bmp_GetTemperature(fd,&temperature),temperature);
		printf ("Return:%i\tPressure\t:%.2f hPa/mB\tThis is known as station pressure\n",bmp_GetPressure(fd,&pressure),pressure);
		PAlt=bmp_altitude(pressure);
		printf ("\t\tP.Altitude\t:%.0f ft\tThis is known as a Pressure Altitude\n",PAlt);
		printf ("\t\tQNH\t\t:%.2f hPa/mB\tSetting required on an Aircraft\n",bmp_qnh(pressure,5085)); //5085 
ft is my station elevation
		printf ("\t\t\t\t\t\taltimeter for it to read Elevation while on the ground\n"); //use your's
		printf ("\t\t\t\t\t\tand Altitude while in the air\n");
		printf ("\t\t\t\t\t\tSensor Elevation MUST BE KNOWN ACCURATELY\n");
		printf ("\t\tD.Altitude\t:%.0f ft\tThis is known as a density Altitude\n\n",ppl_DensityAlt(PAlt,temperature));
	   close (fd);
		return 0;
	}
