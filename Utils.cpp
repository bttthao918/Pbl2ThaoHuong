#include "Utils.h"
#include <algorithm>
#include <regex>
#include <random>

using namespace std;

namespace Utils
{

    bool isValidPhoneNumber(const string &phone)
    {
        if (phone.length() < 10 || phone.length() > 11)
            return false;
        for (char c : phone)
        {
            if (!isdigit(c))
                return false;
        }
        return true;
    }

    bool isValidEmail(const string &email)
    {
        regex pattern(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
        return regex_match(email, pattern);
    }

    bool isValidLicensePlate(const string &plate)
    {
        if (plate.empty())
            return false;
        regex pattern(R"(\d{2}[A-Z]-?\d{4,5})");
        return regex_match(plate, pattern);
    }

    string getCurrentDateTime()
    {
        time_t now = time(nullptr);
        return timeToString(now);
    }

    time_t stringToTime(const string &timeStr)
    {
        struct tm tm = {};
        istringstream ss(timeStr);
        ss >> get_time(&tm, "%Y-%m-%d %H:%M:%S");
        return mktime(&tm);
    }

    string timeToString(time_t time)
    {
        char buffer[80];
        struct tm *timeinfo = localtime(&time);
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
        return string(buffer);
    }

    long long calculateDuration(time_t start, time_t end)
    {
        return (end - start) / 60;
    }

    string trim(const string &str)
    {
        size_t first = str.find_first_not_of(" \t\n\r");
        if (first == string::npos)
            return "";
        size_t last = str.find_last_not_of(" \t\n\r");
        return str.substr(first, last - first + 1);
    }

    string toUpper(const string &str)
    {
        string result = str;
        transform(result.begin(), result.end(), result.begin(), ::toupper);
        return result;
    }

    string toLower(const string &str)
    {
        string result = str;
        transform(result.begin(), result.end(), result.begin(), ::tolower);
        return result;
    }

    string generateID(const string &prefix)
    {
        static random_device rd;
        static mt19937 gen(rd());
        static uniform_int_distribution<> dis(1000, 9999);

        time_t now = time(nullptr);
        return prefix + to_string(now) + to_string(dis(gen));
    }

} // namespace Utils
