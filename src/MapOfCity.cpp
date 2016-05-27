#include "ros/ros.h"
#include "std_msgs/String.h"
#include <stdio.h>
#include "anro1/mapMessage.h"
#include <cstdlib>
#include <fstream>
#include <vector>
#include <visualization_msgs/Marker.h>
#include "Crossroad.h"
#include "Lane.h"
#include "ReadMatrix.h"
#include "Constants.h"
#include "MapOfCity.h"
#include "anro1/mapNode.h"
#include "anro1/mapNodeMessage.h"
#include "anro1/mapRoute.h"
#include "anro1/mapRouteMessage.h"
#include "anro1/side.h"
#include "anro1/point.h"
#include "ros/package.h"

using namespace std;


void MapOfCity::setLanes(const std::vector<Lane> &lanes)
{
    this->lanes = lanes;
}
std::vector<Lane> MapOfCity::getLanes()
{
    return this->lanes;
}
void MapOfCity::setMap(const std::vector<std::vector<char> > &map)
{
    this->map=map;
}
std::vector<std::vector<char> >  MapOfCity::getMap()
{
    return this->map;
}
void MapOfCity::setCrossroads(const std::vector<Crossroad> &crossroads)
{
    this->crossroads = crossroads;
}
std::vector<Crossroad> MapOfCity::getCrossroads()
{
    return this->crossroads;
}
double MapOfCity::getMaxX()
{
    return this->maxX;
}
void MapOfCity::setMaxX(double m)
{
    this->maxX = m;
}


void MapOfCity::createRvizMap()
{ ros::NodeHandle n;
    visualization_msgs::Marker marker1;
    ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 100);
    ros::Rate loop_rate(100);

    while(marker_pub.getNumSubscribers() < 1)
    {
        ROS_INFO("Create at least one subscriber");
        loop_rate.sleep();
    }

    for(int i =0;i<crossroads.size();i++)
    {
        Crossroad c = crossroads[i];
        uint32_t shape = visualization_msgs::Marker::CUBE;
        marker1.header.frame_id = "/my_frame";
        marker1.header.stamp = ros::Time::now();
        marker1.type = shape;
        marker1.ns = "MapCrossroads";
        marker1.id = i;
        marker1.action = visualization_msgs::Marker::ADD;
        marker1.pose.position.x = c.getOrigin().getX();
        marker1.pose.position.y = c.getOrigin().getY();
        marker1.scale.x = 4*LANE_WIDTH;
        marker1.scale.y = 4*LANE_WIDTH;
        marker1.scale.z = LANE_WIDTH/6;
        marker1.color.r = 0.5f;
        marker1.color.g = 0.5f;
        marker1.color.b = 0.5f;
        marker1.color.a = 1.0;
        marker1.lifetime = ros::Duration();

        marker_pub.publish(marker1);
        loop_rate.sleep();


    }

    for (int i = 0; i <lanes.size(); i++ )
    {
        Lane l = lanes[i];
        uint32_t shape = visualization_msgs::Marker::CUBE;
        marker1.header.frame_id = "/my_frame";
        marker1.header.stamp = ros::Time::now();
        marker1.type = shape;
        marker1.ns = "MapLanes";
        marker1.id = i;
        marker1.action = visualization_msgs::Marker::ADD;
        marker1.pose.position.x = ((l.getStart().getX() + l.getEnd().getX())/2);
        marker1.pose.position.y = ((l.getStart().getY() + l.getEnd().getY())/2);
        marker1.pose.position.z = 0;
        marker1.scale.x =(l.getStart().getX() - l.getEnd().getX());
        marker1.scale.y =(l.getStart().getY() - l.getEnd().getY());
        if(marker1.scale.x==0)
            marker1.scale.x = VIZ_LANE_WIDTH;
        marker1.scale.y = (l.getStart().getY() - l.getEnd().getY());
        if(marker1.scale.y==0)
            marker1.scale.y = VIZ_LANE_WIDTH;
        marker1.scale.z =LANE_WIDTH/8;

        if(l.getDir() == 'E')//blue
        {
            marker1.color.r = 0.4f;
            marker1.color.g = 0.4f;
            marker1.color.b = 1.0f;
        }
        else if(l.getDir() == 'W')//red
        {
            marker1.color.b = 0.4f;
            marker1.color.g = 0.4f;
            marker1.color.r = 1.0f;
        }
        else if(l.getDir() == 'N')//green
        {
            marker1.color.b = 0.2f;
            marker1.color.g = 0.8f;
            marker1.color.r = 0.2f;
        }
        else if(l.getDir() == 'S')//pinky
        {
            marker1.color.b = 1.0f;
            marker1.color.g = 0.4f;
            marker1.color.r = 1.0f;
        }
        else//black
        {
            marker1.color.b = 0.0f;
            marker1.color.g = 0.0f;
            marker1.color.r = 0.0f;
        }
        marker1.color.a = 1.0;
        marker1.lifetime = ros::Duration();

        marker_pub.publish(marker1);
        loop_rate.sleep();


    }

}


