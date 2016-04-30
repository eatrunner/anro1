#include "ReadMatrix.h"
#include "ros/ros.h"
#include <stdio.h>
//#include "anro1/routeMessage.h"
#include <cstdlib>
#include "Crossroad.h"
std::vector<Crossroad> crossroads;


ReadMatrix::ReadMatrix()
{
//crossroads = new
}
ReadMatrix::~ReadMatrix()
{
  //dtor
}

std::vector<Crossroad> ReadMatrix::buildInfo(std::vector<std::vector<char> > mapInfo)
{
  const int mapSize = 1000;
  const double xRatio = 50;//mapSize / mapInfo.size();
  const double yRatio = 50;//mapSize / mapInfo[0].size();
  bool in;
  double x, y,xRoute, yRoute;

  for (size_t i = 0; i < mapInfo.size(); i++)
  {
    for (size_t j = 0; j<mapInfo[i].size(); j++)
    {
      if (mapInfo[i][j] != '+')
        continue;
      x = j * xRatio;
      y = i * yRatio;

      Crossroad c(x,y);
      /*WSCHOD*/
      if (j != mapInfo[0].size() - 1)
      {
        for (int k = 0; k < mapInfo[i][j + 1]; k++)
        {
          if (k < mapInfo[i][j + 2])
            in = true;
          else
            in = false;
          xRoute = x + 2;

          if (mapInfo[i][j + 1] == 1)
            yRoute = y;
          else if (mapInfo[i][j + 1] == 2)
            yRoute = y - 1 + k * 2;
          else if (mapInfo[i][j + 1] == 3)
            yRoute = y - 1 + k ;
          else if(mapInfo[i][j + 1] == 4)
            yRoute = y - 1.5 + k;

          int size = crossroads.size();
          //crossroadsEast.push_back(Crossroad(x, y));
          c.addNewRoute(xRoute, yRoute, in,'E');

        }
      }

      /*ZACHOD*/
      if (j != 0)
      {
        for (int k = mapInfo[i][j - 1]; k > 0; k--)
        {
          if (k > mapInfo[i][j - 2])
            in = false;
          else
            in = true;
          xRoute = x - 2;

          if (mapInfo[i][j - 1] == 1)
            yRoute = y;
          else if (mapInfo[i][j - 1] == 2)
            yRoute = y - 1 + k * 2;
          else if (mapInfo[i][j - 1] == 3)
            yRoute = y - 1 + k;
          else if (mapInfo[i][j - 1] == 4)
            yRoute = y - 1.5 + k;
          int size = crossroads.size();
          //crossroadsWest.push_back(Crossroad(x, y));
          //crossroads[size].addNewRoute(xRoute, yRoute, in);
          c.addNewRoute(xRoute, yRoute, in,'W');
        }
      }

      /*POLNOC*/
      if (i != 0)
      {
        for (int k = 0 ; k < mapInfo[i - 1][j]; k++)
        {
          if (k < mapInfo[i - 2][j])
            in = true;
          else
            in = false;

          yRoute = y - 2;

          if (mapInfo[i - 1][j] == 1)
            xRoute = x;
          else if (mapInfo[i - 1][j] == 2)
            xRoute = x - 1 + k * 2;
          else if (mapInfo[i - 1][j] == 3)
            xRoute = x - 1 + k;
          else if (mapInfo[i-1][j] == 4)
            xRoute = x - 1.5 + k;
          int size = crossroads.size();
          //crossroadsNorth.push_back(Crossroad(x, y));
          //crossroads[size].addNewRoute(xRoute, yRoute, in);
          c.addNewRoute(xRoute, yRoute, in,'N');
        }
      }

      /*POLUDNIE*/
      if (i != mapInfo.size() - 1)
      {
        for (int k = mapInfo[i + 1][j]; k > 0; k--)
        {
          if (k > mapInfo[i + 2][j])
            in = false;
          else
            in = true;

          yRoute = y - 2;

          if (mapInfo[i + 1][j] == 1)
            xRoute = x;
          else if (mapInfo[i + 1][j] == 2)
            xRoute = x - 1 + k * 2;
          else if (mapInfo[i + 1][j] == 3)
            xRoute = x - 1 + k;
          else if (mapInfo[i + 1][j] == 4)
            xRoute = x - 1.5 + k;
          int size = crossroads.size();
          //crossroadsSouth.push_back(Crossroad(x, y));
          //crossroads[size].addNewRoute(xRoute, yRoute, in);
          c.addNewRoute(xRoute, yRoute, in,'S');
        }
      }
      crossroads.push_back(c);
    }
  }
  return crossroads;
}
/*
void ReadMatrix::sendRouteInfo()
{
  ros::NodeHandle n;
  ros::Publisher marker_pub = n.advertise<anro1::routeMessage>("route_info", 1000);
  anro1::routeMessage wykurw_message;
  /* vector Crossroad *
  for (int i = 0; i < crossroads.size(); i++)
  {
    wykurw_message.push_back(crossroads[i]);
  }

  while (route_info.getNumSubscribers() < 1)
  {

    ROS_WARN_ONCE("Please create a subscriber to the marker");
    loop_rate.sleep();
  }

  route_info.publish(wykurw_message);
}*/
