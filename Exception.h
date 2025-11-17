#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>
using namespace std;

// 1. Ngoại lệ đăng nhập
class LoginFailedException : public exception {
private:
    string message;
public:
    LoginFailedException(string username) {
        message = "Lỗi: Đăng nhập thất bại cho user '" + username + "'!";
    }
    const char* what() const noexcept override {
        return message.c_str();
    }
};

// 2. Ngoại lệ quyền truy cập
class UnauthorizedAccessException : public exception {
private:
    string message;
public:
    UnauthorizedAccessException(string userType, string action) {
        message = "Lỗi: " + userType + " không có quyền thực hiện: " + action;
    }
    const char* what() const noexcept override {
        return message.c_str();
    }
};

// 3. Ngoại lệ biển số trùng
class DuplicateVehicleException : public exception {
private:
    string message;
public:
    DuplicateVehicleException(string vehicleID) {
        message = "Lỗi: Xe " + vehicleID + " đã có trong bãi!";
    }
    const char* what() const noexcept override {
        return message.c_str();
    }
};

// 4. Ngoại lệ số điện thoại không hợp lệ
class InvalidPhoneException : public exception {
private:
    string message;
public:
    InvalidPhoneException(string phone) {
        message = "Lỗi: SĐT '" + phone + "' không hợp lệ (phải 10 chữ số)!";
    }
    const char* what() const noexcept override {
        return message.c_str();
    }
};

// 5. Ngoại lệ vị trí đã có xe
class SlotOccupiedException : public exception {
private:
    string message;
public:
    SlotOccupiedException(string slotID) {
        message = "Lỗi: Vị trí " + slotID + " đã có xe!";
    }
    const char* what() const noexcept override {
        return message.c_str();
    }
};

// 6. Ngoại lệ khu vực đã đầy
class ParkingLotFullException : public exception {
private:
    string message;
public:
    ParkingLotFullException(string zone) {
        message = "Lỗi: Khu " + zone + " đã đầy!";
    }
    const char* what() const noexcept override {
        return message.c_str();
    }
};

// 7. Ngoại lệ không tìm thấy xe
class VehicleNotFoundException : public exception {
private:
    string message;
public:
    VehicleNotFoundException(string vehicleID) {
        message = "Lỗi: Không tìm thấy xe " + vehicleID + "!";
    }
    const char* what() const noexcept override {
        return message.c_str();
    }
};

// 8. Ngoại lệ thời gian không hợp lệ
class InvalidTimeException : public exception {
private:
    string message;
public:
    InvalidTimeException(string time) {
        message = "Lỗi: Thời gian '" + time + "' không hợp lệ!";
    }
    const char* what() const noexcept override {
        return message.c_str();
    }
};

// 9. Ngoại lệ xe không thuộc về khách hàng
class VehicleOwnershipException : public exception {
private:
    string message;
public:
    VehicleOwnershipException(string vehicleID, string customerName) {
        message = "Lỗi: Xe " + vehicleID + " không thuộc về " + customerName + "!";
    }
    const char* what() const noexcept override {
        return message.c_str();
    }
};

#endif
