/*
 * rosserial PubSub Example
 * Prints "hello world!" and toggles led
 */
 
#include <TimerOne.h>
#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Char.h>
#include <geometry_msgs/Pose2D.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX

ros::NodeHandle  nh;
int coorX;
int coorY;
uint8_t x1, x2;
uint8_t y1, y2; 
char gamestate;

void messageCb( const geometry_msgs::Pose2D& msg){
  coorX = msg.x;
  coorY = msg.y;
}

void messageTCP(const std_msgs::Char& msg){
  gamestate = msg.data;
}

ros::Subscriber<geometry_msgs::Pose2D> sub("msg", messageCb );
ros::Subscriber<std_msgs::Char> sub2("tcpcom", messageTCP );

void interruptHandler()
{
  x1 = coorX/10;
  x2 = coorX%10;
  y1 = coorY/10;
  y2 = coorY%10;

  mySerial.write("q");
  mySerial.write(x1);
  mySerial.write("w");
  mySerial.write(x2);
  mySerial.write("e");
  mySerial.write(y1);
  mySerial.write("r");
  mySerial.write(y2);
  mySerial.write("g");
  mySerial.write(gamestate);
  
}

//std_msgs::String str_msg;
//ros::Publisher chatter("chatter", &str_msg);



void setup()
{
  pinMode(13, OUTPUT);
  nh.initNode();
//  nh.advertise(chatter);
  nh.subscribe(sub);
  mySerial.begin(9600);
  Timer1.initialize(50000);
  Timer1.attachInterrupt(interruptHandler);
}

void loop()
{
//  str_msg.data = hello;
//  chatter.publish( &str_msg );
//  mySerial.print(coorX);
//  kirimKoordinat();
  nh.spinOnce();
  delay(500);
}
