#include <ros/ros.h>
#include "anro1/turn.h"
#include "anro1/turnsVector.h"

anro1::turn setMsg(bool ns, bool we, double x, double y){
    anro1::turn msgTurn;
    msgTurn.NS = ns;
    msgTurn.WE = we;
    msgTurn.x = x;
    msgTurn.y = y;
    return msgTurn;
}


int main(int argc, char** argv){
    ros::init(argc, argv, "talker");
    ros::NodeHandle n;
    ros::Publisher pub = n.advertise<anro1::turnsVector>("turns_info", 10);
    ros::Rate rate(10);
    anro1::turnsVector msgVector;
    msgVector.turns.push_back(setMsg(false, false, 0.0d, 1.0d));
    msgVector.turns.push_back(setMsg(true, false, 0.0d, 0.0d));
    msgVector.turns.push_back(setMsg(true, true, 1.0d, 0.0d));
    msgVector.turns.push_back(setMsg(false, true, 1.0d, 1.0d));
    ROS_INFO_STREAM(msgVector.turns[0].x << "   " << msgVector.turns[0].y);

    while(ros::ok){
        pub.publish(msgVector);
        ros::spinOnce();
        rate.sleep();
    }

    ROS_INFO("KONIEC");

    return 0;
}
