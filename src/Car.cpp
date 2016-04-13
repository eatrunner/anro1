#include "Car.h"
#include <ros/ros.h>
#include <math.h>

void turnsCallback(const anro1::turnsVector::ConstPtr& msg);
void lightsCallback(const anro1::lightsVector::ConstPtr& msg);

Car::Car(int id){
  this->id = id;
  speed = 0.01;
  moving = false;
  scale = 0.25;
}

Car::~Car(){

}

void Car::move(){
  if(moving){
     x += vecX * speed;
     y += vecY * speed;
  }
}

void Car::setMoving(bool move){
    moving = move;
}

bool Car::isMoving(){
    return moving;
}

double Car::getX(){
    return x;}
double Car::getY(){
    return y;
}
int Car::getVecX(){return vecX;}
int Car::getVecY(){return vecY;}
void Car::setX(double x){this->x = x;}
void Car::setY(double y){this->y = y;}
void Car::setVector(int vecX, int vecY){
    this->vecX = vecX;
    this->vecY = vecY;
}
double Car::getScale(){return scale;}

Car car(1);
bool isFirst = true;

int main(int argc, char** argv){
  ros::init(argc, argv, "c");
  ros::NodeHandle nodeHandle;
  //ros::Publisher carPublisher =
  ros::Subscriber turnsSubscriber = nodeHandle.subscribe("turns_info", 1000, turnsCallback);
  //ros::Subscriber lightsSubscriber;
  ros::Rate rate(30);

  while(ros::ok()){
      ros::spinOnce();
      car.move();
  }

  return 0;
}

void turnsCallback(const anro1::turnsVector::ConstPtr& msg){
  std::vector<anro1::turn> turns = msg->turns;
  int vecX;
  int vecY;
  if(isFirst){
      car.setX(turns[0].x);
      car.setY(turns[0].y);
      vecX = 0;
      vecY = turns[0].NS ? 1 : -1;
      isFirst = false;
      car.setVector(vecX,vecY);
      car.setMoving(true);
  }
  double x = car.getX();
  double y = car.getY();
  vecX = car.getVecX();
  vecY = car.getVecY();
  for(int i = 0 ; i < msg->size ; i++){
      if((double)turns[i].x == x && (double)turns[i].y == y){
          if(vecX == 0){
              vecY = 0;
              vecX = (turns[i].WE == true? -1 : 1);
          }
          else if(vecY == 0){
              vecX = 0;
              vecY = turns[i].NS == true ? 1 : -1;
          }
          car.setVector(vecX,vecY);
      }
  }
}

void lightsCallback(const anro1::lightsVector::ConstPtr &msg){
    std::vector<anro1::light> lights = msg->lights;
    double radius = car.getScale();
    double x = car.getX();
    double y = car.getY();
    for(int i = 0 ; i < msg->size ; i++){
        if(y == lights[i].y && (fabs(x - lights[i].x) == radius) && !lights[i].WE){
            car.setMoving(false);
            return;
        }
        else if (x == lights[i].x && (fabs(y - lights[i].y) == radius) && !lights[i].NS){
            car.setMoving(false);
            return;
        }
       else
            car.setMoving(true);
    }
}

