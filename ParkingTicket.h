#ifndef PARKING_TICKET_H
#define PARKING_TICKET_H

#include <string>
#include <ctime>
#include <iostream>
using namespace std;

enum class TicketStatus {
    ACTIVE,
    PAID,
    CANCELLED
};

class ParkingTicket {
private:
    string ticketId;
    string customerId;
    string vehicleId;
    string slotId;
    string bookingId;
    time_t checkInTime;
    time_t checkOutTime;
    double fee;
    TicketStatus status;

public:
    ParkingTicket();
    ParkingTicket(const string &id, const string &custId, const string &vehId, const string &slot);

    // Getters
    string getTicketId() const { return ticketId; }
    string getCustomerId() const { return customerId; }
    string getVehicleId() const { return vehicleId; }
    string getSlotId() const { return slotId; }
    string getBookingId() const { return bookingId; }
    time_t getCheckInTime() const { return checkInTime; }
    time_t getCheckOutTime() const { return checkOutTime; }
    double getFee() const { return fee; }
    TicketStatus getStatus() const { return status; }

    // Setters
    void setBookingId(const string &id) { bookingId = id; }
    void setSlotId(const string &id) { slotId = id; }
    void setStatus(TicketStatus s) { status = s; }

    // Methods
    void checkOut(double calculatedFee);
    void cancel();
    bool isActive() const { return status == TicketStatus::ACTIVE; }
    long long getParkingDuration() const;

    void displayInfo() const;
    string toFileString() const;
    void fromFileString(const string &line);

    friend ostream &operator<<(ostream &os, const ParkingTicket &ticket);
    bool operator==(const ParkingTicket &other) const;

    static string statusToString(TicketStatus status);
    static TicketStatus stringToStatus(const string &str);
};

#endif