MapOfCity::MapOfCity()
{

}
MapOfCity::~MapOfCity()
{

}
void MapOfCity::readMap()
{
    std::string file_path_string = ros::package::getPath("anro1") + "/mapa1.txt";
    const char* file_path = file_path_string.c_str();
    std::ifstream file(file_path);
    if(!file.is_open())
        ROS_INFO("Damn! file does not exist");
    else
        ROS_INFO("File opened properly.");
    std::vector<std::vector<char> > map;
    std::string row;
    char temp;
    for (int i = 0; std::getline(file, row); i++)
    {
        std::stringstream  line(row);
        map.push_back(std::vector<char>());
        while (line >> temp)
            map[i].push_back(temp);
    }

    for (int i = 0; i < map.size(); i++)
        for (int j = 0; j < map[i].size(); j++)
            map[i][j]=(map[i][j] - '0');

    setMap(map);
}
void MapOfCity::doJob()
{
    readMap();
    setCrossroads(ReadMatrix::buildInfo(getMap()));
    setMaxX(ReadMatrix::getMaxX());
    generate_lanes();
    mapDiagnostics();
    createRvizMap();
    sendInfo();//TODO: FUNCKJA DO WYSYŁANIA INFO DO INNYCH MODULÓW
}
void MapOfCity::sendInfo()
{
    //std::vector<Lane> lanes = getLanes();
    //std::vector<Crossroad> crossroads = getCrossroads();
    anro1::mapNodeMessage nodemsg;
    for(int i=0;i<crossroads.size();i++){
        anro1::mapNode node;
        node.x = crossroads[i].origin.x;
        node.y = crossroads[i].origin.y;
        anro1::side sideS;//przeciwnie do wskazowek zegara od poludnia
        for(int j=0;j<crossroads[i].S.size();j++){
            anro1::point p;
            p.x=crossroads[i].S[j].getOrigin().getX();
            p.y=crossroads[i].S[j].getOrigin().getY();
            if(crossroads[i].S[j].getIn()){//skad wziac wyjazdowe
                sideS.in.push_back(p);
            }
            else{
                sideS.out.push_back(p);
            }
        }
        node.sides.push_back(sideS);
        anro1::side sideE;
        for(int j=0;j<crossroads[i].E.size();j++){
            anro1::point p;
            p.x=crossroads[i].E[j].getOrigin().getX();
            p.y=crossroads[i].E[j].getOrigin().getY();
            if(crossroads[i].E[j].getIn()){
                sideE.in.push_back(p);
            }
            else{
                sideE.out.push_back(p);
            }
        }
        node.sides.push_back(sideE);
        anro1::side sideN;
        for(int j=0;j<crossroads[i].N.size();j++){
            anro1::point p;
            p.x=crossroads[i].N[j].getOrigin().getX();
            p.y=crossroads[i].N[j].getOrigin().getY();
            if(crossroads[i].N[j].getIn()){
                sideN.in.push_back(p);
            }
            else{
                sideN.out.push_back(p);
            }
        }
        node.sides.push_back(sideN);
        anro1::side sideW;

        for(int j=0;j<crossroads[i].W.size();j++){
            anro1::point p;
            p.x=crossroads[i].W[j].getOrigin().getX();
            p.y=crossroads[i].W[j].getOrigin().getY();
            if(crossroads[i].W[j].getIn()){
                sideW.in.push_back(p);
            }
            else{
                sideW.out.push_back(p);
            }
        }
        node.sides.push_back(sideW);
        nodemsg.nodes.push_back(node);
    }
    anro1::mapRouteMessage routemsg;
    for(int i=0;i<lanes.size();i++){
        anro1::mapRoute route;
        Point begin, end;
        begin = lanes[i].getStart();
        end=lanes[i].getEnd();
        route.begin.x=begin.x;
        route.begin.y=begin.y;
        route.end.x=route.end.x;
        route.end.y=route.end.y;
        routemsg.routes.push_back(route);
    }
    ros::NodeHandle n;
    ros::Publisher node_info = n.advertise<anro1::mapNodeMessage>("map_info_nodes", 1000);
    ros::Publisher route_info = n.advertise<anro1::mapRouteMessage>("map_info_routes", 1000);
    while(node_info.getNumSubscribers() < 1||route_info.getNumSubscribers() < 1)
    {
        ros::Rate loop_rate(10000);
        loop_rate.sleep();
    }

    node_info.publish(nodemsg);
    route_info.publish(routemsg);
    ros::Rate loop_rate(10);
    loop_rate.sleep();

}
void MapOfCity::mapDiagnostics()
{
    std::vector<std::vector<char> > map = getMap();
    ROS_INFO("ODCZYT MAPY:");
    ROS_INFO("liczba wierszy: [%d]", (int)map.size());
    int i;
    for (i = 0; i < map.size(); i++)
    {
        ROS_INFO("Rozmiar wiersza nr. [%d]: %d",i,(int)map[i].size());
    }

    for (i = 0; i < map.size(); i++)
    {
        for (int j = 0; j < map[i].size(); j++)
        {
            ROS_INFO("%d", (int)map[i][j]);
        }
        ROS_INFO("\n");
    }
    ROS_INFO("ODCZYT SKRZYZOWAN:");
    std::vector<Crossroad> cv = getCrossroads();
    ROS_INFO("Number of crossroads: [%d]", (int)cv.size());
    for(i=0;i<cv.size();i++)
    {
        ROS_INFO("\n");
        ROS_INFO("cross nr [%d] ", i);
        ROS_INFO("\n");
        ROS_INFO("origin x: [%f]", cv[i].getOrigin().getX());
        ROS_INFO("origin y: [%f]", cv[i].getOrigin().getY());
        ROS_INFO("size north: [%lu]", cv[i].getN().size());
        ROS_INFO("size east: [%lu]", cv[i].getE().size());
        ROS_INFO("size south: [%lu]",cv[i].getS().size());
        ROS_INFO("size west : [%lu]", cv[i].getW().size());


        if(cv[i].getW().size()!=0)
        {
            for(int j = 0; j<cv[i].getW().size();j++)
            {
                ROS_INFO("Origin of route: West[%d]: x: %f, y: %f", j, cv[i].getW()[j].getOrigin().getX(),cv[i].getW()[j].getOrigin().getY() );
            }
        }
        if(cv[i].getN().size()!=0)
        {
            for(int j = 0; j<cv[i].getN().size();j++)
            {
                ROS_INFO("Origin of route: North[%d]: x: %f, y: %f", j, cv[i].getN()[j].getOrigin().getX(),cv[i].getN()[j].getOrigin().getY() );
            }
        }
        if(cv[i].getE().size()!=0)
        {
            for(int j = 0; j<cv[i].getE().size();j++)
            {
                ROS_INFO("Origin of route: East[%d]: x: %f, y: %f", j, cv[i].getE()[j].getOrigin().getX(),cv[i].getE()[j].getOrigin().getY() );
            }
        }
        if(cv[i].getS().size()!=0)
        {
            for(int j = 0; j<cv[i].getS().size();j++)
            {
                ROS_INFO("Origin of route: South[%d]: x: %f, y: %f", j, cv[i].getS()[j].getOrigin().getX(),cv[i].getS()[j].getOrigin().getY() );
            }
        }
    }

    ROS_INFO("ODCZYT PASOW:");
    std::vector<Lane> rt = getLanes();
    ROS_INFO("Number of lanes: [%d]", (int)rt.size());
    for (i = 0; i < rt.size(); i++)
    {
        ROS_INFO("lane nr [%d], pocz: [%f] i [%f], koniec: [%f] i [%f], kier: %c",i,rt[i].getStart().getX(),rt[i].getStart().getY(),rt[i].getEnd().getX(),rt[i].getEnd().getY(),rt[i].getDir());
    }
}



