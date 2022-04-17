/*
 * rosserial PubSub Example
 * Prints "hello world!" and toggles led
 */
 
#include <TimerOne.h>
#include <Wire.h>
#include <ros.h>
#include <std_msgs/Char.h>
#include <geometry_msgs/Pose2D.h>
#include <SoftwareSerial.h>

#define KUADRAN1 121
#define KUADRAN2 971
#define KUADRAN3 1772
#define KUADRAN4 2697

#define Address 0x60

SoftwareSerial mySerial(10, 11); // RX, TX

ros::NodeHandle  nh;

uint8_t xOmni1, xOmni2;
uint8_t yOmni1, yOmni2; 
uint8_t xFront1, xFront2;
uint8_t yFront1, yFront2;
int camOmniX, camOmniY;
int camFrontX, camFrontY;
int coorX, coorY;
char conData, conSelData;
uint8_t buffer[2]; // I2C buffer
float derajat;
int anglefix;
int anglefix2;

void camOmniCb( const geometry_msgs::Pose2D& camOmni){
  camOmniX = camOmni.x;
  camOmniY = camOmni.y;
}

void control( const std_msgs::Char& control){
  conData = control.data;
}

void control_select( const std_msgs::Char& control_select){
  conSelData = control_select.data;
}

ros::Subscriber<geometry_msgs::Pose2D> sub("bot1camOmni", camOmniCb );
ros::Subscriber<std_msgs::Char> control_subscriber("control", control );
ros::Subscriber<std_msgs::Char> control_select_subscriber("control", control_select );

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

  anglefix2 = anglefix % 100;
  anglefix = anglefix / 100;
}

void interruptHandler()
{
  
  xOmni1 = camOmniX/10;
  xOmni2 = camOmniX%10;
  yOmni1 = camOmniY/10;
  yOmni2 = camOmniY%10;

  mySerial.write("q");
  mySerial.write(xOmni1);
  mySerial.write("w");
  mySerial.write(xOmni2);
  mySerial.write("e");
  mySerial.write(yOmni1);
  mySerial.write("r");
  mySerial.write(yOmni2);

  mySerial.write("t");
  mySerial.write(conData);
  mySerial.write("y");
  mySerial.write(conSelData);

  mySerial.write("C");
  mySerial.write(anglefix);
  mySerial.write("O");
  mySerial.write(anglefix2);
}

void pyserial(){
  data = Serial.read();
  if(data = "x"){coorX=Serial.read()}
  if(data = "y"){coory=Serial.read()}
}

//std_msgs::String str_msg;
//ros::Publisher chatter("chatter", &str_msg);

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  nh.initNode();
  coorX = Serial.Read()
//  nh.advertise(chatter);
//  nh.subscribe(sub);
  mySerial.begin(9600);
  Timer1.initialize(50000);
  Timer1.attachInterrupt(interruptHandler);
}

void loop()
{
  cmpsLoop(KUADRAN1, KUADRAN2, KUADRAN3, KUADRAN4);
//  nh.spinOnce();
}
