#ifndef USER_H
#define USER_H

#include <string>
using namespace std;

class User
{
protected:
    string userID;
    string username;
    string password;
    string fullName;
    string phoneNumber;
    string email;

public:
    // Constructor
    User(string id, string user, string pass, string name,
         string phone, string mail);

    // Pure virtual methods
    virtual void display() = 0;
    virtual string getUserType() = 0;
    virtual void showMenu() = 0;
    virtual void saveToFile(const string &filename);
    virtual void loadFromFile(const string &filename);

    // Common methods
    bool login(string user, string pass);
    void changePassword(string oldPass, string newPass);
    bool validatePhone(string phone);
    bool validateEmail(string email);
    void updateProfile(string name, string phone, string email);

    // Getters
    string getUserID() const { return userID; }
    string getUsername() const { return username; }
    string getFullName() const { return fullName; }
    string getPhoneNumber() const { return phoneNumber; }
    string getEmail() const { return email; }

    virtual ~User() {}
};

#endif
