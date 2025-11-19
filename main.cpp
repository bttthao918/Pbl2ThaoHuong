#include <iostream>
#include <limits>
#include "ParkingManager.h"

using namespace std;

// Forward declarations
void showMainMenu();
void handleLogin(ParkingManager &manager);
void handleRegister(ParkingManager &manager);
void showCustomerMenu(ParkingManager &manager);
void showAdminMenu(ParkingManager &manager);

// Customer functions
void customerVehicleManagement(ParkingManager &manager);
void customerBookingManagement(ParkingManager &manager);
void customerTicketManagement(ParkingManager &manager);
void customerAccountManagement(ParkingManager &manager);

// Admin functions
void adminUserManagement(ParkingManager &manager);
void adminVehicleManagement(ParkingManager &manager);
void adminSlotManagement(ParkingManager &manager);
void adminBookingManagement(ParkingManager &manager);
void adminTicketManagement(ParkingManager &manager);
void adminStatistics(ParkingManager &manager);

int main()
{
    ParkingManager manager;

    // Initialize some sample parking slots if empty
    if (manager.getTotalSlots() == 0)
    {
        cout << "Khoi tao du lieu mau...\n";
        for (int i = 1; i <= 20; i++)
        {
            string slotNum = string("A") + (i < 10 ? "0" : "") + to_string(i);
            manager.addParkingSlot(slotNum, VehicleType::MOTORCYCLE);
        }
        for (int i = 1; i <= 15; i++)
        {
            string slotNum = string("B") + (i < 10 ? "0" : "") + to_string(i);
            manager.addParkingSlot(slotNum, VehicleType::CAR);
        }
        for (int i = 1; i <= 10; i++)
        {
            string slotNum = string("C") + (i < 10 ? "0" : "") + to_string(i);
            manager.addParkingSlot(slotNum, VehicleType::ELECTRIC_BIKE);
        }
        cout << "Da khoi tao 45 cho do xe!\n";
        Utils::pause();
    }

    while (true)
    {
        Utils::clearScreen();
        showMainMenu();

        int choice;
        cout << "Nhap lua chon: ";
        cin >> choice;
        cin.ignore();

        try
        {
            switch (choice)
            {
            case 1:
                handleLogin(manager);
                break;
            case 2:
                handleRegister(manager);
                break;
            case 0:
                cout << "Cam on da su dung! Tam biet!\n";
                return 0;
            default:
                cout << "Lua chon khong hop le!\n";
                Utils::pause();
            }
        }
        catch (const exception &e)
        {
            cout << "Loi: " << e.what() << endl;
            Utils::pause();
        }
    }

    return 0;
}

void showMainMenu()
{
    cout << "╔═══════════════════════════════════════╗\n";
    cout << "║   HE THONG QUAN LY NHA XE - PBL2     ║\n";
    cout << "╠═══════════════════════════════════════╣\n";
    cout << "║  1. Dang nhap                         ║\n";
    cout << "║  2. Dang ky tai khoan                 ║\n";
    cout << "║  0. Thoat                             ║\n";
    cout << "╚═══════════════════════════════════════╝\n";
}

void handleLogin(ParkingManager &manager)
{
    Utils::clearScreen();
    cout << "========== DANG NHAP ==========\n";

    string username, password;

    while (true) // vòng lặp nhập lại nếu sai
    {
        cout << "Ten dang nhap: ";
        getline(cin, username);
        cout << "Mat khau: ";
        getline(cin, password);

        if (manager.login(username, password))
        {
            cout << "\nDang nhap thanh cong!\n";

            auto user = manager.getCurrentUser();
            if (user->getRole() == UserRole::CUSTOMER)
            {
                showCustomerMenu(manager);
            }
            else
            {
                showAdminMenu(manager);
            }
            break; // thoát vòng lặp khi login thành công
        }
        else
        {
            cout << "\nSai ten dang nhap hoac mat khau! Vui long nhap lai.\n";
        }
    }
}

