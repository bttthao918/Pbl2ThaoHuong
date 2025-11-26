#ifndef BOOKING_H
#define BOOKING_H

#include <string>
#include <ctime>
#include <iostream>

enum class BookingStatus
{
    PENDING,
    CONFIRMED,
    CANCELLED,
    COMPLETED
};

class Booking
{
private:
    std::string bookingId;
    std::string customerId;
    std::string vehicleId;
    std::string slotId;
    time_t bookingTime;
    time_t expectedArrival;
    BookingStatus status;
    std::string ticketId;

public:
    Booking();
    Booking(const std::string &id, const std::string &custId, const std::string &vehId, time_t arrival);

    // Getters
    std::string getBookingId() const { return bookingId; }
    std::string getCustomerId() const { return customerId; }
    std::string getVehicleId() const { return vehicleId; }
    std::string getSlotId() const { return slotId; }
    time_t getBookingTime() const { return bookingTime; }
    time_t getExpectedArrival() const { return expectedArrival; }
    BookingStatus getStatus() const { return status; }
    std::string getTicketId() const { return ticketId; }

    // Setters
    void setStatus(BookingStatus s) { status = s; }
    void setTicketId(const std::string &id) { ticketId = id; }
    void setSlotId(const std::string &id) { slotId = id; }
    void setExpectedArrival(time_t arrival) { expectedArrival = arrival; }

    // Methods
    void confirm();
    void cancel();
    void complete();
    bool isExpired() const;

    void displayInfo() const;
    void displayTableRow() const;
    std::string toFileString() const;
    void fromFileString(const std::string &line);

    friend std::ostream &operator<<(std::ostream &os, const Booking &booking);
    bool operator==(const Booking &other) const;

    static std::string statusToString(BookingStatus status);
    static BookingStatus stringToStatus(const std::string &str);
};

#endif