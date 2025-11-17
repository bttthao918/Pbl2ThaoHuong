#include "User.h"
#include <iostream>
#include <regex>
using namespace std;

// Constructor
User::User(string id, string user, string pass, string name, 
           string phone, string mail)
{
    userID = id;
    username = user;
    password = pass;
    fullName = name;
    phoneNumber = phone;
    email = mail;
    registeredDate = "N/A";
    isActive = true;
}

// Đăng nhập
bool User::login(string user, string pass) {
    return (username == user && password == pass);
}

// Đổi mật khẩu
void User::changePassword(string oldPass, string newPass) {
    if (password == oldPass) {
        password = newPass;
        cout << ">>> Đổi mật khẩu thành công!\n";
    } else {
        cout << ">>> Sai mật khẩu cũ!\n";
    }
}

// Kiểm tra số điện thoại hợp lệ (10 chữ số)
bool User::validatePhone(string phone) {
    if (phone.length() != 10) return false;
    for (char c : phone)
        if (!isdigit(c)) return false;
    return true;
}

// Kiểm tra email hợp lệ
bool User::validateEmail(string email) {
    // Regex email đơn giản
    regex pattern(R"((\w+)(\.?\w+)*@\w+\.\w+)");
    return regex_match(email, pattern);
}

// Cập nhật thông tin
void User::updateProfile(string name, string phone, string email) {
    if (!validatePhone(phone)) {
        cout << ">>> Số điện thoại không hợp lệ!\n";
        return;
    }
    if (!validateEmail(email)) {
        cout << ">>> Email không hợp lệ!\n";
        return;
    }

    fullName = name;
    phoneNumber = phone;
    this->email = email;

    cout << ">>> Cập nhật thông tin thành công!\n";
}