void handleRegister(ParkingManager &manager)
{
    Utils::clearScreen();
    cout << "========== DANG KY TAI KHOAN ==========\n";

    string username, password, fullName, phone, email;
    int roleChoice;

    // ===== NHẬP USERNAME =====
    cout << "Ten dang nhap: ";
    getline(cin, username);

    // ===== NHẬP PASSWORD =====
    cout << "Mat khau: ";
    getline(cin, password);

    // ===== NHẬP HỌ TÊN =====
    cout << "Ho ten: ";
    getline(cin, fullName);

    // ===== NHẬP SỐ ĐIỆN THOẠI =====
    while (true)
    {
        cout << "So dien thoai: ";
        getline(cin, phone);

        if (Utils::isValidPhoneNumber(phone))
            break;

        cout << "So dien thoai khong hop le! Vui long nhap lai.\n";
    }

    // ===== NHẬP EMAIL =====
    while (true)
    {
        cout << "Email: ";
        getline(cin, email);

        if (Utils::isValidEmail(email))
            break;

        cout << "Email khong hop le! Vui long nhap lai.\n";
    }

    // ===== CHỌN LOẠI TÀI KHOẢN =====
    while (true)
    {
        cout << "Loai tai khoan (1-Khach hang, 2-Quan ly): ";
        cin >> roleChoice;

        if (roleChoice == 1 || roleChoice == 2)
            break;

        cout << "Lua chon khong hop le! Nhap lai.\n";
    }
    cin.ignore(); // clear newline

    UserRole role = (roleChoice == 2) ? UserRole::ADMIN : UserRole::CUSTOMER;

    // ===== GỌI REGISTER =====
    try
    {
        if (manager.registerUser(username, password, fullName, phone, email, role))
        {
            cout << "\nDang ky thanh cong!\n";
        }
        else
        {
            cout << "\nDang ky that bai!\n";
        }
    }
    catch (const exception &e)
    {
        cout << "\nLoi: " << e.what() << endl;
    }

    Utils::pause();
}

void showCustomerMenu(ParkingManager &manager)
{
    while (manager.isLoggedIn())
    {
        Utils::clearScreen();
        auto user = manager.getCurrentUser();
        cout << "╔═══════════════════════════════════════╗\n";
        cout << "║        MENU KHACH HANG               ║\n";
        cout << "╠═══════════════════════════════════════╣\n";
        cout << "║  Xin chao: " << user->getFullName() << endl;
        cout << "╠═══════════════════════════════════════╣\n";
        cout << "║  1. Quan ly xe cua toi                ║\n";
        cout << "║  2. Dat cho truoc                     ║\n";
        cout << "║  3. Quan ly ve gui xe                 ║\n";
        cout << "║  4. Quan ly tai khoan                 ║\n";
        cout << "║  0. Dang xuat                         ║\n";
        cout << "╚═══════════════════════════════════════╝\n";

        int choice;
        cout << "Nhap lua chon: ";
        cin >> choice;
        cin.ignore();

        try
        {
            switch (choice)
            {
            case 1:
                customerVehicleManagement(manager);
                break;
            case 2:
                customerBookingManagement(manager);
                break;
            case 3:
                customerTicketManagement(manager);
                break;
            case 4:
                customerAccountManagement(manager);
                break;
            case 0:
                manager.logout();
                cout << "Da dang xuat!\n";
                return;
            default:
                cout << "Lua chon khong hop le!\n";
                Utils::pause();
            }
        }
        catch (const exception &e)
        {
            cout << "Loi: " << e.what() << endl;
            Utils::pause();
        }
    }
}

