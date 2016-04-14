/*
 * Copyright (c) 2010, Willow Garage, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Willow Garage, Inc. nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
/*
 * Prosty program szescianu jadacego po kwadracie, opisy poszczegolnych funkcji sa szczegolowo
 * opisane w tutorialu. Aby odpalic program, nalezy w jednej konsoli odpalic ros (polecenie
 * roscore) w nastepnej rviz (rosrun rviz rviz) oraz ten program (rosrun anro1 test_olek)
 * (trzeba wczesniej sie scourcowac do swojego workspace)
 * W rviz ustawiamy fixed frame na /my_frame, a marker topic na visuzalization_marker
 *
 */
#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include "std_msgs/String.h"
#include "anro1/carMessage.h"
#include "anro1/lightsMessage.h"
#include "anro1/turnsMessage.h"

void initMarker(visualization_msgs::Marker& marker){
  marker.type = visualization_msgs::Marker::CUBE;

  marker.pose.position.x = 0;
  marker.pose.position.y = 0;
  marker.pose.position.z = 0;
  marker.scale.x = 0.7;
  marker.scale.y = 0.7;
  marker.scale.z = 0.7;
  marker.color.r = 0.0f;
  marker.color.g = 1.0f;
  marker.color.b = 0.0f;
  marker.color.a = 1.0f;

  marker.header.frame_id = "/my_frame";
  marker.ns = "marker_test";
}

/**
 * Subskrybenci swiatel i zakretow
 */
void lights_infoCallback(const anro1::lightsMessage::ConstPtr& msg)
{
  ROS_INFO("I heard color: [%d]", (int)(msg->color));
}

void turns_infoCallback(const anro1::turnsMessage::ConstPtr& msg)
{
  ROS_INFO("I heard direction: [%d]", (int)(msg->direction));
}

void moveMarker(visualization_msgs::Marker& marker, int& state, float move){
  if(state == 0){
    if((float)marker.pose.position.x == 1.0f)
       state = 1;
    else
       marker.pose.position.x += move;
  }
  else if(state == 1){
    if((float)marker.pose.position.y == 1.0f)
      state = 2;
    else
      marker.pose.position.y += move;
  }
  else if(state == 2){
    if((float)marker.pose.position.x == 0.0f)
      state = 3;
    else
      marker.pose.position.x -= move;
  }
  else if(state == 3){
    if((float)marker.pose.position.y == 0.0f)
      state = 0;
    else
      marker.pose.position.y -= move;
  }

  //ROS_WARN_STREAM("Marker ID : " << marker.id << " x = " << (float)marker.pose.position.x
                //  << " y = " << (float) marker.pose.position.y);
}

int main( int argc, char** argv )
{
  ros::init(argc, argv, "basic_shapes");
  ros::NodeHandle n;
  ros::Rate r(30);
  /*Publishers*/
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
  ros::Publisher car_info_pub = n.advertise<anro1::carMessage>("car_info", 1000);
  /*Subscribers*/	
  ros::Subscriber lights_info_sub = n.subscribe("lights_info", 1000, lights_infoCallback);
  ros::Subscriber turns_info_sub = n.subscribe("turns_info", 1000, turns_infoCallback);
  visualization_msgs::Marker m1;

  int state_m1 = 0;
  float move_m1 = 0.01;
  
  m1.id = 1;
  initMarker(m1);


  while (ros::ok())
  {
    moveMarker(m1,state_m1,move_m1);
    marker_pub.publish(m1);
    
    anro1::carMessage car_info;
    car_info.id = m1.id;
    car_info.x = m1.pose.position.x;
    car_info.y = m1.pose.position.y;
    car_info_pub.publish(car_info);
    
    r.sleep();
  }
}
