#include <stdio.h>      /*标准输入输出定义*/
#include <stdlib.h>     /*标准函数库定义*/
#include <unistd.h>     /*Unix 标准函数定义*/
#include <sys/types.h> 
#include <sys/stat.h>   
#include <fcntl.h>      /*文件控制定义*/
#include <termios.h>    /*PPSIX 终端控制定义*/
#include <errno.h>      /*错误号定义*/
#include <string.h>
#include "../ArduinoConnector/arduino.h"
#include "tools.h"
#include <cstdio>
#include <cstdlib>

const char *pinName[] = {"PIN0", "PIN1", "PIN2", "PIN3", "PIN4", "PIN5",
						  "PIN6", "PIN7", "PIN8", "PIN9", "PIN10", "PIN11",
						  "PIN12", "PIN13", "PIN14", "PIN15", "PIN16", "PIN17", "PIN18", "PIN19"};

const char *analogPinName[] = {"A0", "A1", "A2", "A3", "A4", "A5"};

int main() {
	Arduino arduino("/dev/ttyACM0");
	if (arduino.init()) {
	} else {
		printf("%s\n", "can not open device");
		return 0;
	}
	while(true) {
		printf("> ");
		char buffer[10];
		char *readed_char = gets(buffer);
		readed_char = buffer;
		size_t length = strlen(buffer);

		if (!strcmp("quit", buffer)) {
			break;
		}

		if (!strcmp("q", buffer)) {
			break;
		}

		if (StringCharCount(buffer, '=') == 1) {
			char *pin_buffer = readed_char;
			int equalLocation = SearchCharactorInString(buffer, '=');
			const char *pinString = pin_buffer;
			const char *valueString = pin_buffer + equalLocation + 1;
			// =右侧区域
			const char *value_buffer = valueString;
			// 引脚编号
			Arduino::Pin match_pin = (Arduino::Pin)MatchedPin(pinString, equalLocation);
			if (match_pin < 0) {
				printf("%s\n", "Don`t find valid pin in comman!");
				continue;
			}
			if (*buffer == 'P') {
				//数字量引脚操作
				if (*value_buffer == 'O') {
					//设置输出模式
					if (arduino.setPinMode(match_pin, Arduino::PIN_MODE_OUTPUT)) {
						printf("%s号引脚设置输出模式\n", pinName[match_pin]);
					} else {
						printf("%s\n", "write failed!");
					}
				}
				if (*value_buffer == 'I') {
					//设置输入模式
					if (arduino.setPinMode(match_pin, Arduino::PIN_MODE_INPUT)) {
						printf("%s号引脚设置输入模式\n", pinName[match_pin]);
					} else {
						printf("%s\n", "write failed!");
					}
				}
				if (*value_buffer == '1') {
					//shezhi gaodianping
					if (arduino.digitalWrite(match_pin, Arduino::PIN_STATUS_HIGH)) {
						printf("%s号引脚输出高电平\n", pinName[match_pin]);
					} else {
						printf("%s\n", "write failed!");
					}
				}
				if (*value_buffer == '0') {
					//
					if (arduino.digitalWrite(match_pin, Arduino::PIN_STATUS_LOW)) {
						printf("%s号引脚输出低电平\n", pinName[match_pin]);
					} else {
						printf("%s\n", "write failed!");
					}
				}
				if (*value_buffer == '?') {
					//dds
					Arduino::PinStatus status = arduino.digitalRead(match_pin);
					if (status == Arduino::PIN_STATUS_LOW) {
						printf("%s号引脚输出低电平\n", pinName[match_pin]);
					} else {
						printf("%s号引脚输出ga电平\n", pinName[match_pin]);
					}
				}
			}
			if (*buffer == 'A') {
				//模拟量引脚操作
				
				if (CharactorIsNumber(*value_buffer)) {
					unsigned char value = (unsigned char)atoi_ex(value_buffer, strlen(value_buffer));
					//输chu模拟量
					if (arduino.analogWrite((Arduino::AnalogPin)match_pin, value)) {
						printf("%s号引脚输出%d\n", analogPinName[match_pin], value);
					} else {
						printf("%s\n", "write failed!");
					}
				}
				if (*value_buffer == '?') {
					//读取模拟量
					unsigned char value = arduino.analogRead((Arduino::AnalogPin)match_pin);
					printf("%s号引脚电平为\n", analogPinName[match_pin], value);
				}
				
			}
		}
	}
	return 0;
}