void customerVehicleManagement(ParkingManager &manager)
{
    while (true)
    {
        Utils::clearScreen();
        cout << "========== QUAN LY XE ==========\n";
        cout << "1. Dang ky xe moi\n";
        cout << "2. Xem danh sach xe\n";
        cout << "3. Cap nhat thong tin xe\n";
        cout << "4. Xoa xe\n";
        cout << "0. Quay lai\n";

        int choice;
        cout << "Nhap lua chon: ";
        cin >> choice;
        cin.ignore();

        if (choice == 0)
            break;

        try
        {
            switch (choice)
            {
            case 1:
            {
                string plate, brand, model, color;
                int typeChoice;

                cout << "\n--- DANG KY XE MOI ---\n";
                cout << "Bien so xe: ";
                getline(cin, plate);
                cout << "Loai xe (1-Xe may, 2-O to, 3-Xe dap dien): ";
                cin >> typeChoice;
                cin.ignore();
                cout << "Hang xe: ";
                getline(cin, brand);
                cout << "Model: ";
                getline(cin, model);
                cout << "Mau sac: ";
                getline(cin, color);

                VehicleType type;
                switch (typeChoice)
                {
                case 1:
                    type = VehicleType::MOTORCYCLE;
                    break;
                case 2:
                    type = VehicleType::CAR;
                    break;
                case 3:
                    type = VehicleType::ELECTRIC_BIKE;
                    break;
                default:
                    type = VehicleType::MOTORCYCLE;
                }

                if (manager.registerVehicle(plate, type, brand, model, color,
                                            manager.getCurrentUser()->getUserId()))
                {
                    cout << "\nDang ky xe thanh cong!\n";
                }
                break;
            }
            case 2:
            {
                auto vehicles = manager.getVehiclesByCustomer(
                    manager.getCurrentUser()->getUserId());

                cout << "\n--- DANH SACH XE CUA BAN ---\n";
                if (vehicles.empty())
                {
                    cout << "Chua co xe nao duoc dang ky.\n";
                }
                else
                {
                    int count = 1;
                    for (auto it = vehicles.begin(); it != vehicles.end(); ++it)
                    {
                        cout << "\n"
                             << count++ << ". ";
                        (*it)->displayInfo();
                        cout << "----------------------------\n";
                    }
                }
                break;
            }
            case 3:
            {
                string vehicleId, brand, model, color;
                cout << "Nhap Vehicle ID: ";
                getline(cin, vehicleId);
                cout << "Hang xe moi: ";
                getline(cin, brand);
                cout << "Model moi: ";
                getline(cin, model);
                cout << "Mau sac moi: ";
                getline(cin, color);

                if (manager.updateVehicle(vehicleId, brand, model, color))
                {
                    cout << "\nCap nhat thanh cong!\n";
                }
                break;
            }
            case 4:
            {
                string vehicleId;
                cout << "Nhap Vehicle ID can xoa: ";
                getline(cin, vehicleId);

                if (manager.deleteVehicle(vehicleId))
                {
                    cout << "\nXoa xe thanh cong!\n";
                }
                break;
            }
            default:
                cout << "Lua chon khong hop le!\n";
            }
        }
        catch (const exception &e)
        {
            cout << "Loi: " << e.what() << endl;
        }
        Utils::pause();
    }
}

void customerBookingManagement(ParkingManager &manager)
{
    while (true)
    {
        Utils::clearScreen();
        cout << "========== DAT CHO TRUOC ==========\n";
        cout << "1. Dat cho moi\n";
        cout << "2. Xem danh sach dat cho\n";
        cout << "3. Huy dat cho\n";
        cout << "0. Quay lai\n";

        int choice;
        cout << "Nhap lua chon: ";
        cin >> choice;
        cin.ignore();

        if (choice == 0)
            break;

        try
        {
            switch (choice)
            {
            case 1:
            {
                string vehicleId;
                int hours;

                cout << "\n--- DAT CHO MOI ---\n";
                cout << "Vehicle ID: ";
                getline(cin, vehicleId);
                cout << "Du kien den sau bao nhieu gio?: ";
                cin >> hours;
                cin.ignore();

                time_t expectedArrival = time(nullptr) + (hours * 3600);

                if (manager.createBooking(manager.getCurrentUser()->getUserId(),
                                          vehicleId, expectedArrival))
                {
                    cout << "\nDat cho thanh cong!\n";
                }
                break;
            }
            case 2:
            {
                auto bookings = manager.getBookingsByCustomer(
                    manager.getCurrentUser()->getUserId());

                cout << "\n--- DANH SACH DAT CHO ---\n";
                if (bookings.empty())
                {
                    cout << "Chua co booking nao.\n";
                }
                else
                {
                    int count = 1;
                    for (auto it = bookings.begin(); it != bookings.end(); ++it)
                    {
                        cout << "\n"
                             << count++ << ". ";
                        it->displayInfo();
                        cout << "----------------------------\n";
                    }
                }
                break;
            }
            case 3:
            {
                manager.cancelBooking();
                break;
            }
            default:
                cout << "Lua chon khong hop le!\n";
            }
        }
        catch (const exception &e)
        {
            cout << "Loi: " << e.what() << endl;
        }

        Utils::pause();
    }
}

