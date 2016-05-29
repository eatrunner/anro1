#include "Car.h"
#include "boost/assign/std/vector.hpp"
#include "anro1/lightsmsg.h"
#include "anro1/EmergencyService.h"//Emergency
#include "anro1/EmergencyMessage.h"//Emergency

class SideAndPoint{
public:
    int side;
    anro1::point point;
};

enum Side{
    LEFT,STRAIGHT,RIGHT
};

class SideAndPointEnum{
public:
    Side side;
    anro1::point point;
};

class DistanceAndPoint{
public:
    double distance;
    anro1::point point;
};

double eps;

void turnCar(anro1::accessPoint accessPoint);
void checkForTurn(std::vector<anro1::node> nodes);
void nodesCallback(const anro1::nodeMessage::ConstPtr& msg);
void carsCallback(const anro1::car& msg);
SideAndPoint* findSpawn(anro1::node);
std::vector<std::vector<anro1::accessPoint> >  getAccessPoints(anro1::node);
DistanceAndPoint* blindWay(anro1::node node);
void setStartPoint(std::vector<anro1::node>);


Car *car;
bool isFirst = true;
bool initiated = false;

void emergencyMsg(const anro1::EmergencyMessage msg)//Emergency
{
	car->setMoving(msg.move);
	if(msg.move)ROS_INFO("Emergency Start");
	else ROS_INFO("Emergency Stop");
}

bool emergencySrv(anro1::EmergencyService::Request &req,anro1::EmergencyService::Response &res)//Emergency
{
	if(car->getId()==req.id)
	{
		if(!req.move)
		{
			if(req.move==car->isMoving())res.failure=true;
			else ROS_INFO("Emergency Stop");
		}
		else
		{
			if(req.move==car->isMoving())res.failure=true;
			else ROS_INFO("Emergency Start");
		}
		car->setMoving(req.move);
		res.resid=req.id;
	}
	return true;
}

ros::Publisher carPublisher;
int main(int argc, char** argv){
  double speed = 0.005;
  eps = speed * 0.55;
  char c = *argv[1];
  car = new Car((int)c, eps, speed);
  ros::init(argc, argv, std::string(1,c));
  ros::NodeHandle nodeHandle;
  ros::Subscriber nodesSubscriber = nodeHandle.subscribe("nodes_info", 1000, nodesCallback);
  ros::Subscriber carSubscriber = nodeHandle.subscribe("car_info", 1000, carsCallback);
  carPublisher = nodeHandle.advertise<anro1::car>("car_info", 1000);
  ros::Rate loop_rate(RATE*1000);
  ros::ServiceServer emergencyService=nodeHandle.advertiseService(argv[1],emergencySrv);//Emergency
  ros::Subscriber emergencySubscriber = nodeHandle.subscribe("emergency_msg", 1000, emergencyMsg);//Emergency
  srand(time(NULL));

  while(ros::ok()){
      ros::spinOnce();
      loop_rate.sleep();
  }

  return 0;
}

//void turnsCallback(const anro1::turnsVector::ConstPtr& msg){
//  std::vector<anro1::turn> turns = msg->turns;
//  int vecX;
//  int vecY;
//  if(isFirst){
//      ROS_INFO_STREAM(turns[0].x << " FIRST MSG " << turns[0].y);
//      car->setX(turns[0].x);
//      car->setY(turns[0].y);
//      vecX = 0;
//      vecY = turns[0].NS ? 1 : -1;
//      isFirst = false;
//      car->setVector(vecX,vecY);
//      car->setMoving(true);
//      return;
//  }
//  double x = car->getX();
//  double y = car->getY();
//  vecX = car->getVecX();
//  vecY = car->getVecY();
//  for(int i = 0 ; i < turns.size() ; i++){
//      if(car->checkCoordinates(turns[i].x,turns[i].y)){
//          if(vecX == 0){
//              vecY = 0;
//              vecX = (turns[i].WE == true? -1 : 1);
//          }
//          else if(vecY == 0){
//              vecX = 0;
//              vecY = turns[i].NS == true ? 1 : -1;
//          }
//          car->setVector(vecX,vecY);
//      }
//  }
//}

