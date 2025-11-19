#include "Car.h"
#include "PricingConfig.h"
#include <sstream>
using namespace std;

Car::Car() : Vehicle(), seatCount(0), isLuxury(false)
{
    type = VehicleType::CAR;
}

Car::Car(const string &id, const string &plate, const string &custId,
         const string &br, const string &mod, const string &col,
         int seats, bool luxury)
    : Vehicle(id, plate, custId, VehicleType::CAR, br, mod, col),
      seatCount(seats), isLuxury(luxury) {}

void Car::displayInfo() const
{
    Vehicle::displayInfo();
    cout << "So cho: " << seatCount << endl;
    cout << "Xe sang: " << (isLuxury ? "Co" : "Khong") << endl;
}

double Car::calculateParkingFee(long long minutes) const
{
    PricingConfig *pricing = PricingConfig::getInstance();
    const double HOURLY_RATE = pricing->getPricePerHour(VehicleType::CAR, isLuxury);
    int minMinutes = pricing->getMinimumMinutes();

    if (minutes < minMinutes)
        minutes = minMinutes;
    return (minutes / 60.0) * HOURLY_RATE;
}

string Car::getTypeString() const
{
    return "Car";
}

string Car::toFileString() const
{
    ostringstream oss;
    oss << Vehicle::toFileString() << "|" << seatCount << "|" << (isLuxury ? "1" : "0");
    return oss.str();
}

void Car::fromFileString(const string &line)
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
    string luxuryStr;
    iss >> seatCount;
    iss.ignore();
    getline(iss, luxuryStr);
    isLuxury = (luxuryStr == "1");
}