void customerTicketManagement(ParkingManager &manager)
{
    while (true)
    {
        Utils::clearScreen();
        cout << "========== QUAN LY VE GUI XE ==========\n";
        cout << "1. Check-in (gui xe)\n";
        cout << "2. Check-out (lay xe)\n";
        cout << "3. Xem lich su gui xe\n";
        cout << "4. Xem bang gia ve\n";
        cout << "0. Quay lai\n";

        int choice;
        cout << "Nhap lua chon: ";
        cin >> choice;
        cin.ignore();

        if (choice == 0)
            break;

        try
        {
            switch (choice)
            {
            case 1:
            {
                string vehicleId, bookingId;

                cout << "\n--- CHECK-IN ---\n";
                cout << "Vehicle ID: ";
                getline(cin, vehicleId);
                cout << "Booking ID (bo trong neu khong dat truoc): ";
                getline(cin, bookingId);

                string ticketId = manager.checkIn(
                    manager.getCurrentUser()->getUserId(),
                    vehicleId, bookingId);

                cout << "\nCheck-in thanh cong!\n";
                cout << "Ticket ID: " << ticketId << "\n";
                break;
            }
            case 2:
            {
                string ticketId;
                cout << "Nhap Ticket ID: ";
                getline(cin, ticketId);

                if (manager.checkOut(ticketId))
                {
                    auto ticket = manager.getTicket(ticketId);
                    cout << "\nCheck-out thanh cong!\n";
                    cout << "Phi gui xe: " << ticket->getFee() << " VND\n";
                }
                break;
            }
            case 3:
            {
                auto tickets = manager.getTicketsByCustomer(
                    manager.getCurrentUser()->getUserId());

                cout << "\n--- LICH SU GUI XE ---\n";
                if (tickets.empty())
                {
                    cout << "Chua co lich su gui xe.\n";
                }
                else
                {
                    int count = 1;
                    for (auto it = tickets.begin(); it != tickets.end(); ++it)
                    {
                        cout << "\n"
                             << count++ << ". ";
                        it->displayInfo();
                        cout << "----------------------------\n";
                    }
                }
                break;
            }
            case 4:
            {
                cout << "\n--- BANG GIA VE GUI XE ---\n";
                manager.customerViewPricing();
                break;
            }
            default:
                cout << "Lua chon khong hop le!\n";
            }
        }
        catch (const exception &e)
        {
            cout << "Loi: " << e.what() << endl;
        }
        Utils::pause();
    }
}

void customerAccountManagement(ParkingManager &manager)
{
    Utils::clearScreen();
    auto user = manager.getCurrentUser();
    Customer *customer = dynamic_cast<Customer *>(user.get());

    cout << "========== QUAN LY TAI KHOAN ==========\n";
    customer->displayInfo();

    cout << "\n1. Nap tien vao tai khoan\n";
    cout << "2. Cap nhat thong tin\n";
    cout << "0. Quay lai\n";

    int choice;
    cout << "Nhap lua chon: ";
    cin >> choice;
    cin.ignore();

    try
    {
        switch (choice)
        {
        case 1:
        {
            double amount;
            cout << "Nhap so tien can nap: ";
            cin >> amount;
            cin.ignore();

            if (manager.addBalance(user->getUserId(), amount))
            {
                cout << "\nNap tien thanh cong!\n";
            }
            break;
        }
        case 2:
        {
            string fullName, phone, email;
            cout << "Ho ten moi: ";
            getline(cin, fullName);
            cout << "So dien thoai moi: ";
            getline(cin, phone);
            cout << "Email moi: ";
            getline(cin, email);

            if (manager.updateCustomerInfo(user->getUserId(), fullName, phone, email))
            {
                cout << "\nCap nhat thanh cong!\n";
            }
            break;
        }
        }
    }
    catch (const exception &e)
    {
        cout << "Loi: " << e.what() << endl;
    }
    Utils::pause();
}

