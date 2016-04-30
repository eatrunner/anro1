#include "Crossroad.h"

Crossroad::Crossroad(double x, double y) : origin(x,y)
{
  this->E = std::vector<Route>();
  this->S = std::vector<Route>();
  this->W = std::vector<Route>();
  this->N = std::vector<Route>();
  //origin =  Point(x, y);
}

Crossroad::~Crossroad()
{
  //dtor
}
Point Crossroad::getOrigin()
{
  return origin;
}
std::vector<Route> Crossroad::getN()
{
  return N;
}
std::vector<Route> Crossroad::getE()
{
  return E;
}
std::vector<Route> Crossroad::getS()
{
  return S;
}
std::vector<Route> Crossroad::getW()
{
  return W;
}
void Crossroad::addNewRoute(double x, double y, bool in, char symb)
{
  if(symb == 'N')
    N.push_back(Route(x, y, in));
  else if(symb == 'E')
    E.push_back(Route(x, y, in));
  else if(symb == 'S')
    S.push_back(Route(x, y, in));
  else if(symb == 'W')
    W.push_back(Route(x, y, in));
}
