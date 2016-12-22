

.PHONY=clean

ArduinoConnector.a:arduino.cpp arduino.h uart.cpp uart.h Makefile
	make -C ../kit/
	g++ -c -o uart.o uart.cpp
	g++ -c -o arduino.o arduino.cpp
	ar -r ArduinoConnector.a uart.o arduino.o ../kit/kit.a

clean:
	make -C ../kit/ clean
	-rm arduino.o
	-rm uart.o
	-rm ArduinoConnector.a
