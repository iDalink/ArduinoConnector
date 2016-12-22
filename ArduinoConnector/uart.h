#include "../kit/language/buildin.h"
#include "../kit/library/memory.h"
#include <stdio.h>

/**
 * 打开串口
 * #param port 串口路径
 * #return 返回打开的设备文件描述符
 */
int UARTOpen(const char *port);

/**
 * 关闭串口
 * #param fd 串口路径
 * #return 是否关闭
 */
bool UARTClose(int fd);

/**
 * 关闭串口
 * #param fd 设备文件描述符
 * #param speed 速度 波特率
 * #param flow_ctrl
 * #param databits
 * #param stopbits
 * #param parity
 * #return 是否成功初始化
 */
bool UARTInit(int fd, int speed,int flow_ctrl,int databits,int stopbits,int parity);

/**
 * 读取指定长度的数据
 * #param fd 设备文件描述符
 * #param expectedSize 期望的读取数据长度
 * #return 读取的数据
 */
MemorySection UARTAscertainedRead(int fd, size_t expectedSize);

/**
 * 读取数据，遇到结束符号'\0'结束
 * #param fd 设备文件描述符
 * #return 读取到的数据
 */
MemorySection UARTRead(int fd);

/**
 * 写入数据
 * #param fd 设备文件描述符
 * #param data 要写入的数据
 * #return 是否成功
 */
bool UARTWrite(int fd, MemorySection data);