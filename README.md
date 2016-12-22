## **ArduinoConnector** - 让电脑连接Arduino UNO

### 介绍
- **ArduinoConnector** 可以让您的电脑通过UART与Arduino UNO设备(以下简称Arduino)连接。电脑可以通过串口发送协议数据，从而操作Arduino的IO。

### 功能

- **ArduinoConnector**实现功能主要有GPIO的模式设置、数字引脚的读取与输出、模拟量引脚的输入与输出。

### 演示实例

- kit目录是C语言编译的依赖，例子中几乎所有的C代码都会依赖于它。

- ArduinoConnector目录是**ArduinoConnector**的功能核心模块。编译后得到的文件ArduinoConnector.a是一个静态链接库，您可以在自己的代码中使用它。

- Commandline目录是演示事例1。他编译后得到一个可执行文件。该程序提供一个简单的命令行界面，你可以通过输入命令操作Arduino。

- ControlToShow1206目录是演示事例2。他编译后得到一个可执行文件。该程序直接控制Arduino引脚，从而驱动1206的显示。

### 演示实例效果
- 演示事例1支持的命令行格式如下 
1. PIN[number]=OUTPUT 设置引脚为输出模式

2. PIN[number]=INPUT 设置引脚为输入模式
 
3. PIN[number]=1 设置引脚为高电平

4. PIN[number]=0 设置引脚为低电平

5. PIN[number]=? 读取引脚电平

6. A[number]=[value] 设置模拟量引脚为指定电平

7. A[number]=? 读取模拟量引脚电平

8. quit 退出命令行程序

![](https://github.com/iDalink/ArduinoConnector/blob/master/Commandline/%E6%BC%94%E7%A4%BA%E7%85%A7%E7%89%87.png?raw=true)
- 演示事例2直接操作GPIO控制1206

```
	arduino.digitalWrite(EN, LOW);
	writeCommand(arduino, 0x38);
	writeCommand(arduino, 0x0c);
	writeCommand(arduino, 0x06);
	writeCommand(arduino, 0x01);
	writeCommand(arduino, 0x80);

	displayString(arduino, "Hello,everybody!");
```

![](https://github.com/iDalink/ArduinoConnector/blob/master/ControlToShow1206/%E6%BC%94%E7%A4%BA%E7%85%A7%E7%89%87.png?raw=true)
- 每个演示目录的Arduino Program目录下的代码都是该演示事例对应的Arduino代码。
- 进入演示目录下执行make命令编译，或则是执行make run命令编译并运行。注意波特率的设置和串口设备路径。
#### [linux]$ cd Commandline

#### [linux]$ make run

### 使用
- 您可以在自己的项目中引入**ArduinoConnector**目录的代码编译即可。**ArduinoConnector**目录依赖kit目录。同时您需要注意UART波特率的设置。
