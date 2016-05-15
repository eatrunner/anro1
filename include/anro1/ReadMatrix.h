#ifndef READMATRIX_H
#define READMATRIX_H
#include <vector>
#include "Route.h"
#include "Crossroad.h"
class ReadMatrix
{
public:
	ReadMatrix();
	virtual ~ReadMatrix();
  //static std::vector<Crossroad> matrix_crossroads;
  static std::vector<Crossroad> &buildInfo(std::vector<std::vector<char> > mapInfo);
  //static void sendRouteInfo();
  static double getMaxX();
protected:
private:
};

#endif // READMATRIX_H
