/*
 * rosserial PubSub Example
 * Prints "hello world!" and toggles led
 */
 
#include <TimerOne.h>
#include <ros.h>
#include <std_msgs/Char.h>
#include <std_msgs/String.h>
#include <SoftwareSerial.h>


SoftwareSerial mySerial(10, 11); // RX, TX

ros::NodeHandle  nh1;


char conData, gameState;

void control( const std_msgs::Char& control){
  conData = control.data;
}

void gamestate( const std_msgs::String& gamestate){
  gameState = gamestate.data;
}

ros::Subscriber<std_msgs::Char> control_subscriber("control", control );
ros::Subscriber<std_msgs::String> gamestate_subscriber("gamestate", gamestate );

void interruptHandler()
{
  mySerial.write("t");
  mySerial.write(conData);
  
  mySerial.write("g");
  mySerial.write(gameState);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  nh1.subscribe(control_subscriber);
//  nh1.subscribe(gamestate_subscriber);
  mySerial.begin(9600);
//  Timer1.initialize(50000);
//  Timer1.attachInterrupt(interruptHandler);

}

void loop() {
  // put your main code here, to run repeatedly:
  nh1.spinOnce();
}
