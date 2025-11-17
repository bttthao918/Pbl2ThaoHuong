#include "Motorcycle.h"
#include <iostream>
#include <ctime>
#include <cmath>
#include <sstream>
#include <iomanip>
using namespace std;

Motorcycle::Motorcycle(string id, string owner, string phone,
                       string entry, string slot, string ticket,
                       int capacity, bool helmet)
    : Vehicle(id, owner, phone, entry, slot, ticket),
      engineCapacity(capacity), hasHelmet(helmet) {}

void Motorcycle::display() const {
    cout << "===== MOTORCYCLE =====\n";
    cout << "ID: " << vehicleID << endl;
    cout << "Owner: " << ownerName << endl;
    cout << "Phone: " << phoneNumber << endl;
    cout << "Entry: " << entryTime << endl;
    cout << "Exit: " << exitTime << endl;
    cout << "Slot: " << parkingSlotID << endl;
    cout << "Ticket: " << ticketID << endl;
    cout << "Engine: " << engineCapacity << " cc\n";
    cout << "Helmet: " << (hasHelmet ? "Yes" : "No") << endl;
}
double Motorcycle::calculateParkingFee(const RateManager& rateManager) const {
    // Tính số giờ gửi xe
    time_t now = time(nullptr);

    // Convert entryTime (stored as string) to time_t.
    // Try epoch-seconds string first, otherwise try parsing "YYYY-MM-DD HH:MM:SS".
    time_t entry_t = 0;
    bool parsed = false;
    try {
        entry_t = static_cast<time_t>(std::stoll(entryTime));
        parsed = true;
    } catch (...) {
        // fall through to try parsing human-readable format
    }

    if (!parsed) {
        std::tm tm = {};
        std::istringstream ss(entryTime);
        ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
        if (!ss.fail()) {
            entry_t = mktime(&tm);
            parsed = true;
        }
    }

    if (!parsed) {
        // Fallback: if parsing failed, assume entry time equals now so duration is minimum 0.
        entry_t = now;
    }

    double hours = difftime(now, entry_t) / 3600.0;
    if (hours < 1) hours = 1; // tối thiểu 1 giờ

    // Lấy giá từ RateManager
    double baseRate = rateManager.getRate("Motorbike");

    // Tính phí
    double fee = hours * baseRate;
    return (fee > 20000 ? 20000 : fee); // giới hạn tối đa
}

