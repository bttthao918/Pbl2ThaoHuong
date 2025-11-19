#ifndef ELECTRICBIKE_H
#define ELECTRICBIKE_H

#include "Vehicle.h"
using namespace std;

class ElectricBike : public Vehicle
{
private:
    int batteryCapacity;
    int maxSpeed;

public:
    ElectricBike();
    ElectricBike(const string &id, const string &plate, const string &custId,
                 const string &br, const string &mod, const string &col,
                 int battery, int speed);

    int getBatteryCapacity() const;
    int getMaxSpeed() const;
    void setBatteryCapacity(int battery);
    void setMaxSpeed(int speed);

    void displayInfo() const override;
    double calculateParkingFee(long long minutes) const override;
    string getTypeString() const override;
    string toFileString() const override;
    void fromFileString(const string &line) override;
};

#endif