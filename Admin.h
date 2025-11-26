#ifndef ADMIN_H
#define ADMIN_H

#include "User.h"

class Admin : public User
{
private:
    std::string employeeID;

public:
    Admin();
    Admin(const std::string &id, const std::string &uname, const std::string &pwd,
          const std::string &name, const std::string &phone, const std::string &mail,
          const std::string &empId);

    std::string getEmployeeID() const { return employeeID; }
    void setEmployeeID(const std::string &id) { employeeID = id; }
    void displayInfo() const override;
    void displayTableRow() const override;
    std::string toFileString() const override;
    void fromFileString(const std::string &line) override;
};

#endif
