g++ -c -Wall main.cpp BMP085.cpp
g++ -o /etc/geigerDaemon main.o BMP085.o -std=c++0x -lm -lwiringPi
