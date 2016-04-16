#include "ros/ros.h"
#include "std_msgs/String.h"
#include <stdio.h>
#include "Model.h"
#include <cstdlib>
#include <visualization_msgs/Marker.h>
#include "anro1/car.h"
#include <sstream>
void visualizeCar(const anro1::car& msg);

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
    ros::init(argc, argv, "model");
    ros::NodeHandle n;
    ros::Publisher rviz_publisher = n.advertise<visualization_msgs::Marker>("visualization_marker", 100);
    ros::Subscriber cars_subscriber= n.subscribe("car_info", 1000, visualizeCar);

    modelrviz.setPub(rviz_publisher);
    ros::Rate loop_rate(10);

    ros::spin();


    return 0;
}

void visualizeCar(const anro1::car& msg)
{   ROS_INFO("Rendering car, id: [%d]", msg.id);

    visualization_msgs::Marker marker1;
    uint32_t shape = visualization_msgs::Marker::CUBE;
    marker1.header.frame_id = "/my_frame";
    marker1.header.stamp = ros::Time::now();
    marker1.type = shape;
    marker1.ns = "Model";
    marker1.id = msg.id;
    marker1.action = visualization_msgs::Marker::ADD;
    marker1.pose.position.x = msg.x;
    marker1.pose.position.y = msg.y;
    marker1.pose.position.z = 0;
    marker1.pose.orientation.x = 0.0;
    marker1.pose.orientation.y = 0.0;
    marker1.pose.orientation.z = 0.0;
    marker1.pose.orientation.w = 1.0;
    marker1.scale.x = 30*msg.scale;
    marker1.scale.y = 30*msg.scale;
    marker1.scale.z = 30*msg.scale;
    marker1.color.r = 1.0f;
    marker1.color.g = 0.0f;
    marker1.color.b = 0.0f;
    marker1.color.a = 1.0;
    marker1.lifetime = ros::Duration();

    modelrviz.getPub().publish(marker1);
}
