#include "arduino.h"
#include "uart.h"
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

Arduino::Arduino(const char *ports) {
	this->ports = ports;
}

bool Arduino::init() {
	int fd = UARTOpen(ports);
	if (fd > 0) {
		if (UARTInit(fd,9600,0,8,1,'N')) {
			this->fd = fd;	
			sleep(2);
			return true;
		} else {

		}
	}
	return false;
}

Arduino::~Arduino() {
	if (fd > 0) {
		UARTClose(fd);
		fd = 0;
	}
}

int Arduino::getFd() {
	return fd;
}

bool Arduino::setPinMode(Pin pin, PinMode mode) {
	unsigned char data = (mode?0x7:0x5) << 4 | pin;
	MemorySection requestData = {&data, sizeof(data)};
	MemorySection response = request(requestData, 1);
	char responseChar = *((char *)response.address);
	free(response.address);
	return responseChar == 0x59;
}

bool Arduino::digitalWrite(Pin pin, PinStatus status) {
	unsigned char data = (status?0x4:0x6) << 4 | pin;
	MemorySection requestData = {&data, sizeof(data)};
	MemorySection response = request(requestData, 1);
	char responseChar = *((char *)response.address);
	free(response.address);
	return responseChar == 0x59;
}

Arduino::PinStatus Arduino::digitalRead(Pin pin) {
	unsigned char data = 0xc << 4 | pin;
	MemorySection requestData = {&data, sizeof(data)};
	MemorySection response = request(requestData, 1);
	unsigned char responseChar = *((char *)response.address);
	free(response.address);
	unsigned char highResonseChar = 0xd << 4 | pin;
	unsigned char lowResonseChar = 0xc << 4 | pin;
	if (responseChar == highResonseChar) {
		return PIN_STATUS_HIGH;
	}
	if (responseChar == lowResonseChar) {
		return PIN_STATUS_LOW;
	}
	throw "Can not read pin data!";
}

bool Arduino::analogWrite(AnalogPin pin, unsigned char value) {
	unsigned char data[2];
	data[0] = 0x9 << 4 | ((int)pin - 14);
	data[1] = value;
	MemorySection requestData = {&data, sizeof(data)};
	MemorySection response = request(requestData, 1);
	char responseChar = *((char *)response.address);
	free(response.address);
	return responseChar == 0x59;
}

unsigned char Arduino::analogRead(AnalogPin pin) {
	unsigned char data = 0x8 << 4 | ((int)pin - 14);
	MemorySection requestData = {&data, sizeof(data)};
	MemorySection response = request(requestData, 2);
	if (response.size == 2) {
		unsigned char responseChar = *((unsigned char *)response.address);
		unsigned char certain = 0x8 << 4 | ((int)pin - 14);
		if (responseChar == certain) {
			unsigned char value = *(((unsigned char *)response.address + 1));
			free(response.address);
			return value;
		}
	}
	free(response.address);
	throw "Can not read pin data!";
}

MemorySection Arduino::request(MemorySection requestData) {
	UARTWrite(fd, requestData);
	return UARTRead(fd);
}

MemorySection Arduino::request(MemorySection requestData, size_t expectedResponseDataSize) {
	UARTWrite(fd, requestData);
	return UARTAscertainedRead(fd, expectedResponseDataSize);
}