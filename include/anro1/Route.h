#ifndef ROUTE_H
#define ROUTE_H
#include "Point.h"

class Route
{
public:
  Route(double x, double y, bool in);
  virtual ~Route();
  Point getOrigin();
protected:
private:
  Point origin;
  bool in;
};

#endif // ROUTE_H#pragma once
