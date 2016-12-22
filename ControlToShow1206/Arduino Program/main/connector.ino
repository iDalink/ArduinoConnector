
void processCommander() {
  unsigned char readed = Serial.read();
  unsigned char command = readed & 0xf0;
  int pin = readed & 0x0f;

  bool findValidCommand = false;
  if (command == 0x50) {
    findValidCommand = true;
    //设置读模式
    pinMode(pin, INPUT);
    ack();
  }
  if (command == 0x70) {
    findValidCommand = true;
    //设置写模式
    pinMode(pin, OUTPUT);
    ack();
  }
  if (command == 0x40) {
    findValidCommand = true;
    //设置高电平
    digitalWrite(pin, HIGH);
    ack();
  }
  if (command == 0x60) {
    findValidCommand = true;
    //设置低电平
    digitalWrite(pin, LOW);
    ack();
  }
  if (command == 0xc0) {
    findValidCommand = true;
    //读取引脚电平
    int value = digitalRead(pin);
    unsigned char data = ((value==HIGH)?0xd0:0xc0) | pin;
    Serial.write(data);
  }
  if (command == 0x80) {
    findValidCommand = true;
    //读取模拟量电平
    int value = analogRead(pin);
    Serial.write(readed);
    Serial.write(value * 255/1023);
  }
  if (command == 0x90) {
    findValidCommand = true;
    //设置模拟量电平
    while(!Serial.available());
    int value = Serial.read();
    analogWrite(pin, value * 255/1024);
    ack();
  }

  if (!findValidCommand) {
    unsigned char *string = "Not find![0x  ]";
    unsigned char firstChar = command >> 4;
    if (firstChar > 9) {
      *(string + 12) = firstChar + 65 - 10;
    } else {
      *(string + 12) = firstChar + 48;
    }
    unsigned char secondChar = command & 0x0f;
    if (secondChar > 9) {
      *(string + 13) = secondChar + 65 - 10;
    } else {
      *(string + 13) = secondChar + 48;
    }
    displayString(string);
    
  }
}

void processConnection() {
  if (Serial.available()){
    processCommander();
  }
}

