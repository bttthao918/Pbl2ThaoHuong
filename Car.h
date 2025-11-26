#ifndef CAR_H
#define CAR_H

#include "Vehicle.h"
#include <string>

class Car : public Vehicle
{
private:
    int seatCount;
    bool isLuxury;

public:
    Car();
    Car(const std::string &id, const std::string &plate, const std::string &custId,
        const std::string &br, const std::string &mod, const std::string &col,
        int seats, bool luxury);

    int getSeatCount() const;
    bool getIsLuxury() const;
    void setSeatCount(int seats);
    void setIsLuxury(bool luxury);

    void displayInfo() const override;
    double calculateParkingFee(long long minutes) const override;
    std::string getTypeString() const override;
    std::string toFileString() const override;
    void fromFileString(const std::string &line) override;
};

#endif