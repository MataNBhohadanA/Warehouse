#include "../include/WareHouse.h"
#include "../include/Customer.h"
#include "../include/Volunteer.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include "../include/Action.h"
#include "../include/Volunteer.h"
#include "../include/Order.h"
#include "../include/Customer.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

using namespace std;

extern WareHouse* backup;

WareHouse::WareHouse(const std::string &configFilePath)
        : isOpen(true), actionsLog(), volunteers(), pendingOrders(), inProcessOrders(), completedOrders(), customers(),
          customerCounter(0), volunteerCounter(0), orderCounter(0) {

    std::ifstream configFile(configFilePath);
    if (!configFile.is_open()) {
        throw std::runtime_error("Unable to open configuration file: " + configFilePath);
    }

    std::string line;
    while (getline(configFile, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;
        if (prefix == "customer") {
            std::string name, type;
            int distance, maxOrders;
            iss >> name >> type >> distance >> maxOrders;

            if (type == "soldier") {
                customers.push_back(new SoldierCustomer(++customerCounter, name, distance, maxOrders));
            } else if (type == "civilian") {
                customers.push_back(new CivilianCustomer(++customerCounter, name, distance, maxOrders));
            }
        } else if (prefix == "volunteer") {
            std::string name, role;
            int coolDownOrMaxDistance, distancePerStep = 0, maxOrders = -1; // default values
            iss >> name >> role;
            if (role == "collector") {
                iss >> coolDownOrMaxDistance;
                volunteers.push_back(new CollectorVolunteer(++volunteerCounter, name, coolDownOrMaxDistance));
            } else if (role == "limited_collector") {
                iss >> coolDownOrMaxDistance >> maxOrders;
                volunteers.push_back(new LimitedCollectorVolunteer(++volunteerCounter, name, coolDownOrMaxDistance, maxOrders));
            } else if (role == "driver") {
                iss >> coolDownOrMaxDistance >> distancePerStep;
                volunteers.push_back(new DriverVolunteer(++volunteerCounter, name, coolDownOrMaxDistance, distancePerStep));
            } else if (role == "limited_driver") {
                iss >> coolDownOrMaxDistance >> distancePerStep >> maxOrders;
                volunteers.push_back(new LimitedDriverVolunteer(++volunteerCounter, name, coolDownOrMaxDistance, distancePerStep, maxOrders));
            }
        }
    }

    configFile.close();
}





void WareHouse::addCustomer(const std::string &customerName, const std::string &customerType, int distance, int maxOrders) {
    if (customerType == "Soldier") {
        customers.push_back(new SoldierCustomer(customerCounter++, customerName, distance, maxOrders));
    } else if (customerType == "Civilian") {
        customers.push_back(new CivilianCustomer(customerCounter++, customerName, distance, maxOrders));
    } else {
        // Handle the case where the customer type is neither Soldier nor Civilian
        throw std::invalid_argument("Invalid customer type");
    }
}


void WareHouse::start() {
    for (BaseAction *action : actionsLog) {
        action->act(*this);
    }
}


void WareHouse::printActionLog() const {
    for (const BaseAction* action : actionsLog) {
        if (action != nullptr) {
            cout<< action->toString() << std::endl;
        }
    }
}
void WareHouse::BackupWareHouse() {
    delete backup;    // Clean up existing backup
    backup=new WareHouse(*this);     // Create new backup
}

void WareHouse::addOrder(Order* order) {
    pendingOrders.push_back(order);
}
void WareHouse::addInProcessOrder(Order* order) {
    inProcessOrders.push_back(order);
}
void WareHouse::addCompletedOrder(Order* order) {
      completedOrders.push_back(order);
}
void WareHouse::addToPendingOrder(Order* order) {
    pendingOrders.push_back(order);
    pendingOrders.push_back(order);
}
void WareHouse::stepAllVolunteers() {
    for (Volunteer* volunteer : volunteers) {
        if (volunteer != nullptr) {
            volunteer->step();
        }
    }
}
void WareHouse::addAction(BaseAction* action) {
    actionsLog.push_back(action);
}
Customer &WareHouse::getCustomer(int customerId) const {
    for (Customer *customer : customers) {
        if (customer->getId() == customerId) {
            return *customer;
        }
    }
    throw std::out_of_range("Customer not found");
}
Volunteer &WareHouse::getVolunteer(int volunteerId) const {
    for (Volunteer *volunteer : volunteers) {
        if (volunteer->getId() == volunteerId) {
            return *volunteer;
        }
    }
    throw std::out_of_range("Volunteer not found");
}
Volunteer* WareHouse::findAvailableCollectorVolunteer () const {
    for (Volunteer* volunteer : volunteers) {
        if (!volunteer->isBusy()) {
            CollectorVolunteer* collector = dynamic_cast<CollectorVolunteer*>(volunteer);
            if (collector != nullptr) {
                // Check if the volunteer is not busy and is of type CollectorVolunteer
                return collector;
            }
        }
    }
    return nullptr; // Return nullptr if no available collector volunteer is found
}

Volunteer* WareHouse::findAvailableDriverVolunteer () const {
    for (Volunteer* volunteer : volunteers) {
        if (!volunteer->isBusy()) {
            DriverVolunteer* driver = dynamic_cast<DriverVolunteer*>(volunteer);
            if (driver != nullptr) {
                // Check if the volunteer is not busy and is of type CollectorVolunteer
                return driver;
            }
        }
    }
    return nullptr; // Return nullptr if no available collector volunteer is found
}

void WareHouse::printCustomerStatus(int customerId) const {
    for (const Customer* customer : customers) {
        if (customer != nullptr && customer->getId() == customerId) {
            std::cout << "Customer ID: " << customer->getId() << std::endl;
            std::cout << "Name: " << customer->getName() << std::endl;
            std::cout << "Location Distance: " << customer->getCustomerDistance() << std::endl;
            std::cout << "Max Orders: " << customer->getMaxOrders() << std::endl;
            std::cout << "Number of Orders Made: " << customer->getNumOrders() << std::endl;

            const auto& ordersIds = customer->getOrdersIds();
            if (!ordersIds.empty()) {
                std::cout << "Orders IDs: ";
                for (int orderId : ordersIds) {
                    std::cout << orderId << " ";
                }
                std::cout << std::endl;
            } else {
                std::cout << "No orders made yet." << std::endl;
            }
            return;
        }
    }
    std::cout << "Customer with ID " << customerId << " not found." << std::endl;
}

void WareHouse::printVolunteerStatus(int volunteerId) const {
    for (const Volunteer* volunteer : volunteers) {
        if (volunteer != nullptr && volunteer->getId() == volunteerId) {
            // Assuming Volunteer class has a toString method to get status
            std::cout << volunteer->toString() << std::endl;
            return;
        }
    }
    std::cout << "Volunteer with ID " << volunteerId << " not found." << std::endl;
}

const vector<Order*> &WareHouse::getInProcessOrders() const{
    return inProcessOrders;

}



void WareHouse::printOrderStatus(int orderId) const {
    // Lambda function to search and print order status
    auto searchAndPrint = [orderId](const std::vector<Order*>& orders) -> bool {
        for (const Order* order : orders) {
            if (order != nullptr && order->getId() == orderId) {
                std::cout << "Order ID: " << order->getId()
                          << ", Status: " << order->toString()  // getStatusString() to be implemented in Order class
                          << std::endl;
                return true;
            }
        }
        return false;
    };

    // Search in all order containers
    if (!searchAndPrint(pendingOrders) &&
        !searchAndPrint(inProcessOrders) &&
        !searchAndPrint(completedOrders)) {
        std::cout << "Order with ID " << orderId << " not found." << std::endl;
    }
}
const vector<Order*> &WareHouse::getPendingOrders() const{
    return pendingOrders;

}

const vector<Order*> &WareHouse::getCompletedOrders() const{
    return completedOrders;

}
const vector<Volunteer*> &WareHouse::getVolunteers() const{
    return volunteers;

}


// Copy constructor
WareHouse::WareHouse(const WareHouse& other)
: isOpen(other.isOpen), actionsLog(other.actionsLog), volunteers(other.volunteers), pendingOrders(other.pendingOrders), inProcessOrders(other.inProcessOrders), completedOrders(other.completedOrders), customers(other.customers),
  customerCounter(other.customerCounter), volunteerCounter(other.customerCounter), orderCounter(other.orderCounter)
{
    // Deep copy of dynamically allocated customers
    for (const auto& customer : other.customers) {
        customers.push_back(customer->clone());
    }
    // Deep copy of dynamically allocated volunteers
    for (const auto& volunteer : other.volunteers) {
        volunteers.push_back(volunteer->clone());
    }
    // Deep copy of orders
    for (const auto& order : other.pendingOrders) {
        pendingOrders.push_back(new Order(*order));
    }
    for (const auto& order : other.inProcessOrders) {
        inProcessOrders.push_back(new Order(*order));
    }
    for (const auto& order : other.completedOrders) {
        completedOrders.push_back(new Order(*order));
    }
    // Deep copy of actions log
    for (const auto& action : other.actionsLog) {
        actionsLog.push_back(action->clone());
    }
}

// Copy assignment operator
WareHouse& WareHouse::operator=(const WareHouse& other) {
    if (this != &other) {
        // Clean up the current state
        for (auto* customer : customers) delete customer;
        for (auto* volunteer : volunteers) delete volunteer;
        for (auto* order : pendingOrders) delete order;
        for (auto* order : inProcessOrders) delete order;
        for (auto* order : completedOrders) delete order;
        for (auto* action : actionsLog) delete action;

        // Resetting vectors
        customers.clear();
        volunteers.clear();
        pendingOrders.clear();
        inProcessOrders.clear();
        completedOrders.clear();
        actionsLog.clear();

        // Copying new state
        isOpen = other.isOpen;
        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        orderCounter = other.orderCounter;

        // Repeat deep copy logic from copy constructor
        for (const auto& customer : other.customers) {
            customers.push_back(customer->clone());
        }
        for (const auto& volunteer : other.volunteers) {
            volunteers.push_back(volunteer->clone());
        }
        for (const auto& order : other.pendingOrders) {
            pendingOrders.push_back(new Order(*order));
        }
        for (const auto& order : other.inProcessOrders) {
            inProcessOrders.push_back(new Order(*order));
        }
        for (const auto& order : other.completedOrders) {
            completedOrders.push_back(new Order(*order));
        }
        for (const auto& action : other.actionsLog) {
            actionsLog.push_back(action->clone());
        }
    }
    return *this;
}


WareHouse& WareHouse::operator=( WareHouse&& other) noexcept {
    // Guard against self-assignment
    if (this != &other) {
        // Free the existing resources
        for (auto* customer : customers) delete customer;
        for (auto* volunteer : volunteers) delete volunteer;
        for (auto* order : pendingOrders) delete order;
        for (auto* order : inProcessOrders) delete order;
        for (auto* order : completedOrders) delete order;
        for (auto* action : actionsLog) delete action;

        // Transfer ownership of resources from 'other' to 'this'
        customers = std::move(other.customers);
        volunteers = std::move(other.volunteers);
        pendingOrders = std::move(other.pendingOrders);
        inProcessOrders = std::move(other.inProcessOrders);
        completedOrders = std::move(other.completedOrders);
        actionsLog = std::move(other.actionsLog);

        // Copy primitive types
        isOpen = other.isOpen;
        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        orderCounter = other.orderCounter;

        // Reset 'other' to a valid state
        other.customers.clear();
        other.volunteers.clear();
        other.pendingOrders.clear();
        other.inProcessOrders.clear();
        other.completedOrders.clear();
        other.actionsLog.clear();
        other.isOpen = false; // or some appropriate default state
        other.customerCounter = 0;
        other.volunteerCounter = 0;
        other.orderCounter = 0;
    }
    return *this;
}



Order &WareHouse::getOrder(int orderId) const {
    for (Order *order : pendingOrders) {
        if (order->getId() == orderId) {
            return *order;
        }
    }
    for (Order *order : inProcessOrders) {
        if (order->getId() == orderId) {
            return *order;
        }
    }
    for (Order *order : completedOrders) {
        if (order->getId() == orderId) {
            return *order;
        }
    }
    throw std::out_of_range("Order not found");
}

const vector<BaseAction*> &WareHouse::getActions() const {
    return actionsLog;
}

void WareHouse::close() {
  std::cout<<"Closing warehouse and printing all orders with their statuses: \n";
  std::vector<Order*> allOrders;
  allOrders.insert(allOrders.end(),pendingOrders.begin(),pendingOrders.end());
  allOrders.insert(allOrders.end(),inProcessOrders.begin(),inProcessOrders.end());
  allOrders.insert(allOrders.end(),completedOrders.begin(),completedOrders.end());
  for(const auto* order:allOrders){
    std::cout<<order->toString()<<std::endl;

  } 

  isOpen=false;
   
}


void WareHouse::open() {
    isOpen = true;
}

void WareHouse::removeMaxedOutVolunteers(){
    for(int i=volunteers.size()-1;i>=0;i--){
        if(!volunteers[i]->hasOrdersLeft()){
            delete volunteers[i];
            volunteers.erase(volunteers.begin()+i);

        }
    }
}

int WareHouse::generateUniqueOrderId() {
    return orderCounter++;
}

void WareHouse::RestoreWareHouse() {
    if (backup) {
        // Assuming WareHouse has an assignment operator for deep copying
        *this = *backup; // Copy the state from backup to this instance
    }
    // Note: Depending on your needs, you might also want to handle the case where there's no backup available
}


WareHouse::~WareHouse() {
    for (auto *customer : customers) {
        delete customer;
    }
    for (auto *volunteer : volunteers) {
        delete volunteer;
    }
    for (auto *order : pendingOrders) {
        delete order;
    }
    for (auto *order : inProcessOrders) {
        delete order;
    }
    for (auto *order : completedOrders) {
        delete order;
    }
    for (auto *action : actionsLog) {
        delete action;
    }
}