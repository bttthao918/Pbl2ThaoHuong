#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>

using namespace std;

namespace Utils
{
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

    string generateID(const string &prefix);
}

#endif
