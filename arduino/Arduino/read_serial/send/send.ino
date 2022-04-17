#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); //Rx, Tx

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  mySerial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(mySerial.available())
  {
    Serial.write(mySerial.read());
  }
}
