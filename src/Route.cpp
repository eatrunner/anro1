#include "Route.h"

Route::Route(double x, double y, bool in_or_out) : origin(x,y)
{
	//origin = Point(x, y);
	this->in = in_or_out;
}

Route::~Route()
{
	//dtor
}
Point Route::getOrigin()
{
  return origin;
}
