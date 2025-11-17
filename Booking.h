#ifndef BOOKING_H
#define BOOKING_H

#include <string>
#include <ctime>
#include "ParkingTicket.h"
using namespace std;

class Booking {
private:
    string bookingID;
    string customerID;
    string licensePlate;
    string vehicleType;
    int slotNumber;
    time_t checkInTime;
    string status;         // "Pending" hoặc "Confirmed"
    string paymentMethod;  // "Cash" hoặc "QR"
    ParkingTicket ticket;  // Vé gửi xe sau khi thanh toán

public:
    // Constructor
    Booking(string bID = "", string cID = "", string plate = "", string vType = "", int slot = 0);

    // Methods
    void confirmPayment(const string& method, double fee);
    void display() const;
    void showTicket() const;
    void saveToFile(const string& filename) const;
    static void loadFromFile(const string& filename);

    // Getters
    string getBookingID() const { return bookingID; }
    string getStatus() const { return status; }
};

#endif