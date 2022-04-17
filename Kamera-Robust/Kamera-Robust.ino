//
// begin license header
//
// This file is part of Pixy CMUcam5 or "Pixy" for short
//
// All Pixy source code is provided under the terms of the
// GNU General Pu blic License v2 (http://www.gnu.org/licenses/gpl-2.0.html).
// Those wishing to use Pixy source code, software and/or
// technologies under different licensing terms should contact us at
// cmucam@cs.cmu.edu. Such licensing terms are available for
// all portions of the Pixy codebase presented here.
// end license header//

//
// This sketch is a simple tracking demo that uses the pan/tilt unit.  For
// more information, go here:
//
// http://cmucam.org/projects/cmucam5/wiki/Run_the_Pantilt_Demo
//

#include <SPI.h>
#include <Pixy.h>
#include <SoftwareSerial.h>
#include <TimerOne.h>
#include <Wire.h>
#include <HMC5883L.h>
#include <math.h>
#include "compass.h"
#include <Wire.h>
#include "Kalman.h" // Source: https://github.com/TKJElectronics/KalmanFilter
#define PI 3.14159265
#define X_CENTER        ((PIXY_MAX_X-PIXY_MIN_X)/2)
#define Y_CENTER        ((PIXY_MAX_Y-PIXY_MIN_Y)/2)
#define Address 0x60
#define LINE 200

Kalman kalmanX;
Kalman kalmanY;
Kalman kalmanZ;
Kalman kalmank;
Kalman kalmanK;

double zeroValue[5] = { -200, 44, 660, 52.3, -18.5}; // Found by experimenting

/* All the angles start at 180 degrees */
double gyroXangle = 180;
double gyroYangle = 180;
double gyroZangle = 180;

double compAngleX = 180;
double compAngleY = 180;
double compAngleZ = 180;

unsigned long timer;

uint8_t buffer[2]; // I2C buffer

float derajat;
int anglefix;
int anglefix2;

Pixy pixy;
SoftwareSerial stmSerial(5, 6);
HMC5883L compass;
Vector norm;
uint8_t ball = 0;
//search for ball

//line sensor
char lineSerial[6];
uint8_t lineL;
uint8_t lineR;
uint8_t lineF;

class ServoLoop
{
  public:
    ServoLoop(int32_t pgain, int32_t dgain);

    void update(int32_t error);

    int32_t m_pos;
    int32_t m_prevError;
    int32_t m_pgain;
    int32_t m_dgain;
};


ServoLoop panLoop(300, 500);
ServoLoop tiltLoop(300, 500);

ServoLoop::ServoLoop(int32_t pgain, int32_t dgain)
{
  m_pos = PIXY_RCS_CENTER_POS;
  m_pgain = pgain;
  m_dgain = dgain;
  m_prevError = 0x80000000L;
}

void ServoLoop::update(int32_t error)
{
  long int vel;
  char buf[32];
  if (m_prevError != 0x80000000)
  {
    vel = (error * m_pgain + (error - m_prevError) * m_dgain) >> 10;
    //sprintf(buf, "%ld\n", vel);
    //Serial.print(buf);
    m_pos += vel;
    if (m_pos > PIXY_RCS_MAX_POS)
      m_pos = PIXY_RCS_MAX_POS;
    else if (m_pos < PIXY_RCS_MIN_POS)
      m_pos = PIXY_RCS_MIN_POS;
  }
  m_prevError = error;
}


bool getball() {
  int i;
  uint16_t blocks;
  for (i = 0; i < 30; i++) {
    blocks = pixy.getBlocks();
    if (blocks) {
      return true;
    }
  }

  return false;
}

void toSerial() {
  while (stmSerial.available() == !0) {
    Serial.write(stmSerial.read());
  }
  stmSerial.write("a");
}

void compassSetup() {
  Wire.begin();
  compass_x_offset = 291.67;
  compass_y_offset = 118.69;
  compass_z_offset = -227.39;
  compass_x_gainError = 1.12;
  compass_y_gainError = 1.13;
  compass_z_gainError = 1.03;

  compass_init(2);
}

void compassLoop() {
  compass_scalled_reading();
  compass_heading();
}

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

