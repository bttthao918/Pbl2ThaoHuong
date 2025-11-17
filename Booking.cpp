#include "Booking.h"
#include <iostream>
#include <fstream>
using namespace std;

// Constructor
Booking::Booking(string bID, string cID, string plate, string vType, int slot)
    : bookingID(bID), customerID(cID), licensePlate(plate), vehicleType(vType),
      slotNumber(slot), checkInTime(time(nullptr)), status("Pending"), paymentMethod("") {}

// Xác nhận thanh toán và tạo vé
void Booking::confirmPayment(const string& method, double fee) {
    paymentMethod = method;
    status = "Confirmed";

    ticket = ParkingTicket();
    ticket.setTicketID("T" + bookingID);
    ticket.setLicensePlate(licensePlate);
    ticket.setCustomerName(customerID);
    ticket.setVehicleType(vehicleType);
    ticket.setSlotNumber(slotNumber);
    ticket.setCheckInTime(checkInTime);
    ticket.setCheckOutTime(time(nullptr));
    ticket.setFee(fee);
    ticket.setCheckedOut(true);

    cout << ">>> Thanh toán thành công bằng " << method << "!\n";
}

// Hiển thị thông tin đặt chỗ
void Booking::display() const {
    cout << "BookingID: " << bookingID
         << " | Khách: " << customerID
         << " | Xe: " << vehicleType
         << " | Biển số: " << licensePlate
         << " | Chỗ: " << slotNumber
         << " | Trạng thái: " << status << endl;
}

// Hiển thị vé
void Booking::showTicket() const {
    if (status == "Confirmed") ticket.display();
    else cout << ">>> Chưa thanh toán, chưa có vé!\n";
}

// Lưu vào file
void Booking::saveToFile(const string& filename) const {
    ofstream outFile(filename, ios::app);
    if (!outFile) {
        cerr << "Không thể mở file để ghi!\n";
        return;
    }
    outFile << bookingID << "|" << customerID << "|" << licensePlate << "|"
            << vehicleType << "|" << slotNumber << "|" << status << "|" << paymentMethod << "\n";
    outFile.close();
}

// Đọc từ file
void Booking::loadFromFile(const string& filename) {
    ifstream inFile(filename);
    if (!inFile) {
        cerr << "Không thể mở file để đọc!\n";
        return;
    }
    string line;
    cout << "\n===== LỊCH SỬ ĐẶT CHỖ =====\n";
    while (getline(inFile, line)) {
        cout << line << endl;
    }
    inFile.close();
}