//void lightsCallback(const anro1::lightsVector::ConstPtr &msg){
//    std::vector<anro1::light> lights = msg->lights;
//    double offsetL = 10;
//    double offsetH = 15;
//    for(int i = 0 ; i < lights.size() ; i++){           //Odczytaj wszystkie wiadomosci z wektora
//        if(car->checkCoordinateY(lights[i].y)           //Sprawdz czy swiatlo jest na twojej linii jazdy
//                && isInIterval(offsetL,offsetH,car->getDistanceX(lights[i].x))  //Sprawdz odleglosc od swiatla
//                                                                           //Czy znajduje sie w przedziale
//                && !lights[i].WE){                      //Sprawdz czy swiatla sa czerwone
//            car->setMoving(false);
//            return;
//        }
//        else if (car->checkCoordinateX(lights[i].x)     //Działa analogicznie do powyzszej
//                 && isInIterval(offsetL,offsetH,car->getDistanceY(lights[i].y))
//                 && !lights[i].NS){
//            car->setMoving(false);
//            return;
//        }
//       else if (!(car -> isCarNear()))                  // Jezeli nie ma samochodu obok, to jedz
//            car->setMoving(true);
//    }
//}

//void carsCallback(const anro1::car::ConstPtr &msg){
//    if(msg->id == car->getId())                                 //Jezeli wiadomosc pochodzi od tego samego samochodu, ktorego sterujemy
//        return;
//    if (car->isCarNear() && !(msg->id == car->getCarNearId())){           //Jezeli juz sie zatrzymalismy przez jakis samochod
//        return;                                                 //To sprawdzamy TYLKO wiadomosci od tego samochodu
//    }
//    double distance = -1;
//    if (car->checkCoordinateY(msg->y, car->getScale() * 1.1)){  //wprowadzamy offset aby sie nie zderzaly
//        distance = (msg->x - car->getX()) * car->getVecX();     //przy kacie prostym, obliczamy dystans
//    }                                                           //pomiedzy samochodami
//    else if (car->checkCoordinateX(msg->x, car->getScale() * 1.1)){
//        distance = (msg-> y - car -> getY()) * car->getVecY();
//    }
//    ROS_INFO_STREAM("distance" << distance << " x " << car->getX() << " y " << car->getY());
//    if (distance > 0 && distance <= car->getScale()*1.3){       //Jezeli samochod dostatecznie blisko
//        car->setMoving(false);                                  //Zatrzymujemy go
//        car->setCarNearId(msg->id);                                   //Zapamietujemy, ktory to byl samochod
//        car -> setCarNear(true);
//    }
//    else                                                        //Jezeli nie ma zadnego samochodu w okreslonym dystansie
//        car -> setCarNear(false);
//}

