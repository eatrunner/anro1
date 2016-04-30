#include "Lane.h"
#include "Point.h"


Lane::Lane(double x, double y)
{
  this->start.setCoor(x,y);
}
Lane::Lane(Point p)
{
  this->start.setCoor(p.getX(),p.getY());
}
void Lane::setStart(double x, double y)
{
  this->start.setCoor(x,y);
}
void Lane::setStart(Point p)
{
  this->start.setCoor(p.getX(),p.getY());
}
void Lane::setEnd(double x, double y)
{
  this->end.setCoor(x,y);
}
void Lane::setEnd(Point p)
{
  this->end.setCoor(p.getX(),p.getY());
}
Point Lane::getStart()
{
  return this->start;
}
Point Lane::getEnd()
{
  return this->end;
}
Lane::~Lane()
{
  //dtor
}
