#include "ros/ros.h"
#include "std_msgs/String.h"
#include "anro1/light.h"
#include "anro1/lightsVector.h"
#include "anro1/mapMessage.h"
#include <stdlib.h>
#include <ctime>
#include <sstream>
#include <list>
using namespace std;



//reprezentacja pojedynczego skrzyzowania
class Crossroad{
public:
    Crossroad(double ix, double iy, string s) :
        north(true),
        west(true),
        south(true),
        east(true),
        northGreen(true),
        x(ix),
        y(iy),
        bothRed(false),
        westGreen(false),
        greenTime(5),
        nextNorth(false),
        redTime(0),
        time(0){
          timeToChange=rand()%4+greenTime;//czas w sekundach miedzy zmiana swiatel
         /* for(int i=1;i<s.size();i++){
              if(s[i]=='U')
                  north=true;
              if(s[i]=='L')
                  west=true;
              if(s[i]=='D')
                  south=true;
              if(s[i]=='P')
                  east=true;
          }*/
    }

    bool north, west, south, east;//drogi wylotowe z skrzyzowania
    bool northGreen;
    bool westGreen;
    bool bothRed;
    bool nextNorth;
    int time;
    int timeToChange;
    int greenTime;
    int redTime;
    double x, y;
    void incrementTime(){
        time++;
        if(time>timeToChange){
            time=0;
            if(bothRed){
               if(nextNorth){
                   northGreen=true;
                   westGreen=false;
                   nextNorth=false;
               }
               else{
                   northGreen=false;
                   westGreen=true;
                   nextNorth=true;
               }
              timeToChange=greenTime;
               bothRed=false;
            }
            else{
                northGreen=false;
                westGreen=false;
                bothRed=true;
                timeToChange=redTime;

            }

        }
    }
};

//kontener skrzyzowan
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
        Crossroad crossroad(x, y, type);
        crossroads->push_front(crossroad);
    }
    void tick(){//inkrementacja czasu na skrzyzowaniach
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
    srand(time(NULL));
    ros::NodeHandle n;
    ros::Publisher chatter_pub = n.advertise<anro1::lightsVector>("lights_info", 10);//utworzenie kanalu do nadawania
    ros::Subscriber sub = n.subscribe("map_info", 20, process);//subskrypcja kanalu z informacjami
    ros::Rate loop_rate(10);
    time_t lasttick, thistick;
    time(&lasttick);
    while (ros::ok())
    {
        ros::spinOnce();
        if(!ready){
            continue;
        }
        time(&thistick);
        if (lasttick + 1 < thistick){//uplynela conajmniej sekunda od ostatniej inkrementacji
                    lasttick=lasttick+1;
                    crossroads.tick();
                    }
        anro1::lightsVector lightVector;
        lightVector.size = 0;

        for (list<Crossroad>::iterator it = crossroads.giveList()->begin(); it != crossroads.giveList()->end(); it++)//dodajemy wiadomosci do wektora
        {
            anro1::light lightmsg;
            lightmsg.x = (*it).x;
            lightmsg.y = (*it).y;
            lightmsg.NS = (*it).northGreen;
            lightmsg.WE = (*it).westGreen;
           /* lightmsg.N = (*it).north;
            lightmsg.E = (*it).east;
            lightmsg.S = (*it).south;
            lightmsg.W = (*it).west;*/

            lightVector.lights.push_back(lightmsg);
            lightVector.size++;
        }

        chatter_pub.publish(lightVector);


        loop_rate.sleep();

    }

    return 0;
}
