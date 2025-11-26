#include "ElectricBike.h"
#include "PricingConfig.h"
#include "UI.h"
#include <sstream>
#include <iomanip>
using namespace std;

extern UI ui;

ElectricBike::ElectricBike() : Vehicle(), batteryCapacity(0), maxSpeed(0)
{
    type = VehicleType::ELECTRIC_BIKE;
}

ElectricBike::ElectricBike(const string &id, const string &plate, const string &custId,
                           const string &br, const string &mod, const string &col,
                           int battery, int speed)
    : Vehicle(id, plate, custId, VehicleType::ELECTRIC_BIKE, br, mod, col),
      batteryCapacity(battery), maxSpeed(speed) {}

void ElectricBike::setBatteryCapacity(int capacity)
{
    batteryCapacity = capacity;
}

void ElectricBike::setMaxSpeed(int speed)
{
    maxSpeed = speed;
}

int ElectricBike::getBatteryCapacity() const
{
    return batteryCapacity;
}

int ElectricBike::getMaxSpeed() const
{
    return maxSpeed;
}

void ElectricBike::displayInfo() const
{
    Vehicle::displayInfo();
    ui.printRow("          | Dung luong pin: ", to_string(batteryCapacity) + " mAh");
    ui.printRow("          | Toc do toi da: ", to_string(maxSpeed) + " km/h");
    ui.printHorizontalLine('+', '-', '+');
}

double ElectricBike::calculateParkingFee(long long minutes) const
{
    PricingConfig *pricing = PricingConfig::getInstance();
    const double HOURLY_RATE = pricing->getElectricBikePrice();
    int minMinutes = pricing->getMinimumMinutes();

    if (minutes < minMinutes)
        minutes = minMinutes;
    return (minutes / 60.0) * HOURLY_RATE;
}

string ElectricBike::getTypeString() const
{
    return "ElectricBike";
}

string ElectricBike::toFileString() const
{
    ostringstream oss;
    oss << Vehicle::toFileString() << "|" << batteryCapacity << "|" << maxSpeed;
    return oss.str();
}

void ElectricBike::fromFileString(const string &line)
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
    iss >> batteryCapacity;
    iss.ignore();
    iss >> maxSpeed;
}