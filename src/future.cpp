/*#include "ros/ros.h"
#include "std_msgs/String.h"
#include "anro1/light.h"
#include "anro1/lightsVector.h"
#include "anro1/mapMessage.h"*/
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <ctime>
#include <sstream>
#include <list>
#include <queue>
using namespace std;





class Point{
public:
	int x, y;
	Point(int ix, int iy) : x(ix), y(iy){}
	Point(const Point& p) : x(p.x), y(p.y){}
	Point() : x(0), y(0){}
	bool operator<(const Point& second){
		if (x = second.x)
			return y < second.y;
		else return x < second.x;
	}
	bool operator==(const Point& second){
		return x == second.x&&y == second.y;
	}
};

class Route{
public:
	Point begin;
	Point end;
	bool straight;
	bool left;
	bool right;
	bool leftRoute;
	bool rightRoute;
	Route(){
		straight = false;
		left = false;
		right = false;
		leftRoute = false;
		rightRoute = false;
	}
	bool operator<(const Route& second){
		if (begin == second.begin)
			return end < second.end;
		return begin < second.begin;
	}
};



class Entry : public Point{
public:
	Entry(int ix, int iy) : Point(x,y),light(false){}
	vector<Point> left,straight,right;
	bool light;
};


class Side{
public:
	vector<Entry> entries;
};



class InSide{
public:
	vector<Point> ins, outs;
};
vector<InSide> createPoints(int n, int w, int e, int s){
	vector<InSide> sides(4);
	Point p;
	p.x = 0;
	p.y = 0;
	for (int i = 0; i < s; i++){
		sides[0].ins.push_back(p);
		sides[0].outs.push_back(p);
	}
	for (int i = 0; i < e; i++){
		sides[1].ins.push_back(p);
		sides[1].outs.push_back(p);
	}for (int i = 0; i < w; i++){
		sides[2].ins.push_back(p);
		sides[2].outs.push_back(p);
	}for (int i = 0; i < n; i++){
		sides[3].ins.push_back(p);
		sides[3].outs.push_back(p);
	}
	return sides;
}

class Light{
	Side sides[4];
	Light(vector<InSide> insides){
		for (int i = 0; i < 4; i++){//ins i outs od srodka pasa
			int insSize = insides[i].ins.size();
			for (int k = 0; k < insSize; k++){//inicjalizacja wjazdow
				Point p = insides[i].ins[k];
				sides[i].entries.push_back(Entry(p.x, p.y));
				//cout << "pushentry" << endl;
			}
		}
	}

};

enum LightState{
	allgreen,allred
};

class Crossroad{
public:
	Side sides[4];
	void addinfo(list<Route*>* routes){
		for (int i = 0; i < 4; i++){
			for (int j = 0; j < sides[i].entries.size(); j++){
				for (list<Route*>::iterator it = routes->begin(); ; it++){//dopisz kierunki
					if (it == routes->end()){
						cout << "Route with entry" << sides[i].entries[j].x << "," << sides[i].entries[j].y << "not found!" << endl;
					}
					if ((*it)->end == sides[i].entries[j]){
						if (sides[i].entries[j].left.size() != 0){
							(*it)->left = true;
						}
						if (sides[i].entries[j].straight.size() != 0){
							(*it)->straight = true;
						}
						if (sides[i].entries[j].right.size() != 0){
							(*it)->right = true;
						}
						break;
					}
				}
				for (int k = 0; k < sides[i].entries[j].left.size(); k++){
					for (list<Route*>::iterator it = routes->begin();; it++){
						if (it == routes->end()){
							cout << "Route with end" << sides[i].entries[j].left[k].x << "," << sides[i].entries[j].left[k].y << "not found!" << endl;
						}
						if ((*it)->begin == sides[i].entries[j].left[k]){
							if (k > 0){
								(*it)->leftRoute = true;
							}
							if (k < sides[i].entries[j].left.size() - 1){
								(*it)->rightRoute = true;
							}
						}
						break;
					}
				}
				for (int k = 0; k < sides[i].entries[j].straight.size(); k++){
					for (list<Route*>::iterator it = routes->begin();; it++){
						if (it == routes->end()){
							cout << "Route with end" << sides[i].entries[j].straight[k].x << "," << sides[i].entries[j].straight[k].y << "not found!" << endl;
						}
						if ((*it)->begin == sides[i].entries[j].straight[k]){
							if (k > 0){
								(*it)->leftRoute = true;
							}
							if (k < sides[i].entries[j].straight.size() - 1){
								(*it)->rightRoute = true;
							}
						}
					}
				}
				for (int k = 0; k < sides[i].entries[j].right.size(); k++){
					for (list<Route*>::iterator it = routes->begin();; it++){
						if (it == routes->end()){
							cout << "Route with end" << sides[i].entries[j].right[k].x << "," << sides[i].entries[j].right[k].y << "not found!" << endl;
						}
						if ((*it)->begin == sides[i].entries[j].right[k]){
							if (k > 0){
								(*it)->leftRoute = true;
							}
							if (k < sides[i].entries[j].right.size() - 1){
								(*it)->rightRoute = true;
							}
						}
					}
				}

			}
		}
	}
	void createAccessPoints(vector<InSide> insides){
		for (int i = 0; i < 4; i++){//ins i outs od srodka pasa
			int insSize = insides[i].ins.size();
			for (int k = 0; k < insSize; k++){//inicjalizacja wjazdow
				Point p = insides[i].ins[k];
				sides[i].entries.push_back(Entry(p.x, p.y));
				//cout << "pushentry" << endl;
			}
		}
	}