int readGyroX() { // This really measures the y-axis of the gyro
  uint8_t* data = i2cRead(Address, 18, 2);
  return ((data[0] << 8) + data[1]);
}

int readGyroY() { // This really measures the x-axis of the gyro
  uint8_t* data = i2cRead(Address, 20, 2);
  return ((data[0] << 8) + data[1]);
}

int readGyroZ() { // This really measures the x-axis of the gyro
  uint8_t* data = i2cRead(Address, 22, 2);
  return ((data[0] << 8) + data[1]);
}

double getXangle() {
  double accXval = (double)readAccX(); - zeroValue[0];
  double accZval = (double)readAccZ(); - zeroValue[2];
  double angle = (atan2(accXval, accZval) + PI) * RAD_TO_DEG;
  return angle;
}

double getYangle() {
  double accYval = (double)readAccY(); - zeroValue[1];
  double accZval = (double)readAccZ(); - zeroValue[2];
  double angle = (atan2(accYval, accZval) + PI) * RAD_TO_DEG;
  return angle;
}

double getZangle() {
  double accXval = (double)readAccX(); - zeroValue[0];
  double accYval = (double)readAccY(); - zeroValue[1];
  double angle = (atan2(accXval, accYval) + PI) * RAD_TO_DEG;
  return angle;
}

double getAngle() {
  uint8_t* data = i2cRead(Address, 1, 1);
  return (data[0] * 1.4117647058823529411764705882353);
}

double getAngleHigh() {
  uint8_t* data = i2cRead(Address, 2, 2);
  return (((data[0] << 8) + data[1]));
}

int readAccX() {
  uint8_t* data = i2cRead(Address, 6, 2);
  return ((data[0] << 8) + data[1]);
}

int readAccY() {
  uint8_t* data = i2cRead(Address, 8, 2);
  return ((data[0] << 8) + data[1]);
}

int readAccZ() {
  uint8_t* data = i2cRead(Address, 10, 2);
  return ((data[0] << 8) + data[1]);
}

void cmpsSetup()
{
  kalmanX.setAngle(180); // Set starting angle
  kalmanY.setAngle(180);
  kalmanZ.setAngle(180);
  kalmank.setAngle(180);
  kalmanK.setAngle(180);
  timer = micros();
}

//void cmpsLoop()
//{
//  double gyroXrate = -(((double)readGyroX() /*- zeroValue[3]*/) / 14.375);
//  gyroXangle += gyroXrate * ((double)(micros() - timer) / 1000000); // Without any filter
//
//  double gyroYrate = (((double)readGyroY() /*- zeroValue[4]*/) / 14.375);
//  gyroYangle += gyroYrate * ((double)(micros() - timer) / 1000000); // Without any filter
//
//  double gyroZrate = (((double)readGyroZ() /*- zeroValue[4]*/) / 14.375);
//  gyroZangle += gyroZrate * ((double)(micros() - timer) / 1000000); // Without any filter
//
//  double accXangle = getXangle();
//  double accYangle = getYangle();
//  double accZangle = getZangle();
//  double acckangle = getAngle();
//  double accKangle = getAngleHigh();
//
//  compAngleX = (0.93 * (compAngleX + (gyroXrate * (double)(micros() - timer) / 1000000))) + (0.07 * accXangle);
//  compAngleY = (0.93 * (compAngleY + (gyroYrate * (double)(micros() - timer) / 1000000))) + (0.07 * accYangle);
//  compAngleZ = (0.93 * (compAngleZ + (gyroZrate * (double)(micros() - timer) / 1000000))) + (0.07 * accZangle);
//
//  double xAngle = kalmanX.getAngle(accXangle, gyroXrate, (double)(micros() - timer)); // calculate the angle using a Kalman filter
//  double yAngle = kalmanY.getAngle(accYangle, gyroYrate, (double)(micros() - timer)); // calculate the angle using a Kalman filter
//  double zAngle = kalmanZ.getAngle(accZangle, gyroZrate, (double)(micros() - timer)); // calculate the angle using a Kalman filter
//  double kAngle = kalmank.getAngle(acckangle, gyroZrate, (double)(micros() - timer)); // calculate the angle using a Kalman filter
//  double KAngle = kalmanK.getAngle(accKangle, gyroZrate, (double)(micros() - timer)); // calculate the angle using a Kalman filter
//
//  timer = micros();
//
//  /* print data to processing */
//  //  Serial.print(gyroXangle); Serial.print("\t");
//  //  Serial.print(gyroYangle); Serial.print("\t");
//  double angleHigh = getAngleHigh();
//
////  Serial.println(angleHigh);
////  Serial.print("  ");
////  Serial.println(anglefix);
//
//  anglefix = angleHigh;
//
//  ///////////////////////////////////////////////////////////////////////////////////////
//// 2800 3545 899 2005
//
//  if ((angleHigh >= 3545) && (angleHigh < 899))
//  {
//    anglefix = map(angleHigh, 3545, 898, 0, 900);
//  }
//  if ((angleHigh >= 899) && (angleHigh < 2005))
//  {
//    anglefix = map(angleHigh, 899, 2004, 900, 1800);
//  }
//  else if ((angleHigh >= 2005) && (angleHigh < 2800))
//  {
//    anglefix = map(angleHigh, 2005, 2799, 1800, 2700);
//  }
//  else if ((angleHigh >= 2800) && (angleHigh < 3545))
//  {
////    if(angleHigh < 39)
////      angleHigh += 3599;
//    
//    anglefix = map(angleHigh, 2800, 3544, 2700, 3599);
//  }
//
//  anglefix2 = anglefix % 100;
//  anglefix = anglefix / 100;
//}
//195 684 1608 3246 nomer 3
//847 2220 2978 3553 nomer 2
int kuadran1 = 847;
int kuadran2 = 2220;
int kuadran3 = 2978;
int kuadran4 = 3553;

