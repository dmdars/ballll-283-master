#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); //Rx, Tx

char buff;
char flag;
int count = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  mySerial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available())
  {
    buff = Serial.read();

    if(flag == 'X' && count < 3)
    {
      mySerial.write(buff);
      count++;
    }
    else if(flag == 'Y' && count < 3)
    {
      mySerial.write(buff);
      count++;
    }
    else
    {
      flag = buff;
      count = 0;
      mySerial.write("\n");
    }
  }
  delay(10);
}