void showAdminMenu(ParkingManager &manager)
{
    while (manager.isLoggedIn())
    {
        Utils::clearScreen();
        auto user = manager.getCurrentUser();
        cout << "╔═══════════════════════════════════════╗\n";
        cout << "║        MENU QUAN LY                  ║\n";
        cout << "╠═══════════════════════════════════════╣\n";
        cout << "║  Xin chao: " << user->getFullName() << endl;
        cout << "╠═══════════════════════════════════════╣\n";
        cout << "║  1. Quan ly nguoi dung                ║\n";
        cout << "║  2. Quan ly xe                        ║\n";
        cout << "║  3. Quan ly cho do                    ║\n";
        cout << "║  4. Quan ly booking                   ║\n";
        cout << "║  5. Quan ly ticket                    ║\n";
        cout << "║  6. Thong ke & Bao cao                ║\n";
        cout << "║  0. Dang xuat                         ║\n";
        cout << "╚═══════════════════════════════════════╝\n";

        int choice;
        cout << "Nhap lua chon: ";
        cin >> choice;
        cin.ignore();

        try
        {
            switch (choice)
            {
            case 1:
                adminUserManagement(manager);
                break;
            case 2:
                adminVehicleManagement(manager);
                break;
            case 3:
                adminSlotManagement(manager);
                break;
            case 4:
                adminBookingManagement(manager);
                break;
            case 5:
                adminTicketManagement(manager);
                break;
            case 6:
                adminStatistics(manager);
                break;
            case 0:
                manager.logout();
                cout << "Da dang xuat!\n";
                return;
            default:
                cout << "Lua chon khong hop le!\n";
                Utils::pause();
            }
        }
        catch (const exception &e)
        {
            cout << "Loi: " << e.what() << endl;
            Utils::pause();
        }
    }
}

void adminUserManagement(ParkingManager &manager)
{
    Utils::clearScreen();
    cout << "========== QUAN LY NGUOI DUNG ==========\n";
    cout << "1. Xem tat ca khach hang\n";
    cout << "2. Xem tat ca quan ly\n";
    cout << "3. Tim kiem nguoi dung\n";
    cout << "0. Quay lai\n";

    int choice;
    cout << "Nhap lua chon: ";
    cin >> choice;
    cin.ignore();

    try
    {
        switch (choice)
        {
        case 1:
        {
            auto customers = manager.getAllCustomers();
            cout << "\n--- TAT CA KHACH HANG ---\n";
            int count = 1;
            for (auto it = customers.begin(); it != customers.end(); ++it)
            {
                cout << "\n"
                     << count++ << ". ";
                (*it)->displayInfo();
                cout << "----------------------------\n";
            }
            break;
        }
        case 2:
        {
            auto admins = manager.getAllAdmins();
            cout << "\n--- TAT CA QUAN LY ---\n";
            int count = 1;
            for (auto it = admins.begin(); it != admins.end(); ++it)
            {
                cout << "\n"
                     << count++ << ". ";
                (*it)->displayInfo();
                cout << "----------------------------\n";
            }
            break;
        }
        case 3:
        {
            while (true)
            {
                Utils::clearScreen();
                string keyword;
                cout << "\n=== TIM KIEM NGUOI DUNG ===\n";
                cout << "Nhap tu khoa tim kiem (Nhan Enter de thoat): ";
                getline(cin, keyword);

                if (keyword.empty()) // Nếu người dùng nhấn Enter mà không nhập gì
                {
                    cout << "\nThoat tim kiem.\n";
                    break;
                }
                auto results = manager.searchUsers(keyword);
                cout << "\n--- KET QUA TIM KIEM ---\n";
                if (results.empty())
                {
                    cout << "Khong tim thay ket qua nao.\n";
                }
                else
                {
                    int count = 1;
                    for (auto it = results.begin(); it != results.end(); ++it)
                    {
                        cout << "\n"
                             << count++ << ". ";
                        (*it)->displayInfo();
                        cout << "----------------------------\n";
                    }
                }
                Utils::pause(); // Chờ người dùng trước khi quay lại vòng lặp
            }
            break;
        }
        }
    }
    catch (const exception &e)
    {
        cout << "Loi: " << e.what() << endl;
    }
    Utils::pause();
}

