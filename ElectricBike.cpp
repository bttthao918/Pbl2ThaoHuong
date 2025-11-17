#include "ElectricBike.h"
#include <iostream>
using namespace std;

ElectricBike::ElectricBike(string id, string owner, string phone,
                           string entry, string slot, string ticket,
                           int battery, string b)
    : Vehicle(id, owner, phone, entry, slot, ticket),
      batteryCapacity(battery), brand(b) {}

void ElectricBike::display() const {
    cout << "===== ELECTRIC BIKE =====\n";
    cout << "ID: " << vehicleID << endl;
    cout << "Owner: " << ownerName << endl;
    cout << "Phone: " << phoneNumber << endl;
    cout << "Entry: " << entryTime << endl;
    cout << "Exit: " << exitTime << endl;
    cout << "Slot: " << parkingSlotID << endl;
    cout << "Ticket: " << ticketID << endl;
    cout << "Battery: " << batteryCapacity << " mAh\n";
    cout << "Brand: " << brand << endl;
}

double ElectricBike::calculateParkingFee() const {
    double hours = 1;
    double fee = hours * 2000;
    return (fee > 15000 ? 15000 : fee);
}
