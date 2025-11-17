#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>
#include "RateManager.h"
using namespace std;

class Vehicle {
protected:
    string vehicleID;
    string ownerName;
    string phoneNumber;
    string entryTime;
    string exitTime;
    string parkingSlotID;
    string ticketID;

public:
    Vehicle(string id = "", string owner = "", string phone = "",
            string entry = "", string slot = "", string ticket = "");

    virtual ~Vehicle() {}

    void setExitTime(string exit);

    virtual void display() const = 0;
    virtual double calculateParkingFee(const RateManager& rateManager) const = 0;
};

#endif
