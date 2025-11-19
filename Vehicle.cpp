#include "Vehicle.h"
#include "Exceptions.h"
#include "Utils.h"
#include <sstream>
#include <iomanip>
using namespace std;

Vehicle::Vehicle() : type(VehicleType::MOTORCYCLE) {}

Vehicle::Vehicle(const string &id, const string &plate, const string &custId,
                 VehicleType t, const string &br, const string &mod, const string &col)
    : vehicleId(id), licensePlate(plate), customerId(custId), type(t),
      brand(br), model(mod), color(col) {
    if (!Utils::isValidLicensePlate(plate)) {
        throw InvalidInputException("Bien so xe khong hop le");
    }
}

void Vehicle::setLicensePlate(const string &plate) {
    if (!Utils::isValidLicensePlate(plate)) {
        throw InvalidInputException("Bien so xe khong hop le");
    }
    licensePlate = plate;
}

void Vehicle::displayInfo() const {
    cout << "Vehicle ID: " << vehicleId << endl;
    cout << "Bien so: " << licensePlate << endl;
    cout << "Loai xe: " << getTypeString() << endl;
    cout << "Hang: " << brand << endl;
    cout << "Model: " << model << endl;
    cout << "Mau: " << color << endl;
}

string Vehicle::toFileString() const {
    ostringstream oss;
    oss << vehicleId << "|" << licensePlate << "|" << customerId << "|"
        << vehicleTypeToString(type) << "|" << brand << "|" << model << "|" << color;
    return oss.str();
}

void Vehicle::fromFileString(const string &line) {
    istringstream iss(line);
    string typeStr;
    getline(iss, vehicleId, '|');
    getline(iss, licensePlate, '|');
    getline(iss, customerId, '|');
    getline(iss, typeStr, '|');
    type = stringToVehicleType(typeStr);
    getline(iss, brand, '|');
    getline(iss, model, '|');
    getline(iss, color);
}

ostream &operator<<(ostream &os, const Vehicle &vehicle) {
    os << "Bien so: " << vehicle.licensePlate << ", Loai: " << vehicle.getTypeString();
    return os;
}

bool Vehicle::operator==(const Vehicle &other) const {
    return vehicleId == other.vehicleId;
}

string Vehicle::vehicleTypeToString(VehicleType type) {
    switch (type) {
    case VehicleType::MOTORCYCLE: return "MOTORCYCLE";
    case VehicleType::CAR: return "CAR";
    case VehicleType::ELECTRIC_BIKE: return "ELECTRIC_BIKE";
    default: return "UNKNOWN";
    }
}

VehicleType Vehicle::stringToVehicleType(const string &str) {
    if (str == "MOTORCYCLE") return VehicleType::MOTORCYCLE;
    if (str == "CAR") return VehicleType::CAR;
    if (str == "ELECTRIC_BIKE") return VehicleType::ELECTRIC_BIKE;
    return VehicleType::MOTORCYCLE;
