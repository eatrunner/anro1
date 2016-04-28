#ifndef CROSSROAD_H
#define CROSSROAD_H
#include "Point.h"
#include "Route.h"
#include <vector>
class Crossroad
{
public:
	Crossroad(double x, double y);
	void addNewRoute(double x, double y, bool in);
	virtual ~Crossroad();
protected:
private:
	Point origin;
	std::vector<Route> routes ;

};

#endif // CROSSROAD_H#pragma once

