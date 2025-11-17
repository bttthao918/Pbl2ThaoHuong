#include "Car.h"
#include <iostream>
#include <ctime>
#include <cmath>
#include <sstream>
#include <iomanip>

using namespace std;

Car::Car(string id, string owner, string phone,
         string entry, string slot, string ticket,
         string plateType, int seats)
    : Vehicle(id, owner, phone, entry, slot, ticket),
      licensePlateType(plateType), numberOfSeats(seats) {}

void Car::display() const {
    cout << "===== CAR =====\n";
    cout << "ID: " << vehicleID << endl;
    cout << "Owner: " << ownerName << endl;
    cout << "Phone: " << phoneNumber << endl;
    cout << "Entry: " << entryTime << endl;
    cout << "Exit: " << exitTime << endl;
    cout << "Slot: " << parkingSlotID << endl;
    cout << "Ticket: " << ticketID << endl;
    cout << "Plate type: " << licensePlateType << endl;
    cout << "Seats: " << numberOfSeats << endl;
}

double Car::calculateParkingFee(const RateManager& rateManager) const {
    time_t now = time(nullptr);

    time_t entry_t = 0;
    bool parsed = false;
    try {
        entry_t = static_cast<time_t>(std::stoll(entryTime));
        parsed = true;
    } catch (...) { }

    if (!parsed) {
        std::tm tm = {};
        std::istringstream ss(entryTime);
        ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
        if (!ss.fail()) {
            entry_t = mktime(&tm);
            parsed = true;
        }
    }

    if (!parsed) entry_t = now;

    double hours = difftime(now, entry_t) / 3600.0;
    if (hours < 1) hours = 1;

    double baseRate = rateManager.getRate("Car");
    double fee = hours * baseRate;

    return (fee > 50000 ? 50000 : fee);
}