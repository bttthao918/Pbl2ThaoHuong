#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "User.h"
#include <string>

class Customer : public User
{
private:
    int loyaltyPoints;

public:
    Customer();
    Customer(const std::string &id, const std::string &uname, const std::string &pwd,
             const std::string &name, const std::string &phone, const std::string &mail);

    int getLoyaltyPoints() const { return loyaltyPoints; }
    void addLoyaltyPoints(int points) { loyaltyPoints += points; }
    void setLoyaltyPoints(int points)
    {
        if (points >= 0)
        {
            loyaltyPoints = points;
        }
    }

    void displayInfo() const override;
    void displayTableRow() const override;
    std::string toFileString() const override;
    void fromFileString(const std::string &line) override;
};

#endif
