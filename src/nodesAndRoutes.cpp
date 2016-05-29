#include "ros/ros.h"
#include "std_msgs/String.h" 
#include "anro1/accessPoint.h" 
#include "anro1/direction.h" 
#include "anro1/light.h" 
#include "anro1/lightsmsg.h" 
#include "anro1/mapNode.h" 
#include "anro1/mapNodeMessage.h" 
#include "anro1/mapRoute.h" 
#include "anro1/mapRouteMessage.h" 
#include "anro1/node.h" 
#include "anro1/nodeMessage.h" 
#include "anro1/point.h" 
#include "anro1/route.h" 
#include "anro1/routemsg.h"
#include "anro1/side.h" 
#include "Constants.h"
#include <iostream> 
#include <stdlib.h> 
#include <vector> 
#include <ctime> 
#include <sstream> 
#include <list> 
#include <queue> 

using namespace std;
class MyException{

};

class Point
{
public:     double x, y;
    Point(double ix, double iy) : x(ix), y(iy)
    {
    }
    Point(const Point& p) : x(p.x), y(p.y)
    {
    }
    Point() : x(0), y(0)
    {
    }
    bool operator<(const Point& second)
    {
        if (x = second.x)             return y < second.y;
        else return x < second.x;
    }
    bool operator==(const Point& second)
    {
        return x == second.x&&y == second.y;
    }
    bool operator!=(const Point& second)
    {
        return x != second.x||y == second.y;
    }
    double calculateDistance(double ix,double iy){
        double iix=ix-x;
        double iiy=iy-y;
        iix*=iix;
        iiy*=iiy;
        return iix+iiy;
    }
    double calculateDistance(Point second){
        return calculateDistance(second.x,second.y);
    }
}
;
class Route
{
public:     Point begin;
    Point end;
    bool straight;
    bool left;
    bool right;
    bool leftRoute;
    bool rightRoute;
    Route(Point beginn, Point endd)
    {
        straight = false;
        left = false;
        right = false;
        leftRoute = false;
        rightRoute = false;
        begin = beginn;
        end = endd;
    }
    bool operator<(const Route& second)
    {
        if (begin == second.begin)             return end < second.end;
        return begin < second.begin;
    }
}
;
class Entry : public Point
{
public:     Entry(double ix, double iy) : Point(ix,iy),light(false)
    {
    }
    vector<Point> left,straight,right;
    bool light;
}
;
class ModCounter{
public:
    int pos;
    int mod;
    ModCounter(int modulo){
        pos=0;
        mod=modulo;
    }
    void operator++(){
        pos++;
        pos=pos%mod;
    }
};