void cmpsLoop()
{
  double angleHigh = getAngleHigh();
//
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
//  Serial.println(angleHigh);
//  Serial.print("        ");
//  Serial.println(anglefix);

//  if(stmSerial.available())
//    stmSerial.read();

  anglefix2 = anglefix % 100;
  anglefix = anglefix / 100;
}


uint8_t checkLine(int pin0, int pin1, int pin2, int pin3){
  if(analogRead(pin0) < LINE)
    return 1;
  else if(analogRead(pin1) < LINE)
    return 1;
  else if(analogRead(pin2) < LINE)
    return 1;
  else if(analogRead(pin3) < LINE)
    return 1;
  else
    return 0;
}

void calibrate()
{
  delay(2000);

  Wire.beginTransmission(Address);
  Wire.write(0);
  Wire.write(0xF0);
  Wire.endTransmission();
  delay(25);

  Wire.beginTransmission(Address);
  Wire.write(0);
  Wire.write(0xF5);
  Wire.endTransmission();
  delay(25);
  
  Wire.beginTransmission(Address);
  Wire.write(0);
  Wire.write(0xF7);
//  Wire.write(0xF6);
  Wire.endTransmission();
  delay(25);

  while(digitalRead(7) == HIGH){}

  Wire.beginTransmission(Address);
  Wire.write(0);
  Wire.write(0xF8);
  Wire.endTransmission();
  delay(25);  
}

void calibrateDefault()
{
  Wire.beginTransmission(Address);
  Wire.write(0);
  Wire.write(0x20);
  Wire.endTransmission();
  delay(25);

  Wire.beginTransmission(Address);
  Wire.write(0);
  Wire.write(0x2A);
  Wire.endTransmission();
  delay(25);
  
  Wire.beginTransmission(Address);
  Wire.write(0);
  Wire.write(0x60);
  Wire.endTransmission();
  delay(25);
}

