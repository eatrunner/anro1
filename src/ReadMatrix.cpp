#include "ReadMatrix.h"
#include "ros/ros.h"
#include <stdio.h>
//#include "anro1/routeMessage.h"
#include <cstdlib>
#include "Crossroad.h"
#include "Constants.h"
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
  const double xRatio = MATRIX_DISTANCE;
  const double yRatio = MATRIX_DISTANCE;
  bool in;
  double x, y,xRoute, yRoute;
  double verSize = mapInfo.size();
  double horSize = mapInfo.size();
  int maxj = 0;
  for (size_t i = 0; i < mapInfo.size(); i++)
  {
    for (size_t j = 0; j < mapInfo[i].size(); j++)
    {
      if(j>maxj)
        maxj = j;
    }
  }
  for (size_t i = 0; i < mapInfo.size(); i++)
  {
    for (size_t j = 0; j < mapInfo[i].size(); j++)
    {
      if ((mapInfo[i][j] + '0' )!= '+')
        continue;
      x = j * xRatio;
      y = i*yRatio;
      y=-y;
      Crossroad c(x,y);
      /*WSCHOD*/
      if (j != mapInfo[0].size() - 1)
      {
        int lane_num = mapInfo[i][j + 1];
        int lane_in = mapInfo[i][j + 2];
        int lane_out = lane_num - lane_in;
        xRoute = x + 2*LANE_WIDTH;
        if(lane_num!=0)
        { for (int k = 1; k <= lane_in; k++)
          {
            yRoute = y + (0.5*LANE_WIDTH + (k-1)*LANE_WIDTH);
            c.addNewRoute(xRoute, yRoute, true,'E');
          }
          for (int k = 1; k <= lane_out; k++)
          {
            yRoute = y -  (0.5*LANE_WIDTH + (k-1)*LANE_WIDTH);
            c.addNewRoute(xRoute, yRoute, false,'E');
          }
        }
      }

      /*ZACHOD*/
      if (j != 0)
      {
        int lane_num = mapInfo[i][j - 1];
        int lane_in = mapInfo[i][j - 2];
        int lane_out = lane_num - lane_in;
        xRoute = x - 2*LANE_WIDTH;
        if(lane_num!=0)
        { for (int k = 1; k <= lane_out; k++)
          {
            yRoute = y + (0.5*LANE_WIDTH + (k-1)*LANE_WIDTH);
            c.addNewRoute(xRoute, yRoute, true,'W');
          }
          for (int k = 1; k <= lane_in; k++)
          {
            yRoute = y - (0.5*LANE_WIDTH + (k-1)*LANE_WIDTH);
            c.addNewRoute(xRoute, yRoute, false,'W');
          }

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
        for (int k = 0; k <mapInfo[i + 1][j] ; k++)
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
      ROS_INFO("daaaaaaaaaaaaaaaaaaaamn! E size: [%d]",c.getE().size());
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
