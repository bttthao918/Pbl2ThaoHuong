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

    bool isValidPhoneNumber(const std::string &phone);
    bool isValidEmail(const std::string &email);
    bool isValidLicensePlate(const std::string &plate);

    std::string getCurrentDateTime();
    time_t stringToTime(const std::string &timeStr);
    std::string timeToString(time_t time);
    long long calculateDuration(time_t start, time_t end);

    std::string trim(const std::string &str);
    std::string toUpper(const std::string &str);
    std::string toLower(const std::string &str);

    std::string createInitials(const std::string &fullName);

    std::string generateID(const std::string &prefix);

    std::string generateReadableVehicleID(const std::string &ownerName,
                                          const std::string &phoneNumber,
                                          int vehicleCount);
}

#endif