class Side
{
public:     vector<Entry> entries;
}
;
class InSide
{
public:     vector<Point> ins, outs;
}
;
vector<InSide> createPoints(int n, int w, int e, int s)
{
    vector<InSide> sides(4);     Point p;     p.x = 1;     p.y = 1;     for (int i = 0; i < s; i++)
    {
        sides[0].ins.push_back(p);
        sides[0].outs.push_back(p);
    }
    for (int i = 0; i < e; i++)
    {
        sides[1].ins.push_back(p);
        sides[1].outs.push_back(p);
    }
    for (int i = 0; i < w; i++)
    {
        sides[2].ins.push_back(p);
        sides[2].outs.push_back(p);
    }
    for (int i = 0; i < n; i++)
    {
        sides[3].ins.push_back(p);
        sides[3].outs.push_back(p);
    }
    return sides;
}
class Light
{
    Side sides[4];
    Light(vector<InSide> insides)
    {
        for (int i = 0; i < 4; i++)
        {
            //ins i outs od srodka pasa
            int insSize = insides[i].ins.size();
            for (int k = 0; k < insSize; k++)
            {
                //inicjalizacja wjazdow
                Point p = insides[i].ins[k];
                sides[i].entries.push_back(Entry(p.x, p.y));
                //cout << "pushentry" << endl;
            }
        }
    }
}
;
enum LightState
{
    allgreen,allred
}
;
class Crossroad
{
public:     Side sides[4];
    void addinfo(list<Route*>* routes)
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < sides[i].entries.size(); j++)
            {
                for (list<Route*>::iterator it = routes->begin(); ; it++)
                {
                    //dopisz kierunki
                    if (it == routes->end())
                    {
                        ROS_INFO("ROUTE NOT FOUND END [%d] [%d]",(int)sides[i].entries[j].x,(int)sides[i].entries[j].y);
                    }
                    if ((*it)->end == sides[i].entries[j])
                    {
                        if (sides[i].entries[j].left.size() != 0)
                        {
                            (*it)->left = true;
                        }
                        if (sides[i].entries[j].straight.size() != 0)
                        {
                            (*it)->straight = true;
                        }
                        if (sides[i].entries[j].right.size() != 0)
                        {
                            (*it)->right = true;
                        }
                        break;
                    }
                }
                for (int k = 0; k < sides[i].entries[j].left.size(); k++)
                {
                    for (list<Route*>::iterator it = routes->begin();; it++)
                    {
                        if (it == routes->end())
                        {
                            cout << "Route with end" << sides[i].entries[j].left[k].x << "," << sides[i].entries[j].left[k].y << "not found!" << endl;
                        }
                        if ((*it)->begin == sides[i].entries[j].left[k])
                        {
                            if (k > 0)
                            {
                                (*it)->leftRoute = true;
                            }
                            if (k < sides[i].entries[j].left.size() - 1)
                            {
                                (*it)->rightRoute = true;
                            }
                        }
                        break;
                    }
                }
                for (int k = 0; k < sides[i].entries[j].straight.size(); k++)
                {
                    for (list<Route*>::iterator it = routes->begin();; it++)
                    {
                        if (it == routes->end())
                        {
                            cout << "Route with end" << sides[i].entries[j].straight[k].x << "," << sides[i].entries[j].straight[k].y << "not found!" << endl;
                        }
                        if ((*it)->begin == sides[i].entries[j].straight[k])
                        {
                            if (k > 0)
                            {
                                (*it)->leftRoute = true;
                            }
                            if (k < sides[i].entries[j].straight.size() - 1)
                            {
                                (*it)->rightRoute = true;
                            }
                        }
                    }
                }
                for (int k = 0; k < sides[i].entries[j].right.size(); k++)
                {
                    for (list<Route*>::iterator it = routes->begin();; it++)
                    {
                        if (it == routes->end())
                        {
                            cout << "Route with end" << sides[i].entries[j].right[k].x << "," << sides[i].entries[j].right[k].y << "not found!" << endl;
                        }
                        if ((*it)->begin == sides[i].entries[j].right[k])
                        {
                            if (k > 0)
                            {
                                (*it)->leftRoute = true;
                            }
                            if (k < sides[i].entries[j].right.size() - 1)
                            {
                                (*it)->rightRoute = true;
                            }
                        }
                    }
                }
            }
        }
    }
    void createAccessPoints(vector<InSide> insides)
    {
        for (int i = 0; i < 4; i++)
        {
            //ins i outs od srodka pasa

            int insSize = insides[i].ins.size();
            for (int k = 0; k < insSize; k++)
            {
                //inicjalizacja wjazdow
                Point p = insides[i].ins[k];
                sides[i].entries.push_back(Entry(p.x, p.y));
                //cout << "pushentry" << endl;
                //ROS_INFO("WJAZD [%d][%d]",(int)p.x,(int)p.y);
                // ROS_INFO("WJAZD [%d][%d]",(int)sides[i].entries[0].x,(int)sides[i].entries[0].y);
            }
        }
    }
    void createStraightConnections(vector<InSide> insides)
    {
        for (int i = 0; i < 4; i++)
        {
            //make straights
            int opposideIndex = i + 2;
            opposideIndex %= 4;
            int outsSize = insides[opposideIndex].outs.size();
            int entriesSize = sides[i].entries.size();
            for (int l = 0; l < entriesSize && l<outsSize; l++)
            {
                Point p = insides[opposideIndex].outs[l];
                sides[i].entries[l].straight.push_back(p);
                //cout << "pushstraight" << endl;
            }
        }
    }
    void createRightConnections(vector<InSide> insides)
    {
        for (int i = 0; i < 4; i++)
        {
            //prawo
            int rightIndex = i + 1;
            rightIndex %= 4;
            int rightLane = sides[i].entries.size() - 1;
            if(sides[i].entries.size()==0)
                continue;
            int outsSize = insides[rightIndex].outs.size();

            int breakEarlier = 0;
            int opposideIndex = i + 2;
            opposideIndex %= 4;
            if (insides[opposideIndex].outs.size() == 1 && insides[i].ins.size() == 1)
            {
                //jesli naprzeciw 1 pas to break
                breakEarlier = outsSize - 1;
                if(breakEarlier<0)
                    breakEarlier=0;
            }
            for (int j = outsSize - 1; j >= 0 + breakEarlier; j--)
            {
                Point p = insides[rightIndex].outs[j];
                sides[i].entries[rightLane].right.push_back(p);
                //cout << "pushright" << endl;
            }
        }
    }
    void createLeftConnections(vector<InSide> insides)
    {
        for (int i = 0; i < 4; i++)
        {
            //lefts wszystkie!!
            int leftIndex = i + 3;
            leftIndex %= 4;
            int leftLane = 0;
            if(sides[i].entries.size()==0)
                continue;
            int outsSize = insides[leftIndex].outs.size();
            int opposideIndex = i + 2;
            opposideIndex %= 4;
           /* if (insides[opposideIndex].outs.size() == 1 && insides[i].ins.size() == 1)
            {
                //jesli naprzeciw 1 pas to break                 outsSize--;
            }*/
            for (int j = 0; j < outsSize; j++)
            {
                //jesli naprzeciw 1 pas to break
                Point p = insides[leftIndex].outs[j];
                sides[i].entries[leftLane].left.push_back(p);
            }
        }
    }
    void createRemainingRightConnections(vector<InSide> insides)
    {
        for (int i = 0; i < 4; i++)
        {
            int rightIndex = i + 1;
            rightIndex %= 4;
            int leftIndex=i+3;
            leftIndex%=4;
            int opposideIndex=i+2;
            opposideIndex%=4;
            for (int k = sides[i].entries.size()-1; k >= 0; k--)
            {
                Entry* thisentry = &(sides[i].entries[k]);
                if (thisentry->left.size() == 0 && thisentry->straight.size() == 0 && thisentry->right.size() == 0)
                {
                    Point p;
                    if(insides[opposideIndex].ins.size()==0&&insides[opposideIndex].outs.size()){
                        if(insides[rightIndex].outs.size()!=0){
                            p=insides[rightIndex].outs[insides[rightIndex].outs.size()-1];
                            thisentry->right.push_back(p);
                            continue;
                        }
                        if(insides[leftIndex].outs.size()!=0){
                            p=insides[leftIndex].outs[insides[leftIndex].outs.size()-1];
                            thisentry->left.push_back(p);
                            continue;
                        }
                    }


                    if(insides[rightIndex].outs.size()==0){//TODO
                       continue;
                    }

                    if (insides[rightIndex].outs.size() >= k+1)
                    {
                        p = insides[rightIndex].outs[k];
                    }
                    else
                    {
                        p = insides[rightIndex].outs[insides[rightIndex].outs.size() - 1];
                    }
                    thisentry->right.push_back(p);
                }
            }
        }
    }
    Crossroad(vector<InSide> insides) :         nsTogether(false), redtime(2), greentime(6),         weTogether(false),         state(0),         side(0),         time(0),timeToChange(0)
    {
        //kazdy kolejny po prawej   0 na poludniu             if (insides.size() != 4)

        if (insides[0].ins.size() == 1 && insides[2].ins.size() == 1)
        {
            nsTogether = true;
        }
        if (insides[1].ins.size() == 1 && insides[3].ins.size() == 1)
        {
            weTogether = true;
        }
        createAccessPoints(insides);
        createStraightConnections(insides);
        createRightConnections(insides);
        createLeftConnections(insides);
        createRemainingRightConnections(insides);
        int counter=0;
        for(int i=0;i<4;i++){
            if(insides[i].ins.size()!=0||insides[i].outs.size()!=0)
                counter++;
        }
        if(counter<=2){
            turn=true;
        }
        setLightsStates();
        test(insides);
    }
    bool turn;
    bool nsTogether, weTogether;
    int time,timeToChange;
    int side,state;
    int greentime, redtime;
    vector<LightState> lightstate;
    void setLightsStates()
    {
        //lightstate.push_back(allred);
        lightstate.push_back(allgreen);
        lightstate.push_back(allred);
    }
    bool goToState(LightState state, int side)
    {
        if (sides[side].entries.size()==0){
            return true;
        }
        switch (state)
        {
        case allred:
            cout<<"allred"<<side<<endl;
            for (int i = 0; i < sides[side].entries.size(); i++)
            {
                sides[side].entries[i].light = false;
            }
            timeToChange = 1;
            break;
        case allgreen:
            for (int i = 0; i < sides[side].entries.size(); i++)
            {
                cout<<"allgreen"<<side<<endl;
                sides[side].entries[i].light = true;
            }
            timeToChange = 1;
            break;
        }
        return false;
    }
    anro1::node giveMessage()
    {
        anro1::node node;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < sides[i].entries.size(); j++)
            {
                anro1::accessPoint ap;
                ap.x = sides[i].entries[j].x;
                ap.y = sides[i].entries[j].y;
		ap.green = sides[i].entries[j].light;
                //ROS_INFO("WJAZD w msg [%d][%d]",(int)ap.x,(int)ap.y);
                for (int k = 0; k < sides[i].entries[j].straight.size();k++)
                {
                    anro1::point p;
                    p.x = sides[i].entries[j].straight[k].x;
                    p.y = sides[i].entries[j].straight[k].y;
                    ap.straight.push_back(p);
                }
                for (int k = 0; k < sides[i].entries[j].left.size();k++)
                {
                    anro1::point p;
                    p.x = sides[i].entries[j].left[k].x;
                    p.y = sides[i].entries[j].left[k].y;
                    ap.left.push_back(p);
                }
                for (int k = 0; k < sides[i].entries[j].right.size();k++)
                {
                    anro1::point p;
                    p.x = sides[i].entries[j].right[k].x;
                    p.y = sides[i].entries[j].right[k].y;
                    ap.right.push_back(p);
                }
                switch (i)
                {
                case 0:                     node.s.push_back(ap);
                    break;
                case 1:                     node.e.push_back(ap);
                    break;
                case 2:                     node.n.push_back(ap);
                    break;
                case 3:                     node.w.push_back(ap);
                    break;
                }
            }
        }
        return node;
    }
    bool test(vector<InSide> insides)
    {
        /*for(int i=0;i<4;i++){
            cout<<"strona"<<i<<endl;
            for(int j=0;j<insides[i].ins.size();j++){
                 cout<<"wjazd"<<insides[i].ins[j].x<<" "<<insides[i].ins[j].y<<endl;
            }
            for(int j=0;j<insides[i].outs.size();j++){
                cout<<"wyjazd"<<insides[i].outs[j].x<<" "<<insides[i].outs[j].y<<endl;
            }
        }*/
        /*for(int i=0; i<4 ;i++){
            cout<<"strona"<<strona<<endl;
            for(int j=0;j<sides[i].entries.size();j++){
                for(int m=0;m<sides[i].entries[j].left.size()){
                    cout<<"lewo"<<endl;
                    for(int k=0;k<sides[i].entries[j].left[])
                }

            }
        }*/


        vector<Point> points;
        for(int i=0;i<4;i++){
            for(int j=0;j<sides[i].entries.size();j++)
                points.push_back(Point(sides[i].entries[j].x,sides[i].entries[j].y));
        }
        //cout<<"pointssize"<<points.size()<<endl;
        for(int i=0;i<points.size();i++){
            for(int j=i+1;j<points.size();j++){
                if(points[i]==points[j]){
                    cout<<"disaster";
                    for(int num=0;num<4;num++)
                        for(int l=0;l<insides[num].ins.size();l++){
                            cout<<insides[num].ins[l].x<<" "<<insides[num].ins[l].y<<endl;
                        }
                    cout<<"mojecd punkty"<<endl;
                    for(int num=0;num<points.size();num++){
                        cout<<points[num].x<<" "<<points[num].y<<endl;
                    }
                }
            }
        }
        vector<Point> lefts, straights, rights;
        for(int i=0;i<4;i++){
            for(int j=0;j<sides[i].entries.size();j++){
                for(int k=0;k<sides[i].entries[j].left.size();k++){
                    lefts.push_back(Point(sides[i].entries[j].left[k].x,sides[i].entries[j].left[k].y));
                }
                for(int k=0;k<sides[i].entries[j].straight.size();k++){
                    straights.push_back(Point(sides[i].entries[j].straight[k].x,sides[i].entries[j].straight[k].y));
                }
                for(int k=0;k<sides[i].entries[j].right.size();k++){
                    rights.push_back(Point(sides[i].entries[j].right[k].x,sides[i].entries[j].right[k].y));
                }
            }
            /*for(int j=0;j<sides[i].straight.size();j++){
                lefts.push_back(Point(sides[i].left[j].x,sides[i].left[j].y));
            }
            for(int j=0;j<sides[i].left.size();j++){
                lefts.push_back(Point(sides[i].left[j].x,sides[i].left[j].y));
            }*/
        }
        /*for(int i=0;i<lefts.size();i++){
            for(int j=i+1;j<lefts.size();j++){
                if(lefts[i]==lefts[j]){
                    cout<<"disaster";
                }
                //else cout<<"testleft";
            }
        }*/
        /*for(int i=0;i<rights.size();i++){
            for(int j=i+1;j<rights.size();j++){
                if(rights[i]==rights[j]){
                    cout<<"disaster";
                }
                //else cout<<"testright";
            }
        }*/
        for(int i=0;i<straights.size();i++){
            for(int j=i+1;j<straights.size();j++){
                if(straights[i]==straights[j]){
                    cout<<"2 punkty na wprost kieruja do jednego";
                }
                // else cout<<"teststraight";
            }
        }
        for(int i=0;i<rights.size();i++){
            lefts.push_back(rights[i]);
        }
        for(int i=0;i<straights.size();i++){
            lefts.push_back(straights[i]);
        }
        for(int i=0;i<lefts.size();i++){
            for(int j=0;j<points.size();j++){
                if(lefts[i]==points[j]){
                    cout<<"disaster";
                }
            }
        }
        for(int i=0;i<4;i++){
            for(int j=0;j<sides[i].entries.size();j++){
                if(sides[i].entries[j].left.size()==0 && sides[i].entries[j].straight.size()==0 && sides[i].entries[j].right.size()==0){
                    cout<<"nie ma gdzie jechac"<<endl;
                    for(int num=0;num<4;num++)
                        for(int l=0;l<insides[num].ins.size();l++){
                            cout<<"wjazdy"<<insides[num].ins[l].x<<" "<<insides[num].ins[l].y<<endl;
                        }
                }
            }
        }
        cout<<"ok"<<endl;
        /*ROS_INFO("TEST");
        if (show)         for (int i = 0; i < 4; i++)
        {
            ROS_INFO("ROZMIARY WJAZDOW [%d]",(int)sides[i].entries.size());
            for(int j=0;j<sides[i].entries.size();j++){
                ROS_INFO("WJAZD [%d][%d]",sides[i].entries[j].x,sides[i].entries[j].y);
                for(int k=0;k<sides[i].entries[j].left.size();k++){
                     ROS_INFO("LEWO [%d][%d]",sides[i].entries[j].left[k].x,sides[i].entries[j].left[k].y);
                    sides[i].entries[j].left[k];
                }
            }

        }
        if (show)         for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < sides[i].entries.size(); j++)
            {
                cout << sides[i].entries[j].left.size()<<"lewo"<<endl;
                cout << sides[i].entries[j].straight.size()<<"prosto"<<endl;
                cout << sides[i].entries[j].right.size() << "prawo" << endl;
            }
            cout << endl;
        }
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < sides[i].entries.size(); j++)
            {
                bool ok = false;
                if (sides[i].entries[j].left.size() != 0)
                {
                    ok = true;
                }
                if (sides[i].entries[j].straight.size() != 0)                     ok = true;
                if (sides[i].entries[j].right.size() != 0)                     ok = true;
                if (!ok)
                {
                    return false;
                }
            }
        }
        return true;*/

    }
    void changelights()
    {
        // while(goToState(lightstate[state], side)){
        //side++;
        //side%=4;
        // }
        if(goToState(lightstate[state], side)){
            state = 0;
            side++;
            side = side % 4;
        }
        else{
            state++;
        }

        if (state>=lightstate.size())
        {
            state = 0;
            side++;
            side = side % 4;
        }
    }
    void incrementTime()
    {
        /*for(int i=0;i<4;i++){
            for(int j=0;j<sides[i].entries.size();j++){
                sides[i].entries[j].light=!sides[i].entries[j].light;
            }
        }*/
        time++;
        if (time > timeToChange)
        {

            time = 0;
            changelights();
        }
    }
    anro1::lightsmsg giveLights(){
        anro1::lightsmsg lightsvector;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < sides[i].entries.size(); j++)
            {
                anro1::light light;
                light.cords.x = sides[i].entries[j].x;
                light.cords.y = sides[i].entries[j].y;
                light.green = sides[i].entries[j].light;
                lightsvector.lights.push_back(light);
            }
        }
        //ROS_INFO("SWIATLA %d",lightsvector.lights.size());
        return lightsvector;
    }
};
class Crossroads
{
public:     void createRoutes(list<Route*>* routesToCheck)
    {
        for (list<Crossroad*>::iterator it = crossroads->begin(); it!=crossroads->end(); it++)
        {
            (*it)->addinfo(routesToCheck);
        }
    }
    Crossroads()
    {
        crossroads = new list<Crossroad*>;
    }
    void reset()
    {
        while (!crossroads->empty())
        {
            crossroads->pop_front();
        }
    }
    void addCrossroad(vector<InSide> vec)
    {
        Crossroad* crossroad = new Crossroad(vec);
        crossroads->push_front(crossroad);
    }
    void tick()
    {
        //inkrementacja czasu na skrzyzowaniach
        for (list<Crossroad*>::iterator it = giveList()->begin(); it != giveList()->end(); it++)
        {
            (*it)->incrementTime();
        }
    }
    list<Crossroad*>* giveList()
    {
        return crossroads;
    }
    anro1::nodeMessage giveMessage()
    {
        anro1::nodeMessage msg;
        for (list<Crossroad*>::iterator it = crossroads->begin(); it != crossroads->end(); it++)
        {
            msg.nodes.push_back((*it)->giveMessage());
        }
        return msg;
    }

