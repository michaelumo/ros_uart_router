#include <iostream>
#include <stdio.h>
#include <cstdint>
#include <sstream>
#include <new>
#include "ros/ros.h"
#include "ros_uart_router/crc16.hpp"
#include "ros_uart_router/UART.hpp"
#include "ros_uart_router/BufferOperation.hpp"
#include "ros_uart_router/query.h"

bool checkData(BufferOperation*, uint8_t);

bool request(ros_uart_router::query::Request &req,
							ros_uart_router::query::Respond &res)
{
	UART device(res.port, res.baudrate);
	device.setup();
	BufferOperation buff;
	std::vector<uint8_t> data;
	data.insert(data.begin());
	auto it = data.begin();

	buff.data = req.payload;
	buff.datalen = sizeof_payload;
	device.writeBuffer();
}

int main(int argc, char **argv){
	ros::init(argc, argv, "ros_uart_router");
	ros::NodeHandle main_node;
	ros::ServiceServer router = main_node.advertiseService("ros_uart_router", request);
	ROS_INFO("ros_uart_router is running..");
	ros::spin();
	return 0;
}

bool checkData(BufferOperation& buff, uint8_t keyword){
	if(buff.data[0] != 0xAA || buff.data[1] != keyword)return false;
	else return true;
}
