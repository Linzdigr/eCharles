#!/bin/bash

#sudo g++ -c main.cpp BMP085.cpp mcp3008/mcp3008Spi.cpp DHT22/dht22.c -std=c++0x
#sudo g++ -o /etc/geigerDaemon main.o BMP085.o mcp3008Spi.o dht22.o -std=c++0x -lm -lwiringPi

sudo g++ -o /etc/geigerDaemon main.cpp BMP085.cpp mcp3008/mcp3008Spi.cpp DHT22/dht22.cpp -std=c++0x -lm -lwiringPi

