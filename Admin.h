#ifndef ADMIN_H
#define ADMIN_H

#include "User.h"

using namespace std;

class Admin : public User
{
private:
    string employeeID;   // Mã nhân viên

public:
    Admin();
    Admin(const string &id, const string &uname, const string &pwd,
          const string &name, const string &phone, const string &mail,
          const string &empId);

    string getEmployeeID() const { return employeeID; }
    void setEmployeeID(const string &id) { employeeID = id; }

    void displayInfo() const override;
    string toFileString() const override;
    void fromFileString(const string &line) override;
};

#endif
