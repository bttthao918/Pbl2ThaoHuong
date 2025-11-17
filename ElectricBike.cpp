#include "ElectricBike.h"
#include <iostream>
#include <ctime>
#include <cmath>
#include <sstream>
#include <iomanip>
using namespace std;

ElectricBike::ElectricBike(string id, string owner, string phone,
                           string entry, string slot, string ticket,
                           int battery, string b)
    : Vehicle(id, owner, phone, entry, slot, ticket),
      batteryCapacity(battery), brand(b) {}

void ElectricBike::display() const {
    cout << "===== ELECTRIC BIKE =====\n";
    cout << "ID: " << vehicleID << endl;
    cout << "Owner: " << ownerName << endl;
    cout << "Phone: " << phoneNumber << endl;
    cout << "Entry: " << entryTime << endl;
    cout << "Exit: " << exitTime << endl;
    cout << "Slot: " << parkingSlotID << endl;
    cout << "Ticket: " << ticketID << endl;
    cout << "Battery: " << batteryCapacity << " mAh\n";
    cout << "Brand: " << brand << endl;
}

double ElectricBike::calculateParkingFee(const RateManager& rateManager) const {
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
    double baseRate = rateManager.getRate("ElectricBike");

    // Tính phí
    double fee = hours * baseRate;
    return (fee > 15000 ? 15000 : fee); // giới hạn tối đa
}