void adminVehicleManagement(ParkingManager &manager)
{
    Utils::clearScreen();
    cout << "========== QUAN LY XE ==========\n";
    cout << "1. Xem tat ca xe\n";
    cout << "2. Tim kiem xe\n";
    cout << "3. Sap xep theo bien so\n";
    cout << "0. Quay lai\n";

    int choice;
    cout << "Nhap lua chon: ";
    cin >> choice;
    cin.ignore();

    try
    {
        switch (choice)
        {
        case 1:
        {
            auto vehicles = manager.getAllVehicles();
            cout << "\n--- TAT CA XE ---\n";
            int count = 1;
            for (auto it = vehicles.begin(); it != vehicles.end(); ++it)
            {
                cout << "\n"
                     << count++ << ". ";
                (*it)->displayInfo();
                cout << "----------------------------\n";
            }
            break;
        }
        case 2:
        {
            string keyword;
            cout << "Nhap tu khoa tim kiem: ";
            getline(cin, keyword);

            auto results = manager.searchVehicles(keyword);
            cout << "\n--- KET QUA TIM KIEM ---\n";
            if (results.empty())
            {
                cout << "Khong tim thay ket qua nao.\n";
            }
            else
            {
                int count = 1;
                for (auto it = results.begin(); it != results.end(); ++it)
                {
                    cout << "\n"
                         << count++ << ". ";
                    (*it)->displayInfo();
                    cout << "----------------------------\n";
                }
            }
            break;
        }
        case 3:
        {
            manager.sortVehiclesByPlate();
            cout << "\nDa sap xep theo bien so!\n";
            break;
        }
        }
    }
    catch (const exception &e)
    {
        cout << "Loi: " << e.what() << endl;
    }
    Utils::pause();
}

void adminSlotManagement(ParkingManager &manager)
{
    Utils::clearScreen();
    cout << "========== QUAN LY CHO DO ==========\n";
    cout << "1. Them cho do moi\n";
    cout << "2. Xem tat ca cho do\n";
    cout << "3. Cap nhat trang thai cho do\n";
    cout << "4. Xem cho trong\n";
    cout << "0. Quay lai\n";

    int choice;
    cout << "Nhap lua chon: ";
    cin >> choice;
    cin.ignore();

    try
    {
        switch (choice)
        {
        case 1:
        {
            string slotNumber;
            int typeChoice;

            cout << "So vi tri: ";
            getline(cin, slotNumber);
            cout << "Loai xe (1-Xe may, 2-O to, 3-Xe dap dien): ";
            cin >> typeChoice;
            cin.ignore();

            VehicleType type;
            switch (typeChoice)
            {
            case 1:
                type = VehicleType::MOTORCYCLE;
                break;
            case 2:
                type = VehicleType::CAR;
                break;
            case 3:
                type = VehicleType::ELECTRIC_BIKE;
                break;
            default:
                type = VehicleType::MOTORCYCLE;
            }

            if (manager.addParkingSlot(slotNumber, type))
            {
                cout << "\nThem cho do thanh cong!\n";
            }
            break;
        }
        case 2:
        {
            auto slots = manager.getAllSlots();
            cout << "\n--- TAT CA CHO DO ---\n";
            int count = 1;
            for (auto it = slots.begin(); it != slots.end(); ++it)
            {
                cout << "\n"
                     << count++ << ". ";
                it->displayInfo();
                cout << "----------------------------\n";
            }
            break;
        }
        case 3:
        {
            string slotId;
            int statusChoice;

            cout << "Slot ID: ";
            getline(cin, slotId);
            cout << "Trang thai moi (0-AVAILABLE, 1-OCCUPIED, 2-RESERVED, 3-MAINTENANCE): ";
            cin >> statusChoice;
            cin.ignore();

            SlotStatus status = static_cast<SlotStatus>(statusChoice);
            if (manager.updateSlotStatus(slotId, status))
            {
                cout << "\nCap nhat thanh cong!\n";
            }
            break;
        }
        case 4:
        {
            cout << "\nCHO DO CON TRONG:\n";
            cout << "- Xe may: " << manager.getAvailableSlotCount(VehicleType::MOTORCYCLE) << " cho\n";
            cout << "- O to: " << manager.getAvailableSlotCount(VehicleType::CAR) << " cho\n";
            cout << "- Xe dap dien: " << manager.getAvailableSlotCount(VehicleType::ELECTRIC_BIKE) << " cho\n";
            break;
        }
        }
    }
    catch (const exception &e)
    {
        cout << "Loi: " << e.what() << endl;
    }
    Utils::pause();
}

