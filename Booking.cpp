#include "Booking.h"
#include "Exeptions.h"
#include <sstream>
using namespace std;

Booking::Booking() : bookingTime(time(nullptr)), expectedArrival(0),
                     status(BookingStatus::PENDING) {}

Booking::Booking(const string &id, const string &custId,
                 const string &vehId, time_t arrival)
    : bookingId(id), customerId(custId), vehicleId(vehId),
      bookingTime(time(nullptr)), expectedArrival(arrival),
      status(BookingStatus::PENDING), ticketId("") {}

void Booking::confirm() {
    if (status != BookingStatus::PENDING) {
        throw InvalidInputException("Chi co the xac nhan booking dang cho");
    }
    status = BookingStatus::CONFIRMED;
}

void Booking::cancel() {
    if (status == BookingStatus::COMPLETED) {
        throw InvalidInputException("Khong the huy booking da hoan thanh");
    }
    status = BookingStatus::CANCELLED;
}

void Booking::complete() {
    if (status != BookingStatus::CONFIRMED) {
        throw InvalidInputException("Booking chua duoc xac nhan");
    }
    status = BookingStatus::COMPLETED;
}

bool Booking::isExpired() const {
    time_t now = time(nullptr);
    return (status == BookingStatus::CONFIRMED || status == BookingStatus::PENDING) &&
           (now > expectedArrival + 1800); // 30 phút
}

void Booking::displayInfo() const {
    cout << "Booking ID: " << bookingId << endl;
    cout << "Khach hang ID: " << customerId << endl;
    cout << "Xe ID: " << vehicleId << endl;
    cout << "Thoi gian dat: " << Utils::timeToString(bookingTime) << endl;
    cout << "Du kien den: " << Utils::timeToString(expectedArrival) << endl;
    cout << "Trang thai: " << statusToString(status) << endl;
    if (!ticketId.empty()) {
        cout << "Ticket ID: " << ticketId << endl;
    }
}

string Booking::toFileString() const {
    ostringstream oss;
    oss << bookingId << "|" << customerId << "|" << vehicleId << "|"
        << bookingTime << "|" << expectedArrival << "|"
        << statusToString(status) << "|" << ticketId;
    return oss.str();
}

void Booking::fromFileString(const string &line) {
    istringstream iss(line);
    string statusStr;
    getline(iss, bookingId, '|');
    getline(iss, customerId, '|');
    getline(iss, vehicleId, '|');
    iss >> bookingTime;
    iss.ignore();
    iss >> expectedArrival;
    iss.ignore();
    getline(iss, statusStr, '|');
    status = stringToStatus(statusStr);
    getline(iss, ticketId);
}

ostream &operator<<(ostream &os, const Booking &booking) {
    os << "Booking " << booking.bookingId << " - "
       << Booking::statusToString(booking.status);
    return os;
}

bool Booking::operator==(const Booking &other) const {
    return bookingId == other.bookingId;
}

string Booking::statusToString(BookingStatus status) {
    switch (status) {
    case BookingStatus::PENDING: return "PENDING";
    case BookingStatus::CONFIRMED: return "CONFIRMED";
    case BookingStatus::CANCELLED: return "CANCELLED";
    case BookingStatus::COMPLETED: return "COMPLETED";
    default: return "UNKNOWN";
    }
}

BookingStatus Booking::stringToStatus(const string &str) {
    if (str == "PENDING") return BookingStatus::PENDING;
    if (str == "CONFIRMED") return BookingStatus::CONFIRMED;
    if (str == "CANCELLED") return BookingStatus::CANCELLED;
    if (str == "COMPLETED") return BookingStatus::COMPLETED;
    return BookingStatus::PENDING;
}