void MapOfCity::generate_lanes()
{
    int i, j;
    double y,x;
    vector<Crossroad> cv = getCrossroads();
    vector < Lane > rt;
    vector < Lane > temp;

    for (i = 0; i < cv.size(); i++)
    {
        Crossroad c = cv[i];

        if (i!=0 && c.getOrigin().getY() == y && c.getW().size() > 0)
        {
            for (j = 0; j <c.getW().size(); j++)
            {
                temp[j].setEnd((c.getW())[j].getOrigin());
                rt.push_back(temp[j]);

            }
        }

        temp.clear();
        y = c.getOrigin().getY();
        if (c.getE().size()> 0)
            for (j = 0; j < c.getE().size(); j++)
            {
                char dir='E';
                if(c.getE()[j].getIn())
                    dir ='W';
                temp.push_back(Lane(c.getE()[j].getOrigin(),dir));
            }
    }

    int m =-1;
    int tempX = -1;
    do
    {
        Crossroad c = cv[++m];
        x = c.getOrigin().getX();
        if(x<=tempX)
            continue;
        tempX=x;
        temp.clear();
        for(int k=m;k<cv.size();k++)
        {
            Crossroad cr = cv[k];
            if (cr.getOrigin().getX() != x)
                continue;

            if (cr.getN().size()> 0)
            {  for (j = 0; j <cr.getN().size(); j++)
                {
                    temp[j].setEnd((cr.getN())[j].getOrigin());

                    rt.push_back(temp[j]);
                }
                temp.clear();
            }
            if (cr.getS().size()> 0)
            {
                for (j = 0; j < cr.getS().size(); j++)
                {
                    char dir='S';
                    if(cr.getS()[j].getIn())
                        dir ='N';
                    temp.push_back(Lane(cr.getS()[j].getOrigin(),dir));
                }


            }
        }

    }while(cv[m].getOrigin().getX()!=maxX);

    setLanes(rt);

}



int main(int argc, char** argv)
{
    ros::init(argc, argv, "MapOfCity");

    MapOfCity mapOfCity;
    mapOfCity.doJob();


    ROS_INFO("Exiting..");
    return 0;
}
