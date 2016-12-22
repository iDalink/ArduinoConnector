

.PHONY=clean run

Commandline:main.cpp tools.cpp tools.h
	make -C ../kit/
	make -C ../ArduinoConnector/
	g++ -o Commandline main.cpp tools.cpp ../ArduinoConnector/ArduinoConnector.a ../kit/kit.a

run:Commandline
	./Commandline

clean:
	make -C ../kit/ clean
	make -C ../ArduinoConnector/ clean
	-rm Commandline