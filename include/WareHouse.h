#pragma once
#include <string>
#include <vector>

#include "../include/Order.h"

class BaseAction;
class Volunteer;
class Customer;
// Warehouse responsible for Volunteers, Customers Actions, and Orders.


class WareHouse {


public:

    WareHouse(const string &configFilePath);
    
    WareHouse(WareHouse&& other ) noexcept;
    WareHouse(const WareHouse& other);
    WareHouse& operator=(const WareHouse& other);

    
    WareHouse& operator=(WareHouse&& other) noexcept;
         
    void start();
    int generateUniqueOrderId();
    void addOrder(Order* order);
    void addInProcessOrder(Order* order);
    void addCompletedOrder(Order* order);
    void printActionLog() const;
    void addToPendingOrder(Order* order);
    void printCustomerStatus(int customerId) const;
    void addAction(BaseAction* action);
    Customer &getCustomer(int customerId) const;
    Volunteer &getVolunteer(int volunteerId) const;
    void printVolunteerStatus(int volunteerId) const;
    void printOrderStatus(int orderId) const;
    Order &getOrder(int orderId) const;
    void stepAllVolunteers();
    const vector<BaseAction*> &getActions() const;
    const vector<Order*> &getPendingOrders() const;
    const vector<Order*> &getInProcessOrders() const ;
    const vector<Order*> &getCompletedOrders() const ;
    void addCustomer(const string &customerName, const string &customerType, int distance, int maxOrders);
    const vector<Volunteer*> &getVolunteers() const;
    void removeMaxedOutVolunteers();
    Volunteer* findAvailableCollectorVolunteer() const;
    Volunteer* findAvailableDriverVolunteer() const;
    void RestoreWareHouse();
    ~WareHouse();
    void close();
    void BackupWareHouse();
    void open();

private:

    bool isOpen;
    vector<BaseAction*> actionsLog;
    vector<Volunteer*> volunteers;
    vector<Order*> pendingOrders;
    vector<Order*> inProcessOrders;
    vector<Order*> completedOrders;
    vector<Customer*> customers;
    int customerCounter; //For assigning unique customer IDs
    int volunteerCounter; //For assigning unique volunteer IDs
    int orderCounter;
};