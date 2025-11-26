#ifndef MOTORCYCLE_H
#define MOTORCYCLE_H

#include "Vehicle.h"
#include <string>

class Motorcycle : public Vehicle
{
private:
    int engineCapacity;

public:
    Motorcycle();
    Motorcycle(const std::string &id, const std::string &plate, const std::string &custId,
               const std::string &br, const std::string &mod, const std::string &col, int capacity);

    int getEngineCapacity() const;
    void setEngineCapacity(int capacity);

    void displayInfo() const override;
    double calculateParkingFee(long long minutes) const override;
    std::string getTypeString() const override;
    std::string toFileString() const override;
    void fromFileString(const std::string &line) override;
};

#endif