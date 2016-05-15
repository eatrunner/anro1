#ifndef MAPOFCITY_H
#define MAPOFCITY_H

#include "Crossroad.h"
#include "Lane.h"
#include <vector>

class MapOfCity
{
public:
  MapOfCity();
  virtual ~MapOfCity();
  void setLanes(const std::vector<Lane> &lanes);
  std::vector<Lane> getLanes();
  void setMap(const std::vector<std::vector<char> > &map);
  std::vector<std::vector<char> > getMap();
  void setCrossroads(const std::vector<Crossroad> &crossroads);
  std::vector<Crossroad> getCrossroads();
  double getMaxX();
  void setMaxX(double m);
  void createRvizMap();//(std::vector<Lane> lanes,std::vector<Crossroad> crossroads);
  void readMap();
  void sendInfo();
  void generate_lanes();
  void doJob();
  void mapDiagnostics();
protected:
private:

  double maxX;
  std::vector<std::vector<char> > map;
  std::vector<Crossroad> crossroads;
  std::vector<Lane> lanes;
};

#endif // MAPOFCITY#pragma once
