#include "Motorcycle.h"
#include "PricingConfig.h"
#include "UI.h"
#include <sstream>
#include <iomanip>
using namespace std;

extern UI ui;

Motorcycle::Motorcycle() : Vehicle(), engineCapacity(0)
{
    type = VehicleType::MOTORCYCLE;
}

Motorcycle::Motorcycle(const string &id, const string &plate, const string &custId,
                       const string &br, const string &mod, const string &col, int capacity)
    : Vehicle(id, plate, custId, VehicleType::MOTORCYCLE, br, mod, col),
      engineCapacity(capacity) {}

void Motorcycle::setEngineCapacity(int capacity)
{
    engineCapacity = capacity;
}

int Motorcycle::getEngineCapacity() const
{
    return engineCapacity;
}

void Motorcycle::displayInfo() const
{
    Vehicle::displayInfo();
    ui.printRow("          | ", "Phan khoi: " + to_string(engineCapacity) + " cc");
    ui.printHorizontalLine('+', '-', '+');
}

double Motorcycle::calculateParkingFee(long long minutes) const
{
    PricingConfig *pricing = PricingConfig::getInstance();
    const double HOURLY_RATE = pricing->getMotorcyclePrice();
    int minMinutes = pricing->getMinimumMinutes();

    if (minutes < minMinutes)
        minutes = minMinutes;
    return (minutes / 60.0) * HOURLY_RATE;
}

string Motorcycle::getTypeString() const
{
    return "Motorcycle";
}

string Motorcycle::toFileString() const
{
    return Vehicle::toFileString() + "|" + to_string(engineCapacity);
}

void Motorcycle::fromFileString(const string &line)
{
    istringstream iss(line);
    string baseData;
    for (int i = 0; i < 7; i++)
    {
        string part;
        getline(iss, part, '|');
        if (i > 0)
            baseData += "|";
        baseData += part;
    }
    Vehicle::fromFileString(baseData);
    iss >> engineCapacity;
}