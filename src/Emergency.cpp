#include "ros/ros.h"
#include "anro1/EmergencyService.h"
#include "anro1/EmergencyMessage.h"
#include <cstdlib>
#include <string.h>

int main(int argc, char **argv)
{
	ros::init (argc,argv,"EmergencyStop");
	if(argc==2)
	{
		ros::NodeHandle n;
		ros::Publisher publisher = n.advertise<anro1::EmergencyMessage>("emergency_msg", 1000);
		anro1::EmergencyMessage msg;
		if(strcmp(argv[1],"start")==0)
		{
			msg.move=true;
			ROS_INFO("Sending start message to every car.");
		}
		else if(strcmp(argv[1],"stop")==0)
		{
			msg.move=false;
			ROS_INFO("Sending stop message to every car.");
		}
		else
		{
			ROS_INFO("Usage: 'Emergency stop/start' or 'EmergencyStop stop/start car_name'");
			return 1;
		}
		ros::Rate rate(10);
		while(ros::ok())
		{
			publisher.publish(msg);
			ros::spinOnce();
			rate.sleep();
		}
	}
	else if (argc==3)
	{
		ros::NodeHandle n;
		ros::ServiceClient client=n.serviceClient<anro1::EmergencyService>(argv[2]);
		anro1::EmergencyService srv;
		if(strcmp(argv[1],"start")==0)srv.request.move=true;
		else if(strcmp(argv[1],"stop")==0)srv.request.move=false;
		else
		{
			ROS_INFO("Usage: 'Emergency stop/start' or 'EmergencyStop stop/start car_name'");
			return 1;
		}
		char id=*argv[2];
		srv.request.id=(int)id;
		if(client.call(srv))
		{
			if(srv.response.failure&&!srv.request.move) ROS_INFO("Car number %ld is already standing!!",(long int)srv.response.resid);
			else if(!srv.response.failure&&!srv.request.move)ROS_INFO("Car number %ld stops.",(long int)srv.response.resid);
			else if(srv.response.failure&&srv.request.move) ROS_INFO("Car number %ld is already moving!!",(long int)srv.response.resid);
			else if(!srv.response.failure&&srv.request.move)ROS_INFO("Car number %ld starts moving.",(long int)srv.response.resid);
		}	
		else
		{
			ROS_ERROR("Failed to call service EmergencyStop");
			ROS_INFO("Usage: 'Emergency stop/start' or 'EmergencyStop stop/start car_name'");
			return 1;
		}
	}
	else
	{
		ROS_INFO("Usage: 'Emergency stop/start' or 'EmergencyStop stop/start car_name'");
		return 1;
	}
	return 0;
}
