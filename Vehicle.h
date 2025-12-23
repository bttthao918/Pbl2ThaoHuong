#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>
#include <iostream>

enum class VehicleType
{
    MOTORCYCLE,
    CAR,
    ELECTRIC_BIKE
};

class Vehicle
{
protected:
    std::string vehicleId;
    std::string licensePlate;
    std::string customerId;
    VehicleType type;
    std::string brand;
    std::string model;
    std::string color;

public:
    Vehicle();
    Vehicle(const std::string &id, const std::string &plate, const std::string &custId,
            VehicleType t, const std::string &br, const std::string &mod, const std::string &col);
    virtual ~Vehicle() {}

    std::string getVehicleId() const;
    std::string getLicensePlate() const;
    std::string getCustomerId() const;
    VehicleType getType() const;
    std::string getBrand() const;
    std::string getModel() const;
    std::string getColor() const;
    void setLicensePlate(const std::string &plate);
    void setBrand(const std::string &br);
    void setModel(const std::string &mod);
    void setColor(const std::string &col);

    virtual void displayInfo() const;
    void displayTableRow() const;
    virtual double calculateParkingFee(long long minutes) const = 0;
    virtual std::string getTypeString() const = 0;
    virtual std::string toFileString() const;
    virtual void fromFileString(const std::string &line);

    friend std::ostream &operator<<(std::ostream &os, const Vehicle &vehicle);
    bool operator==(const Vehicle &other) const;

    static std::string vehicleTypeToString(VehicleType type);
    static VehicleType stringToVehicleType(const std::string &str);
};

#endif