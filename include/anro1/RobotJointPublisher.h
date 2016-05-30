#ifndef ROBOTJOINTPUBLISHER_H
#define ROBOTJOINTPUBLISHER_H

#include <ros/ros.h>
#include "std_msgs/String.h"
#include <stdio.h>
#include <cstdlib>
#include <visualization_msgs/Marker.h>
#include <sensor_msgs/JointState.h>
#include <tf/transform_broadcaster.h>
#include "anro1/car.h"
#include "anro1/light.h"
#include "anro1/lightsVector.h"
#include "anro1/turn.h"
#include "anro1/turnsVector.h"

class RobotJointPublisher{
public:
    RobotJointPublisher();
    ~RobotJointPublisher();

    void visualizeCar(const anro1::car& msg);
    ros::Publisher getPub();
    void setPub(ros::Publisher& pub);
   ros::Publisher getJointPub();
    void setJointPub(ros::Publisher& pub);
     char getNs();
     void setNs(char ns);
private:
    ros::Subscriber cars_subscriber;
    ros::Publisher joint_pub;
    char ns;
};

#endif