void adminBookingManagement(ParkingManager &manager)
{
    Utils::clearScreen();
    cout << "========== QUAN LY BOOKING ==========\n";
    cout << "1. Xem tat ca booking\n";
    cout << "2. Xac nhan booking\n";
    cout << "3. Huy booking\n";
    cout << "0. Quay lai\n";

    int choice;
    cout << "Nhap lua chon: ";
    cin >> choice;
    cin.ignore();

    try
    {
        switch (choice)
        {
        case 1:
        {
            auto bookings = manager.getAllBookings();
            cout << "\n--- TAT CA BOOKING ---\n";
            int count = 1;
            for (auto it = bookings.begin(); it != bookings.end(); ++it)
            {
                cout << "\n"
                     << count++ << ". ";
                it->displayInfo();
                cout << "----------------------------\n";
            }
            break;
        }
        case 2:
        {
            string bookingId;
            cout << "Booking ID: ";
            getline(cin, bookingId);

            if (manager.confirmBooking(bookingId))
            {
                cout << "\nXac nhan booking thanh cong!\n";
            }
            break;
        }
        case 3:
        {
            manager.cancelBooking();
            break;
        }
        }
    }
    catch (const exception &e)
    {
        cout << "Loi: " << e.what() << endl;
    }
    Utils::pause();
}
// Hàm quản lý ticket cho Admin
void adminTicketManagement(ParkingManager &manager)
{
    Utils::clearScreen();
    cout << "========== QUAN LY VE XE  ==========\n";
    cout << "1. Xem tat ca ve xe\n";
    cout << "2. Xem ve xe dang hoat dong\n";
    cout << "3. Quan ly gia ve\n";
    cout << "4. Tim kiem ve xe\n";
    cout << "5. Sap xep theo thoi gian\n";
    cout << "0. Quay lai\n";

    int choice;
    cout << "Nhap lua chon: ";
    cin >> choice;
    cin.ignore();

    try
    {
        switch (choice)
        {
        case 1:
        {
            auto tickets = manager.getAllTickets();
            cout << "\n--- TAT CA TICKET ---\n";
            if (tickets.empty())
            {
                cout << "Chua co ticket nao.\n";
            }
            else
            {
                int count = 1;
                for (auto it = tickets.begin(); it != tickets.end(); ++it)
                {
                    cout << "\n"
                         << count++ << ". ";
                    it->displayInfo();
                    cout << "----------------------------\n";
                }
            }
            break;
        }
        case 2:
        {
            auto tickets = manager.getActiveTickets();
            cout << "\n--- TICKET DANG HOAT DONG ---\n";
            if (tickets.empty())
            {
                cout << "Khong co ticket nao dang hoat dong.\n";
            }
            else
            {
                int count = 1;
                for (auto it = tickets.begin(); it != tickets.end(); ++it)
                {
                    cout << "\n"
                         << count++ << ". ";
                    it->displayInfo();
                    cout << "----------------------------\n";
                }
            }
            break;
        }

        case 3:
        {
            manager.adminPriceManagement(manager);
            break;
        }
        case 4:
        {
            string keyword;
            cout << "Nhap tu khoa tim kiem: ";
            getline(cin, keyword);

            auto results = manager.searchTickets(keyword);
            cout << "\n--- KET QUA TIM KIEM ---\n";
            if (results.empty())
            {
                cout << "Khong tim thay ket qua nao.\n";
            }
            else
            {
                int count = 1;
                for (auto it = results.begin(); it != results.end(); ++it)
                {
                    cout << "\n"
                         << count++ << ". ";
                    it->displayInfo();
                    cout << "----------------------------\n";
                }
            }
            break;
        }
        case 5:
        {
            manager.sortTicketsByTime();
            cout << "\nDa sap xep ticket theo thoi gian (moi nhat truoc)!\n";
            break;
        }
        }
    }
    catch (const exception &e)
    {
        cout << "Loi: " << e.what() << endl;
    }
    Utils::pause();
}