	void createStraightConnections(vector<InSide> insides){
		for (int i = 0; i < 4; i++){//make straights
			int opposideIndex = i + 2;
			opposideIndex %= 4;
			int outsSize = insides[opposideIndex].outs.size();
			int entriesSize = sides[i].entries.size();
			for (int l = 0; l < entriesSize && l<outsSize; l++){
				Point p = insides[opposideIndex].outs[l];
				sides[i].entries[l].straight.push_back(p);
				//cout << "pushstraight" << endl;
			}
		}
	}

	void createRightConnections(vector<InSide> insides){
		for (int i = 0; i < 4; i++){//prawo
			int rightIndex = i + 1;
			rightIndex %= 4;
			int rightLane = sides[i].entries.size() - 1;

			int outsSize = insides[rightIndex].outs.size();//jesli naprzeciw 1 pas to break
			int breakEarlier = 0;
			int opposideIndex = i + 2;
			opposideIndex %= 4;
			if (insides[opposideIndex].outs.size() == 1 && insides[i].ins.size() == 1){//jesli naprzeciw 1 pas to break
				breakEarlier = outsSize - 1;
			}
			for (int j = outsSize - 1; j >= 0 + breakEarlier; j--){
				Point p = insides[rightIndex].outs[j];
				sides[i].entries[rightLane].right.push_back(p);
				//cout << "pushright" << endl;
			}
		}
	}

	void createLeftConnections(vector<InSide> insides){
		for (int i = 0; i < 4; i++){//lefts wszystkie!!
			int leftIndex = i + 3;
			leftIndex %= 4;
			int leftLane = 0;
			int outsSize = insides[leftIndex].outs.size();
			int opposideIndex = i + 2;
			opposideIndex %= 4;
			if (insides[opposideIndex].outs.size() == 1 && insides[i].ins.size() == 1){//jesli naprzeciw 1 pas to break
				outsSize--;
			}
			for (int j = 0; j < outsSize; j++){//jesli naprzeciw 1 pas to break
				Point p = insides[leftIndex].outs[j];
				sides[i].entries[leftLane].left.push_back(p);
			}
		}
	}

	void createRemainingRightConnections(vector<InSide> insides){
		for (int i = 0; i < 4; i++){
			int rightIndex = i + 1;
			rightIndex %= 4;
			for (int k = sides[i].entries.size()-1; k >= 0; k--){
				Entry* thisentry = &(sides[i].entries[k]);
				if (thisentry->left.size() == 0 && thisentry->straight.size() == 0 && thisentry->right.size() == 0){
					Point p;
					if (insides[rightIndex].outs.size() >= k){
						p = insides[rightIndex].outs[k-1];
					}
					else{
						p = insides[rightIndex].outs[insides[rightIndex].outs.size() - 1];
					}
					thisentry->left.push_back(p);
				}
			}
		}
	}
	Crossroad(vector<InSide> insides) : 
		nsTogether(false), redtime(2), greentime(6),
		weTogether(false),
		state(0),
		side(0),
		time(0)
		{//kazdy kolejny po prawej   0 na poludniu
			if (insides.size() != 4){
				cout << "DISASTER";
			}

			if (insides[0].ins.size() == 1 && insides[2].ins.size() == 1){
				nsTogether = true;
			}
			if (insides[1].ins.size() == 1 && insides[3].ins.size() == 1){
				weTogether = true;
			}

			createAccessPoints(insides);
			createStraightConnections(insides);
			createRightConnections(insides);
			createLeftConnections(insides);
			createRemainingRightConnections(insides);
	}
	bool nsTogether, weTogether;
	int time,timeToChange;
	int side,state;
	int greentime, redtime;
	vector<LightState> lightstate;
	void setLightsStates(){
		lightstate.push_back(allred);
		lightstate.push_back(allgreen);
	}
	void goToState(LightState state, int side){
		switch (state){
		case allred:
			for (int i = 0; i < sides[side].entries.size(); i++){
				sides[side].entries[i].light = false;
			}
			timeToChange = 2;
			break;
		case allgreen:
			for (int i = 0; i < sides[side].entries.size(); i++){
				sides[side].entries[i].light = true;
			}
			timeToChange = 6;
		}

	}

