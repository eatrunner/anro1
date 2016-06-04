#ifndef MODEL_H
#define MODEL_H

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

class Model{
public:
    Model();
    ~Model();
    ros::Publisher getPub();
    void setPub(ros::Publisher& pub);
   ros::Publisher getJointPub();
    void setJointPub(ros::Publisher& pub);
 //  tf::TransformBroadcaster getTfBroadcaster();
  // void setTfBroadcaster(tf::TransformBroadcaster& broad);

private:

    ros::Publisher rviz_publisher;
    ros::Publisher joint_pub;
   // tf::TransformBroadcaster tfbroadcaster;


};

#endif

