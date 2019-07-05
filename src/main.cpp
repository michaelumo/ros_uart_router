/*! @file ros_uart_router/main.cpp
 *  @version 0.9.0
 *
 *  @Copyright (c) 2019 spikeumo@gmail.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include <iostream>
#include <stdio.h>
#include <cstdint>
#include <sstream>
#include <vector>
#include <algorithm>
#include "ros/ros.h"
#include "ros_uart_router/crc16.hpp"
#include "ros_uart_router/UART.hpp"
#include "ros_uart_router/BufferOperation.hpp"
#include "ros_uart_router/query.h"

std::vector<std::string> openedDevices;

bool request(ros_uart_router::query::Request &req, ros_uart_router::query::Response &res)
{
	UART device(req.device.c_str(), req.baudrate);
	//TODO:if(!(std::find(openedDevices.begin(), openedDevices.end(), req.device.c_str()) == openedDevices.end())){
		openedDevices.push_back(req.device.c_str());

		if (!device.setup()){
			ROS_INFO("%s",req.device.c_str());
			ROS_ERROR("Error - Unable to open UART.  Ensure it is not in use by another application");
			return false;
		}
	//}
	BufferOperation buff;
	if(!req.listen_only){
		buff.data.insert(buff.data.begin(), std::begin(req.payload), std::end(req.payload));
		//buff.data.insert(buff.data.begin(), std::begin(req.head), std::end(req.head));
		//if(req.payload.size() && req.crc)buff.data.push_back(crc16(req.payload.data(), req.payload.size()));
		device.writeBuffer(buff.data.data(), buff.data.size());
	}
	buff.clearBuffer();
	//-------------------------------------------------
	device.readBuffer(buff, req.time_out);
	if(!buff.data.size()){
		if(!req.listen_only)ROS_ERROR("Error - Reading Time Out");
		return false;
	}
	res.data = buff.data;
	if(buff.data.size() && req.crc){
		ROS_ERROR("Generated cksm: %d",buff.generateChecksum(3, 2));
		ROS_ERROR("Received  cksm: %d",buff.getChecksum());
		for(int i = 0; i < buff.data.size(); i++){
			std::cout<<"0x"<<std::hex<<(int)buff.data[i]<<std::endl;
		}
		if(buff.getChecksum() == buff.generateChecksum(3, 2)){
			res.ack = true;
		}
	}else if(buff.data.size() && req.crc){
		res.ack = false;
	}
	//device.closeDevice();
	/*TODO:if(req.close_device){
		device.closeDevice();
		openedDevices.erase(std::remove(openedDevices.begin(), openedDevices.end(), req.device.c_str()), openedDevices.end());
	}*/
	return true;
}

int main(int argc, char **argv){
	ros::init(argc, argv, "ros_uart_router");
	ros::NodeHandle main_node;
	ros::ServiceServer router = main_node.advertiseService("ros_uart_router", request);
	ROS_INFO("ros_uart_router is running..");
	ros::spin();
	return 0;
}
