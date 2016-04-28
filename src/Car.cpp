#include "Car.h"

void turnsCallback(const anro1::turnsVector::ConstPtr& msg);
void lightsCallback(const anro1::lightsVector::ConstPtr& msg);
void carsCallback(const anro1::car::ConstPtr &msg);
bool isInIterval(double lower, double higher, double value){
    return value >= lower && value <= higher;
}

Car::Car(int id){
  this->id = id;
  speed = 2.5;
  moving = false;
  scale = 8;
  carNear = false;
}

Car::~Car(){

}

bool Car::checkCoordinates(double x, double y){
  double xOffset = fabs(this->x - x);
  double yOffset = fabs(this->y - y);
  return xOffset < speed && yOffset < speed;
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
  ROS_INFO_STREAM(x << "  " << y);
  if(moving){
     x += vecX * speed;
     y += vecY * speed;
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
    carMsg.x = x;
    carMsg.y = y;
    carMsg.id = id;
    carMsg.scale = scale;
    return carMsg;
}

Car *car;
bool isFirst = true;
int carNearId = 0;

int main(int argc, char** argv){
  char c = *argv[1];
  car = new Car((int)c);
  ros::init(argc, argv, std::string(1,c));
  ros::NodeHandle nodeHandle;
  ros::Publisher carPublisher = nodeHandle.advertise<anro1::car>("car_info", 1000);
  ros::Subscriber carSubscriber = nodeHandle.subscribe("car_info", 1000, carsCallback);
  ros::Subscriber turnsSubscriber = nodeHandle.subscribe("turns_info", 1000, turnsCallback);
  ros::Subscriber lightsSubscriber = nodeHandle.subscribe("lights_info", 1000, lightsCallback);
  ros::Rate rate(20);

  while(ros::ok()){
      ros::spinOnce();
      car->move();
      carPublisher.publish(car-> getMsg());
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
      car->setX(turns[0].x);
      car->setY(turns[0].y);
      vecX = 0;
      vecY = turns[0].NS ? 1 : -1;
      isFirst = false;
      car->setVector(vecX,vecY);
      car->setMoving(true);
      return;
  }
  double x = car->getX();
  double y = car->getY();
  vecX = car->getVecX();
  vecY = car->getVecY();
  for(int i = 0 ; i < turns.size() ; i++){
      if(car->checkCoordinates(turns[i].x,turns[i].y)){
          if(vecX == 0){
              vecY = 0;
              vecX = (turns[i].WE == true? -1 : 1);
          }
          else if(vecY == 0){
              vecX = 0;
              vecY = turns[i].NS == true ? 1 : -1;
          }
          car->setVector(vecX,vecY);
      }
  }
}

void lightsCallback(const anro1::lightsVector::ConstPtr &msg){
    std::vector<anro1::light> lights = msg->lights;
    double offsetL = 10;
    double offsetH = 15;
    for(int i = 0 ; i < lights.size() ; i++){           //Odczytaj wszystkie wiadomosci z wektora
        ROS_INFO_STREAM("distance" << car->getDistanceX(lights[i].x));
        if(car->checkCoordinateY(lights[i].y)           //Sprawdz czy swiatlo jest na twojej linii jazdy
                && isInIterval(offsetL,offsetH,car->getDistanceX(lights[i].x))  //Sprawdz odleglosc od swiatla
                                                                           //Czy znajduje sie w przedziale
                && !lights[i].WE){                      //Sprawdz czy swiatla sa czerwone
            car->setMoving(false);
            return;
        }
        else if (car->checkCoordinateX(lights[i].x)     //Działa analogicznie do powyzszej
                 && isInIterval(offsetL,offsetH,car->getDistanceY(lights[i].y))
                 && !lights[i].NS){
            car->setMoving(false);
            return;
        }
       else if (!(car -> isCarNear()))                  // Jezeli nie ma samochodu obok, to jedz
            car->setMoving(true);
    }
}

void carsCallback(const anro1::car::ConstPtr &msg){
    if(msg->id == car->getId())     //Jezeli wiadomosc pochodzi od tego samego samochodu, ktorego sterujemy
        return;
    if (car->isCarNear() && !(msg->id == carNearId)){ //Jezeli juz sie zatrzymalismy przez jakis samochod
        return;                                           //To sprawdzamy TYLKO wiadomosci od tego samochodu
    }
    double distance = 0;
    if (car->getVecY() == 0 && car->checkCoordinateY(msg->y, car->getScale())){
        distance = (msg->x - car->getX()) * car->getVecX();
    }
    else if (car -> getVecX() == 0 && car->checkCoordinateX(msg->x, car->getScale())){
        distance = (msg-> y - car -> getY()) * car->getVecY();
    }
    if (distance > 0 && distance <= car->getScale()*1.5){
        car->setMoving(false);
        carNearId = msg -> id;
        car -> setCarNear(true);
    }
    else{
        car -> setCarNear(false);
    }
}
