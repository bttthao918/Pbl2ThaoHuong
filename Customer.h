#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "User.h"
#include "DoubleLinkedList.h"
#include <string>
#include <vector>

using namespace std;

struct BookingRecord
{
    string bookingID;
    string customerID;
    string licensePlate;
    string vehicleType;
    int slotNumber;
    string checkIn;
    string checkOut;
    string status;
    string paymentMethod;
    double fee;
};

class Customer : public User
{
private:
    DoubleLinkedList<BookingRecord> bookingHistory;
    static const string CUSTOMER_FILE;
    static const string BOOKING_FILE;
    static vector<string> splitLine(const string &line, char delim = '|');

public:
    Customer(string id = "", string user = "", string pass = "",
             string name = "", string phone = "", string mail = "");

    // Pure virtual overrides từ User
    void display() override;
    string getUserType() override;
    void showMenu() override;

    // Quản lý tài khoản
    void viewAccountInfo() const;
    void editAccountInfo(const string &name, const string &phone, const string &email);

    // Lưu / đọc danh sách customers
    static DoubleLinkedList<Customer> loadAllCustomers();
    static bool saveAllCustomers(const DoubleLinkedList<Customer> &list);
    void saveCustomerToFileAppend() const;
    void updateCustomerInFile() const;

    // Lịch sử gửi xe
    static DoubleLinkedList<BookingRecord> loadAllBookings();
    static bool saveBookingRecordAppend(const BookingRecord &rec);
    void loadMyBookingHistory();
    void viewBookingHistory() const;

    const DoubleLinkedList<BookingRecord> &getBookingHistory() const { return bookingHistory; }
};

#endif