 /*************************************************************************
 * Author: Abhinav Jain
 * Contact: abhinavjain241@gmail.com, abhinav.jain@heig-vd.ch
 * Date: 28/06/2016
 *
 * This file contains source code to the client node of the ROS package
 * comm_tcp developed at LaRA (Laboratory of Robotics and Automation)
 * as part of my project during an internship from May 2016 - July 2016.
 *
 * (C) All rights reserved. LaRA, HEIG-VD, 2016 (http://lara.populus.ch/)
 ***************************************************************************/
#include <ros/ros.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include "std_msgs/String.h"
#include "std_msgs/Char.h"

#define MESSAGE_FREQ 20

void error(const char *msg) {
    perror(msg);
    exit(0);
}

class Listener {
// private:
//     char topic_message[256] = { 0 };
public:
    void callback(const std_msgs::String::ConstPtr& msg);
    char* getMessageValue();
};

//void Listener::callback(const std_msgs::Char::ConstPtr& msg) {
//    memset(topic_message, 0, 256);
//    strcpy(topic_message, msg->data.c_str());
//    ROS_INFO("I heard:[%s]", msg->data.c_str());
//}

// char* Listener::getMessageValue() {
//     return topic_message;
// }

int main(int argc, char *argv[]) {
	ros::init(argc, argv, "client_node");
	ros::NodeHandle nh;

	ros::Publisher tcppub = nh.advertise<std_msgs::String>("tcpcom", 1000);

    ros::Rate loop_rate(MESSAGE_FREQ); // set the rate as defined in the macro MESSAGE_FREQ
//	Listener listener;
//      ros::Subscriber client_sub = nh.subscribe("/client_messages", 1, &Listener::callback, &listener);

	std_msgs::String gamestate;

    int sockfd, portno, n, choice = 1;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[2];
    char data = 0;

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
    {
        error("ERROR opening socket");
    }

    

    server = gethostbyname(argv[1]);
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

		    n = read(sockfd,buffer,2);
		    printf("masuk: %s\n", buffer);

    int flags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

	while(ros::ok()) 
    {
		bzero(buffer,2);
	    n = read(sockfd, buffer, 1);
	    printf("masuk: %c\n", data);
	    
        if(n == 1)
        {
            data = buffer[0];
        }
        gamestate.data = data;
		tcppub.publish(gamestate);

	    ros::spinOnce();
	    loop_rate.sleep();
	}
	return 0;
}
