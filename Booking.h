#ifndef BOOKING_H
#define BOOKING_H

#include <string>
#include <ctime>
#include <iostream>
using namespace std;

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
    string bookingId;
    string customerId;
    string vehicleId;
    string slotId;
    time_t bookingTime;
    time_t expectedArrival;
    BookingStatus status;
    string ticketId;

public:
    Booking();
    Booking(const string &id, const string &custId, const string &vehId, time_t arrival);

    // Getters
    string getBookingId() const { return bookingId; }
    string getCustomerId() const { return customerId; }
    string getVehicleId() const { return vehicleId; }
    string getSlotId() const { return slotId; }
    time_t getBookingTime() const { return bookingTime; }
    time_t getExpectedArrival() const { return expectedArrival; }
    BookingStatus getStatus() const { return status; }
    string getTicketId() const { return ticketId; }

    // Setters
    void setStatus(BookingStatus s) { status = s; }
    void setTicketId(const string &id) { ticketId = id; }
    void setSlotId(const std::string &id) { slotId = id; }
    void setExpectedArrival(time_t arrival) { expectedArrival = arrival; }

    // Methods
    void confirm();
    void cancel();
    void complete();
    bool isExpired() const;

    void displayInfo() const;
    string toFileString() const;
    void fromFileString(const string &line);

    friend ostream &operator<<(ostream &os, const Booking &booking);
    bool operator==(const Booking &other) const;

    static string statusToString(BookingStatus status);
    static BookingStatus stringToStatus(const string &str);
};

#endif