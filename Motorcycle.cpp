#include "Motorcycle.h"
#include <iostream>
using namespace std;

Motorcycle::Motorcycle(string id, string owner, string phone,
                       string entry, string slot, string ticket,
                       int capacity, bool helmet)
    : Vehicle(id, owner, phone, entry, slot, ticket),
      engineCapacity(capacity), hasHelmet(helmet) {}

void Motorcycle::display() const {
    cout << "===== MOTORCYCLE =====\n";
    cout << "ID: " << vehicleID << endl;
    cout << "Owner: " << ownerName << endl;
    cout << "Phone: " << phoneNumber << endl;
    cout << "Entry: " << entryTime << endl;
    cout << "Exit: " << exitTime << endl;
    cout << "Slot: " << parkingSlotID << endl;
    cout << "Ticket: " << ticketID << endl;
    cout << "Engine: " << engineCapacity << " cc\n";
    cout << "Helmet: " << (hasHelmet ? "Yes" : "No") << endl;
}

double Motorcycle::calculateParkingFee() const {
    double hours = 1; // Em thêm tính giờ sau
    double fee = hours * 3000;
    return (fee > 20000 ? 20000 : fee);
}
