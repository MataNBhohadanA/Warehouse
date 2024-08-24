#include <vector>
using namespace std;

#include "../include/Volunteer.h"
#include <iostream>
#include <string>


Volunteer::Volunteer(int id, const std::string &name)
        : completedOrderId(NO_ORDER), activeOrderId(NO_ORDER), id(id), name(name) {}

int Volunteer::getId() const {

    return id;
}

const string &Volunteer::getName() const{
    return name;
}

int Volunteer::getActiveOrderId() const{
    return activeOrderId;

}

int Volunteer::getCompletedOrderId() const{
    return completedOrderId;
}
bool Volunteer::isBusy( ) const{
    return activeOrderId!=NO_ORDER;

}
// Collector Volunteer implementation
CollectorVolunteer::CollectorVolunteer(int id,const string &name,int coolDown)\
:Volunteer(id,name),coolDown(coolDown),timeLeft(0)
{

}

void CollectorVolunteer::step(){
    if(timeLeft>0){
        timeLeft--;

        if(timeLeft==0){
            completedOrderId=activeOrderId;
            activeOrderId=NO_ORDER;

        }

    }

}



int CollectorVolunteer::getCoolDown() const{
    return coolDown;

}

int CollectorVolunteer::getTimeLeft() const{
    return timeLeft;
}
bool CollectorVolunteer::decreaseCoolDown(){
    if(timeLeft>0){
        timeLeft--;
        return timeLeft==0;
    }
    return false;

}
bool CollectorVolunteer::hasOrdersLeft() const{
    return true;

}
bool CollectorVolunteer::canTakeOrder(const Order &order) const{
    return !isBusy();

}
void CollectorVolunteer::acceptOrder(const Order &order){
    activeOrderId=order.getId();
    timeLeft=coolDown;

}

string CollectorVolunteer::toString() const{
    return "Collector Volunteer "+this->getName();

}

//Limited Collector Volunteer


LimitedCollectorVolunteer::LimitedCollectorVolunteer(int id,const string &name,int coolDown
        ,int maxOrders):CollectorVolunteer(id,name,coolDown),maxOrders(maxOrders),ordersLeft(maxOrders){}


LimitedCollectorVolunteer* LimitedCollectorVolunteer::clone()const{
    return new LimitedCollectorVolunteer(*this);

}

bool LimitedCollectorVolunteer::hasOrdersLeft() const{
    return ordersLeft>0;

}

bool LimitedCollectorVolunteer::canTakeOrder(const Order &order) const{
    return ordersLeft>0 && CollectorVolunteer::canTakeOrder(order);

}

void LimitedCollectorVolunteer::acceptOrder(const Order &order){
    CollectorVolunteer::acceptOrder(order);
    ordersLeft--;

}

int LimitedCollectorVolunteer::getMaxOrders() const{
    return maxOrders;

}
int LimitedCollectorVolunteer::getNumOrdersLeft() const
{
    return ordersLeft;

}

string LimitedCollectorVolunteer::toString() const{
    return "LimitedCollectorVolunteer "+this->getName();

}


//Driver Volunteer implementation


DriverVolunteer::DriverVolunteer(int id,const string &name, int maxDistance,int distancePerStep)
        :Volunteer(id,name) ,maxDistance(maxDistance),distancePerStep(distancePerStep),distanceLeft(0){}

DriverVolunteer* DriverVolunteer::clone()const{
    return new DriverVolunteer(*this);

}


int DriverVolunteer::getDistanceLeft() const{
    return distanceLeft;

}

int DriverVolunteer::getMaxDistance() const{
    return maxDistance;

}

int DriverVolunteer::getDistancePerStep() const {
    return distancePerStep;
}
bool DriverVolunteer::decreaseDistanceLeft(){
    if(distanceLeft>0){
        distanceLeft-=distancePerStep;
        return distanceLeft<=0;
    }
    return false;

}

bool DriverVolunteer::hasOrdersLeft() const{

    return true;
}

bool DriverVolunteer::canTakeOrder(const Order &order) const{

    return !isBusy() && order.getDistance()<=maxDistance;
}

void DriverVolunteer::acceptOrder(const Order &order){
    activeOrderId=order.getId();
    distanceLeft=order.getDistance();


}


void DriverVolunteer::step(){
    if(distanceLeft>0){
        distanceLeft-=distancePerStep;
        if(distanceLeft<=0){
            completedOrderId=activeOrderId;
            activeOrderId=NO_ORDER;

        }
    }

}

string DriverVolunteer::toString() const{
    return "DriverVolunteer "+this->getName();

}


LimitedDriverVolunteer::LimitedDriverVolunteer(int id, const string &name,
                                               int maxDistance,int distancePerStep,int maxOrders):DriverVolunteer(id,name,maxDistance,distancePerStep),
                                                                                                  maxOrders(maxOrders),ordersLeft(maxOrders){

}

LimitedDriverVolunteer* LimitedDriverVolunteer::clone() const{
    return new LimitedDriverVolunteer(*this);
}

int LimitedDriverVolunteer::getMaxOrders() const{
    return maxOrders;
}

int LimitedDriverVolunteer::getNumOrdersLeft() const{
    return ordersLeft;
}
bool LimitedDriverVolunteer::hasOrdersLeft() const{
    return ordersLeft>0;
}

bool LimitedDriverVolunteer::canTakeOrder(const Order &order)const{
    return ordersLeft>0 && DriverVolunteer::canTakeOrder(order);

}


void LimitedDriverVolunteer::acceptOrder(const Order &order){
    DriverVolunteer::acceptOrder(order);
    ordersLeft--;
}

string LimitedDriverVolunteer::toString() const{
    return "LimitedDriverVolunteer "+this->getName();

}


Volunteer::~Volunteer() {
  
}


CollectorVolunteer::~CollectorVolunteer() {
   
}

LimitedCollectorVolunteer::~LimitedCollectorVolunteer() {
    
}

DriverVolunteer::~DriverVolunteer() {
   
}

LimitedDriverVolunteer::~LimitedDriverVolunteer() {
  
}