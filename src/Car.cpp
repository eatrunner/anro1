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

bool Car::checkCoordinates(double x, double y){
  double xOffset = fabs(this->x - x);
  double yOffset = fabs(this->y - y);
  return xOffset < speed / 2 && yOffset < speed / 2;
}

bool Car::checkCoordinateX(double x){
  return fabs(this->x - x) < speed / 2;
}

bool Car::checkCoordinateY(double y){
  return fabs(this->y - y) < speed / 2;
}

void Car::move(){
  ROS_INFO_STREAM(x << "  " << y);
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
anro1::car Car::getMsg(){
    anro1::car carMsg;
    carMsg.x = x;
    carMsg.y = y;
    carMsg.id = id;
    carMsg.scale = scale;
    return carMsg;
}

bool Car::checkDistanceX(double x){
    double offset = fabs(this->x - x);
    return offset < (scale / 2 + speed / 2) && offset > (scale / 2 - speed / 2);
}

bool Car::checkDistanceY(double y){
    double offset = fabs(this->y - y);
    return offset < (scale / 2 + speed / 2) && offset > (scale / 2 - speed / 2);
}


Car car(1);
bool isFirst = true;

int main(int argc, char** argv){
  ros::init(argc, argv, "c");
  ros::NodeHandle nodeHandle;
  ros::Publisher carPublisher = nodeHandle.advertise<anro1::car>("car_info", 1000);
  ros::Subscriber turnsSubscriber = nodeHandle.subscribe("turns_info", 1000, turnsCallback);
  ros::Subscriber lightsSubscriber = nodeHandle.subscribe("lights_info", 1000, lightsCallback);
  ros::Rate rate(10);

  while(ros::ok()){
      ros::spinOnce();
      car.move();
      carPublisher.publish(car.getMsg());
      rate.sleep();
  }

  return 0;
}

void turnsCallback(const anro1::turnsVector::ConstPtr& msg){
  std::vector<anro1::turn> turns = msg->turns;
  int vecX;
  int vecY;
  if(isFirst){
      ROS_INFO_STREAM(turns[0].x << " FIRST MSG " << turns[0].y);
      car.setX(turns[0].x);
      car.setY(turns[0].y);
      vecX = 0;
      vecY = turns[0].NS ? 1 : -1;
      isFirst = false;
      car.setVector(vecX,vecY);
      car.setMoving(true);
      return;
  }
  double x = car.getX();
  double y = car.getY();
  vecX = car.getVecX();
  vecY = car.getVecY();
  for(int i = 0 ; i < turns.size() ; i++){
      if(car.checkCoordinates(turns[i].x,turns[i].y)){
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
    for(int i = 0 ; i < lights.size() ; i++){
        if(car.checkCoordinateY(lights[i].y) && car.checkDistanceX(lights[i].x) && !lights[i].WE){ // swiatlo na osi x
            car.setMoving(false);
            return;
        }
        else if (car.checkCoordinateX(lights[i].x) && car.checkDistanceY(lights[i].y) && !lights[i].NS){
            car.setMoving(false);
            return;
        }
       else
            car.setMoving(true);
    }
}//

