#include "ReadMatrix.h"
#include "ros/ros.h"
#include <stdio.h>
//#include "anro1/routeMessage.h"
#include <cstdlib>
#include "Crossroad.h"
#include "Constants.h"
std::vector<Crossroad> crossroads;

static double maxX = 0;
ReadMatrix::ReadMatrix()
{

  //crossroads = new
}
ReadMatrix::~ReadMatrix()
{
  //dtor
}
double ReadMatrix::getMaxX()
{
  return maxX;
}
std::vector<Crossroad> ReadMatrix::buildInfo(std::vector<std::vector<char> > mapInfo)
{
  const double xRatio = MATRIX_DISTANCE;
  const double yRatio = MATRIX_DISTANCE;
  bool in;
  double x, y,xRoute, yRoute;
  double verSize = mapInfo.size();
  double horSize = mapInfo.size();

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

      if (x>maxX)
        maxX = x;

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
            c.addNewRoute(xRoute, yRoute, false,'W');
          }
          for (int k = 1; k <= lane_in; k++)
          {
            yRoute = y - (0.5*LANE_WIDTH + (k-1)*LANE_WIDTH);
            c.addNewRoute(xRoute, yRoute, true,'W');
          }

        }
      }

      /*POLNOC*/
      if (i != 0)
      {
        int lane_num = mapInfo[i- 1][j];
        int lane_in = mapInfo[i- 2][j];
        int lane_out = lane_num - lane_in;
        yRoute = y + 2*LANE_WIDTH;
        if(lane_num!=0)
        {
          for (int k = 1; k <= lane_in; k++)
          {
            xRoute = x - (0.5*LANE_WIDTH + (k-1)*LANE_WIDTH);
            c.addNewRoute(xRoute, yRoute, true,'N');
          }
          for (int k = 1; k <= lane_out; k++)
          {
            xRoute = x + (0.5*LANE_WIDTH + (k-1)*LANE_WIDTH);
            c.addNewRoute(xRoute, yRoute, false,'N');
          }


        }
      }

      /*POLUDNIE*/
      if (i != mapInfo.size() - 1)
      {
        int lane_num = mapInfo[i+1][j];
        int lane_in = mapInfo[i+2][j];
        int lane_out = lane_num - lane_in;
        yRoute = y - 2*LANE_WIDTH;
        if(lane_num!=0)
        { for (int k = 1; k <= lane_out; k++)
          {
            xRoute = x - (0.5*LANE_WIDTH + (k-1)*LANE_WIDTH);
            c.addNewRoute(xRoute, yRoute, false,'S');
          }
          for (int k = 1; k <= lane_in; k++)
          {
            xRoute = x + (0.5*LANE_WIDTH + (k-1)*LANE_WIDTH);
            c.addNewRoute(xRoute, yRoute, true,'S');
          }

        }
      }crossroads.push_back(c);
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
