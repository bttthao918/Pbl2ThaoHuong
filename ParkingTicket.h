#ifndef PARKING_TICKET_H
#define PARKING_TICKET_H

#include <string>
using namespace std;

class ParkingTicket {
private:
    string ticketID;        // TK20241117001
    string vehicleID;       // biển số xe
    string customerID;      // mã khách hàng
    string managerID;       // mã nhân viên check-in
    string entryTime;       // HH:MM DD/MM/YYYY
    string exitTime;        // nếu chưa ra -> ""
    string parkingSlotID;   // A01, B15,...
    double fee;             // phí gốc
    double discount;        // giảm giá
    double finalFee;        // phí sau giảm
    string status;          // Active / Completed

public:
    ParkingTicket(string vehicleID, string customerID, string managerID,
                  string entryTime, string slotID);

    double calculateDuration() const;       // tính số giờ
    void closeTicket(string exitTime, double fee, double discount);
    void printTicket() const;
    void display() const;

    // Getters
    string getTicketID() const { return ticketID; }
    string getStatus() const { return status; }
    double getFinalFee() const { return finalFee; }
};

#endif
