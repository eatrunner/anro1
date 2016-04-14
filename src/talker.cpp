#include <ros/ros.h>
#include "anro1/turn.h"
#include "anro1/turnsVector.h"
#include "anro1/light.h"
#include "anro1/lightsVector.h"

anro1::turn getTurnMsg(bool ns, bool we, double x, double y){
    anro1::turn msgTurn;
    msgTurn.NS = ns;
    msgTurn.WE = we;
    msgTurn.x = x;
    msgTurn.y = y;
    return msgTurn;
}

anro1::light getLightMsg(bool ns, bool we, double x, double y){
    anro1::light msgLight;
    msgLight.NS = ns;
    msgLight.WE = we;
    msgLight.x = x;
    msgLight.y = y;
    return msgLight;
}


int main(int argc, char** argv){
    ros::init(argc,argv,"talker");
    ros::NodeHandle n;
    ros::Publisher turnPub = n.advertise<anro1::turnsVector>("turns_info", 10);
    ros::Publisher lightPub = n.advertise<anro1::lightsVector>("lights_info", 100);
    ros::Rate rate(10);
    anro1::turnsVector turnsMsgVector;
    anro1::lightsVector lightsMsgVector;
    turnsMsgVector.turns.push_back(getTurnMsg(false, false, 0.0d, 1.0d));
    turnsMsgVector.turns.push_back(getTurnMsg(true, false, 0.0d, 0.0d));
    turnsMsgVector.turns.push_back(getTurnMsg(true, true, 1.0d, 0.0d));
    turnsMsgVector.turns.push_back(getTurnMsg(false, true, 1.0d, 1.0d));
    lightsMsgVector.lights.push_back(getLightMsg(true,false,0.5d,0.0d));
    ROS_INFO_STREAM(turnsMsgVector.turns[0].x << "   " << turnsMsgVector.turns[0].y);

    while(ros::ok){
        turnPub.publish(turnsMsgVector);
        lightPub.publish(lightsMsgVector);
        ros::spinOnce();
        rate.sleep();
    }

    ROS_INFO("KONIEC");


    return 0;
}
