#ifndef ROUTE_H
#define ROUTE_H
#include "Point.h"

class Route
{
public:
  Route(double x, double y, bool in);
  virtual ~Route();
  Point getOrigin();
  bool getIn();
  Point origin;
  bool in;
protected:
private:

};

#endif // ROUTE_H#pragma once
