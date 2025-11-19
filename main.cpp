#include <iostream>
#include <string>
using namespace std;

// ====== MANAGERS ======
#include "CustomerManager.h"
#include "AdminManager.h"
#include "VehicleManager.h"
#include "BookingManager.h"
#include "TicketManager.h"
#include "ParkingSlotManager.h"

// ====== KHAI BÁO HÀM MENU ======
void homeMenu(CustomerManager& customerMgr,
              AdminManager& adminMgr,
              VehicleManager& vehicleMgr,
              BookingManager& bookingMgr,
              TicketManager& ticketMgr,
              ParkingSlotManager& slotMgr);

void customerMenu(Customer* customer,
                  VehicleManager& vehicleMgr,
                  BookingManager& bookingMgr,
                  TicketManager& ticketMgr,
                  ParkingSlotManager& slotMgr);

void adminMenu(Admin* admin,
               CustomerManager& customerMgr,
               VehicleManager& vehicleMgr,
               BookingManager& bookingMgr,
               TicketManager& ticketMgr,
               ParkingSlotManager& slotMgr);


// ================================================================
//                              MAIN
// ================================================================
int main() 
{
    CustomerManager customerMgr("customers.txt");
    AdminManager adminMgr("admins.txt");
    VehicleManager vehicleMgr("vehicles.txt");
    BookingManager bookingMgr("bookings.txt");
    TicketManager ticketMgr("tickets.txt");
    ParkingSlotManager slotMgr("slots.txt");

    // Load data
    customerMgr.loadFromFile();
    adminMgr.loadFromFile();
    vehicleMgr.loadFromFile();
    bookingMgr.loadFromFile();
    ticketMgr.loadFromFile();
    slotMgr.loadFromFile();

    // Mở menu home
    homeMenu(customerMgr, adminMgr, vehicleMgr, bookingMgr, ticketMgr, slotMgr);
    return 0;
}



// ================================================================
//                         HOME MENU
// ================================================================
void homeMenu(CustomerManager& customerMgr,
              AdminManager& adminMgr,
              VehicleManager& vehicleMgr,
              BookingManager& bookingMgr,
              TicketManager& ticketMgr,
              ParkingSlotManager& slotMgr)
{
    while (true) {
        cout << "\n========== HOME MENU ==========\n";
        cout << "1. Đăng ký khách hàng\n";
        cout << "2. Đăng nhập khách hàng\n";
        cout << "3. Đăng nhập admin\n";
        cout << "4. Thoát\n";
        cout << "================================\n";
        cout << "Chọn: ";

        int choice;
        cin >> choice;

        if (choice == 1) {
            string username, password, fullName, phone, email;

            cout << "Tên đăng nhập: "; cin >> username;
            cout << "Mật khẩu: "; cin >> password;
            cin.ignore();
            cout << "Họ tên: "; getline(cin, fullName);
            cout << "SĐT: "; cin >> phone;
            cout << "Email: "; cin >> email;

            customerMgr.registerCustomer(username, password, fullName, phone, email);
            customerMgr.saveToFile();

            cout << ">>> Đăng ký thành công!\n";
        }

        else if (choice == 2) {
            string username, password;
            cout << "Tên đăng nhập: "; cin >> username;
            cout << "Mật khẩu: "; cin >> password;

            Customer* c = customerMgr.login(username, password);
            if (!c) {
                cout << ">>> Sai tài khoản hoặc mật khẩu!\n";
            } else {
                cout << ">>> Đăng nhập thành công!\n";
                customerMenu(c, vehicleMgr, bookingMgr, ticketMgr, slotMgr);
            }
        }

        else if (choice == 3) {
            string username, password;
            cout << "Tên đăng nhập admin: "; cin >> username;
            cout << "Mật khẩu: "; cin >> password;

            Admin* a = adminMgr.login(username, password);
            if (!a) {
                cout << ">>> Đăng nhập admin thất bại!\n";
            } else {
                cout << ">>> Xin chào quản trị viên!\n";
                adminMenu(a, customerMgr, vehicleMgr, bookingMgr, ticketMgr, slotMgr);
            }
        }

        else if (choice == 4) {
            cout << ">>> Tạm biệt!\n";
            break;
        }

        else {
            cout << ">>> Không hợp lệ!\n";
        }
    }
}



