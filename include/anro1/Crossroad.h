#ifndef CROSSROAD_H
#define CROSSROAD_H

#include "Point.h"
#include "Route.h"
#include <vector>

class Crossroad
{
public:
	Crossroad(double x, double y);
  void addNewRoute(double x, double y, bool in,char symb);
	virtual ~Crossroad();
  Point getOrigin();
  std::vector<Route> getN();
  std::vector<Route> getE();
  std::vector<Route> getS();
  std::vector<Route> getW();
//  int getMaxJ();
//  void setMaxJ(int j);
  Point origin;
  //d maxj;
  std::vector<Route> N;
  std::vector<Route> E;
  std::vector<Route> S;
  std::vector<Route> W;
protected:
private:

};

#endif // CROSSROAD_H#pragma once

