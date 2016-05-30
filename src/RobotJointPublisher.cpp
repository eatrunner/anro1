#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sensor_msgs/JointState.h>
#include <tf/transform_broadcaster.h>
#include <stdio.h>
#include "RobotJointPublisher.h"
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


ros::Publisher RobotJointPublisher::getJointPub(){
  return joint_pub;
}
void RobotJointPublisher::setJointPub(ros::Publisher& pub){
  joint_pub = pub;
}
char RobotJointPublisher::getNs()
{
  return ns;
}
void RobotJointPublisher::setNs(char cns)
{
  ns =cns;
}

//tf::TransformBroadcaster Model::getTfBroadcaster(){
//  return tfbroadcaster;
//}
//void Model::setTfBroadcaster(tf::TransformBroadcaster& broad){
//  tfbroadcaster = broad;
//}


RobotJointPublisher::RobotJointPublisher(){

}

RobotJointPublisher::~RobotJointPublisher(){

}


RobotJointPublisher rjp;

int main(int argc, char **argv)
{
  ros::init(argc, argv, "rjp");
  std::string ns(ros::this_node::getNamespace());
  if(ns.empty())
      return 0;
  char c = ns.at(1);
  rjp.setNs(c);
  ROS_INFO("char at: %c",c);
  ros::NodeHandle n;
  std::string s(1, c);
  s = ns;
  ros::Publisher joint_pub = n.advertise<sensor_msgs::JointState>("joint_states", 10);
  //ros::Publisher joint_pub2 = n.advertise<sensor_msgs::JointState>(std::string(1, rjp.getNs()), 100);
  ros::Publisher joint_pub2 = n.advertise<sensor_msgs::JointState>("/"+s+s+s+"ocosoch", 100);
  ros::Subscriber cars_subscriber= n.subscribe("/car_info", 10, visualizeCar);
  rjp.setJointPub(joint_pub);

  ros::Rate loop_rate(RATE*1000);
  while(ros::ok())
  {
    ros::spinOnce();

    loop_rate.sleep();
  }

  return 0;
}

void visualizeCar(const anro1::car& msg)
{
 if(msg.id!=ros::this_node::getNamespace()[3])
   return;
  std::string name = std::string(1, msg.id)+"/bumblebee_";

  geometry_msgs::TransformStamped odom_trans;
  sensor_msgs::JointState joint_state;
  odom_trans.header.frame_id = "/my_frame";
  odom_trans.child_frame_id =  name+ "car_body";


  joint_state.header.stamp = ros::Time::now();
  joint_state.name.resize(4);
  joint_state.position.resize(4);
  joint_state.name[0] ="bumblebee_left_front_wheel_joint";
  joint_state.position[0] = wheel;
  joint_state.name[1] ="bumblebee_right_front_wheel_joint";
  joint_state.position[1] = wheel;
  joint_state.name[2] ="bumblebee_left_rear_wheel_joint";
  joint_state.position[2] = wheel;
  joint_state.name[3] ="bumblebee_right_rear_wheel_joint";
  joint_state.position[3] = wheel;

  odom_trans.header.stamp = ros::Time::now();
  odom_trans.transform.translation.x = msg.x;
  odom_trans.transform.translation.y = msg.y;
  odom_trans.transform.translation.z = 0.4;
  odom_trans.transform.rotation = tf::createQuaternionMsgFromRollPitchYaw (0,0,msg.orientation);
  if(msg.moving)
  wheel += degree;
  //send the joint state and transform
  static tf::TransformBroadcaster br;
  rjp.getJointPub().publish(joint_state);

  br.sendTransform(odom_trans);
}

