#include "../ArduinoConnector/arduino.h"
#include <unistd.h>

#define RS Arduino::PIN5
#define RW Arduino::PIN4
#define EN Arduino::PIN3
#define DATA Arduino::PIN6

#define OUTPUT Arduino::PIN_MODE_OUTPUT
#define INPUT Arduino::PIN_MODE_INPUT
#define HIGH Arduino::PIN_STATUS_HIGH
#define LOW Arduino::PIN_STATUS_LOW

void writePins(Arduino &arduino, char value, Arduino::Pin index) {
	for (char i = 0; i < 8; i ++) {
		arduino.digitalWrite((Arduino::Pin)(i + (int)index), ((value >> i) & 0x01)?HIGH:LOW);
	}
}

void writeCommand(Arduino &arduino, char com) {
	arduino.digitalWrite(RS, LOW);
	arduino.digitalWrite(RW, LOW);
	writePins(arduino, com, DATA);
	usleep(10);
	arduino.digitalWrite(EN, HIGH);
	usleep(10);
	arduino.digitalWrite(EN, LOW);
}

void writeData(Arduino &arduino, char data) {
	arduino.digitalWrite(RS, HIGH);
	arduino.digitalWrite(RW, LOW);
	writePins(arduino, data, DATA);
	usleep(10);
	arduino.digitalWrite(EN, HIGH);
	usleep(10);
	arduino.digitalWrite(EN, LOW);
}

void displayString(Arduino &arduino, const char *string) {
	writeCommand(arduino, 0x80);
	while(*string) {
		writeData(arduino, *string);
		string ++;
	}
}

int main() {

	Arduino arduino("/dev/ttyACM0");
	if (arduino.init()) {
	} else {
		printf("%s\n", "can not open device");
		return 0;
	}

	//Setup
	arduino.setPinMode(RW, OUTPUT);
	arduino.setPinMode(RS, OUTPUT);
	arduino.setPinMode(EN, OUTPUT);
	for (int i = 0; i < 8; i++) {
		arduino.setPinMode((Arduino::Pin)(i + (int)DATA), OUTPUT);
	}
	arduino.digitalWrite(EN, LOW);
	writeCommand(arduino, 0x38);
	writeCommand(arduino, 0x0c);
	writeCommand(arduino, 0x06);
	writeCommand(arduino, 0x01);
	writeCommand(arduino, 0x80);

	displayString(arduino, "Hello,everybody!");

	sleep(10);
	return 0;
}