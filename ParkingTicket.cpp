#include "ParkingTicket.h"
#include <iostream>
#include <iomanip>
using namespace std;

// Tạo ID vé dạng TK + yyyyMMddHHmm + random số
static int AUTO_ID = 1;

// Constructor
ParkingTicket::ParkingTicket(string vehicleID, string customerID,
                             string managerID, string entryTime,
                             string slotID)
{
    this->vehicleID = vehicleID;
    this->customerID = customerID;
    this->managerID = managerID;
    this->entryTime = entryTime;
    this->parkingSlotID = slotID;

    this->exitTime = "";
    this->fee = 0;
    this->discount = 0;
    this->finalFee = 0;
    this->status = "Active";

    // Auto sinh ticket ID: TK0001, TK0002,...
    ticketID = "TK" + to_string(1000 + AUTO_ID);
    AUTO_ID++;
}

// Tính số giờ
double ParkingTicket::calculateDuration() const {
    if (exitTime == "") return 0;

    // Ở đây anh demo, không parse thời gian thật
    // Em muốn tính giờ chính xác -> anh viết hàm parse ngày giờ riêng

    return 1.0; // tạm thời = 1 giờ cho demo
}

// Đóng vé
void ParkingTicket::closeTicket(string exitTime, double fee, double discount) {
    this->exitTime = exitTime;
    this->fee = fee;
    this->discount = discount;
    this->finalFee = fee - discount;
    if (this->finalFee < 0) this->finalFee = 0;
    this->status = "Completed";
}

// In vé đơn giản
void ParkingTicket::printTicket() const {
    cout << "\n===== PARKING TICKET =====\n";
    cout << "Ticket ID:   " << ticketID << endl;
    cout << "Vehicle ID:  " << vehicleID << endl;
    cout << "Customer ID: " << customerID << endl;
    cout << "Manager ID:  " << managerID << endl;
    cout << "Entry Time:  " << entryTime << endl;
    cout << "Exit Time:   " << exitTime << endl;
    cout << "Slot:        " << parkingSlotID << endl;
    cout << "Fee:         " << fee << " VND\n";
    cout << "Discount:    " << discount << " VND\n";
    cout << "Final Fee:   " << finalFee << " VND\n";
    cout << "Status:      " << status << endl;
}

// Hiển thị ngắn gọn
void ParkingTicket::display() const {
    cout << left << setw(10) << ticketID
         << setw(12) << vehicleID
         << setw(12) << customerID
         << setw(10) << parkingSlotID
         << setw(10) << status
         << endl;
}
