#ifndef LANE_H
#define LANE_H
#include "Point.h"

class Lane
{
public:
  Lane(double x, double y);
  Lane(Point p);
  void setStart(double x, double y);
  void setEnd(double x, double y);
  void setStart(Point p);
  void setEnd(Point p);
  Point getStart();
  Point getEnd();
  virtual ~Lane();
protected:
private:
  Point start;
  Point end;
  char dir;
};

#endif // LANE_H#pragma once