	bool test(bool show=false){
		if (show)
		for (int i = 0; i < 4; i++){
			cout << sides[i].entries.size()<<"ent"<<endl;
		}
		if (show)
		for (int i = 0; i < 4; i++){
			for (int j = 0; j < sides[i].entries.size(); j++){
				cout << sides[i].entries[j].left.size()<<"lewo"<<endl;
				cout << sides[i].entries[j].straight.size()<<"prosto"<<endl;
				cout << sides[i].entries[j].right.size() << "prawo" << endl;

			}
			cout << endl;
		}
		for (int i = 0; i < 4; i++){
			for (int j = 0; j < sides[i].entries.size(); j++){
				bool ok = false;
				if (sides[i].entries[j].left.size() != 0){
					ok = true;
				}
				if (sides[i].entries[j].straight.size() != 0)
					ok = true;
				if (sides[i].entries[j].right.size() != 0)
					ok = true;
				if (!ok){
					return false;
				}
			}
		}
		return true;
	}

	void changelights(){
		goToState(lightstate[state], side);
		state++;
		if (state>lightstate.size()){
			state = 0;
			side++;
			side = side % 4;
		}
	}
	void incrementTime(){
		time++;
		if (time > timeToChange){
			time = 0;
			changelights();
		}
	}
};



class Crossroads{
public:
	void createRoutes(list<Route*>* routesToCheck){
		for (list<Crossroad>::iterator it = crossroads->begin(); it!=crossroads->end(); it++){
			(*it).addinfo(routesToCheck);
		}
	}


	Crossroads(){
		crossroads = new list<Crossroad>;
	}
	void reset(){
		while (!crossroads->empty()){
			crossroads->pop_front();
		}
	}

	void addCrossroad(vector<InSide> vec){
		Crossroad crossroad(vec);
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

Crossroads crossroads;

bool testAllPossibilities(int size){
	bool ok;
	for (int i = 1; i <= size; i++){
		for (int k = 1; k <= size; k++){
			for (int m = 1; m <= size; m++){
				for (int n = 1; n <= size; n++){
					vector<InSide> insides = createPoints(i, k, m, n);
					Crossroad c(insides);
					ok = c.test();
					if (!ok)
						cout << i << " " << k << " " << m << " " << n << endl;
					/*if (c.nsTogether || c.weTogether){
						cout << i << " " << k << " " << m << " " << n << endl;
					}*/
				}
			}
		}
	}
	return ok;
}










int main(){

}





/*int main(){
	//return 0;
	vector<InSide> insides = createPoints(1, 1, 1, 1);
	Crossroad c(insides);
	//cout << c.test(true);
	int x;
	//cin >> x;
	bool ok = true;
	int counter = 0;
	int size = 3;
	ok = testAllPossibilities(size);
	cout << ok;
	cin >> x;





}*/
/*
bool ready=false;

void process(const anro1::mapMessage::ConstPtr& msg){
	ROS_INFO("Ja slyszu");
	if (msg->type == "fourLanes"){
		crossroads.addCrossroad(msg->x, msg->y, msg->type);
		ready = true;
	}
}



int main(int argc, char **argv)
{

	ros::init(argc, argv, "Nodes");
	srand(time(NULL));
	ros::NodeHandle n;
	ros::Publisher chatter_pub = n.advertise<anro1::lightsVector>("lights_info", 1);//utworzenie kanalu do nadawania
	ros::Subscriber sub = n.subscribe("map_info", 1, process);//subskrypcja kanalu z informacjami
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
