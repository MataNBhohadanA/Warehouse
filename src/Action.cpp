#include "../include/Action.h"
#include "../include/WareHouse.h"
#include "../include/Volunteer.h"
#include "../include/Customer.h"
#include <stdexcept>
BaseAction::BaseAction():errorMsg(""),status(ActionStatus::ERROR){}

ActionStatus BaseAction::getStatus() const{
    return status;

}
void BaseAction::complete(){
    status=ActionStatus::COMPLETED;

}
void BaseAction::error(string errorMsg){
    status=ActionStatus::ERROR;
    this->errorMsg=std::move(errorMsg);

}
string BaseAction::getErrorMsg() const{
    return errorMsg;
}
SimulateStep::SimulateStep(int numOfSteps):numOfSteps(numOfSteps){}

void SimulateStep::act(WareHouse &wareHouse){
    for(Order *order:wareHouse.getPendingOrders()){
        Volunteer *ptr=nullptr;
        switch (order->getStatus())
        {
            case OrderStatus::PENDING:
                ptr=wareHouse.findAvailableCollectorVolunteer();
                ptr->acceptOrder((*order));
                order->setStatus(OrderStatus::COLLECTING);
                wareHouse.addInProcessOrder(order);
                break;
            case OrderStatus::COLLECTING:
                ptr=wareHouse.findAvailableDriverVolunteer();
                ptr->acceptOrder((*order));
                order->setStatus(OrderStatus::DELIVERING);
                wareHouse.addInProcessOrder(order);
                break;
            default:
                break;
        }
    }
    wareHouse.stepAllVolunteers();
    for(Volunteer *volunteer:wareHouse.getVolunteers()){
        if(volunteer->getCompletedOrderId()!=NO_ORDER){
            Order& completedOrder=wareHouse.getOrder(volunteer->getCompletedOrderId());
            if(completedOrder.getStatus()==OrderStatus::DELIVERING){
                completedOrder.setStatus(OrderStatus::COMPLETED);
                wareHouse.addCompletedOrder(&completedOrder);
            }else{
                wareHouse.addToPendingOrder(&completedOrder);
            }

        }
    }

    wareHouse.removeMaxedOutVolunteers();
}

string SimulateStep::toString() const{
    return "Simulate "+std::to_string(numOfSteps)+" steps";
}

SimulateStep *SimulateStep::clone() const{
    return new SimulateStep(*this);
}

AddOrder::AddOrder(int id):customerId(id){}

void AddOrder::act(WareHouse &wareHouse) {
    try {

        Customer &customer = wareHouse.getCustomer(customerId);
        if (!customer.canMakeOrder()) {
            error("Cannot place this order: Customer has reached max orders limit.");
            return;
        }
        int orderId = wareHouse.generateUniqueOrderId();

        // Create a new Order with PENDING status
        Order* newOrder = new Order(orderId, customerId,customer.getCustomerDistance());

        // Add the new Order to the pendingOrders list in WareHouse
        wareHouse.addToPendingOrder(newOrder);

        // Update customer's order list
        customer.addOrder(orderId);

        // Mark this action as completed
        complete();
    } catch (const std::out_of_range& e) {
        // Handle case where customer ID doesn't exist
        error("Cannot place this order: Customer ID does not exist.");
    }
}

string AddOrder::toString() const{
    return "Add order for customer "+std::to_string(customerId);

}

AddOrder *AddOrder::clone() const{
    return new AddOrder(*this);

}



AddCustomer::AddCustomer(const string &customerName, const string &customerType, int distance, int maxOrders)
        : customerName(customerName),
          customerType(customerType == "Soldier" ? CustomerType::Soldier : CustomerType::Civilian),
          distance(distance),
          maxOrders(maxOrders)
{

}

void AddCustomer::act(WareHouse &wareHouse) {
    std::string customerTypeStr;
    switch (customerType) {
        case CustomerType::Soldier:
            customerTypeStr = "Soldier";
            break;
        case CustomerType::Civilian:
            customerTypeStr = "Civilian";
            break;
        default:
            error("Invalid customer type");
            return;
    }

    wareHouse.addCustomer(customerName, customerTypeStr, distance, maxOrders);
    complete();
}

string AddCustomer::toString() const {
    std::string customerTypeStr;
    switch (customerType) {
        case CustomerType::Soldier:
            customerTypeStr = "Soldier";
            break;
        case CustomerType::Civilian:
            customerTypeStr = "Civilian";
            break;


    }
    return "Add " + customerTypeStr +
           " Customer: " + customerName;
}

AddCustomer *AddCustomer::clone() const{
    return new AddCustomer(*this);

}

PrintOrderStatus::PrintOrderStatus(int id):orderId(id){}

void PrintOrderStatus::act(WareHouse &WareHouse){
    WareHouse.printOrderStatus(orderId);
    complete();

}
string PrintOrderStatus::toString() const{
    return "Print Order Status for Order"+std::to_string(orderId);

}

PrintOrderStatus *PrintOrderStatus::clone() const{
    return new PrintOrderStatus(*this);

}

PrintCustomerStatus::PrintCustomerStatus(int customerId):customerId(customerId){}

void PrintCustomerStatus::act(WareHouse &wareHouse){
    wareHouse.printCustomerStatus(customerId);
    complete();
}

string PrintCustomerStatus::toString() const{
    return "Print Customer Status for Customer "+std::to_string(customerId);
}

PrintCustomerStatus *PrintCustomerStatus::clone() const{
    return new PrintCustomerStatus(*this);

}

PrintVolunteerStatus::PrintVolunteerStatus(int id):volunteerId(id){}

void PrintVolunteerStatus::act(WareHouse &wareHouse){
    wareHouse.printVolunteerStatus(volunteerId);
    complete();

}

string PrintVolunteerStatus::toString() const{
    return "Print Volunteer Status for Volunteer "+std::to_string(volunteerId);
}

PrintVolunteerStatus *PrintVolunteerStatus::clone() const{
    return new PrintVolunteerStatus(*this);

}

PrintActionsLog::PrintActionsLog(){}

void PrintActionsLog::act(WareHouse &WareHouse){
    WareHouse.printActionLog();
    complete();

}

string PrintActionsLog::toString() const{
    return "Print Action Log";
}

PrintActionsLog *PrintActionsLog::clone() const {
    return new PrintActionsLog(*this);
}


Close::Close(){}

void Close::act(WareHouse &wareHouse){
    wareHouse.close();
    complete();
}

string Close::toString() const{
    return "Close Warehouse";
}
Close *Close::clone() const{
    return new Close(*this);
}


BackupWareHouse::BackupWareHouse(){}

void BackupWareHouse::act(WareHouse &wareHouse){
    wareHouse.BackupWareHouse();
    complete();
}

string BackupWareHouse::toString() const{
    return "BackUP Warehouse";

}

BackupWareHouse *BackupWareHouse::clone() const{
    return new BackupWareHouse(*this);
}


RestoreWareHouse::RestoreWareHouse() {}

void RestoreWareHouse::act(WareHouse &wareHouse){
    wareHouse.RestoreWareHouse();
    complete();
}

string RestoreWareHouse::toString() const{
    return "Restore WareHouse";

}

RestoreWareHouse *RestoreWareHouse::clone() const{
    return new RestoreWareHouse(*this);

}