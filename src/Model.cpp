#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sensor_msgs/JointState.h>
#include <tf/transform_broadcaster.h>
#include <stdio.h>
#include "Model.h"
#include <cstdlib>
#include <string>
#include <visualization_msgs/Marker.h>
#include "anro1/car.h"
#include "anro1/light.h"
#include "anro1/lightsVector.h"
#include "anro1/lightsmsg.h"
#include "anro1/point.h"
#include "Constants.h"
#include <sstream>
void visualizeCar(const anro1::car& msg);
void visualizeLights(const anro1::lightsmsg& msg);

const double degree = M_PI/180;
double wheel = degree;


ros::Publisher Model::getJointPub(){
  return joint_pub;
}
void Model::setJointPub(ros::Publisher& pub){
  joint_pub = pub;
}

//tf::TransformBroadcaster Model::getTfBroadcaster(){
//  return tfbroadcaster;
//}
//void Model::setTfBroadcaster(tf::TransformBroadcaster& broad){
//  tfbroadcaster = broad;
//}


ros::Publisher Model::getPub(){
  return rviz_publisher;
}
void Model::setPub(ros::Publisher& pub){
  rviz_publisher = pub;
}

Model::Model(){

}

Model::~Model(){

}

Model modelrviz;

int main(int argc, char **argv)
{
  ROS_INFO("model starting...");
  ros::init(argc, argv, "model");
  ros::NodeHandle n;
  ros::Publisher rviz_publisher = n.advertise<visualization_msgs::Marker>("visualization_marker", 100);
  ros::Publisher joint_pub = n.advertise<sensor_msgs::JointState>("b/joint_states", 100);
  //ros::Publisher joint_pub2 = n.advertise<sensor_msgs::JointState>("robot2/joint_states", 100);
  ros::Subscriber lights_subscriber= n.subscribe("lights_info", 100, visualizeLights);

  modelrviz.setPub(rviz_publisher);
  modelrviz.setJointPub(joint_pub);


  ros::Rate loop_rate(RATE*1000);
  while(ros::ok())
  {
    ros::spinOnce();

    loop_rate.sleep();
  }

  return 0;
}


void visualizeLights(const anro1::lightsmsg& msg)
{
    int i=0;
    double scale = LANE_WIDTH/2;
    for(i=0;i<msg.lights.size();i++)
    {
        anro1::light light = msg.lights[i];
        ROS_INFO("Rendering lights:[%d]",light.green);

            visualization_msgs::Marker marker1;
            uint32_t shape = visualization_msgs::Marker::SPHERE;
            marker1.header.frame_id = "/my_frame";
            marker1.header.stamp = ros::Time::now();
            marker1.type = shape;
            marker1.ns = "Model1";
            marker1.id = i;
            marker1.action = visualization_msgs::Marker::ADD;
            marker1.pose.position.x = light.cords.x;
            marker1.pose.position.y = light.cords.y;

            marker1.pose.position.z = LANE_WIDTH;
            marker1.pose.orientation.x = 0.0;
            marker1.pose.orientation.y = 0.0;
            marker1.pose.orientation.z = 0.0;
            marker1.pose.orientation.w = 1.0;
            marker1.scale.x = scale;
            marker1.scale.y = scale;
            marker1.scale.z = scale;
            marker1.color.r = 0.0f;
            marker1.color.g = 0.0f;
            marker1.color.b = 0.0f;
            if(light.green)
                    marker1.color.g = 1.0f;
            else
                    marker1.color.r = 1.0f;
            marker1.color.a = 1.0;
            marker1.lifetime = ros::Duration();

            modelrviz.getPub().publish(marker1);

    }
}
