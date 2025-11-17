#include "Customer.h"
#include "DoubleLinkedList.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

const string Customer::CUSTOMER_FILE = "customers.txt";
const string Customer::BOOKING_FILE = "bookings.txt";

Customer::Customer(string id, string user, string pass,
                   string name, string phone, string mail)
    : User(id, user, pass, name, phone, mail)
{
}

// Pure virtual overrides
void Customer::display()
{
    cout << "=== CUSTOMER ===\n";
    cout << "ID: " << userID << " | Username: " << username
         << " | Name: " << fullName << endl;
}

string Customer::getUserType()
{
    return "Customer";
}

void Customer::showMenu()
{
    cout << "\n--- Customer Menu ---\n";
    cout << "1. View Account\n";
    cout << "2. Edit Account\n";
    cout << "3. View Booking History\n";
    cout << "4. Exit\n";
    cout << "Choose: ";
}

// helpers
vector<string> Customer::splitLine(const string &line, char delim)
{
    vector<string> res;
    string token;
    istringstream ss(line);
    while (getline(ss, token, delim))
        res.push_back(token);
    return res;
}

// Account
void Customer::viewAccountInfo() const
{
    cout << "UserID: " << userID << "\nUsername: " << username
         << "\nName: " << fullName << "\nPhone: " << phoneNumber
         << "\nEmail: " << email << endl;
}

void Customer::editAccountInfo(const string &name, const string &phone, const string &mail)
{
    // cập nhật bộ nhớ (User có thể có hàm helper; nếu không có, gán trực tiếp)
    fullName = name;
    phoneNumber = phone;
    email = mail;
    updateCustomerInFile();
}

// Customers file operations
DoubleLinkedList<Customer> Customer::loadAllCustomers()
{
    DoubleLinkedList<Customer> list;
    ifstream in(CUSTOMER_FILE);
    if (!in.is_open())
        return list;
    string line;
    while (getline(in, line))
    {
        if (line.empty())
            continue;
        auto parts = splitLine(line, '|');
        // format: userID|username|password|fullName|phone|email
        if (parts.size() < 6)
            continue;
        list.push_back(Customer(parts[0], parts[1], parts[2], parts[3], parts[4], parts[5]));
    }
    in.close();
    return list;
}

bool Customer::saveAllCustomers(const DoubleLinkedList<Customer> &list)
{
    ofstream out(CUSTOMER_FILE, ios::trunc);
    if (!out.is_open())
        return false;
    Node<Customer> *cur = list.getHead();
    while (cur != nullptr)
    {
        const Customer &c = cur->data;
        out << c.userID << "|" << c.username << "|" << c.password << "|"
            << c.fullName << "|" << c.phoneNumber << "|" << c.email << "\n";
        cur = cur->next;
    }
    out.close();
    return true;
}

void Customer::saveCustomerToFileAppend() const
{
    ofstream out(CUSTOMER_FILE, ios::app);
    if (!out.is_open())
        return;
    out << userID << "|" << username << "|" << password << "|"
        << fullName << "|" << phoneNumber << "|" << email << "\n";
    out.close();
}

void Customer::updateCustomerInFile() const
{
    // đọc tất cả, thay dòng tương ứng userID, ghi đè
    DoubleLinkedList<Customer> all = loadAllCustomers();
    Node<Customer> *cur = all.getHead();
    bool found = false;
    while (cur != nullptr)
    {
        if (cur->data.userID == userID)
        {
            cur->data = *this;
            found = true;
            break;
        }
        cur = cur->next;
    }
    if (!found)
    {
        // nếu không tồn tại thì append
        all.push_back(*this);
    }
    saveAllCustomers(all);
}

// Booking history operations
DoubleLinkedList<BookingRecord> Customer::loadAllBookings()
{
    DoubleLinkedList<BookingRecord> list;
    ifstream in(BOOKING_FILE);
    if (!in.is_open())
        return list;
    string line;
    while (getline(in, line))
    {
        if (line.empty())
            continue;
        auto p = splitLine(line, '|');
        // expected: bookingID|customerID|licensePlate|vehicleType|slotNumber|checkIn|checkOut|status|paymentMethod|fee
        if (p.size() < 10)
            continue;
        BookingRecord r;
        r.bookingID = p[0];
        r.customerID = p[1];
        r.licensePlate = p[2];
        r.vehicleType = p[3];
        r.slotNumber = stoi(p[4]);
        r.checkIn = p[5];
        r.checkOut = p[6];
        r.status = p[7];
        r.paymentMethod = p[8];
        try
        {
            r.fee = stod(p[9]);
        }
        catch (...)
        {
            r.fee = 0.0;
        }
        list.push_back(r);
    }
    in.close();
    return list;
}

bool Customer::saveBookingRecordAppend(const BookingRecord &rec)
{
    ofstream out(BOOKING_FILE, ios::app);
    if (!out.is_open())
        return false;
    out << rec.bookingID << "|" << rec.customerID << "|" << rec.licensePlate << "|"
        << rec.vehicleType << "|" << rec.slotNumber << "|" << rec.checkIn << "|"
        << rec.checkOut << "|" << rec.status << "|" << rec.paymentMethod << "|"
        << rec.fee << "\n";
    out.close();
    return true;
}

void Customer::loadMyBookingHistory()
{
    bookingHistory.clear();
    DoubleLinkedList<BookingRecord> all = loadAllBookings();
    Node<BookingRecord> *cur = all.getHead();
    while (cur != nullptr)
    {
        if (cur->data.customerID == userID)
        {
            bookingHistory.push_back(cur->data);
        }
        cur = cur->next;
    }
}

void Customer::viewBookingHistory() const
{
    if (bookingHistory.get_size() == 0)
    {
        cout << "Không có lịch sử gửi xe.\n";
        return;
    }
    cout << "--- Lịch sử gửi xe của " << fullName << " ---\n";
    Node<BookingRecord> *cur = bookingHistory.getHead();
    while (cur != nullptr)
    {
        const BookingRecord &r = cur->data;
        cout << "BookingID: " << r.bookingID
             << " | Slot: " << r.slotNumber
             << " | Vehicle: " << r.licensePlate << " (" << r.vehicleType << ")"
             << " | In: " << r.checkIn << " | Out: " << r.checkOut
             << " | Status: " << r.status
             << " | Pay: " << r.paymentMethod
             << " | Fee: " << r.fee << "\n";
        cur = cur->next;
    }
}