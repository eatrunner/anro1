#ifndef CAR_H
#define CAR_H

#include "anro1/light.h"
#include "anro1/lightsVector.h"
#include "anro1/turn.h"
#include "anro1/turnsVector.h"
#include "anro1/car.h"

class Car{
public:
  Car(int id);
  ~Car();
  void move();
  void setVector(int vecX, int vecY);
  double getX();
  double getY();
  void setX(double x);
  void setY(double y);
  int getVecX();
  int getVecY();
  double getScale();
  bool isMoving();
  void setMoving(bool move);
  bool checkCoordinates(double x, double y);
  anro1::car getMsg();

private:
  double x;
  double y;
  int id;
  double speed;
  double scale;
  int vecX;
  int vecY;
  bool moving;
};

#endif
