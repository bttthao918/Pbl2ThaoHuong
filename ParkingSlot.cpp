#include "ParkingSlot.h"
#include <iostream>
using namespace std;

// Default constructor
ParkingSlot::ParkingSlot()
    : slotNumber(0), slotID(""), zone(""), floor(0), vehicleType(""),
      occupied(false), currentVehicleID("") {}

// Param constructor
ParkingSlot::ParkingSlot(int slotNumber, string zone, int floor, string vType)
    : slotNumber(slotNumber), slotID(""), zone(zone), floor(floor),
      vehicleType(vType), occupied(false), currentVehicleID("") {}

// Chiếm chỗ (biển số)
void ParkingSlot::occupy(const string& vehicleID)
{
    if (occupied) {
        cout << ">>> Lỗi: Chỗ " << slotNumber << " đang có xe!\n";
        return;
    }
    occupied = true;
    currentVehicleID = vehicleID;
}

// Chiếm chỗ (biển số + cập nhật loại xe)
void ParkingSlot::occupy(const string& vehicleID, const string& vehicleType)
{
    if (occupied) {
        cout << ">>> Lỗi: Chỗ " << slotNumber << " đang có xe!\n";
        return;
    }
    occupied = true;
    currentVehicleID = vehicleID;
    this->vehicleType = vehicleType;
}

// Giải phóng chỗ
void ParkingSlot::release()
{
    occupied = false;
    currentVehicleID = "";
}

// Hiển thị thông tin
void ParkingSlot::display() const
{
    cout << "Slot #" << slotNumber;
    if (!slotID.empty()) cout << " (" << slotID << ")";
    cout << "  | Zone: " << zone << " | Floor: " << floor
         << " | Type: " << vehicleType
         << " | Occupied: " << (occupied ? "Yes" : "No");
    if (occupied) cout << " | Vehicle: " << currentVehicleID;
    cout << endl;
}
