#include "User.h"
#include "Utils.h"
#include "UI.h"
#include <sstream>
#include <iomanip>

extern UI ui;

using namespace std;

User::User() : role(UserRole::CUSTOMER) {}

User::User(const string &id, const string &uname, const string &pwd,
           const string &name, const string &phone, const string &mail, UserRole r)
    : userId(id), username(uname), password(pwd), fullName(name),
      phoneNumber(phone), email(mail), role(r)
{
    if (!Utils::isValidPhoneNumber(phone))
        throw InvalidInputException("So dien thoai khong hop le");

    if (!Utils::isValidEmail(mail))
        throw InvalidInputException("Email khong hop le");
}

void User::setPhoneNumber(const string &phone)
{
    if (!Utils::isValidPhoneNumber(phone))
        throw InvalidInputException("So dien thoai khong hop le");
    phoneNumber = phone;
}

void User::setEmail(const string &mail)
{
    if (!Utils::isValidEmail(mail))
        throw InvalidInputException("Email khong hop le");
    email = mail;
}

void User::displayInfo() const
{
    ui.printHorizontalLine('+', '-', '+');
    ui.printRow("          | User ID:      ", userId);
    ui.printRow("          | Username:     ", username);
    ui.printRow("          | Ho ten:       ", fullName);
    ui.printRow("          | Dien thoai:   ", phoneNumber);
    ui.printRow("          | Email:        ", email);
}

void User::displayTableRow() const
{
    // int widths[] = {20, 25, 15, 25};
    string name = fullName;
    if (name.length() > 21)
        name = name.substr(0, 18) + "...";

    string mail = email;
    if (mail.length() > 24)
        mail = mail.substr(0, 21) + "...";

    cout << "          | " << setw(18) << left << userId // 19 - 2
         << " | " << setw(23) << left << name            // 21 - 2
         << " | " << setw(13) << left << phoneNumber     // 14 - 2
         << " | " << setw(23) << left << mail            // 24 - 2
         << " |" << endl;
}

string User::toFileString() const
{
    ostringstream oss;
    oss << userId << "|" << username << "|" << password << "|"
        << fullName << "|" << phoneNumber << "|" << email << "|"
        << (role == UserRole::CUSTOMER ? "CUSTOMER" : "ADMIN");
    return oss.str();
}

void User::fromFileString(const string &line)
{
    istringstream iss(line);
    string roleStr;

    getline(iss, userId, '|');
    getline(iss, username, '|');
    getline(iss, password, '|');
    getline(iss, fullName, '|');
    getline(iss, phoneNumber, '|');
    getline(iss, email, '|');
    getline(iss, roleStr);

    role = (roleStr == "CUSTOMER") ? UserRole::CUSTOMER : UserRole::ADMIN;
}

ostream &operator<<(ostream &os, const User &user)
{
    os << "ID: " << user.userId << ", Ten: " << user.fullName
       << ", SDT: " << user.phoneNumber;
    return os;
}

bool User::operator==(const User &other) const
{
    return userId == other.userId;
}
