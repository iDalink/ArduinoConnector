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
}

void loop() {
  processConnection();
}
