/*
 * rosserial PubSub Example
 * Prints "hello world!" and toggles led
 */

//#include <stdio.h>
//
//#define USE_USBCON

#include <TimerOne.h>
#include <Wire.h>
#include <ros.h>
#include <std_msgs/Char.h>
#include <std_msgs/String.h>
#include <geometry_msgs/Pose2D.h>
#include <SoftwareSerial.h>

#define USE_USBCON

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

#define Address 0x60

SoftwareSerial mySerial(10, 11); // RX, TX

ros::NodeHandle  nh;

uint8_t xOmni1, xOmni2;
uint8_t yOmni1, yOmni2;
int camOmniX, camOmniY;
char conData, conSetData, conSelData;
String gameStateData;
uint8_t buffer[2]; // I2C buffer
int anglefix;
int anglefix2;

void camOmniCb( const geometry_msgs::Pose2D& camOmni){
  camOmniX = camOmni.x;
  camOmniY = camOmni.y;
}

void control( const std_msgs::Char& control){
  conData = control.data;
}

void tcpcom( const std_msgs::String& tcpcom){
  gameStateData = tcpcom.data;
}

ros::Subscriber<geometry_msgs::Pose2D> sub("bot1camOmni", camOmniCb );
ros::Subscriber<std_msgs::Char> control_subscriber("control", control );
ros::Subscriber<std_msgs::String> gamestate_subscriber("tcpcom", tcpcom );


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
  uint8_t* data = i2cRead(Address, 2, 2);
  return (((data[0] << 8) + data[1]));
}

int anglefix3;
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

  Serial.print(angleHigh);
  Serial.print("        ");
  Serial.println(anglefix);

  anglefix2 = anglefix / 10;

  if(anglefix2 > 255)
  {
    anglefix3 = anglefix2 - 255; 
    anglefix2 = 255;
  }
  else
    anglefix3 = 0;

//  anglefix2 = anglefix % 100;
//  anglefix = anglefix / 100;
}

void interruptHandler()
{

  xOmni1 = camOmniX/10;
  xOmni2 = camOmniX%10;
  yOmni1 = camOmniY/10;
  yOmni2 = camOmniY%10;

  mySerial.write("u");
  mySerial.write(xOmni1);
  mySerial.write("i");
  mySerial.write(xOmni2);
  mySerial.write("o");
  mySerial.write(yOmni1);
  mySerial.write("p");
  mySerial.write(yOmni2);

  mySerial.write("t");
  mySerial.write(conData);
  
  mySerial.write("g");
  if(gameStateData[0] != -1)
  {
    mySerial.write(gameStateData[0]);
  }
  
  mySerial.write("C");
  mySerial.write(anglefix2);
  mySerial.write("O");
  mySerial.write(anglefix3);

//  Serial.println(gameStateData);/
  
}

//std_msgs::String str_msg;
//ros::Publisher chatter("chatter", &str_msg);

void setup()
{
  Serial.begin(9600);
  Wire.begin();
//  nh.getHardware()->setBaud(9600);
  nh.initNode();
  delay(1000);
//  nh.advertise(chatter);
  nh.subscribe(sub);
  delay(1000);
  nh.subscribe(control_subscriber);
  delay(1000);
  nh.subscribe(gamestate_subscriber);
  delay(1000);
  mySerial.begin(9600);
  Timer1.initialize(50000);
  Timer1.attachInterrupt(interruptHandler);
}

void loop()
{
  cmpsLoop(KUADRAN1, KUADRAN2, KUADRAN3, KUADRAN4);
  nh.spinOnce();
}
