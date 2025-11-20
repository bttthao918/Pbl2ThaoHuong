#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "User.h"

using namespace std; // <<< THÊM VÀO

class Customer : public User
{
private:
    int loyaltyPoints;

public:
    Customer();
    Customer(const string &id, const string &uname, const string &pwd,
             const string &name, const string &phone, const string &mail);

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
    string toFileString() const override;
    void fromFileString(const string &line) override;
};

#endif
