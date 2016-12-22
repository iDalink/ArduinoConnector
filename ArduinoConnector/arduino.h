#include "../kit/language/buildin.h"
#include "../kit/library/memory.h"
#include <stdio.h>

class Arduino {
public:
	typedef enum {
		PIN_MODE_INPUT = 0,
		PIN_MODE_OUTPUT = 1
	} PinMode;

	typedef enum {
		PIN_STATUS_LOW = 0,
		PIN_STATUS_HIGH = 1
	} PinStatus;

	typedef enum {
		PIN0,PIN1,PIN2,
		PIN3,PIN4,PIN5,
		PIN6,PIN7,PIN8,
		PIN9,PIN10,PIN11,
		PIN12,PIN13,
		PIN14,PIN15,PIN16,
		PIN17,PIN18,PIN19
	} Pin;

	typedef enum {
		A0,A1,A2,A3,A4,A5
	} AnalogPin;
private:
	const char *ports;
	int fd;
public:
	Arduino(const char *ports);
	bool init();
	~Arduino();

	int getFd();
public:
	// 设置引脚模式
 	bool setPinMode(Pin, PinMode);
	// 输出数字量
 	bool digitalWrite(Pin, PinStatus);
	// 读取数字量
 	PinStatus digitalRead(Pin);
	// 输出模拟量
	// value 取值范围[0 255]
 	bool analogWrite(AnalogPin, unsigned char);
	// 读取模拟量
	// value 取值范围[0 255]
 	unsigned char analogRead(AnalogPin);

	// 请求数据
 	MemorySection request(MemorySection);
	// 请求数据
 	MemorySection request(MemorySection, size_t);
};