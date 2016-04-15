#include "ros/ros.h"
#include "std_msgs/String.h"
#include "anro1/light.h"
#include "anro1/lightsVector.h"
#include "anro1/mapMessage.h"
#include <sstream>
#include <list>
using namespace std;


class Crossroad{
public:
    Crossroad(double ix, double iy) :
        north(true),
        west(true),
        south(true),
        east(true),
        timeToChange(1000),
        northgreen(true),
        x(ix),
        y(iy),
        time(0){
        }

        bool north, west, south, east;
        bool northgreen;
        int time;
        int timeToChange;
        double x, y;
        void incrementTime(){
        time++;
        if(time>timeToChange){
            northgreen=!northgreen;
            time=0;
        }
    }
};


class Crossroads{
public:
    Crossroads(){
        crossroads = new list<Crossroad>;
    }
    void reset(){
        while (!crossroads->empty()){
            crossroads->pop_front();
        }
    }

    void addCrossroad(int x, int y, string type){
        Crossroad crossroad(x, y);
        crossroads->push_front(crossroad);
    }
    void tick(){
        for (list<Crossroad>::iterator it = giveList()->begin(); it != giveList()->end(); it++){
            (*it).incrementTime();
        }
    }

    list<Crossroad>* giveList()
    {
        return crossroads;
    }

private:
    list<Crossroad>* crossroads;

};

bool ready = false;


Crossroads crossroads;
void process(const anro1::mapMessage::ConstPtr& msg){
    ROS_INFO("Ja slyszu");
    if (msg->type == "fourLanes"){
        crossroads.addCrossroad(msg->x, msg->y, msg->type);
        ready = true;
    }
}
int main(int argc, char **argv)
{

    ros::init(argc, argv, "Lights");



    ros::NodeHandle n;
    ros::Publisher chatter_pub = n.advertise<anro1::lightsVector>("lights_info", 10);
    ros::Subscriber sub = n.subscribe("map_info", 20, process);
    ros::Rate loop_rate(10);


    while (ros::ok())
    {
        ros::spinOnce();
        if(!ready){
            continue;
        }
        anro1::lightsVector lightVector;
       lightVector.size = 0;

        for (list<Crossroad>::iterator it = crossroads.giveList()->begin(); it != crossroads.giveList()->end(); it++)
        {
            anro1::light lightmsg;
            lightmsg.x = (*it).x;
            lightmsg.y = (*it).y;
            lightmsg.NS = (*it).northgreen;
            lightmsg.WE = !(*it).northgreen;
            lightVector.lights.push_back(lightmsg);
            lightVector.size++;
        }

        chatter_pub.publish(lightVector);


        loop_rate.sleep();

    }

    return 0;
}
