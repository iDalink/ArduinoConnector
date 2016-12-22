void ack() {
  Serial.write(0x59);
}

void processString(unsigned char *string, unsigned char pin) {
  unsigned char firstLocation = 0;
  if (*string == 'P') {
    firstLocation = 3;
  } else {
    firstLocation = 1;
  }
  if (pin > 9) {
    *(string + firstLocation) = pin/10 + 48;
    *(string + firstLocation + 1) = pin - pin/10*10 + 48;
  } else {
    *(string + firstLocation) = pin + 48;
  }
}


#define RS 5
#define RW 4
#define EN 3
#define DATA 6

void setup() {
  Serial.begin(9600);
  pinMode(RW, OUTPUT);
  pinMode(RS, OUTPUT);
  pinMode(EN, OUTPUT);
  for (char i = 0; i < 8; i++) {
    pinMode(i + DATA, OUTPUT);
  }
  digitalWrite(EN, LOW);

  writeCommand(0x38);  //设置显示模式
  writeCommand(0x0c);  //开显示不显示光标，光标不闪烁
  writeCommand(0x06);  //写一个指针加1
  writeCommand(0x01);  //清屏
  writeCommand(0x80);  //设置数据指针起点
}

void loop() {
  processConnection();
}
