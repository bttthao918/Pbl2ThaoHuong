#include "User.h"
#include "Utils.h"
#include <sstream>
#include <iomanip>

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
    cout << "User ID: " << userId << endl;
    cout << "Username: " << username << endl;
    cout << "Ho ten: " << fullName << endl;
    cout << "Dien thoai: " << phoneNumber << endl;
    cout << "Email: " << email << endl;
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