// Hàm thống kê và báo cáo cho Admin
void adminStatistics(ParkingManager &manager)
{
    while (true)
    {
        Utils::clearScreen();
        cout << "========== THONG KE & BAO CAO ==========\n";
        cout << "1. Bao cao ngay\n";
        cout << "2. Bao cao thang\n";
        cout << "3. Thong ke tong quat\n";
        cout << "4. Doanh thu theo khoang thoi gian\n";
        cout << "0. Quay lai\n";

        int choice;
        cout << "Nhap lua chon: ";
        cin >> choice;
        cin.ignore();

        if (choice == 0)
            break;

        try
        {
            switch (choice)
            {
            case 1:
            {
                manager.generateDailyReport();
                break;
            }
            case 2:
            {
                manager.generateMonthlyReport();
                break;
            }
            case 3:
            {
                cout << "\n========== THONG KE TONG QUAT ==========\n";
                cout << "Tong so nguoi dung: " << manager.getAllCustomers().size()
                     << " khach hang, " << manager.getAllAdmins().size() << " quan ly\n";
                cout << "Tong so xe dang ky: " << manager.getAllVehicles().size() << "\n";
                cout << "Tong so cho do: " << manager.getTotalSlots() << "\n";
                cout << "   - Dang su dung: " << manager.getOccupiedSlotCount() << "\n";
                cout << "   - Con trong (Xe may): "
                     << manager.getAvailableSlotCount(VehicleType::MOTORCYCLE) << "\n";
                cout << "   - Con trong (O to): "
                     << manager.getAvailableSlotCount(VehicleType::CAR) << "\n";
                cout << "   - Con trong (Xe dap dien): "
                     << manager.getAvailableSlotCount(VehicleType::ELECTRIC_BIKE) << "\n";
                cout << "Tong so booking: " << manager.getAllBookings().size() << "\n";
                cout << "Tong so ticket: " << manager.getAllTickets().size() << "\n";
                cout << "   - Dang hoat dong: " << manager.getActiveTickets().size() << "\n";
                cout << "Tong doanh thu: " << fixed << setprecision(2)
                     << manager.getTotalRevenue() << " VND\n";
                cout << "========================================\n";
                break;
            }
            case 4:
            {
                int startHours, endHours;
                cout << "Thoi gian bat dau (so gio truoc tinh tu bay gio): ";
                cin >> startHours;
                cout << "Thoi gian ket thuc (so gio truoc tinh tu bay gio): ";
                cin >> endHours;
                cin.ignore();

                time_t now = time(nullptr);
                time_t startTime = now - (startHours * 3600);
                time_t endTime = now - (endHours * 3600);

                double revenue = manager.getRevenueByPeriod(startTime, endTime);
                cout << "\nDoanh thu trong khoang thoi gian: "
                     << fixed << setprecision(2) << revenue << " VND\n";
                break;
            }
            default:
                cout << "Lua chon khong hop le!\n";
            }
        }
        catch (const exception &e)
        {
            cout << "Loi: " << e.what() << endl;
        }
    }
}