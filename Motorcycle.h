#ifndef MOTORCYCLE_H
#define MOTORCYCLE_H

#include "Vehicle.h"
#include "RateManager.h"

class Motorcycle : public Vehicle {
private:
    int engineCapacity;
    bool hasHelmet;

public:
    Motorcycle(string id, string owner, string phone, string entry,
               string slot, string ticket, int capacity, bool helmet);

    void display() const override;
   double calculateParkingFee(const RateManager& rateManager) const;
};

#endif
