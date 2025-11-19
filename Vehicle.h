#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>
#include <iostream>
using namespace std;

enum class VehicleType
{
    MOTORCYCLE,
    CAR,
    ELECTRIC_BIKE
};

class Vehicle
{
protected:
    string vehicleId;
    string licensePlate;
    string customerId;
    VehicleType type;
    string brand;
    string model;
    string color;

public:
    Vehicle();
    Vehicle(const string &id, const string &plate, const string &custId,
            VehicleType t, const string &br, const string &mod, const string &col);
    virtual ~Vehicle() {}

    // Getters & Setters
    string getVehicleId() const;
    string getLicensePlate() const;
    string getCustomerId() const;
    VehicleType getType() const;
    string getBrand() const;
    string getModel() const;
    string getColor() const;

    void setLicensePlate(const string &plate);
    void setBrand(const string &br);
    void setModel(const string &mod);
    void setColor(const string &col);

    // Virtual methods
    virtual void displayInfo() const;
    virtual double calculateParkingFee(long long minutes) const = 0;
    virtual string getTypeString() const = 0;
    virtual string toFileString() const;
    virtual void fromFileString(const string &line);

    friend ostream &operator<<(ostream &os, const Vehicle &vehicle);
    bool operator==(const Vehicle &other) const;

    static string vehicleTypeToString(VehicleType type);
    static VehicleType stringToVehicleType(const string &str);
};

#endif