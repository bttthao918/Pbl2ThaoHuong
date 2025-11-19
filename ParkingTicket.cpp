#include "ParkingTicket.h"
#include "Exceptions.h"
#include "Utils.h"
#include <iostream>
#include <sstream>
using namespace std;

ParkingTicket::ParkingTicket() : checkInTime(time(nullptr)), checkOutTime(0),
                                 fee(0.0), status(TicketStatus::ACTIVE) {}

ParkingTicket::ParkingTicket(const string &id, const string &custId,
                             const string &vehId, const string &slot)
    : ticketId(id), customerId(custId), vehicleId(vehId), slotId(slot),
      bookingId(""), checkInTime(time(nullptr)), checkOutTime(0),
      fee(0.0), status(TicketStatus::ACTIVE) {}

void ParkingTicket::checkOut(double calculatedFee)
{
    if (status != TicketStatus::ACTIVE)
    {
        throw InvalidInputException("Ticket khong con hoat dong");
    }
    checkOutTime = time(nullptr);
    fee = calculatedFee;
    status = TicketStatus::PAID;
}

void ParkingTicket::cancel()
{
    if (status == TicketStatus::PAID)
    {
        throw InvalidInputException("Khong the huy ticket da thanh toan");
    }
    status = TicketStatus::CANCELLED;
    checkOutTime = time(nullptr);
}

long long ParkingTicket::getParkingDuration() const
{
    if (checkOutTime == 0)
    {
        return Utils::calculateDuration(checkInTime, time(nullptr));
    }
    return Utils::calculateDuration(checkInTime, checkOutTime);
}

void ParkingTicket::displayInfo() const
{
    cout << "Ticket ID: " << ticketId << endl;
    cout << "Khach hang ID: " << customerId << endl;
    cout << "Xe ID: " << vehicleId << endl;
    cout << "Vi tri: " << slotId << endl;
    if (!bookingId.empty())
    {
        cout << "Booking ID: " << bookingId << endl;
    }
    cout << "Check-in: " << Utils::timeToString(checkInTime) << endl;
    if (checkOutTime > 0)
    {
        cout << "Check-out: " << Utils::timeToString(checkOutTime) << endl;
        cout << "Thoi gian gui: " << getParkingDuration() << " phut" << endl;
        cout << "Phi: " << fee << " VND" << endl;
    }
    cout << "Trang thai: " << statusToString(status) << endl;
}

string ParkingTicket::toFileString() const
{
    ostringstream oss;
    oss << ticketId << "|" << customerId << "|" << vehicleId << "|"
        << slotId << "|" << bookingId << "|"
        << checkInTime << "|" << checkOutTime << "|"
        << fee << "|" << statusToString(status);
    return oss.str();
}

void ParkingTicket::fromFileString(const string &line)
{
    istringstream iss(line);
    string statusStr;
    getline(iss, ticketId, '|');
    getline(iss, customerId, '|');
    getline(iss, vehicleId, '|');
    getline(iss, slotId, '|');
    getline(iss, bookingId, '|');
    iss >> checkInTime;
    iss.ignore();
    iss >> checkOutTime;
    iss.ignore();
    iss >> fee;
    iss.ignore();
    getline(iss, statusStr);
    status = stringToStatus(statusStr);
}

ostream &operator<<(ostream &os, const ParkingTicket &ticket)
{
    os << "Ticket " << ticket.ticketId << " - "
       << ParkingTicket::statusToString(ticket.status);
    return os;
}

bool ParkingTicket::operator==(const ParkingTicket &other) const
{
    return ticketId == other.ticketId;
}

string ParkingTicket::statusToString(TicketStatus status)
{
    switch (status)
    {
    case TicketStatus::ACTIVE:
        return "ACTIVE";
    case TicketStatus::PAID:
        return "PAID";
    case TicketStatus::CANCELLED:
        return "CANCELLED";
    default:
        return "UNKNOWN";
    }
}

TicketStatus ParkingTicket::stringToStatus(const string &str)
{
    if (str == "ACTIVE")
        return TicketStatus::ACTIVE;
    if (str == "PAID")
        return TicketStatus::PAID;
    if (str == "CANCELLED")
        return TicketStatus::CANCELLED;
    return TicketStatus::ACTIVE;
}