#include "ros/ros.h"
#include "std_msgs/String.h"
#include "anro1/light.h"
#include "anro1/lightsVector.h"
#include "anro1/mapMessage.h"
#include <sstream>
#include <list>
using namespace std;

class CrossRoad{
public:
	CrossRoad(double ix, double iy) :
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
		if (time>timeToChange){
			northgreen = !northgreen;
			time = 0;
		}
	}
};

class CrossRoads
{
public:
	void reset(){
		CrossRoad* c;
		for (list<CrossRoad*>::iterator it = crossRoads.begin(); it != crossRoads.end(); it++){
			delete *it;

		}

	}


	void tick(){
		for (list<CrossRoad*>::iterator it = crossRoads.begin(); it != crossRoads.end(); it++){
			(*it)->incrementTime();
		}
	}
	void addCrossRoad(double x, double y){
		CrossRoad* ptr = new CrossRoad(x, y);
		crossRoads.push_back(ptr);
	}

	list<CrossRoad*> giveList()
	{
		return crossRoads;
	}

private:
	list<CrossRoad*> crossRoads;

};



bool ready = false;
CrossRoads crossRoads;
void process(const anro1::mapMessage::ConstPtr& msg){
	if (msg->type == "fourLanes"){
		crossRoads.addCrossRoad(msg->x, msg->y);
		ready = true;
	}
}
int main(int argc, char **argv)
{
	
	ros::init(argc, argv, "Lights");



	ros::NodeHandle n;
	ros::Publisher chatter_pub = n.advertise<anro1::lightsVector>("lights_info", 10);
	ros::Subscriber sub = n.subscribe("map_info", 10, process);
	ros::Rate loop_rate(10);
	anro1::light *lightmsg;
	anro1::lightsVector* lightVector;



	while (ros::ok())
	{
		ros::spinOnce();
		if (!ready)
			continue;

		lightVector = new anro1::lightsVector;
		lightVector->size = 0;
		for (list<CrossRoad*>::iterator it = crossRoads.giveList().begin(); it != crossRoads.giveList().end(); it++)
		{
			lightmsg = new anro1::light;
			lightmsg->x = (*it)->x;
			lightmsg->y = (*it)->y;
			lightmsg->NS = (*it)->northgreen;
			lightmsg->WE = !(*it)->northgreen;
			lightVector->lights.push_back(*lightmsg);
			lightVector->size++;
		}






		chatter_pub.publish(*lightVector);

		delete &(lightVector->lights);



		crossRoads.tick();
		loop_rate.sleep();
	}


	return 0;
}