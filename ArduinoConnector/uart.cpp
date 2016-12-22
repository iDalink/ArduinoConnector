#include "uart.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h> 
#include <termios.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

int UARTOpen(const char *port) {
	int fd = open(port, O_RDWR|O_NOCTTY|O_NDELAY);
	if (fd < 0) {
		perror("Can't Open Serial Port");
		return fd;
	}
	//恢复串口为阻塞状态                               
	if(fcntl(fd, F_SETFL, 0) < 0) {
		printf("fcntl failed!\n");
		goto failed;
	} else {
		printf("fcntl=%d\n", fcntl(fd, F_SETFL,0));
	}
	//测试是否为终端设备    
	if(!isatty(STDIN_FILENO)) {
		printf("standard input is not a terminal device\n");
		goto failed;
	} else {
		printf("isatty success!\n");
	}
	printf("fd->open=%d\n",fd);
	return fd;
	failed:
	UARTClose(fd);
	return -1;
}

bool UARTClose(int fd) {
	return close(fd);
}

int UARTSet(int fd,int speed,int flow_ctrl,int databits,int stopbits,int parity) {
	int   i;
	int   status;
	int   speed_arr[] = { 115200, 19200, 9600, 4800, 2400, 1200, 300};
	int   name_arr[] = {115200,  19200,  9600,  4800,  2400,  1200,  300};

	struct termios options;
   
	//tcgetattr(fd,&options)得到与fd指向对象的相关参数，并将它们保存于options,该函数还可以测试配置是否正确，该串口是否可用等。若调用成功，函数返回值为0，若调用失败，函数返回值为1.
	if (tcgetattr(fd, &options) != 0) {
		perror("SetupSerial 1");    
		goto failed;
	}
  
	//设置串口输入波特率和输出波特率
	for (i = 0; i < sizeof(speed_arr) / sizeof(int); i++) {
		if  (speed == name_arr[i]) {             
			cfsetispeed(&options, speed_arr[i]); 
			cfsetospeed(&options, speed_arr[i]);  
		}
	}

	//修改控制模式，保证程序不会占用串口
	options.c_cflag |= CLOCAL;
	//修改控制模式，使得能够从串口中读取输入数据
	options.c_cflag |= CREAD;

	//设置数据流控制
	switch(flow_ctrl) {
		case 0://不使用流控制
			options.c_cflag &= ~CRTSCTS;
			break;
		case 1://使用硬件流控制
			options.c_cflag |= CRTSCTS;
 			break;
		case 2://使用软件流控制
			options.c_cflag |= IXON | IXOFF | IXANY;
			break;
	}
	//设置数据位
	//屏蔽其他标志位
	options.c_cflag &= ~CSIZE;
	switch (databits) {  
		case 5:
			options.c_cflag |= CS5;
			break;
		case 6:
			options.c_cflag |= CS6;
			break;
		case 7:    
			options.c_cflag |= CS7;
			break;
		case 8:    
			options.c_cflag |= CS8;
			break;  
		default:   
			fprintf(stderr,"Unsupported data size\n");
	        return -1;
	}
	//设置校验位
	switch (parity) {  
		case 'n':
		case 'N': //无奇偶校验位。
			options.c_cflag &= ~PARENB; 
			options.c_iflag &= ~INPCK;    
			break; 
		case 'o':  
		case 'O'://设置为奇校验    
			options.c_cflag |= (PARODD | PARENB); 
			options.c_iflag |= INPCK;             
			break; 
		case 'e': 
		case 'E'://设置为偶校验  
			options.c_cflag |= PARENB;       
			options.c_cflag &= ~PARODD;       
			options.c_iflag |= INPCK;      
			break;
		case 's':
		case 'S': //设置为空格 
			options.c_cflag &= ~PARENB;
			options.c_cflag &= ~CSTOPB;
			break; 
		default:  
			fprintf(stderr,"Unsupported parity\n");    
			goto failed;
	} 
	// 设置停止位 
	switch (stopbits) {  
		case 1:   
			options.c_cflag &= ~CSTOPB;
			break; 
		case 2:   
			options.c_cflag |= CSTOPB;
			break;
		default:   
			fprintf(stderr,"Unsupported stop bits\n"); 
			goto failed;
	}
   
	//修改输出模式，原始数据输出
	options.c_oflag &= ~OPOST;
  
	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);//我加的
	//options.c_lflag &= ~(ISIG | ICANON);
   
	//设置等待时间和最小接收字符
	options.c_cc[VTIME] = 1; /* 读取一个字符等待1*(1/10)s */  
	options.c_cc[VMIN] = 1; /* 读取字符的最少个数为1 */
   
	//如果发生数据溢出，接收数据，但是不再读取 刷新收到的数据但是不读
	tcflush(fd,TCIFLUSH);
   
	//激活配置 (将修改后的termios数据设置到串口中）
	if (tcsetattr(fd,TCSANOW,&options) != 0) {
		perror("com set error!\n");  
		goto failed;
	}
	return fd;
	failed:
	UARTClose(fd);
	return -1;
}

bool UARTInit(int fd, int speed,int flow_ctrl,int databits,int stopbits,int parity) {
	//设置串口数据帧格式
	return UARTSet(fd,speed,0,8,1,'N');
}

MemorySection UARTAscertainedRead(int fd, size_t expectedSize) {
	void *buffer = malloc(expectedSize);
	size_t readedSize = 0;
	while(readedSize < expectedSize) {
		readedSize += read(fd, buffer, expectedSize - readedSize);
	}
	MemorySection result = {buffer, expectedSize};
	return result;
}

MemorySection UARTRead(int fd) {
	size_t bufferSize = 2;
	void *buffer = malloc(bufferSize);
	
	size_t readedSize = 0;
	while(true) {
		if (bufferSize > readedSize) {
			readedSize += read(fd, (char *)buffer + readedSize, bufferSize - readedSize);
			for (int i = 0; i < readedSize; i++) {
				if (*((char *)buffer + i) == '\0') {
					break;
				}
			}
		} else {
			MemorySection section = {buffer, bufferSize};
			buffer = MemoryExpand(section, bufferSize * 2);
			bufferSize = bufferSize * 2;
		}
	}
	MemorySection result = {buffer, readedSize};
	return result;
}

bool UARTWrite(int fd, MemorySection data) {
	size_t writedSize = 0;
	while (writedSize < data.size) {
		writedSize += write(fd, (char *)data.address + writedSize, data.size - writedSize);	
	}
	return true;
}