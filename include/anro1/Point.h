#ifndef POINT_H
#define POINT_H


class Point
{
public:
  Point();
	Point(double x, double y);
	double getX();
	double getY();
  void setCoor(double x, double y);
	virtual ~Point();
	double x, y;
protected:
private:

};

#endif // POINT_H#pragma once

