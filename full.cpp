#include "ros/ros.h"
#include "std_msgs/String.h"
#include "anro1/turn.h"
#include "anro1/turnsVector.h"
#include "anro1/mapMessage.h"
#include <sstream>
#include <list>

using namespace std;
bool ready = false;

class Turn{
public:
	bool north;
	bool west;
	double x;
	double y;
	Turn(double ix, double iy, string type){
		x = ix;
		y = iy;
		north = transform(type)[0];
		west = transform(type)[1];
	}
	bool* transform(string type)
	{
		bool result[2];
		if (type == "turningUpLeft")
		{
			result[0] = true; result[1] = true;
		}
		if (type == "turningUpRight")
		{
			result[0] = true; result[1] = false;
		}
		if (type == "turningDownLeft")
		{
			result[0] = false; result[1] = true;
		}
		if (type == "turningDownRight")
		{
			result[0] = false; result[1] = false;
		}
		return result;
	}
};


class Turns{
public:
	void reset(){
		Turn* c;
		for (list<Turn*>::iterator it = turns.begin(); it != turns.end(); it++){
			delete *it;

		}
	}

	void addTurn(int x, int y, string type){
		Turn* ptr = new Turn(x, y, type);
		turns.push_back(ptr);
	}

	list<Turn*> giveList()
	{
		return turns;
	}

private:
	list<Turn*> turns;

};

Turns turns;
void process(const anro1::mapMessage::ConstPtr& msg){
	if (msg->type != "fourLanes"){
		turns.addTurn(msg->x, msg->y, msg->type);
		ready = true;
	}
}

int main(int argc, char **argv)
{

	ros::init(argc, argv, "Lights");
	ros::NodeHandle n;
	ros::Publisher chatter_pub = n.advertise<anro1::turnsVector>("turns_info", 10);
	ros::Subscriber sub = n.subscribe("map_info", 10, process);
	ros::Rate loop_rate(10);
	anro1::turn *turnmsg;
	anro1::turnsVector* turnVector;

	while (ros::ok())
	{
		ros::spinOnce();
		if (!ready)
			continue;

		turnVector = new anro1::turnsVector;
		turnVector->size = 0;
		for (list<Turn*>::iterator it = turns.giveList().begin(); it != turns.giveList().end(); it++)
		{
			turnmsg = new anro1::turn;
			turnmsg->x = (*it)->x;
			turnmsg->y = (*it)->y;
			turnmsg->NS = (*it)->north;
			turnmsg->WE = (*it)->west;
			turnVector->turns.push_back(*turnmsg);
			turnVector->size++;
		}

		chatter_pub.publish(*turnVector);
		delete &(turnVector->turns);
		loop_rate.sleep();
	}
	return 0;
}