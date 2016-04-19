/*#include "ros/ros.h"
#include "std_msgs/String.h"
#include "anro1/light.h"
#include "anro1/lightsVector.h"
#include "anro1/mapMessage.h"*/
#include <stdlib.h>
#include <vector>
#include <ctime>
#include <sstream>
#include <list>
#include <queue>
using namespace std;

class Out{
public:
	int x, y;
	Out(int ix, int iy){
		x = ix;
		y = iy;
	}
};


class Entry{
public:
	int x, y;
	bool light;
	Entry(int ix, int iy) : 
		light(false)
	{
		x = ix;
		y = iy;
	}
	vector<Out> left,straight,right;
};



class Side{
public:
	vector<Entry> entries;
};

class Point{
public:
	int x, y;
};

class InSide{
public:
	vector<Point> ins, outs;
};

class Crossroad{
public:
	Side sides[4];
	/*Crossroad(InSide insides[4]){//kazdy kolejny po prawej
		for (int i = 0; i < 4; i++){//ins i outs od srodka pasa
			int n = insides[i].ins.size();
			for (int k = 0; k < n; k++){//inicjalizacja wjazdow
				Point p = insides[i].ins[k];
				sides[i].entries.push_back(Entry(p.x, p.y));
			}
		}
		for (int i = 0; i < 4; i++){//make straights
			int k = i + 2;
			k = k % 4;
			int m = insides[k].outs.size();
			int n = sides[i].entries.size();
			for (int l= 0; l < n&&l<m; l++){
				Point p = insides[k].outs[l];
				sides[i].entries[l].straight.push_back(Out(p.x,p.y));
			}
		}

		for (int i = 0; i < 4; i++){//make rights
			int k = i + 1;
			k = k % 4;
			int z = sides[i].entries.size();
			int m = insides[k].outs.size();
			int n = sides[i].entries.size();
			Point p = insides[k].outs[m-1];
			sides[i].entries[z-1].right.push_back(Out(p.x, p.y));
		}

		for (int i = 0; i < 4; i++){//lefts
			int k = i + 3;
			k = k % 4;
			
			int m = insides[k].outs.size();
			int n = sides[i].entries.size();
			for (int j = 0; j < insides[k].outs.size - 1; j++){
				Point p = insides[k].outs[j];
				sides[i].entries[0].left.push_back(Out(p.x, p.y));
			}
		}
		*/
	Crossroad(InSide insides[4]) : 
		nsTogether(false),
		weTogether(false)
		{//kazdy kolejny po prawej   0 na poludniu
		for (int i = 0; i < 4; i++){//ins i outs od srodka pasa
			int insSize = insides[i].ins.size();
			for (int k = 0; k < insSize; k++){//inicjalizacja wjazdow
				Point p = insides[i].ins[k];
				sides[i].entries.push_back(Entry(p.x, p.y));
			}
		}
		for (int i = 0; i < 4; i++){//make straights
			int opposideIndex = i + 2;
			opposideIndex %=4;
			int outsSize = insides[opposideIndex].outs.size();
			int entriesSize = sides[i].entries.size();
			for (int l = 0; l < entriesSize && l<outsSize; l++){
				Point p = insides[opposideIndex].outs[l];
				sides[i].entries[l].straight.push_back(Out(p.x, p.y));
			}
		}

		for (int i = 0; i < 4; i++){//make 
			int rightIndex = i + 1;
			rightIndex %= 4;
			int rightLane = sides[i].entries.size() - 1;
			
			int outsSize = insides[rightIndex].outs.size();//jesli naprzeciw 1 pas to break
			int breakEarlier = 0;;
			int opposideIndex = i + 2;
			opposideIndex %= 4;
			if (insides[opposideIndex].outs.size == 1 && insides[i].ins.size() == 1){
				breakEarlier = outsSize-1;
			}
			for (int j = outsSize-1; j >= 0 + breakEarlier; j++){
				Point p = insides[rightIndex].outs[j];
				sides[i].entries[rightLane].right.push_back(Out(p.x, p.y));
			}
		}

		for (int i = 0; i < 4; i++){//lefts wszystkie!!
			int leftIndex = i + 3;
			leftIndex %= 4;
			int leftLane = 0;
			int outsSize = insides[leftIndex].outs.size();
			int opposideIndex = i + 2;
			opposideIndex %= 4;
			if (insides[opposideIndex].outs.size == 1 && insides[i].ins.size()==1){
				outsSize--;
				if (i % 2 == 0){
					nsTogether = true;
				}
				else{
					weTogether = true;
				}
			}
			for (int j = 0; j < outsSize; j++){//jesli naprzeciw 1 pas to break
				Point p = insides[leftIndex].outs[j];
				sides[i].entries[leftLane].left.push_back(Out(p.x, p.y));
			}
		}
		for (int i = 0; i < 4; i++){
			int rightIndex = i + 1;
			rightIndex %= 4;
			for (int k = sides[i].entries.size(); k >= 0; k--){
				Entry* thisentry = &(sides[i].entries[k]);
				if (thisentry->left.size() == 0 && thisentry->straight.size() == 0 && thisentry->right.size() == 0){
					Point p;
					if (insides[rightIndex].outs.size() >= k){
						 p = insides[rightIndex].outs[k];
					}
					else{
						p = insides[rightIndex].outs[insides[rightIndex].outs.size() - 1];
					}
					thisentry->left.push_back(Out(p.x, p.y));
				}
			}
		}


	}
	bool nsTogether, weTogether;

	void changelights(){

	}

};
/*
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
		if (!ready){
			continue;
		}
		time(&thistick);
		if (lasttick + 1 < thistick){//uplynela conajmniej sekunda od ostatniej inkrementacji
			lasttick = lasttick + 1;
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
			

			lightVector.lights.push_back(lightmsg);
			lightVector.size++;
		}

		chatter_pub.publish(lightVector);


		loop_rate.sleep();

	}

	return 0;
}
*/
