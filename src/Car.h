#ifndef CAR_H
#define CAR_H

#include "anro1/light.h"
#include "anro1/lightsVector.h"
#include "anro1/turn.h"
#include "anro1/turnsVector.h"
#include "anro1/car.h"
#include <ros/ros.h>
#include <math.h>
#include "anro1/Constants.h"
#include "anro1/accessPoint.h"
#include "anro1/node.h"
#include "anro1/nodeMessage.h"
#include "anro1/point.h"


class Car{
public:
    Car(int id, double eps, double speed);
    ~Car();
    void move();
    bool checkCoordinates(double x, double y);
    bool checkCoordinateX(double x);
    bool checkCoordinateX(double x, double y);
    bool checkCoordinateY(double x, double y);
    bool checkCoordinateY(double y);
    double getDistanceX(double x);
    double getDistanceY(double y);
    double getOrientation();
    anro1::car getMsg();

    void setPointToGo(anro1::point);

    /***********************************************************************
   *
   * Getters, setters
   *
   * ********************************************************************/

    void setVector(int vecX, int vecY){this->vecX = vecX; this->vecY = vecY;}
    double getX(){return x;}
    void setX(double x){this->x = x;}
    int getVecX(){return vecX;}
    double getY(){return y;}
    void setY(double y){this->y = y;}
    int getVecY(){return vecY;}
    double getScale(){return scale;}
    bool isMoving(){return moving;}
    void setMoving(bool move){this->moving = move;}
    void setCarNear(bool isCar){carNear = isCar;}
    bool isCarNear(){return carNear;}
    int getId(){return id;}
    void setCarNearId(int carNearId){this->carNearId = carNearId;}
    int getCarNearId(){return carNearId;}
    void setSide(int side);

    anro1::point point;
    double vecX;
    double vecY;
    int side;
    anro1::point pointToGo;
    bool isOnCrossroad;
    double speed;

private:
    double x;
    double y;
    char id;
    double scale;
    bool moving;
    bool carNear;
    int carNearId;
    double eps;
};

#endif
