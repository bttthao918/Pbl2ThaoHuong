#ifndef ELECTRICBIKE_H
#define ELECTRICBIKE_H

#include "Vehicle.h"

class ElectricBike : public Vehicle {
private:
    int batteryCapacity;
    string brand;

public:
    ElectricBike(string id, string owner, string phone, string entry,
                 string slot, string ticket, int battery, string brand);

    void display() const override;
    double calculateParkingFee() const override;
};

#endif
