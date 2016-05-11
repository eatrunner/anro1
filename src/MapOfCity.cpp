#include "ros/ros.h"
#include "std_msgs/String.h"
#include <stdio.h>
#include "anro1/mapMessage.h"
#include "anro1/mapNode.h"
#include "anro1/mapNodeMessage.h"
#include "anro1/mapRoute.h"
#include "anro1/mapRouteMessage.h"
#include "anro1/side.h"
#include <cstdlib>
#include <fstream>
#include <vector>
#include <visualization_msgs/Marker.h>
#include "Crossroad.h"
#include "Lane.h"
#include "ReadMatrix.h"
#include "anro1/point.h"
#include "Constants.h"

using namespace std;


class MapOfCity{

public:
  double maxX;
  visualization_msgs::Marker marker1, marker2, marker3, tree, leaves, tree1, leaves1, tree2, leaves2;
  ros::NodeHandle n;
  ros::Publisher marker_pub;
  std::vector<std::vector<char> > map;


  MapOfCity()
  {
    //sendCrossroadsInfo();
    configureRviz();
    //sendMapVisualization();
    const std::vector<std::vector<char> > map1(readMap());
    std::vector<Crossroad> crossroads(ReadMatrix::buildInfo(map1));
    maxX = ReadMatrix::getMaxX();
    std::vector<Lane> lanes(generate_lanes(crossroads));
    create_map(lanes,crossroads);
    sendInfo(lanes,crossroads);//TODO: FUNCKJA DO WYSYŁANIA INFO DO INNYCH MODULÓW
  }

