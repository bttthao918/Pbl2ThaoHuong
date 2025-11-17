#include "ParkingSlot.h"
#include <iostream>
using namespace std;

// Constructor
ParkingSlot::ParkingSlot(string id, string zone, int floor, string vType)
{
    this->slotID = id;
    this->zone = zone;
    this->floor = floor;
    this->vehicleType = vType;
    this->isOccupied = false;
    this->currentVehicleID = "";
}

// Chiếm chỗ
void ParkingSlot::occupy(string vehicleID)
{
    if (isOccupied) {
        cout << ">>> Lỗi: Vị trí " << slotID << " đã có xe!\n";
        return;
    }

    isOccupied = true;
    currentVehicleID = vehicleID;
}

// Giải phóng chỗ
void ParkingSlot::release()
{
    isOccupied = false;
    currentVehicleID = "";
}

// Hiển thị thông tin
void ParkingSlot::display() const
{
    cout << "===== PARKING SLOT =====\n";
    cout << "Slot ID:     " << slotID << endl;
    cout << "Zone:        " << zone << endl;
    cout << "Floor:       " << floor << endl;
    cout << "VehicleType: " << vehicleType << endl;
    cout << "Occupied:    " << (isOccupied ? "Yes" : "No") << endl;
    if (isOccupied) {
        cout << "Vehicle ID:  " << currentVehicleID << endl;
    }
}