// ================================================================
//                       CUSTOMER MENU
// ================================================================
void customerMenu(Customer* customer,
                  VehicleManager& vehicleMgr,
                  BookingManager& bookingMgr,
                  TicketManager& ticketMgr,
                  ParkingSlotManager& slotMgr)
{
    while (true) {
        cout << "\n========== CUSTOMER MENU ==========\n";
        cout << "1. Thông tin tài khoản\n";
        cout << "2. Đăng ký xe\n";
        cout << "3. Xem xe của tôi\n";
        cout << "4. Đặt chỗ gửi xe\n";
        cout << "5. Thanh toán & tạo vé\n";
        cout << "6. Lịch sử gửi xe\n";
        cout << "7. Xe đang gửi?\n";
        cout << "8. Đăng xuất\n";
        cout << "====================================\n";
        cout << "Chọn: ";

        int choice;
        cin >> choice;

        // 1. Thông tin tài khoản
        if (choice == 1) {
            customer->display();
        }

        // 2. Đăng ký xe
        else if (choice == 2) {
            string plate, type;
            cin.ignore();
            cout << "Nhập biển số xe: ";
            getline(cin, plate);
            cout << "Loại xe (Car/Motorcycle/ElectricBike): ";
            cin >> type;

            Vehicle* v = nullptr;

            if (type == "Car") {
                int seats; string plateType;
                cout << "Số chỗ: "; cin >> seats;
                cout << "Loại biển số: "; cin >> plateType;
                v = new Car(plate, customer->getFullName(),
                            customer->getPhoneNumber(), "0", "0", "0",
                            plateType, seats);
            }
            else if (type == "Motorcycle") {
                int cc; int helmet;
                cout << "Phân khối: "; cin >> cc;
                cout << "Có BHTN? (1/0): "; cin >> helmet;
                v = new Motorcycle(plate, customer->getFullName(),
                                   customer->getPhoneNumber(), "0", "0", "0",
                                   cc, helmet == 1);
            }
            else {
                int battery; string brand;
                cout << "Dung lượng pin: "; cin >> battery;
                cout << "Hãng xe: "; cin >> brand;
                v = new ElectricBike(plate, customer->getFullName(),
                                      customer->getPhoneNumber(), "0", "0", "0",
                                      battery, brand);
            }

            v->setOwnerID(customer->getUserID());
            vehicleMgr.addVehicle(v);
            vehicleMgr.saveToFile();

            cout << ">>> Đăng ký xe thành công!\n";
        }

        // 3. Xem xe của tôi
        else if (choice == 3) {
            auto myCars = vehicleMgr.getVehiclesOfCustomer(customer->getUserID());
            if (myCars.is_empty()) cout << ">>> Bạn chưa có xe nào!\n";
            else myCars.for_each([](Vehicle* v){ v->display(); });
        }

        // 4. Đặt chỗ
        else if (choice == 4) {
            string plate;
            cout << "Biển số xe: ";
            cin >> plate;

            Vehicle* v = vehicleMgr.findVehicle(plate);
            if (!v) {
                cout << ">>> Không có xe này trong hệ thống!\n";
                continue;
            }

            auto freeSlots = slotMgr.getAvailableSlots(v->getVehicleType());
            if (freeSlots.is_empty()) {
                cout << ">>> Không có slot phù hợp!\n";
                continue;
            }

            cout << "\n===== Slot trống phù hợp =====\n";
            freeSlots.for_each([](ParkingSlot s){ s.display(); });

            int slotNum;
            cout << "Chọn slot: ";
            cin >> slotNum;

            slotMgr.reserveSlot(slotNum, plate);
            slotMgr.saveToFile();

            Booking* b = bookingMgr.createBooking(customer->getUserID(),
                                                  plate,
                                                  v->getVehicleType(),
                                                  slotNum);
            cout << ">>> Đặt chỗ thành công! Mã booking: "
                 << b->getBookingID() << "\n";
        }

        // 5. Thanh toán & tạo vé
        else if (choice == 5) {
            string id;
            cout << "Nhập mã booking: ";
            cin >> id;

            Booking* b = bookingMgr.findBooking(id);
            if (!b) {
                cout << ">>> Booking không tồn tại!\n";
                continue;
            }

            string pm;
            cout << "Thanh toán (Cash/QR): ";
            cin >> pm;

            double fee;
            cout << "Nhập phí gửi (theo giờ thực tế): ";
            cin >> fee;

            bookingMgr.confirmBooking(id, pm, fee);
            bookingMgr.saveToFile();

            cout << ">>> Vé gửi xe đã tạo!\n";
        }

        // 6. Lịch sử gửi xe
        else if (choice == 6) {
            auto list = ticketMgr.getTicketsOfCustomer(customer->getPhoneNumber());
            list.for_each([](ParkingTicket t){ t.display(); });
        }

        // 7. Xe đang gửi?
        else if (choice == 7) {
            auto list = ticketMgr.getTicketsOfCustomer(customer->getPhoneNumber());
            bool found = false;

            list.for_each([&](ParkingTicket t){
                if (!t.isCheckedOut()) {
                    found = true;
                    cout << "Xe đang gửi tại slot: " << t.getSlotNumber() << "\n";
                }
            });

            if (!found) cout << ">>> Không có xe nào đang gửi.\n";
        }

        // 8. Đăng xuất
        else if (choice == 8) {
            cout << ">>> Đăng xuất.\n";
            break;
        }

        else cout << ">>> Không hợp lệ!\n";
    }
}



// ================================================================
//                           ADMIN MENU
// ================================================================
void adminMenu(Admin* admin,
               CustomerManager& customerMgr,
               VehicleManager& vehicleMgr,
               BookingManager& bookingMgr,
               TicketManager& ticketMgr,
               ParkingSlotManager& slotMgr)
{
    while (true) {
        cout << "\n========== ADMIN MENU ==========\n";
        cout << "1. Xem danh sách khách hàng\n";
        cout << "2. Xem danh sách xe\n";
        cout << "3. Xem danh sách slot\n";
        cout << "4. Xem toàn bộ booking\n";
        cout << "5. Xem toàn bộ ticket\n";
        cout << "6. Thống kê\n";
        cout << "7. Đăng xuất\n";
        cout << "=================================\n";
        cout << "Chọn: ";

        int c;
        cin >> c;

        if (c == 1) customerMgr.displayAll();
        else if (c == 2) vehicleMgr.displayAll();
        else if (c == 3) slotMgr.displayAllSlots();
        else if (c == 4) bookingMgr.displayAll();
        else if (c == 5) ticketMgr.displayAll();
        else if (c == 6) {
            cout << "Tổng doanh thu: " << ticketMgr.calculateTotalRevenue() << " VND\n";
            cout << "Xe đang gửi: " << ticketMgr.countActiveVehicles() << "\n";
        }
        else if (c == 7) {
            cout << ">>> Đăng xuất admin\n";
            break;
        }
        else cout << ">>> Không hợp lệ!\n";
    }
}
