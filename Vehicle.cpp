#include "Vehicle.h"
#include "Exceptions.h"
#include "Utils.h"
#include "UI.h"
#include <sstream>
#include <iomanip>
using namespace std;

extern UI ui;

Vehicle::Vehicle() : type(VehicleType::MOTORCYCLE) {}

Vehicle::Vehicle(const string &id, const string &plate, const string &custId,
                 VehicleType t, const string &br, const string &mod, const string &col)
    : vehicleId(id), licensePlate(plate), customerId(custId), type(t),
      brand(br), model(mod), color(col)
{
    if (!Utils::isValidLicensePlate(plate))
    {
        throw InvalidInputException("Bien so xe khong hop le");
    }
}

string Vehicle::getVehicleId() const
{
    return vehicleId;
}

string Vehicle::getLicensePlate() const
{
    return licensePlate;
}

string Vehicle::getCustomerId() const
{
    return customerId;
}

VehicleType Vehicle::getType() const
{
    return type;
}

string Vehicle::getBrand() const
{
    return brand;
}

string Vehicle::getModel() const
{
    return model;
}

string Vehicle::getColor() const
{
    return color;
}

void Vehicle::setBrand(const string &br)
{
    brand = br;
}

void Vehicle::setModel(const string &md)
{
    model = md;
}

void Vehicle::setColor(const string &cl)
{
    color = cl;
}

void Vehicle::setLicensePlate(const string &plate)
{
    if (!Utils::isValidLicensePlate(plate))
    {
        throw InvalidInputException("Bien so xe khong hop le");
    }
    licensePlate = plate;
}

void Vehicle::displayInfo() const
{
    ui.printHorizontalLine('+', '-', '+');
    ui.printRow("          |    Vehicle ID: ", vehicleId);
    ui.printRow("          |    Bien so:    ", licensePlate);
    ui.printRow("          |    Loai xe:    ", getTypeString());
    ui.printRow("          |    Hang:       ", brand + " " + model);
    ui.printRow("          |    Mau sac:    ", color);
    ui.printRow("          |    Customer ID:  ", customerId);
    ui.printHorizontalLine('+', '-', '+');
}

void Vehicle::displayTableRow() const
{
    string brandModel = brand + " " + model;
    if (brandModel.length() > 26)
        brandModel = brandModel.substr(0, 23) + "...";

    //  int widths[] = {16, 22, 19, 28};
    // Format: "          |" + " content|" + " content|" ...

    cout << "          |"
         << " " << setw(15) << left << vehicleId << "|"
         << " " << setw(21) << left << licensePlate << "|"
         << " " << setw(18) << left << getTypeString() << "|"
         << " " << setw(27) << left << brandModel << "|"
         << endl;
}

string Vehicle::toFileString() const
{
    ostringstream oss;
    oss << vehicleId << "|" << licensePlate << "|" << customerId << "|"
        << vehicleTypeToString(type) << "|" << brand << "|" << model << "|" << color;
    return oss.str();
}

void Vehicle::fromFileString(const string &line)
{
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

ostream &operator<<(ostream &os, const Vehicle &vehicle)
{
    os << "Bien so: " << vehicle.licensePlate << ", Loai: " << vehicle.getTypeString();
    return os;
}

bool Vehicle::operator==(const Vehicle &other) const
{
    return vehicleId == other.vehicleId;
}

string Vehicle::vehicleTypeToString(VehicleType type)
{
    switch (type)
    {
    case VehicleType::MOTORCYCLE:
        return "MOTORCYCLE";
    case VehicleType::CAR:
        return "CAR";
    case VehicleType::ELECTRIC_BIKE:
        return "ELECTRIC_BIKE";
    default:
        return "UNKNOWN";
    }
}

VehicleType Vehicle::stringToVehicleType(const string &str)
{
    if (str == "MOTORCYCLE")
        return VehicleType::MOTORCYCLE;
    if (str == "CAR")
        return VehicleType::CAR;
    if (str == "ELECTRIC_BIKE")
        return VehicleType::ELECTRIC_BIKE;
    return VehicleType::MOTORCYCLE;
}