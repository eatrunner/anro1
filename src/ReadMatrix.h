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
	static void buildInfo(std::vector<std::vector<char>> mapInfo);
	static std::vector<Crossroad> crossroadsNorth, crossroadsSouth, crossroadsEast, crossroadsWest;
	static void sendRouteInfo();
protected:
private:
};

#endif // READMATRIX_H
