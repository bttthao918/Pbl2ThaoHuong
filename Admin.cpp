#include "Admin.h"
#include "UI.h"
#include <sstream>
#include <iomanip>

extern UI ui;

using namespace std;

Admin::Admin() : User(), employeeID("EMP0000")
{
    role = UserRole::ADMIN;
}

Admin::Admin(const string &id, const string &uname, const string &pwd,
             const string &name, const string &phone, const string &mail,
             const string &empId)
    : User(id, uname, pwd, name, phone, mail, UserRole::ADMIN),
      employeeID(empId) {}

void Admin::displayInfo() const
{
    User::displayInfo();
    ui.printRow("          | Ma nhan vien: ", employeeID);
    ui.printHorizontalLine('+', '-', '+');
}

void Admin::displayTableRow() const
{
    User::displayTableRow();
}

string Admin::toFileString() const
{
    ostringstream oss;
    oss << User::toFileString() << "|" << employeeID;
    return oss.str();
}

void Admin::fromFileString(const string &line)
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

    getline(iss, employeeID);
}
