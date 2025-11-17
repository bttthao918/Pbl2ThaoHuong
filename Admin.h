#ifndef ADMIN_H
#define ADMIN_H

#include "User.h"
#include "ParkingSlot.h"
#include "DoubleLinkedList.h"
#include <map>
#include <iostream>
#include "Customer.h"
#include "RateManager.h"
using namespace std;

class Admin : public User
{
private:
    Admin<ParkingSlot> slots;  // Danh sách chỗ gửi xe
    map<string, double> rates; // Giá vé theo loại xe
    RateManager &rateManager;

public:
    Admin(string id, string user, string pass, string name, string phone, string mail, RateManager &rm);

    void display() override;
    string getUserType() override;
    void showMenu(Admin<Customer> &customers);

    // Quản lý chỗ gửi xe
    void viewSlots() const;
    void addSlot();
    void removeSlot();
    void modifySlot();

    // Quản lý giá vé
    void viewRates() const;
    void updateRates();

    // Quản lý khách hàng (demo)
    void manageCustomers(Admin<Customer> &customers);
};

#endif