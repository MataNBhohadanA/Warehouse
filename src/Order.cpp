#include "../include/Order.h"


Order::Order(int id,int customerId,int distance):
        id(id),
        customerId(customerId),
        distance(distance),
        status(OrderStatus::PENDING),
        collectorId(NO_VOLUNTEER), // Ensure this comes before driverId in the header if the warning suggests
        driverId(NO_VOLUNTEER)
{

}

int Order::getId() const{
    return id;
}
int Order::getCustomerId() const{
    return customerId;
}
void Order::setStatus(OrderStatus status){
    this->status=status;

}
void Order::setDriverId(int driverId){
    this->driverId=driverId;

}
int Order::getCollectorId() const{
    return collectorId;

}

int Order::getDistance() const{
    return distance;
}
int Order::getDriverId() const{
    return driverId;

}
OrderStatus Order::getStatus() const{
    return status;

}


const string Order::toString() const{
    string statusStr;
    switch(status){
        case OrderStatus::PENDING:
            statusStr="PENDING";
            break;
        case OrderStatus::COLLECTING:
            statusStr="COLLECTING";
            break;
        case OrderStatus::DELIVERING:
            statusStr="DELIVERING";
            break;
        case OrderStatus::COMPLETED:
            statusStr="COMPLETED";
            break;

    }
    return "Order "+std::to_string(id)+"-Customer "+std::to_string(customerId)
           +" -Distance "+std::to_string(distance)+" -Status "+statusStr;

}
