#include "Admin.h"
#include "Customer.h"
#include <iomanip>
#include "RateManager.h"
using namespace std;

Admin::Admin(string id, string user, string pass, string name,
             string phone, string mail, RateManager &rm)
    : User(id, user, pass, name, phone, mail), rateManager(rm) {}

void Admin::display()
{
    cout << "Admin: " << fullName << " | SĐT: " << phoneNumber << endl;
}

string Admin::getUserType()
{
    return "Admin";
}

void Admin::showMenu(Admin<Customer> &customers)
{
    int choice;
    do
    {
        cout << "\n===== MENU ADMIN =====\n";
        cout << "1. Xem chỗ gửi xe\n";
        cout << "2. Thêm chỗ gửi xe\n";
        cout << "3. Xóa chỗ gửi xe\n";
        cout << "4. Sửa thông tin chỗ gửi xe\n";
        cout << "5. Xem giá vé\n";
        cout << "6. Cập nhật giá vé\n";
        cout << "7. Quản lý khách hàng\n";
        cout << "8. Đăng xuất\n";
        cout << "Chọn: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            viewSlots();
            break;
        case 2:
            addSlot();
            break;
        case 3:
            removeSlot();
            break;
        case 4:
            modifySlot();
            break;
        case 5:
            viewRates();
            break;
        case 6:
            updateRates();
            break;
        case 7:
            manageCustomers(customers);
            break;
        case 8:
            cout << "Đăng xuất...\n";
            break;
        default:
            cout << "Lựa chọn không hợp lệ!\n";
        }
    } while (choice != 8);
}

// ===== Quản lý chỗ gửi xe =====
void Admin::viewSlots() const
{
    cout << "\n===== DANH SÁCH CHỖ GỬI XE =====\n";
    if (slots.is_empty())
    {
        cout << ">>> Chưa có chỗ nào!\n";
        return;
    }
    slots.for_each([](const ParkingSlot &s)
                   { s.display(); });
}

void Admin::addSlot()
{
    int num, floor;
    string zone, type;
    cout << "Nhập số chỗ: ";
    cin >> num;
    cout << "Nhập khu vực (A/B/C): ";
    cin >> zone;
    cout << "Nhập tầng: ";
    cin >> floor;
    cout << "Nhập loại xe (Car/Motorbike/ElectricBike): ";
    cin >> type;
    ParkingSlot newSlot(num, zone, floor, type);
    slots.push_back(newSlot);
    cout << ">>> Thêm chỗ thành công!\n";
}

void Admin::removeSlot()
{
    int num;
    cout << "Nhập số chỗ cần xóa: ";
    cin >> num;
    bool removed = slots.remove_if([&](const ParkingSlot &s)
                                   { return s.getSlotNumber() == num; });
    cout << (removed ? ">>> Xóa chỗ thành công!\n" : ">>> Không tìm thấy chỗ!\n");
}

void Admin::modifySlot()
{
    int num;
    cout << "Nhập số chỗ cần sửa: ";
    cin >> num;
    ParkingSlot *slot = slots.find_if([&](const ParkingSlot &s)
                                      { return s.getSlotNumber() == num; });
    if (slot)
    {
        string zone, type;
        int floor;
        cout << "Nhập khu vực mới: ";
        cin >> zone;
        cout << "Nhập tầng mới: ";
        cin >> floor;
        cout << "Nhập loại xe mới: ";
        cin >> type;
        slot->setZone(zone);
        slot->setFloor(floor);
        slot->setVehicleType(type);
        cout << ">>> Sửa thông tin thành công!\n";
    }
    else
    {
        cout << ">>> Không tìm thấy chỗ!\n";
    }
}

// ===== Quản lý giá vé =====

void Admin::viewRates() const
{
    rateManager.displayRates();
}

void Admin::updateRates()
{
    string type;
    double newRate;
    cout << "Nhập loại xe (Car/Motorbike/ElectricBike): ";
    cin >> type;
    cout << "Nhập giá mới: ";
    cin >> newRate;
    rateManager.updateRate(type, newRate);
    cout << ">>> Cập nhật giá thành công!\n";
}

// ===== Quản lý khách hàng =====
void Admin::manageCustomers(Admin<Customer> &customers)
{
    int choice;
    do
    {
        cout << "\n===== QUẢN LÝ KHÁCH HÀNG =====\n";
        cout << "1. Xem danh sách khách hàng\n";
        cout << "2. Sửa thông tin khách hàng\n";
        cout << "3. Xóa khách hàng\n";
        cout << "4. Quay lại\n";
        cout << "Chọn: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            if (customers.is_empty())
            {
                cout << ">>> Không có khách hàng nào!\n";
            }
            else
            {
                customers.for_each([](const Customer &c)
                                   { const_cast<Customer &>(c).display(); });
            }
            break;
        }
        case 2:
        {
            string id;
            cout << "Nhập ID khách hàng cần sửa: ";
            cin >> id;
            Customer *cust = customers.find_if([&](const Customer &c)
                                               { return c.getUserID() == id; });
            if (cust)
            {
                string name, phone, email;
                cout << "Nhập tên mới: ";
                cin.ignore();
                getline(cin, name);
                cout << "Nhập SĐT mới: ";
                cin >> phone;
                cout << "Nhập email mới: ";
                cin >> email;
                cust->updateProfile(name, phone, email);
            }
            else
            {
                cout << ">>> Không tìm thấy khách hàng!\n";
            }
            break;
        }
        case 3:
        {
            string id;
            cout << "Nhập ID khách hàng cần xóa: ";
            cin >> id;
            bool removed = customers.remove_if([&](const Customer &c)
                                               { return c.getUserID() == id; });
            cout << (removed ? ">>> Xóa thành công!\n" : ">>> Không tìm thấy khách hàng!\n");
            break;
        }
        case 4:
        {
            cout << "Quay lại menu Admin...\n";
            break;
        }
        default:
        {
            cout << "Lựa chọn không hợp lệ!\n";
        }
        }
    } while (choice != 4);
}