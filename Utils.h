#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <limits>

using namespace std;

namespace Utils
{
    inline void clearScreen()
    {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

    inline void pause()
    {
        cout << "\nNhan Enter de tiep tuc...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    bool isValidPhoneNumber(const string &phone);
    bool isValidEmail(const string &email);
    bool isValidLicensePlate(const string &plate);

    string getCurrentDateTime();
    time_t stringToTime(const string &timeStr);
    string timeToString(time_t time);
    long long calculateDuration(time_t start, time_t end);

    string trim(const string &str);
    string toUpper(const string &str);
    string toLower(const string &str);

    // string generateID(const string &prefix);
    string createInitials(const string &fullName);

    string generateID(const string &prefix);

    // Tạo Vehicle ID dễ nhớ
    string generateReadableVehicleID(const string &ownerName,
                                     const string &phoneNumber,
                                     int vehicleCount);
}

#endif