    anro1::lightsmsg giveLights(){
        anro1::lightsmsg toGive;
        int i=0;
        for(list<Crossroad*>::iterator it = crossroads->begin();it!=crossroads->end();it++){
            /* if(i!=7){
            i++;
            continue;
        }*/
            anro1::lightsmsg tmp = (*it)->giveLights();


            for(int i=0;i<tmp.lights.size();i++){
                toGive.lights.push_back(tmp.lights[i]);
            }
            // i++;
            /*if(i==7)
            break;*/

        }
        return toGive;

    }
private:     list<Crossroad*>* crossroads;
}
;
Crossroads crossroads;
bool testAllPossibilities(int size)
{
    bool ok;     for (int i = 1; i <= size; i++)
    {
        for (int k = 1; k <= size; k++)
        {
            for (int m = 1; m <= size; m++)
            {
                for (int n = 1; n <= size; n++)
                {
                    vector<InSide> insides = createPoints(i, k, m, n);
                    Crossroad c(insides);
                    // ok = c.test();
                    if (!ok)                         cout << i << " " << k << " " << m << " " << n << endl;
                }
            }
        }
    }
    return ok;
}
void testo()
{
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
}
bool ready=false;
bool processed = false;
void process(const anro1::mapNodeMessage::ConstPtr& msg)
{
    ROS_INFO("Ja slyszu");
    vector < anro1::mapNode> nodes = msg->nodes;
    for (int i =0;i<nodes.size(); i++)
    {
        // ROS_INFO("PRZETWARZAMY");
        vector<InSide> insides;
        if (nodes[i].sides.size() != 4)
        {
            //     ROS_INFO("disaster");
        }
        for (int j =0; j < nodes[i].sides.size(); j++)
        {
            //  ROS_INFO("PRZETWARZAMY2");
            InSide inside;
            for (int k = 0; k < nodes[i].sides[j].in.size(); k++)
            {
                //   ROS_INFO("PRZETWARZAMY3");
                Point p;
                anro1::point point = nodes[i].sides[j].in[k];
                p.x = point.x;
                p.y = point.y;
                //ROS_INFO("WJAZD [%d][%d]",(int)p.x,(int)p.y);
                inside.ins.push_back(p);
            }
            //    ROS_INFO("PRZETWARZAMYpo3");
            for (int k = 0; k < nodes[i].sides[j].out.size(); k++)
            {
                //  ROS_INFO("PRZETWARZAMY4");
                Point p;
                anro1::point point = nodes[i].sides[j].out[k];
                p.x = point.x;
                p.y = point.y;
                inside.outs.push_back(p);
            }


            insides.push_back(inside);
            // ROS_INFO("PRZETWARZAMYpush");
        }
        Point middle(nodes[i].x,nodes[i].y);
        for(int j=0;j<insides.size();j++){

            bool sorted=false;
            while(!sorted){
                if(insides[j].ins.size()==0)
                    break;
                int k=0;
                for(;k<insides[j].ins.size()-1;k++){
                    cout<<"lecimy"<<k<<endl;
                    double distance1=insides[j].ins[k].calculateDistance(middle);
                    double distance2=insides[j].ins[k+1].calculateDistance(middle);
                    cout<<"sort"<<distance1<<" "<<distance2<<endl;
                    if(distance1>distance2){
                        cout<<"sortujemy"<<endl;
                        cout<<insides[j].ins[k].x<<" "<<insides[j].ins[k].x<<endl;
                        cout<<insides[j].ins[k+1].x<<" "<<insides[j].ins[k+1].x<<endl;

                        Point spare=insides[j].ins[k];
                        insides[j].ins[k]=insides[j].ins[k+1];
                        insides[j].ins[k+1]=spare;
                        cout<<insides[j].ins[k].x<<" "<<insides[j].ins[k].x<<endl;
                        cout<<insides[j].ins[k+1].x<<" "<<insides[j].ins[k+1].x<<endl;

                        k=-1;
                        // cout<<k<<"rozmiar"<<insides[j].ins.size()-1;

                    }
                }

                sorted=true;
            }
        }
        for(int j=0;j<insides.size();j++){

            bool sorted=false;
            while(!sorted){
                if(insides[j].outs.size()==0)
                    break;
                int k=0;
                for(;k<insides[j].outs.size()-1;k++){
                    cout<<"lecimy"<<k<<endl;
                    double distance1=insides[j].outs[k].calculateDistance(middle);
                    double distance2=insides[j].outs[k+1].calculateDistance(middle);
                    cout<<"sort"<<distance1<<" "<<distance2<<endl;
                    if(distance1>distance2){
                        /*cout<<"sortujemy"<<endl;
                        cout<<insides[j].ins[k].x<<" "<<insides[j].ins[k].x<<endl;
                        cout<<insides[j].ins[k+1].x<<" "<<insides[j].ins[k+1].x<<endl;*/

                        Point spare=insides[j].outs[k];
                        insides[j].outs[k]=insides[j].outs[k+1];
                        insides[j].outs[k+1]=spare;
                        // cout<<insides[j].ins[k].x<<" "<<insides[j].ins[k].x<<endl;
                        //cout<<insides[j].ins[k+1].x<<" "<<insides[j].ins[k+1].x<<endl;

                        k=-1;
                        // cout<<k<<"rozmiar"<<insides[j].ins.size()-1;

                    }
                }

                sorted=true;
            }
        }
        for(int j=0;j<insides.size();j++){

            if(insides[j].ins.size()==0)
                continue;
            for(int k=0;k<insides[j].ins.size()-1;k++){
                //cout<<"lecimy"<<k<<endl;
                double distance1=insides[j].ins[k].calculateDistance(middle);
                double distance2=insides[j].ins[k+1].calculateDistance(middle);
                //cout<<"sort"<<distance1<<" "<<distance2<<endl;
                if(distance1>distance2){
                    cout<<"disasternasorcie";
                }
            }


            // cout<<"posortowano"<<endl;



        }
        for(int j=0;j<insides.size();j++){

            if(insides[j].ins.size()<2)
                continue;
            cout<<"sortowano wzgledem"<<middle.x<<" "<<middle.y<<endl;
            for(int k=0;k<insides[j].ins.size();k++){
                cout<<insides[j].ins[k].x<<" "<<insides[j].ins[k].y<<endl;
            }


            // cout<<"posortowano"<<endl;



        }
        for(int j=0;j<insides.size();j++){

            if(insides[j].outs.size()<2)
                continue;
            cout<<"sortowano wzgledem"<<middle.x<<" "<<middle.y<<endl;
            for(int k=0;k<insides[j].outs.size();k++){
                cout<<insides[j].outs[k].x<<" "<<insides[j].outs[k].y<<endl;
            }


            // cout<<"posortowano"<<endl;



        }


        crossroads.addCrossroad(insides);
    }
    ROS_INFO("PRZETORZYLISMY");
    processed = true;
}
vector<Route*>* routes = new vector<Route*>;
anro1::routemsg giveroutes(vector<Route*>* routesto)
{
    anro1::routemsg routemsg;     for (vector<Route*>::iterator it = routesto->begin(); it != routesto->end(); it++)
    {
        anro1::route route;
        Point begin = (*it)->begin;
        route.begin.x = begin.x;
        route.begin.y = begin.y;
        Point end = (*it)->end;
        route.end.x = end.x;
        route.end.x = end.y;
        Route mroute = **it;
        route.leftroute = mroute.leftRoute;
        route.rightroute = mroute.rightRoute;
        route.straight = mroute.straight;
        route.left = mroute.left;
        route.right = mroute.right;
        routemsg.routes.push_back(route);
    }
    return routemsg;
}
bool ready2=false;
void process2(const anro1::mapRouteMessage::ConstPtr& msg)
{
    /* if (!ready)
    {
        cout << "gowno";
        return;
    }
    vector < anro1::mapRoute> msgroutes = msg->routes;
    for (int i = 0; i < msgroutes.size(); i++)
    {
        Point begin(msgroutes[i].begin.x, msgroutes[i].begin.y);
        Point end(msgroutes[i].end.x, msgroutes[i].end.y);
    }*/
    ready2=true;
}
//ready2=true;
int main(int argc, char **argv)
{
    /* vector<InSide> insides=createPoints(1,0,0,2);


     srand(time(NULL));
    Crossroad c(insides);
   time_t lasttick, thistick;
    time(&lasttick);
    while(ros::ok()){

   /*     time(&thistick);
    if (lasttick + 1 < thistick)
    {
            cout<<"ok";
        //uplynela conajmniej sekunda od ostatniej inkrementacji
       // ROS_INFO("%d",i);
        lasttick = lasttick + 1;
          c.incrementTime();
        //
    }*/


    ros::init(argc, argv, "Nodes");
    ros::NodeHandle n;
    ros::Publisher chatter_pub = n.advertise<anro1::nodeMessage>("nodes_info", 10);//utworzenie kanalu do nadawania
    ros::Subscriber lightsSubscriber = n.subscribe("map_info_nodes", 1000, process);
    ros::Publisher route_chatter = n.advertise<anro1::routemsg>("routes_info", 1);//utworzenie kanalu do nadawania     //ros::Subscriber sub = n.subscribe("map_crossroad_info", 1, process);//subskrypcja kanalu z informacjami     //ros::Subscriber sub2 = n.subscribe("map_route_info", 1, process2);
    ros::Subscriber routesSubscriber = n.subscribe("map_info_routes", 1000, process2);
    ros::Rate loop_rate(RATE);
    ros::Publisher light_chatter = n.advertise<anro1::lightsmsg>("lights_info", 100);
    time_t lasttick, thistick;
    while (ros::ok())
    {
        ROS_INFO("COS");
        ros::spinOnce();
        if (ready2)
        {
            break;
        }
    }
    time(&lasttick);

    while (ros::ok())
    {
        //  ROS_INFO("DUPA");
        ros::spinOnce();//pewnie mozna to usunac
        time(&thistick);
        int i=0;
        if (lasttick + 1 < thistick)
        {
            //uplynela conajmniej sekunda od ostatniej inkrementacji
            ROS_INFO("%d",i);
            lasttick = lasttick + 1;
            crossroads.tick();
            anro1::lightsmsg lightsmsg=crossroads.giveLights();
            light_chatter.publish(lightsmsg);
            //
        }

        anro1::nodeMessage nodemsg = crossroads.giveMessage();
        //  ROS_INFO("COSIDZIE");


        chatter_pub.publish(nodemsg);
        // anro1::routemsg routemsg = giveroutes(routes);
        //route_chatter.publish(routemsg);
        loop_rate.sleep();
    }
    return 0;
}
