#ifndef MOTORCYCLE_H
#define MOTORCYCLE_H

#include "Vehicle.h"
using namespace std;

class Motorcycle : public Vehicle
{
private:
    int engineCapacity;

public:
    Motorcycle();
    Motorcycle(const string &id, const string &plate, const string &custId,
               const string &br, const string &mod, const string &col, int capacity);

    int getEngineCapacity() const;
    void setEngineCapacity(int capacity);

    void displayInfo() const override;
    double calculateParkingFee(long long minutes) const override;
    string getTypeString() const override;
    string toFileString() const override;
    void fromFileString(const string &line) override;
};

#endif