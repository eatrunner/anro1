#include "Point.h"

Point::Point(double x, double y)
{
	this->x = x;
	this->y = y;
}

Point::~Point()
{
	//dtor
}

double Point::getX()
{
	return x;
}

double Point::getY()
{
	return y;
}
