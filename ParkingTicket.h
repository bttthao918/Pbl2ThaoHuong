#ifndef PARKING_TICKET_H
#define PARKING_TICKET_H

#include <string>
#include <ctime>
using namespace std;

class ParkingTicket {
private:
    string ticketID;
    string licensePlate;
    string customerPhone;
    string customerName;
    string vehicleType;
    int slotNumber; // số chỗ đỗ
    time_t checkInTime;
    time_t checkOutTime;
    double fee;
    bool checkedOut;
    // Manager adjustment metadata
    bool managerAdjusted;
    string adjustedBy;
    time_t adjustedAt;
    string adjustedNote;
    // Manager who created/handled the ticket
    string managerID;
    string managerName;

public:
    ParkingTicket();

    // Setters
    void setTicketID(const string& id) { ticketID = id; }
    void setLicensePlate(const string& lp) { licensePlate = lp; }
    void setCustomerPhone(const string& phone) { customerPhone = phone; }
    void setCustomerName(const string& name) { customerName = name; }
    void setVehicleType(const string& vt) { vehicleType = vt; }
    void setSlotNumber(int n) { slotNumber = n; }
    void setCheckInTime(time_t t) { checkInTime = t; }
    void setCheckOutTime(time_t t) { checkOutTime = t; }
    void setFee(double f) { fee = f; }
    void setCheckedOut(bool v) { checkedOut = v; }

    // Getters
    string getTicketID() const { return ticketID; }
    string getLicensePlate() const { return licensePlate; }
    int getSlotNumber() const { return slotNumber; }
    bool isCheckedOut() const { return checkedOut; }

    // Utilities
    int calculateParkingHours() const; // số giờ (làm tròn lên)
    void display() const; // in vé
    // Manager can adjust fee after checkout
    void managerAdjustFee(double newFee, const string& managerID, const string& note = "");

    // Getters
    double getFee() const { return fee; }
    bool wasAdjusted() const { return managerAdjusted; }
    string getAdjustedBy() const { return adjustedBy; }
    time_t getAdjustedAt() const { return adjustedAt; }
    string getAdjustedNote() const { return adjustedNote; }
    // Manager setters/getters
    void setManagerID(const string& id) { managerID = id; }
    void setManagerName(const string& name) { managerName = name; }
    string getManagerID() const { return managerID; }
    string getManagerName() const { return managerName; }
};

#endif
