PREFIX = /usr/local

main.o: main.cpp
	g++ -c -Wall main.cpp

bmp180dev3.o:
	g++ -c -Wall BMP180/bmp180dev3.cpp

dht22.o: DHT22/dht22.cpp
	g++ -c -Wall DHT22/dht22.cpp

mcp3008Spi.o: MCP3008/mcp3008Spi.cpp
	g++ -c -Wall MCP3008/mcp3008Spi.cpp

echarles: main.o bmp180dev3.o dht22.o mcp3008Spi.o
	g++ -o echarles main.o bmp180dev3.o dht22.o mcp3008Spi.o -std=c++0x -lm -lwiringPi

.PHONY: install
install: echarles
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	install -m 0755 echarles $(PREFIX)/bin

.PHONY: uninstall
uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/echarles

.PHONY: clean
clean:
	rm -rf *.o
