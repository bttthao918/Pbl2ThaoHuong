#include "Customer.h"
#include "Exceptions.h"
#include <sstream>
#include <iomanip>

using namespace std;

Customer::Customer() : User(), balance(0.0), loyaltyPoints(0)
{
    role = UserRole::CUSTOMER;
}

Customer::Customer(const string &id, const string &uname, const string &pwd,
                   const string &name, const string &phone, const string &mail)
    : User(id, uname, pwd, name, phone, mail, UserRole::CUSTOMER),
      balance(0.0), loyaltyPoints(0) {}

void Customer::addBalance(double amount)
{
    if (amount <= 0)
        throw InvalidInputException("So tien nap phai lon hon 0");
    balance += amount;
}

void Customer::deductBalance(double amount)
{
    if (amount <= 0)
        throw InvalidInputException("So tien khong hop le");

    if (balance < amount)
        throw InvalidInputException("So du khong du");

    balance -= amount;
}

void Customer::displayInfo() const
{
    User::displayInfo();
    cout << "So du: " << fixed << setprecision(2) << balance << " VND\n";
    cout << "Diem tich luy: " << loyaltyPoints << endl;
}

string Customer::toFileString() const
{
    ostringstream oss;
    oss << User::toFileString() << "|" << balance << "|" << loyaltyPoints;
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

    iss >> balance;
    iss.ignore();
    iss >> loyaltyPoints;
}
