#include "ParkingTicket.h"
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

ParkingTicket::ParkingTicket()
    : ticketID(""), licensePlate(""), customerPhone(""), customerName(""),
      vehicleType(""), slotNumber(0), checkInTime(0), checkOutTime(0), fee(0),
    checkedOut(false), managerAdjusted(false), adjustedBy(""), adjustedAt(0), adjustedNote("") {}

int ParkingTicket::calculateParkingHours() const {
    time_t end = checkedOut ? checkOutTime : time(nullptr);
    if (checkInTime == 0) return 0;
    double seconds = difftime(end, checkInTime);
    double hours = seconds / 3600.0;
    // Làm tròn lên mỗi phần giờ => tính theo giờ tính phí
    return (int)ceil(hours <= 0.0 ? 1.0 : hours);
}

void ParkingTicket::display() const {
    cout << "Ticket: " << ticketID << " | Plate: " << licensePlate
         << " | Slot: " << slotNumber << " | Type: " << vehicleType
         << " | Check-in: ";
    if (checkInTime) {
        char buf[64];
        tm* t = localtime(&checkInTime);
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", t);
        cout << buf;
    } else cout << "-";

    cout << " | Check-out: ";
    if (checkedOut && checkOutTime) {
        char buf2[64];
        tm* t2 = localtime(&checkOutTime);
        strftime(buf2, sizeof(buf2), "%Y-%m-%d %H:%M:%S", t2);
        cout << buf2;
    } else cout << "-";

    cout << " | Fee: " << fee << (checkedOut ? " (paid)" : " (open)") << endl;
    if (managerAdjusted) {
        cout << "    [Adjusted by: " << adjustedBy << " at ";
        if (adjustedAt) {
            char buf[64];
            tm* t = localtime(&adjustedAt);
            strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", t);
            cout << buf;
        } else cout << "-";
        cout << "]";
        if (!adjustedNote.empty()) cout << " Note: " << adjustedNote;
        cout << endl;
    }
}

void ParkingTicket::managerAdjustFee(double newFee, const string& managerID, const string& note) {
    fee = newFee;
    managerAdjusted = true;
    adjustedBy = managerID;
    adjustedAt = time(nullptr);
    adjustedNote = note;
}
