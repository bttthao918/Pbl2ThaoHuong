#include "Customer.h"
#include "Exceptions.h"
#include <sstream>
#include <iomanip>

using namespace std;

Customer::Customer() : User(), loyaltyPoints(0)
{
    role = UserRole::CUSTOMER;
}

Customer::Customer(const string &id, const string &uname, const string &pwd,
                   const string &name, const string &phone, const string &mail)
    : User(id, uname, pwd, name, phone, mail, UserRole::CUSTOMER), loyaltyPoints(0) {}

void Customer::displayInfo() const
{
    User::displayInfo();
    cout << "Diem tich luy: " << loyaltyPoints << endl;
}

string Customer::toFileString() const
{
    ostringstream oss;
    oss << User::toFileString() << "|" << loyaltyPoints;
    return oss.str();
}

void Customer::fromFileString(const string &line)
{
    istringstream iss(line);
    string baseData;

    for (int i = 0; i < 7; i++)
    {
        string part;
        getline(iss, part, '|');
        if (i > 0)
            baseData += "|";
        baseData += part;
    }

    User::fromFileString(baseData);

    iss.ignore();
    iss >> loyaltyPoints;
}
