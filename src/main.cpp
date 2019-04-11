#include <iostream>
#include <stdio.h>
#include <cstdint>
#include <sstream>
#include <new>
#include "ros/ros.h"
#include "ros-uart-router/crc16.hpp"
#include "ros-uart-router/UART.hpp"
#include "ros-uart-router/BufferOperation.hpp"
#include "ros-uart-router/query.h"

bool checkData(BufferOperation*, uint8_t);
bool request(ros-uart-router){

}

int main(int argc, char **argv){
	ros::init(argc, argv, "ros-uart-router");
	ros::NodeHandle main_node;
	ros::ServiceServer router = main_node.advertiseService("ros-uart-router", request);
	ROS_INFO("ros-uart-router is running..");
	ros::spin();
	return 0;
}

bool checkData(BufferOperation& buff, uint8_t keyword){
	if(buff.data[0] != 0xAA || buff.data[1] != keyword)return false;
	else return true;
}
