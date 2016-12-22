/**
 * 指令编码
 * 00001DCB D=1 开显示，D=0 关显示；C=1 显示光标，C=0 不显示光标；B=1 光标闪烁，B=0 光标不闪烁；
 * 000001NS N=1 指针加一，光标加一，N=0 指针减一，光标减一；S=1 整屏左移，S=0 整屏不左移
 * 01H 清屏，数据指针清零，显示清零
 * 80H+地址码，设置数据指针地址
 */

/**
 * 地址
 * 00~27
 * 40~67
 */
 

#define RS 5
#define RW 4
#define EN 3
#define DATA 6

void writePins(char value, char index) {
  for (char i = 0; i < 8; i++) {
    digitalWrite(i + index, ((value >> i) & 0x01)?HIGH:LOW);
  }
}

void writeCommand(char com) {
  digitalWrite(RS, LOW);
  digitalWrite(RW, LOW);
  writePins(com, DATA);
  delay(10);
  digitalWrite(EN, HIGH);
  delay(10);
  digitalWrite(EN, LOW);
}

void writeData(char data) {
  digitalWrite(RS, HIGH);
  digitalWrite(RW, LOW);
  writePins(data, DATA);
  delay(10);
  digitalWrite(EN, HIGH);
  delay(10);
  digitalWrite(EN, LOW);
}




void displayString(char *string) {
  writeCommand(0x80);
  while(*string) {
    writeData(*string);
    string++;
  }
}