void interruptHandler() {
  //  Serial.println(bearing);
  uint8_t xCoor;
  uint8_t yCoor;
  uint8_t heading;
  char sendBuffer[10];
  xCoor = (int)panLoop.m_pos / 4;
  yCoor = (int)tiltLoop.m_pos / 4;
  //compassLoop();
  heading = anglefix; //bearing*0.7083;
//  Serial.print(heading);
//  Serial.print("\t");
//  Serial.println(anglefix);
//sprintf(ui,"%d",a);

  sendBuffer[0] = 'X';
  sendBuffer[1] = xCoor;
  sendBuffer[2] = 'Y';
  sendBuffer[3] = yCoor;
  sendBuffer[4] = 'C';
  sendBuffer[5] = anglefix;
  sendBuffer[6] = 'O';
  sendBuffer[7] = anglefix2;
  sendBuffer[8] = 'B';
  sendBuffer[9] = ball;
 

//  Serial.print(angleHigh);
////  Serial.print("  ");
//  Serial.println(anglefix);
  
  Serial.write(sendBuffer[0]);
  Serial.write(sendBuffer[1]);
  Serial.write(sendBuffer[2]);
  Serial.write(sendBuffer[3]);
  Serial.write(sendBuffer[4]);
  Serial.write(sendBuffer[5]);
  Serial.write(sendBuffer[6]);
  Serial.write(sendBuffer[7]);
  Serial.write(sendBuffer[8]);
  Serial.write(sendBuffer[9]);

//  Serial.println(sendBuffer[0]);
//  Serial.println(xCoor);
//  Serial.println(sendBuffer[2]);
//  Serial.println(yCoor);
//  if(Serial.available());
  
   

//  Serial.print(anglefix);
//  Serial.println(anglefix2);  

  //line sensor
//  Serial.write(lineSerial[0]);
//  Serial.write(lineSerial[1]);
//  Serial.write(lineSerial[2]);
//  Serial.write(lineSerial[3]);
//  Serial.write(lineSerial[4]);
//  Serial.write(lineSerial[5]);
}

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  //stmSerial.begin(9600);
  Serial.print("Starting...\n");
//  compassSetup();
  cmpsSetup();
  Timer1.initialize(50000);
  Timer1.attachInterrupt(interruptHandler);

  pixy.init();

  //line sensor
//  lineSerial[0] = 'L';
//  lineSerial[2] = 'R';
//  lineSerial[4] = 'F';
//  pinMode(7, INPUT);
  
//  calibrate();
//  calibrateDefault();
  
}

void loop()
{

  static int i = 0;
  int j;
  int counter;
  uint16_t blocks;
  char buf[32];
  char printSerial[32];
  int counterBallDetection;
  int32_t panError, tiltError;

//  char drajat[2];
//
//  drajat[0] = anglefix;
//  drajat[1] = anglefix2;
  
  while (1) {
    cmpsLoop();
    
//    Serial.print(anglefix);
//    Serial.println(anglefix2);
//    Serial.println(anglefix);

    //    counter++;
    //    if(counter == 2000){
//          compassLoop();
    //      //Serial.println(bearing);
    //      counter =0;
    //    }
    //    else{}
    
    
    blocks = pixy.getBlocks();
    if (blocks)
    {
      ball = 1;
      counterBallDetection = 0;
      panError = pixy.blocks[0].x - X_CENTER;
      tiltError = Y_CENTER - pixy.blocks[0].y;

      panLoop.update(panError);
      tiltLoop.update(tiltError);
      if (tiltLoop.m_pos > PIXY_RCS_MAX_POS - 300) {
        tiltLoop.m_pos = PIXY_RCS_MAX_POS - 300;
//        serial.println(
      }
       if (tiltLoop.m_pos>500)
      {
        tiltLoop.m_pos = 500;
      }
      pixy.setServos(panLoop.m_pos, tiltLoop.m_pos);
//      Serial.println(tiltLoop.m_pos);
     
      //    sprintf(printSerial,"Servo X = %d", panLoop.m_pos);
      //    Serial.println(printSerial);
      //    sprintf(printSerial,"Servo Y = %d", tiltLoop.m_pos);
      //    Serial.println(printSerial);
    }
    else {
      counterBallDetection ++;
      if (counterBallDetection == 1000) {
        tiltLoop.m_pos = 300;
        panLoop.m_pos = PIXY_RCS_CENTER_POS;
        pixy.setServos(panLoop.m_pos, tiltLoop.m_pos);
        counterBallDetection = 0;
        ball = 0;
      }
    }

    //line sensorL
//    lineL = checkLine(A0, A1, A2, A3);
//    lineR = checkLine(A4, A5, A6, A7);
//    lineF = checkLine(A8, A9, A10, A11);
//  
//    lineSerial[1] = lineL;
//    lineSerial[3] = lineR;
//    lineSerial[5] = lineF;
  }
}

