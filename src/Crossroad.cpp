#include "Crossroad.h"

Crossroad::Crossroad(double x, double y) : origin(x,y)
{
	routes = std::vector<Route>();
	//this->origin =  Point(x, y);
}

Crossroad::~Crossroad()
{
	//dtor
}


void Crossroad::addNewRoute(double x, double y, bool in)
{
	routes.push_back(Route(x, y, in));
}
