#include "ros/ros.h"
#include "std_msgs/String.h"
#include "anro1/turn.h"
#include "anro1/turnsVector.h"
#include "anro1/mapMessage.h"
#include <sstream>
#include <list>
using namespace std;



class Turn{
public:
	bool north;
	bool west;
	double x;
	double y;
	Turn(double ix, double iy, string type){
		x = ix;
		y = iy;
		transform(type);
	}
	void transform(string type)
	{
		if (type == "turningUpLeft")
		{
			north = true; west = true;
		}
		if (type == "turningUpRight")
		{
			north = true; west = false;
		}
		if (type == "turningDownLeft")
		{
			north = false; west = true;
		}
		if (type == "turningDownRight")
		{
			north = false; west = false;
		}
		return result;
	}
};


class Turns{
public:
	Turns(){
		
	}
	void reset(){
		while (!turns->empty()){
			turns->pop_front();
		}
	}

	void addTurn(int x, int y, string type){
		Turn turn(x, y, type);
		turns->push_back(turn);
	}

	list<Turn> giveList()
	{
		return turns;
	}

private:
	list<Turn> turns;

};

bool ready = false;


Turns turns;
void process(const anro1::mapMessage::ConstPtr& msg){
	ROS_INFO("Ja slyszu");
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
	//anro1::turn turnmsg;
	//anro1::turnsVector turnVector;



	while (ros::ok())
	{
		ros::spinOnce();
		
		anro1::turnsVector turnVector;
		turnVector.size = 0;
		for (list<Turn*>::iterator it = turns.giveList().begin(); it != turns.giveList().end(); it++)
		{
			anro1::turn turnmsg;
			turnms.>x = (*it)->x;
			turnmsg.y = (*it)->y;
			turnmsg.NS = (*it)->north;
			turnmsg.WE = (*it)->west;
			turnVector.turns.push_back(turnmsg);
			turnVector.size++;
		}

		chatter_pub.publish(turnVector);

		
		loop_rate.sleep();

	}

	return 0;
}
