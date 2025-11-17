#ifndef CAR_H
#define CAR_H

#include "Vehicle.h"

class Car : public Vehicle {
private:
    string licensePlateType;
    int numberOfSeats;

public:
    Car(string id, string owner, string phone, string entry,
        string slot, string ticket, string plateType, int seats);

    void display() const override;
    double calculateParkingFee(const RateManager& rateManager) const;
};

#endif
