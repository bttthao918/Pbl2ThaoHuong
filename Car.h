#ifndef CAR_H
#define CAR_H

#include "Vehicle.h"
using namespace std;

class Car : public Vehicle
{
private:
    int seatCount;
    bool isLuxury;

public:
    Car();
    Car(const string &id, const string &plate, const string &custId,
        const string &br, const string &mod, const string &col,
        int seats, bool luxury);

    int getSeatCount() const;
    bool getIsLuxury() const;
    void setSeatCount(int seats);
    void setIsLuxury(bool luxury);

    void displayInfo() const override;
    double calculateParkingFee(long long minutes) const override;
    string getTypeString() const override;
    string toFileString() const override;
    void fromFileString(const string &line) override;
};

#endif