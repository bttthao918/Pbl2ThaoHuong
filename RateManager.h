#ifndef RATEMANAGER_H
#define RATEMANAGER_H

#include <map>
#include <string>
#include <iostream>
using namespace std;

class RateManager {
private:
    map<string, double> rates; // Car, Motorbike, ElectricBike
public:
    RateManager() {
        rates["Car"] = 5000;          // giá mỗi giờ
        rates["Motorbike"] = 2000;
        rates["ElectricBike"] = 3000;
    }

    double getRate(const string& type) const {
        auto it = rates.find(type);
        return (it != rates.end()) ? it->second : 0.0;
    }

    void updateRate(const string& type, double newRate) {
        if (rates.find(type) != rates.end()) {
            rates[type] = newRate;
        } else {
            cout << ">>> Loại xe không hợp lệ!\n";
        }
    }

    void displayRates() const {
        cout << "\n===== GIÁ VÉ HIỆN TẠI =====\n";
        for (auto& r : rates) {
            cout << r.first << ": " << r.second << " VND/giờ\n";
        }
    }
};

#endif