void nodesCallback(const anro1::nodeMessage::ConstPtr &msg){
    // static bool flag = false;
    // static bool flag2 = false;
    // if(!flag || !flag2){
    //     DistanceAndPoint *dap = NULL;
    //     std::vector<anro1::node> nodes = msg->nodes;
    //     for(int i = 0 ; i < nodes.size() ; i++){
    //         anro1::node node = nodes[i];
    //         if(!flag){
    //             SideAndPoint* spawnPoint = findSpawn(node);
    //             if(spawnPoint){
    //                 ROS_INFO("Spawn point found!");
    //                 car->point = spawnPoint->point;
    //                 car->side = spawnPoint->side;
    //                 delete spawnPoint;
    //                 flag = true;
    //                 break;
    //             }
    //         }
    //         if(flag){
    //             if(!dap)
    //                 dap = blindWay(node);
    //             else{
    //                 DistanceAndPoint* temp = blindWay(node);
    //                 if(temp && temp->distance < dap->distance){
    //                     delete dap;
    //                     dap = temp;
    //                 }
    //                 else
    //                     delete temp;
    //             }
    //         }
    //         std::vector<anro1::accessPoint> accessPoints = node.n;
    //         ROS_INFO_STREAM("-------" << i << "----------");
    //         for(int j = 0 ; j < accessPoints.size() ; j++){
    //             anro1::accessPoint accessPoint = accessPoints[j];
    //             ROS_INFO_STREAM(accessPoint.x << " " << accessPoint.y << "access point node n");
    //             std::vector<anro1::point> points = accessPoint.straight;
    //             for(int l = 0 ; l < points.size() ; l++)
    //                 ROS_INFO_STREAM(points[l].x << "  " << points[l].y << "  point straight");
    //             points = accessPoint.left;
    //             for(int l = 0 ; l < points.size() ; l++)
    //                 ROS_INFO_STREAM(points[l].x << "  " << points[l].y << "  point left");
    //             points = accessPoint.right;
    //             for(int l = 0 ; l < points.size() ; l++)
    //                 ROS_INFO_STREAM(points[l].x << "  " << points[l].y << "  point right");

    //         }
    //         accessPoints = node.e;
    //         for(int j = 0 ; j < accessPoints.size() ; j++){
    //             anro1::accessPoint accessPoint = accessPoints[j];
    //             ROS_INFO_STREAM(accessPoint.x << " " << accessPoint.y << "access point node e");
    //             std::vector<anro1::point> points = accessPoint.straight;
    //             for(int l = 0 ; l < points.size() ; l++)
    //                 ROS_INFO_STREAM(points[l].x << "  " << points[l].y << "  point straight");
    //             points = accessPoint.left;
    //             for(int l = 0 ; l < points.size() ; l++)
    //                 ROS_INFO_STREAM(points[l].x << "  " << points[l].y << "  point left");
    //             points = accessPoint.right;
    //             for(int l = 0 ; l < points.size() ; l++)
    //                 ROS_INFO_STREAM(points[l].x << "  " << points[l].y << "  point right");

    //         }
    //         accessPoints = node.s;
    //         for(int j = 0 ; j < accessPoints.size() ; j++){
    //             anro1::accessPoint accessPoint = accessPoints[j];
    //             ROS_INFO_STREAM(accessPoint.x << " " << accessPoint.y << "access point s");
    //             std::vector<anro1::point> points = accessPoint.straight;
    //             for(int l = 0 ; l < points.size() ; l++)
    //                 ROS_INFO_STREAM(points[l].x << "  " << points[l].y << "  point straight");
    //             points = accessPoint.left;
    //             for(int l = 0 ; l < points.size() ; l++)
    //                 ROS_INFO_STREAM(points[l].x << "  " << points[l].y << "  point left");
    //             points = accessPoint.right;
    //             for(int l = 0 ; l < points.size() ; l++)
    //                 ROS_INFO_STREAM(points[l].x << "  " << points[l].y << "  point right");
    //         }
    //         accessPoints = node.w;
    //         for(int j = 0 ; j < accessPoints.size() ; j++){
    //             anro1::accessPoint accessPoint = accessPoints[j];
    //             ROS_INFO_STREAM(accessPoint.x << " " << accessPoint.y << "access point node w");
    //             std::vector<anro1::point> points = accessPoint.straight;
    //             for(int l = 0 ; l < points.size() ; l++)
    //                 ROS_INFO_STREAM(points[l].x << "  " << points[l].y << "  point straight");
    //             points = accessPoint.left;
    //             for(int l = 0 ; l < points.size() ; l++)
    //                 ROS_INFO_STREAM(points[l].x << "  " << points[l].y << "  point left");
    //             points = accessPoint.right;
    //             for(int l = 0 ; l < points.size() ; l++)
    //                 ROS_INFO_STREAM(points[l].x << "  " << points[l].y << "  point right");
    //         }
    //     }
    if (!initiated) {
            setStartPoint(msg->nodes);
            ROS_INFO_STREAM("DAP " << " POINT TO GO " << car->point.x << " " << car->point.y);
            //flag2 = true;
             checkForTurn(msg->nodes);
             ROS_INFO_STREAM("GOT HERE");
            car->setMoving(true);
            initiated = true;
    }

    if (initiated) {
        checkForTurn(msg->nodes);
        car->move();
        carPublisher.publish(car-> getMsg());
    }
}