  void sendInfo(std::vector<Lane> lanes,std::vector<Crossroad> crossroads){
      anro1::mapNodeMessage nodemsg;
      for(int i=0;i<crossroads.size();i++){
          anro1::mapNode node;
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

  }

  void create_map(vector < Lane > lanes,vector<Crossroad> crossroads)
  {

    visualization_msgs::Marker  points, line_strip, line_list, cubes;
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
      marker1.pose.position.z = 0;
      marker1.pose.orientation.x = 0.0;
      marker1.pose.orientation.y = 0.0;
      marker1.pose.orientation.z = 0.0;
      marker1.pose.orientation.w = 1.0;
      marker1.scale.x = 4*LANE_WIDTH;
      marker1.scale.y = 4*LANE_WIDTH;
      marker1.scale.z = LANE_WIDTH/6;
      marker1.color.r = 0.5f;
      marker1.color.g = 0.5f;
      marker1.color.b = 0.5f;
      marker1.color.a = 1.0;
      marker1.lifetime = ros::Duration();

      marker_pub.publish(marker1);

    }


    int32_t i;
    for (i = 0; i <lanes.size(); i++ )
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
      else
      {
        marker1.color.b = 0.0f;
        marker1.color.g = 0.0f;
        marker1.color.r = 0.0f;
      }
      marker1.color.a = 1.0;
      marker1.lifetime = ros::Duration();

      marker_pub.publish(marker1);
    }

  }
  std::vector<std::vector<char> > readMap()
  {
    std::ifstream file("/home/ant/catkin_ws/src/anro1/mapa1.txt");
    if(!file.is_open())
      ROS_INFO("damn! file does not exist");
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

    ROS_INFO("Zakobnczenie czytania pliku, rozmiar: [%d]",(int)map[0].size());
    int i;
    for (i = 0; i < map.size(); i++)
    {
      ROS_INFO("Rozmiar wiersza nr. [%d], [%d]",i,(int)map[5].size());
    }

    for (i = 0; i < map.size(); i++)
    {
      for (int j = 0; j < map[i].size(); j++)
      {ROS_INFO("[%c]", map[i][j]);}

      ROS_INFO("\n");
    }


    ROS_INFO("liczba wierszy: [%d]", map.size());
    ROS_INFO("liczba kolumn: [%d]", map[0].size());


    for (i = 0; i < map.size(); i++)
    {
      for (int j = 0; j < map[i].size(); j++)
      {
        map[i][j]=(map[i][j] - '0');

      }
    }
    return map;
  }

  vector < Lane >  generate_lanes(vector<Crossroad> cv)
  {
    ROS_INFO("Number of crossroads: [%d]", cv.size());
    int i, j;
    double y,x;
    for(i=0;i<cv.size();i++)
    {
      ROS_INFO("\n");
      ROS_INFO("cross nr [%d] ", i);
      ROS_INFO("\n");
      ROS_INFO("origin x: [%f]", cv[i].getOrigin().getX());
      ROS_INFO("origin y: [%f]", cv[i].getOrigin().getY());
      ROS_INFO("size north: [%d]", cv[i].getN().size());
      ROS_INFO("size east: [%d]", cv[i].getE().size());
      ROS_INFO("size south: [%d]",cv[i].getS().size());
      ROS_INFO("size west : [%d]", cv[i].getW().size());


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

    //return rt;

    //NOWA CZESC:

    int m =-1;
    do
    {
      Crossroad c = cv[++m];
      temp.clear();
      x = c.getOrigin().getX();
      if (c.getS().size()> 0)
        for (j = 0; j < c.getS().size(); j++)
        {
          char dir='S';
          if(c.getS()[j].getIn())
            dir ='N';
          temp.push_back(Lane(c.getS()[j].getOrigin(),dir));
        }
      for(int k=m+1;k<cv.size();k++)
      {
        ROS_INFO("m = %d,k = %d",m,k);
        Crossroad cr = cv[k];
        if (cr.getOrigin().getX() == x && cr.getN().size() > 0)
        {
          ROS_INFO("Znaleziony pion: %f, poszukiwany: %f",cr.getOrigin().getX(),x);
          for (j = 0; j <cr.getN().size(); j++)
          {
            temp[j].setEnd((cr.getN())[j].getOrigin());

          rt.push_back(temp[j]);}

          temp.clear();
          if (cr.getS().size()> 0)
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
    ROS_INFO("list of lanes:");
    for (i = 0; i < rt.size(); i++)
    {
      ROS_INFO("lane nr [%d], pocz: [%f] i [%f], koniec: [%f] i [%f], kier: %c",i,rt[i].getStart().getX(),rt[i].getStart().getY(),rt[i].getEnd().getX(),rt[i].getEnd().getY(),rt[i].getDir());
    }
    return rt;



    /*
    vector<int> x_cross_v;
     for (i = 0; i < cv.size(); i++)
     {
       Crossroad c = cv[i];
       if (x_cross_v.size() != 0)
       {
         for (j = 0; j < x_cross_v.size(); j++)
         {
           if (x_cross_v[j] == c.getOrigin().getX())
             continue;
         }
       }
       x_cross_v.push_back(c.getOrigin().getX());


       for (j = c.getS().size(); j >= 0; j--)
       {

         temp.push_back(Lane(c.getS()[j].getOrigin()));
       }
       for (k = 0; k < cv.size(); k++)
       {

         if (c.getOrigin().getX()!= cv[k].getOrigin().getX())
           continue;
         else
         {
           if (c.getN().size()!= 0)
             for (j = c.getN().size(); j >= 0; j--)
             {
               temp[j].setEnd(c.getN()[j].getOrigin());
               rt.push_back(temp[j]);
             }
           temp.clear();
           if (c.getS().size()!= 0)
             for (j = c.getS().size(); j >= 0; j--)
             {
               temp.push_back(Lane(c.getS()[j].getOrigin()));
               rt.push_back(temp[j]);

             }
         }
       }
       temp.clear();
     }*/
    return rt;
  }

  void configureRviz()
  {
    marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 10);
    ros::Rate loop_rate(10000);
    while(marker_pub.getNumSubscribers() < 1)
    {
      ROS_INFO("Create at least one subscriber");
      loop_rate.sleep();
    }


  }

  // stworzy drzewa , budynki itp
  void createBuildings()
  {

    uint32_t shape = visualization_msgs::Marker::CUBE;
    marker1.header.frame_id = "/my_frame";
    marker1.header.stamp = ros::Time::now();
    marker1.type = shape;
    marker1.ns = "MapOfCity";
    marker1.id = 4;
    marker1.action = visualization_msgs::Marker::ADD;
    marker1.pose.position.x = 75;
    marker1.pose.position.y = 75;
    marker1.pose.position.z = 25;
    marker1.pose.orientation.x = 0.0;
    marker1.pose.orientation.y = 0.0;
    marker1.pose.orientation.z = 0.0;
    marker1.pose.orientation.w = 1.0;
    marker1.scale.x = 5.0;
    marker1.scale.y = 5.0;
    marker1.scale.z = 25.0;
    marker1.color.r = 0.5f;
    marker1.color.g = 0.0f;
    marker1.color.b = 0.0f;
    marker1.color.a = 1.0;
    marker1.lifetime = ros::Duration();


    marker2.header.frame_id = "/my_frame";
    marker2.header.stamp = ros::Time::now();
    marker2.type = shape;
    marker2.ns = "MapOfCity";
    marker2.id = 5;
    marker2.action = visualization_msgs::Marker::ADD;
    marker2.pose.position.x = 75;
    marker2.pose.position.y = 75;
    marker2.pose.position.z = 12.5;
    marker2.pose.orientation.x = 0.0;
    marker2.pose.orientation.y = 0.0;
    marker2.pose.orientation.z = 0.0;
    marker2.pose.orientation.w = 1.0;
    marker2.scale.x = 10.0;
    marker2.scale.y = 10.0;
    marker2.scale.z = 25.0;
    marker2.color.r = 0.0f;
    marker2.color.g = 0.0f;
    marker2.color.b = 0.0f;
    marker2.color.a = 2.0;
    marker2.lifetime = ros::Duration();

    shape = visualization_msgs::Marker::SPHERE;
    marker3.header.frame_id = "/my_frame";
    marker3.header.stamp = ros::Time::now();
    marker3.type = shape;
    marker3.ns = "MapOfCity";
    marker3.id = 6;
    marker3.action = visualization_msgs::Marker::ADD;
    marker3.pose.position.x = 75;
    marker3.pose.position.y = 75;
    marker3.pose.position.z = 55;
    marker3.pose.orientation.x = 70.0;
    marker3.pose.orientation.y = 70.0;
    marker3.pose.orientation.z = 20;
    marker3.pose.orientation.w = 1.0;
    marker3.scale.x = 10.0;
    marker3.scale.y = 10.0;
    marker3.scale.z = 40.0;
    marker3.color.r = 1.0f;
    marker3.color.g = 1.0f;
    marker3.color.b = 1.0f;
    marker3.color.a = 1.0;
    marker3.lifetime = ros::Duration();

    shape = visualization_msgs::Marker::CUBE;
    tree.header.frame_id = "/my_frame";
    tree.header.stamp = ros::Time::now();
    tree.type = shape;
    tree.ns = "MapOfCity";
    tree.id = 7;
    tree.action = visualization_msgs::Marker::ADD;
    tree.pose.position.x = 70;
    tree.pose.position.y = 35;
    tree.pose.position.z = 5;
    tree.pose.orientation.x = 0.0;
    tree.pose.orientation.y = 0.0;
    tree.pose.orientation.z = 0;
    tree.pose.orientation.w = 1.0;
    tree.scale.x = 2.0;
    tree.scale.y = 2.0;
    tree.scale.z = 10.0;
    tree.color.r = 0.5f;
    tree.color.g = 0.25f;
    tree.color.b = 0.07f;
    tree.color.a = 1.0;
    tree.lifetime = ros::Duration();

    shape = visualization_msgs::Marker::SPHERE;
    leaves.header.frame_id = "/my_frame";
    leaves.header.stamp = ros::Time::now();
    leaves.type = shape;
    leaves.ns = "MapOfCity";
    leaves.id = 8;
    leaves.action = visualization_msgs::Marker::ADD;
    leaves.pose.position.x = 70;
    leaves.pose.position.y = 35;
    leaves.pose.position.z = 15;
    leaves.pose.orientation.x = 0.0;
    leaves.pose.orientation.y = 0.0;
    leaves.pose.orientation.z = 0;
    leaves.pose.orientation.w = 1.0;
    leaves.scale.x = 15.0;
    leaves.scale.y = 15.0;
    leaves.scale.z = 10.0;
    leaves.color.r = 0.1f;
    leaves.color.g = 0.8f;
    leaves.color.b = 0.1f;
    leaves.color.a = 1.0;
    leaves.lifetime = ros::Duration();

    tree1 = tree;
    tree1.id = 19;
    tree1.pose.position.x = 85 ;
    tree1.pose.position.y = 25;


    leaves1 = leaves;
    leaves1.id = 10;
    leaves1.pose.position.x = 85 ;
    leaves1.pose.position.y = 25;

    tree2 = tree1;
    tree2.id = 11;
    tree2.pose.position.x = 65 ;
    tree2.pose.position.y = 15;

    leaves2 = leaves;
    leaves2.id = 12;
    leaves2.pose.position.x = 65 ;
    leaves2.pose.position.y = 15;

  }

  // metoda sendCrossroadsInfo wysyła w topicu map_info message mapMessage zawierajace pola okreslajace polozenie oraz typ skrzyzownia/zakretu
  /*void sendCrossroadsInfo()
  {
    //konfiguracja polaczenia
    ros::NodeHandle n;
    ros::Publisher map_info = n.advertise<anro1::mapnodeMessage>("map_info", 1000);
    anro1::mapMessage crossroad1;
    anro1::mapMessage turning;

    //petla czeka okolo 10sek na listener
    ros::Rate loop_rate(10);

    ROS_INFO("I heard: ");
    // wysylanie informacji o polozeniu skrzyzowan

    // czekaj poki nie bedzie trzech subskrypji (lights, turns, model)
    while (map_info.getNumSubscribers() < 2 )
    {

      ROS_WARN_ONCE("Please create a subscriber to the marker");
      loop_rate.sleep();
    }

    int counter=1;
    ROS_WARN_ONCE("Subscribed");
    while(counter>0)
    {
      counter--;
      ROS_INFO("I heard: ");

      crossroad1.x = 50 ;
      crossroad1.y = 50 ;
      crossroad1.type = "fourLanes";
      map_info.publish(crossroad1);

      crossroad1.x = 100 ;
      crossroad1.y = 50 ;
      crossroad1.type = "fourLanes";
      map_info.publish(crossroad1);


      turning.x = 150 ;
      turning.y = 50 ;
      turning.type = "turningUpLeft";
      map_info.publish(turning);

      turning.x = 150 ;
      turning.y = 100 ;
      turning.type = "turningDownLeft";
      map_info.publish(turning);

      turning.x = 100 ;
      turning.y = 100 ;
      turning.type = "turningDownRight";
      map_info.publish(turning);

      turning.x = 100 ;
      turning.y = 0 ;
      turning.type = "turningUpLeft";
      map_info.publish(turning);

      turning.x = 50 ;
      turning.y = 0 ;
      turning.type = "turningUpRight";
      map_info.publish(turning);

      turning.x = 50 ;
      turning.y = 100 ;
      turning.type = "turningDownLeft";
      map_info.publish(turning);

      turning.x = 0 ;
      turning.y = 100 ;
      turning.type = "turningDownRight";
      map_info.publish(turning);

      turning.x = 0 ;
      turning.y = 50 ;
      turning.type = "turningUpRight";
      map_info.publish(turning);
      loop_rate.sleep();
    }

  }
*/
};




int main(int argc, char** argv)
{
  ros::init(argc, argv, "MapOfCity");

  new MapOfCity();
  ROS_INFO("Exiting..");
  return 0;
}

