#include <SoftwareSerial.h>
#include <Wire.h>
#include <VL53L0X.h>
#include <TimerOne.h>

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

SoftwareSerial mySerial(10, 11); //Rx, Tx

char buff;
char flag;
char coorX[4];
char coorY[4];
char sendX[4]={'X',0,0,0};
char sendY[4]={'Y',0,0,0};
int count = 1;
int xCoor;
int yCoor;

uint8_t buffer[2]; // I2C buffer
int anglefix;
int anglefix2;

int type;

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

void cmpsLoop()
{
  double angleHigh = getAngleHigh();
  
  if ((angleHigh >= KUADRAN1) && (angleHigh < KUADRAN2))
  {
    anglefix = map(angleHigh, KUADRAN1, KUADRAN2, 0, 900);
  }
  if ((angleHigh >= KUADRAN2) && (angleHigh < KUADRAN3))
  {
    anglefix = map(angleHigh, KUADRAN2, KUADRAN3, 900, 1800);
  }
  else if ((angleHigh >= KUADRAN3) && (angleHigh < KUADRAN4))
  {
    anglefix = map(angleHigh, KUADRAN3, KUADRAN4, 1800, 2700);
  }
  else if ((angleHigh >= KUADRAN4) || (angleHigh < KUADRAN1))
  {
    if(angleHigh < KUADRAN1)
      angleHigh += 3599;
    
    anglefix = map(angleHigh, KUADRAN4, 3599 + KUADRAN1, 2700, 3599);
  }
}

void sendSerial()
{
  char temp[5];
  mySerial.write('C');  
  //Serial.write('C');
  sprintf(temp, "%04d", anglefix);
//  Serial.write(temp);
  mySerial.write(temp);
  mySerial.write("\n");
//  Serial.write("\n");
  mySerial.write("X");
  sprintf(temp, "%03d", xCoor);
  mySerial.write(temp);
  mySerial.write("Y");
  sprintf(temp, "%03d", yCoor);
  mySerial.write(temp);
  
}

void detectBall()
{
  if(sensor.readRangeSingleMillimeters() < 130)
  {
    mySerial.write('T');
    mySerial.write('1');
  }
  else
  {
    mySerial.write('T');
    mySerial.write('0');
  }
  mySerial.write("\n");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  mySerial.begin(9600);
  Wire.begin();
  Timer1.initialize(100000);//(250000);
  Timer1.attachInterrupt(sendSerial);
  coorX[0]='X';
  coorY[0]='Y';

  sensor.init();
  sensor.setTimeout(500);
  
  #if defined HIGH_ACCURACY
    sensor.setMeasurementTimingBudget(200000);
  #endif
}

void loop() 
{
  // put your main code here, to run repeatedly:
  cmpsLoop();
  detectBall();
//  mySerial.write("START");
  //mySerial.write("\n");

  //Serial.println("TEST");
  
  while(Serial.available() > 0)
  {
      buff = Serial.read();
//      mySerial.write('\n');
//      mySerial.write(buff);
//      mySerial.write('\n');
      if(flag == 'X' && count < 3)
      {
        type = 1;
        coorX[count] = buff;
        count++;
      }
      else if(flag == 'Y' && count < 3)
      {
        type = 2;
        coorY[count] = buff;
        count++;
      }
      else
      {
        flag = buff;
        if(count>3&&type==1){
          xCoor= 100*(coorX[1]) + 10*(coorX[2]) + coorX[3];
        }
        if(count>3&&type==2){
          yCoor= 100*(coorY[1]) + 10*(coorY[2]) + coorY[3];
        }
        
        count = 1;
//        if(type==1)
//        {
//          for(int i = 0; i < 4; i++)
//          {
//            mySerial.write(coorX[i]);
//            Serial.flush();
//          }
//            
//          mySerial.write("\n");
//        }
//        else if(type==2)
//        {
//          for(int i = 0; i < 4; i++)
//          {
//            mySerial.write(coorY[i]);
//          }
//          mySerial.write("\n");
//        }
        
        if(flag == 'S')   //stop
        {
//          mySerial.write('\n');
          mySerial.write('S');
//          mySerial.write('\n');
        }
        else if(flag == 's')    //play
        {
//          mySerial.write('\n');
          mySerial.write('P');
//          mySerial.write('\n');
        }
        type = 0;
//        mySerial.write("\n");
        break;
      }
     delay(10);
  }
}