void checkForTurn(std::vector<anro1::node> nodes){
    for(int i = 0 ; i < nodes.size() ; i++){
        std::vector<std::vector<anro1::accessPoint> > accessPoints = getAccessPoints(nodes[i]);
        std::vector<anro1::accessPoint> accessPointsWithSide = accessPoints[car->side];
        for(int j = 0 ; j < accessPointsWithSide.size() ; ++j){
            double x = fabs(accessPointsWithSide[j].x - car->point.x);
            double y = fabs(accessPointsWithSide[j].y - car->point.y);
            if(x <= eps && y <= eps){ //znaleziono accesspoint
                if(accessPointsWithSide[j].green){
                    turnCar(accessPointsWithSide[j]);
                    car->setMoving(true);
                }
                else{
                    car->setMoving(false);
                }
            }
        }
    }
}

SideAndPoint* findSpawn(anro1::node node){
    std::vector<std::vector<anro1::accessPoint> >  accessPoints = getAccessPoints(node);
    anro1::accessPoint *spawnPoint = NULL;
    int side;
    for(int i = 0 ; i < accessPoints.size(); i++){
        if(accessPoints[i].size() > 0 && spawnPoint)
            return NULL;
        if(accessPoints[i].size() > 0){
            side = i;
            spawnPoint = &accessPoints[i][0];
        }
    }
    if(spawnPoint->left.size() > 0
            || spawnPoint->right.size() > 0
            || spawnPoint->straight.size() > 0)
        return NULL;
    SideAndPoint* sideAndPoint = new SideAndPoint();
    sideAndPoint->side = side;
    sideAndPoint->point.x = spawnPoint->x;
    sideAndPoint->point.y = spawnPoint->y;
    return sideAndPoint;
}

DistanceAndPoint* blindWay(anro1::node node){
    DistanceAndPoint* dap = NULL;
    std::vector<std::vector<anro1::accessPoint> >  accessPoints = getAccessPoints(node);
    for(int l = 0 ; l < accessPoints.size() ; l++){
        std::vector<anro1::accessPoint> accessPointWithSide = accessPoints[l];
        for(int i = 0 ; i < accessPointWithSide.size() ; ++i){
            std::vector<anro1::point> pointsLeft = accessPointWithSide[i].left;
            std::vector<anro1::point> pointsRight = accessPointWithSide[i].right;
            std::vector<anro1::point> pointsStraight = accessPointWithSide[i].straight;
            pointsLeft.insert(pointsLeft.end(), pointsRight.begin(), pointsRight.end());
            pointsLeft.insert(pointsLeft.end(), pointsStraight.begin(), pointsStraight.end());
            for(int j = 0; j < pointsLeft.size() ;j++){
                if(pointsLeft[j].y == car->point.y){
                    if(dap == NULL){
                        dap = new DistanceAndPoint();
                        dap->distance = abs(pointsLeft[j].x - car->point.x);
                        dap->point = pointsLeft[j];
                    }
                    else{
                        double distance = abs(pointsLeft[j].x - car->point.x);
                        if(distance < dap->distance){
                            dap->distance = distance;
                            dap->point = pointsLeft[j];
                        }
                    }
                }
            }
        }
    }
    return dap;
}

std::vector<std::vector<anro1::accessPoint> >  getAccessPoints(anro1::node node){
    std::vector<std::vector<anro1::accessPoint> > accessPoints;
    accessPoints.push_back(node.n);
    accessPoints.push_back(node.e);
    accessPoints.push_back(node.s);
    accessPoints.push_back(node.w);
    return accessPoints;
}

