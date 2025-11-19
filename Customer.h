#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "User.h"

using namespace std; // <<< THÊM VÀO

class Customer : public User
{
private:
    double balance;
    int loyaltyPoints;

public:
    Customer();
    Customer(const string &id, const string &uname, const string &pwd,
             const string &name, const string &phone, const string &mail);

    double getBalance() const { return balance; }
    int getLoyaltyPoints() const { return loyaltyPoints; }

    void addBalance(double amount);
    void deductBalance(double amount);
    void addLoyaltyPoints(int points) { loyaltyPoints += points; }

    void displayInfo() const override;
    string toFileString() const override;
    void fromFileString(const string &line) override;
};

#endif
