#include "ElectricBike.h"
#include "PricingConfig.h"
#include <sstream>
using namespace std;

ElectricBike::ElectricBike() : Vehicle(), batteryCapacity(0), maxSpeed(0)
{
    type = VehicleType::ELECTRIC_BIKE;
}

ElectricBike::ElectricBike(const string &id, const string &plate, const string &custId,
                           const string &br, const string &mod, const string &col,
                           int battery, int speed)
    : Vehicle(id, plate, custId, VehicleType::ELECTRIC_BIKE, br, mod, col),
      batteryCapacity(battery), maxSpeed(speed) {}

void ElectricBike::displayInfo() const
{
    Vehicle::displayInfo();
    cout << "Dung luong pin: " << batteryCapacity << " mAh" << endl;
    cout << "Toc do toi da: " << maxSpeed << " km/h" << endl;
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