void carsCallback(const anro1::car& msg){
	static int foundCarId = -1;
	
	if ( car->getId() == msg.id)
		return;
		
	if ( (fabs(car -> getX() - msg.x) < 2)  && fabs(car -> getY() == msg.y) && (car -> getVecX() > 0) )
	{
		car->setMoving(false);
		foundCarId = msg.id;
		return;
	}
	
	if ( (fabs(car -> getY() - msg.y) < 2)  && fabs(car -> getX() == msg.x) && (car -> getVecY() > 0))
	{
		car->setMoving(false);
		foundCarId = msg.id;
		return;
	}
	
	if ( msg.id == foundCarId)
		{
			car -> setMoving(true);
			foundCarId = -1;
		}
}

void turnCar(anro1::accessPoint accessPoint){
    std::vector<anro1::point> pointsLeft = accessPoint.left;
    std::vector<anro1::point> pointsRight = accessPoint.right;
    std::vector<anro1::point> pointsStraight = accessPoint.straight;

    std::vector<SideAndPointEnum> sideAndPoints;

    for(int i = 0; i < pointsLeft.size() ; ++i){
        SideAndPointEnum sideAndPoint;
        sideAndPoint.point = pointsLeft[i];
        sideAndPoint.side = LEFT;
        sideAndPoints.push_back(sideAndPoint);
    }

    for(int i = 0; i < pointsRight.size() ; ++i){
        SideAndPointEnum sideAndPoint;
        sideAndPoint.point = pointsRight[i];
        sideAndPoint.side = RIGHT;
        sideAndPoints.push_back(sideAndPoint);
    }

    for(int i = 0; i < pointsStraight.size() ; ++i){
        SideAndPointEnum sideAndPoint;
        sideAndPoint.point = pointsStraight[i];
        sideAndPoint.side = STRAIGHT;
        sideAndPoints.push_back(sideAndPoint);
    }

    int turnIndex = rand() % sideAndPoints.size();

    ROS_INFO_STREAM("ACCP " << accessPoint.x << " " <<accessPoint.y);

    SideAndPointEnum sideAndPoint = sideAndPoints[turnIndex];

    car->setPointToGo(sideAndPoint.point);
    car->isOnCrossroad = true;

    switch(sideAndPoint.side){
    case LEFT:
        if(car->side > 0)
            car->side -= 1;
        else
            car->side = 3;
        break;

    case STRAIGHT:
        break;

    case RIGHT:
        if(car->side < 3)
            car->side += 1;
        else
            car->side = 0;
        break;
    }

}

void setStartPoint(std::vector<anro1::node> nodes) {
    int randomNode = rand() % nodes.size(); //losuje node

    std::vector<anro1::accessPoint> aps; //wektor WSZYSTKICH wjazdow do node
    aps.insert(aps.end(), nodes[randomNode].n.begin(), nodes[randomNode].n.end());
    aps.insert(aps.end(), nodes[randomNode].e.begin(), nodes[randomNode].e.end());
    aps.insert(aps.end(), nodes[randomNode].w.begin(), nodes[randomNode].w.end());
    aps.insert(aps.end(), nodes[randomNode].s.begin(), nodes[randomNode].s.end());

    int randomAP = rand() % aps.size(); //losuje wjazd
    car->point.x = aps[randomAP].x;
    car->point.y = aps[randomAP].y;

    //ustawia side
    if(randomAP < nodes[randomNode].n.size()) {
        //north
        car->setSide(0);
        return;
    }
    if(randomAP < nodes[randomNode].n.size() + nodes[randomNode].e.size()) {
        //east
        car->setSide(1);
        return;
    }
    if(randomAP < nodes[randomNode].n.size() + nodes[randomNode].e.size() + nodes[randomNode].w.size()) {
        //west
        car->setSide(3);
        return;
    }
    if(randomAP < nodes[randomNode].n.size() + nodes[randomNode].e.size() + nodes[randomNode].w.size() + nodes[randomNode].s.size()) {
        //south
        car->setSide(2);
        return;
    }
}
