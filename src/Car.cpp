#include "Car.h"

//void turnsCallback(const anro1::turnsVector::ConstPtr& msg);
//void lightsCallback(const anro1::lightsVector::ConstPtr& msg);
//void carsCallback(const anro1::car::ConstPtr &msg);
void nodesCallback(const anro1::nodeMessage::ConstPtr &msg);

/*
 * Funkcja nie nalezy do klasy Car, sluzy do okreslenia czy dana wartosc
 * znajduje sie w podanym przedziale przez lower i high
 */
bool isInIterval(double lower, double higher, double value){
    return value >= lower && value <= higher;
}

Car::Car(int id, double eps, double speed){
    this->id = id;
    this->speed = speed;
    this->eps = eps;
    moving = false;
    scale = LANE_WIDTH;
    carNear = false;
    carNearId = 0;
    isOnCrossroad = false;
}

Car::~Car(){

}

void Car::setSide(int side){
    switch(side){
    case 0:
        vecX = 0;
        vecY = -1;
        break;
    case 1:
        vecX = -1;
        vecY = 0;
        break;
    case 2:
        vecX = 0;
        vecY = 1;
        break;
    case 3:
        vecX = 1;
        vecY = 0;
        break;
    }
    this->side = side;
}

bool Car::checkCoordinates(double x, double y){
    double xOffset = fabs(this->x - x);
    double yOffset = fabs(this->y - y);
    return xOffset < speed/2 && yOffset < speed/2;
}
bool Car::checkCoordinateX(double x){
    return fabs(this->x - x) < speed / 2;
}
bool Car::checkCoordinateX(double x, double offset){
    return fabs(this->x - x) < offset;
}
bool Car::checkCoordinateY(double y){
    return fabs(this->y - y) < speed / 2;
}
bool Car::checkCoordinateY(double y, double offset){
    return fabs(this->y - y) < offset;
}
void Car::move(){
    if(moving){
        point.x += vecX * speed;
        point.y += vecY * speed;
        //ROS_INFO_STREAM(point.x << " " << point.y);
    }
    if(isOnCrossroad){
        double offsetX = fabs(pointToGo.x - point.x);
        double offsetY = fabs(pointToGo.y - point.y);
        if(offsetX < eps && offsetY < eps){
            isOnCrossroad = false;
            setSide(side); //modyfikacja wektora
        }
    }
}
double Car::getDistanceX(double x){
    return (x - this->x) * this->vecX;
}
double Car::getDistanceY(double y){
    return (y - this->y) * this->vecY;
}

anro1::car Car::getMsg(){
    anro1::car carMsg;
    carMsg.x = point.x;
    carMsg.y = point.y;
    carMsg.id = id;
    carMsg.scale = scale;
    carMsg.orientation = getOrientation();
    carMsg.moving = moving;
    return carMsg;
}

double Car::getOrientation(){
    double orientVecX = 1;
    double orientVecY = 0;
    double num = vecX * orientVecX + vecY * orientVecY; //licznik wyrazenia
    double den = sqrt(vecX*vecX + vecY*vecY) * sqrt(orientVecX * orientVecX + orientVecY * orientVecY);
    //ROS_INFO_STREAM("COS = " << acos(num/den));
    return acos(num/den);
}

void Car::setPointToGo(anro1::point pointToGo){
    this->pointToGo = pointToGo;
    ROS_INFO_STREAM(pointToGo.x << " point to go " << pointToGo.y);
    vecX = pointToGo.x - point.x;
    vecY = pointToGo.y - point.y;
    double vecLength = sqrt(vecX * vecX + vecY * vecY);
    vecX /= vecLength;
    vecY /= vecLength;
}
