#ifndef PARKING_TICKET_H
#define PARKING_TICKET_H

#include <string>
#include <ctime>
#include <iostream>

enum class TicketStatus
{
    ACTIVE,
    PAID,
    CANCELLED
};

class ParkingTicket
{
private:
    std::string ticketId;
    std::string customerId;
    std::string vehicleId;
    std::string slotId;
    std::string bookingId;
    time_t checkInTime;
    time_t checkOutTime;
    double fee;
    TicketStatus status;

public:
    ParkingTicket();
    ParkingTicket(const std::string &id, const std::string &custId, const std::string &vehId, const std::string &slot);

    std::string getTicketId() const { return ticketId; }
    std::string getCustomerId() const { return customerId; }
    std::string getVehicleId() const { return vehicleId; }
    std::string getSlotId() const { return slotId; }
    std::string getBookingId() const { return bookingId; }
    time_t getCheckInTime() const { return checkInTime; }
    time_t getCheckOutTime() const { return checkOutTime; }
    double getFee() const { return fee; }
    TicketStatus getStatus() const { return status; }

    void setBookingId(const std::string &id) { bookingId = id; }
    void setSlotId(const std::string &id) { slotId = id; }
    void setStatus(TicketStatus s) { status = s; }

    void checkOut(double calculatedFee);
    void cancel();
    bool isActive() const { return status == TicketStatus::ACTIVE; }
    long long getParkingDuration() const;

    void displayInfo() const;
    void displayTableRow() const;
    std::string toFileString() const;
    void fromFileString(const std::string &line);
    friend std::ostream &operator<<(std::ostream &os, const ParkingTicket &ticket);
    bool operator==(const ParkingTicket &other) const;

    static std::string statusToString(TicketStatus status);
    static TicketStatus stringToStatus(const std::string &str);
};

#endif