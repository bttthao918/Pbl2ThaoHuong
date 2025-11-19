#include "Motorcycle.h"
#include <sstream>
using namespace std;

Motorcycle::Motorcycle() : Vehicle(), engineCapacity(0) {
    type = VehicleType::MOTORCYCLE;
}

Motorcycle::Motorcycle(const string &id, const string &plate, const string &custId,
                       const string &br, const string &mod, const string &col, int capacity)
    : Vehicle(id, plate, custId, VehicleType::MOTORCYCLE, br, mod, col),
      engineCapacity(capacity) {}

void Motorcycle::displayInfo() const {
    Vehicle::displayInfo();
    cout << "Phan khoi: " << engineCapacity << " cc" << endl;
}

double Motorcycle::calculateParkingFee(long long minutes) const {
    const double HOURLY_RATE = 5000.0;
    if (minutes < 30) minutes = 30;
    return (minutes / 60.0) * HOURLY_RATE;
}

string Motorcycle::toFileString() const {
    return Vehicle::toFileString() + "|" + to_string(engineCapacity);
}

void Motorcycle::fromFileString(const string &line) {
    istringstream iss(line);
    string baseData;
    for (int i = 0; i < 7; i++) {
        string part;
        getline(iss, part, '|');
        if (i > 0) baseData += "|";
        baseData += part;
    }
    Vehicle::fromFileString(baseData);
    iss >> engineCapacity;
}