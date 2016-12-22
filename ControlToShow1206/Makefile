

.PHONY=clean run

Controller:main.cpp
	make -C ../kit/
	make -C ../ArduinoConnector/
	g++ -o Controller main.cpp ../ArduinoConnector/ArduinoConnector.a ../kit/kit.a

run:Controller
	./Controller

clean:
	make -C ../kit/ clean
	make -C ../ArduinoConnector/ clean
	-rm Controller