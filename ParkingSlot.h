#ifndef PARKING_SLOT_H
#define PARKING_SLOT_H

#include <string>
using namespace std;

class ParkingSlot {
private:
    int slotNumber;           // Số thứ tự chỗ đỗ (1..n)
    string slotID;            // Mã vị trí (A01, B15,...) - có thể rỗng nếu dùng số
    string zone;              // Khu vực (A/B/C)
    int floor;                // Tầng
    string vehicleType;       // Loại xe cho phép: Motorcycle/Car/ElectricBike
    bool occupied;            // true nếu đang có xe
    string currentVehicleID;  // biển số xe đang đỗ

public:
    // Constructors
    ParkingSlot();
    ParkingSlot(int slotNumber, string zone, int floor, string vType);

    // Methods
    // Chiếm chỗ với biển số và (tuỳ chọn) loại xe
    void occupy(const string& vehicleID, const string& vehicleType);
    // Giải phóng chỗ
    void release();
    // Hiển thị thông tin
    void display() const;

    // Setters
    void setSlotNumber(int num) { slotNumber = num; }
    void setSlotID(const string& id) { slotID = id; }
    void setZone(const string& z) { zone = z; }
    void setFloor(int f) { floor = f; }
    void setVehicleType(const string& vt) { vehicleType = vt; }

    // Getters
    int getSlotNumber() const { return slotNumber; }
    string getSlotID() const { return slotID; }
    bool isOccupied() const { return occupied; }
    string getZone() const { return zone; }
    string getVehicleType() const { return vehicleType; }
    string getCurrentVehicleID() const { return currentVehicleID; }
};

#endif
