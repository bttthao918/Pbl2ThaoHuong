#ifndef ELECTRICBIKE_H
#define ELECTRICBIKE_H

#include "Vehicle.h"
#include <string>

class ElectricBike : public Vehicle
{
private:
    int batteryCapacity;
    int maxSpeed;

public:
    ElectricBike();
    ElectricBike(const std::string &id, const std::string &plate, const std::string &custId,
                 const std::string &br, const std::string &mod, const std::string &col,
                 int battery, int speed);

    int getBatteryCapacity() const;
    int getMaxSpeed() const;
    void setBatteryCapacity(int battery);
    void setMaxSpeed(int speed);

    void displayInfo() const override;
    double calculateParkingFee(long long minutes) const override;
    std::string getTypeString() const override;
    std::string toFileString() const override;
    void fromFileString(const std::string &line) override;
};

#endif