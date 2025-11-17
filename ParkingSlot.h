#ifndef PARKING_SLOT_H
#define PARKING_SLOT_H

#include <string>
using namespace std;

class ParkingSlot {
private:
    string slotID;            // Mã vị trí (A01, B15,...)
    string zone;              // Khu vực (A/B/C)
    int floor;                // Tầng
    string vehicleType;       // Loại xe cho phép: Motorcycle/Car/ElectricBike
    bool isOccupied;          // true nếu đang có xe
    string currentVehicleID;  // biển số xe đang đỗ

public:
    // Constructor
    ParkingSlot(string id, string zone, int floor, string vType);

    // Methods
    void occupy(string vehicleID);  // chiếm chỗ
    void release();                 // trả chỗ
    void display() const;           // hiển thị thông tin

    // Getters
    string getSlotID() const { return slotID; }
    bool getIsOccupied() const { return isOccupied; }
    string getZone() const { return zone; }
    string getVehicleType() const { return vehicleType; }
    string getCurrentVehicleID() const { return currentVehicleID; }
};

#endif
