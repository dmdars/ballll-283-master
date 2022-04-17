#include <Wire.h>
#include <SoftwareSerial.h>
#include <TimerOne.h>
#include <VL53L0X.h>

VL53L0X sensor;
#define HIGH_ACCURACY

//cmps 1
//#define KUADRAN1 121
//#define KUADRAN2 971
//#define KUADRAN3 1772
//#define KUADRAN4 2697

//cmps 2
#define KUADRAN1 122
#define KUADRAN2 999
#define KUADRAN3 2076
#define KUADRAN4 2876

#define CMPS 0x60
#define TOF  0x29 

 // Rx, Tx
SoftwareSerial mySerial(10, 11); // RX, TX

uint8_t xOmni1, xOmni2;
uint8_t yOmni1, yOmni2;
char camOmniX, camOmniY;
char conData, conSetData, conSelData;
String gameStateData;
uint8_t buffer[2]; // I2C buffer
int anglefix;
int anglefix2;
char inByte, flag;
int count;


void i2cWrite(uint8_t address, uint8_t registerAddress) {
  Wire.beginTransmission(address);
  Wire.write(registerAddress);
  Wire.endTransmission();
}

uint8_t* i2cRead(uint8_t address, uint8_t registerAddress, uint8_t nbytes) {
  Wire.beginTransmission(address);
  Wire.write(registerAddress);
  Wire.endTransmission();
  Wire.beginTransmission(address);
  Wire.requestFrom(address, nbytes);
  for (uint8_t i = 0; i < nbytes; i++)
    buffer[i] = Wire.read();
  Wire.endTransmission();
    return buffer;
}

double getAngleHigh() {
  uint8_t* data = i2cRead(CMPS, 2, 2);
  uint16_t result = (data[0] << 8) + data[1];
  return result;//(((data[0] << 8) + data[1]));
}

void cmpsLoop(int kuadran1, int kuadran2, int kuadran3, int kuadran4)
{
  double angleHigh = getAngleHigh();

  if ((angleHigh >= kuadran1) && (angleHigh < kuadran2))
  {
    anglefix = map(angleHigh, kuadran1, kuadran2, 0, 900);
  }
  if ((angleHigh >= kuadran2) && (angleHigh < kuadran3))
  {
    anglefix = map(angleHigh, kuadran2, kuadran3, 900, 1800);
  }
  else if ((angleHigh >= kuadran3) && (angleHigh < kuadran4))
  {
    anglefix = map(angleHigh, kuadran3, kuadran4, 1800, 2700);
  }
  else if ((angleHigh >= kuadran4) || (angleHigh < kuadran1))
  {
    if(angleHigh < kuadran1)
      angleHigh += 3599;
    
    anglefix = map(angleHigh, kuadran4, 3599 + kuadran1, 2700, 3599);
  }
//
//  Serial.print(angleHigh);
//  Serial.print("        ");
//  Serial.println(anglefix);

//  anglefix2 = anglefix / 10;
//
//  if(anglefix2 > 255)
//  {
//    anglefix3 = anglefix2 - 255; 
//    anglefix2 = 255;
//  }
//  else
//    anglefix3 = 0;

  anglefix2 = anglefix % 100;
  anglefix = anglefix / 100;
  
}

void interruptHandler()
{
  
//  mySerial.write("X");
//  mySerial.write(camOmniX);
//  mySerial.write("Y");
//  mySerial.write(camOmniY);
  
//  mySerial.write("g");
//  if(gameStateData[0] != -1)
//  {
//    mySerial.write(gameStateData[0]);
//  }

//  mySerial.write("T");
//  mySerial.write(sensor.readRangeSingleMillimeters());
//  
//  mySerial.write("C");
//  mySerial.write(anglefix);
//  mySerial.write("O");
//  mySerial.write(anglefix2);

  //Serial.println("MASUK");
//  Serial.println(gameStateData);/
  
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin();
  mySerial.begin(9600);
//  Timer1.initialize(50000); //0.05 sec
//  Timer1.attachInterrupt(interruptHandler);
  sensor.init();
  sensor.setTimeout(500);

  #if defined HIGH_ACCURACY
    sensor.setMeasurementTimingBudget(900000);
  #endif
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
//  cmpsLoop(KUADRAN1, KUADRAN2, KUADRAN3, KUADRAN4);
  if(Serial.available())
  {
    inByte = Serial.read();

    if(flag == 'X' && count < 4)
    {
      mySerial.write(inByte);
      count++;
    }
    else if(flag == 'Y' && count < 4)
    {
      mySerial.write(inByte);
      count++;
    }
    else
    {
      flag = inByte;
      count = 1;
      mySerial.write("\n");
    }
  }
  delay(10);
}
  
