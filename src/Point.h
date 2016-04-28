#ifndef POINT_H
#define POINT_H


class Point
{
public:
	Point(double x, double y);
	double getX();
	double getY();
	virtual ~Point();
protected:
private:
	double x, y;
};

#endif // POINT_H#pragma once

