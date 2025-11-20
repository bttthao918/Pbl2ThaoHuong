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
void viewCustomerHistory(ParkingManager &manager);

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
        cout << "Ten dang nhap (nhap 0 de quay lai): ";
        getline(cin, username);
        if (username == "0")
        {
            cout << "Huy dang nhap. Tro ve menu chinh.\n";
            return; // thoát hàm, quay lại menu chính
        }
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

    // Chọn loại tài khoản trước
    while (true)
    {
        cout << "Loai tai khoan (1-Khach hang, 2-Quan ly, 0-Huy): ";
        if (!(cin >> roleChoice))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Lua chon khong hop le. Thu lai.\n";
            continue;
        }
        cin.ignore();
        if (roleChoice == 0)
            return; // hủy đăng ký
        if (roleChoice == 1 || roleChoice == 2)
            break;
        cout << "Lua chon khong hop le! Nhap lai.\n";
    }

    // Nếu chọn Quản lý, yêu cầu mã nhân viên
    if (roleChoice == 2)
    {
        // Danh sách mã nhân viên hợp lệ (có thể chuyển ra file hoặc ParkingManager sau)
        const DoubleLinkedList<string> validCodes = []()
        {
            DoubleLinkedList<string> d;
            d.pushBack("EMP-001");
            d.pushBack("EMP-002");
            d.pushBack("EMP-ADMIN"); // ví dụ
            return d;
        }();

        string empCode;
        bool okCode = false;
        int attempts = 0;
        while (attempts < 3)
        {
            cout << "Nhap ma nhan vien (nhap 0 de huy): ";
            getline(cin, empCode);
            if (empCode == "0")
                return; // hủy
            // Kiểm tra trong DoubleLinkedList
            for (auto it = validCodes.begin(); it != validCodes.end(); ++it)
            {
                if (*it == empCode)
                {
                    okCode = true;
                    break;
                }
            }
            if (okCode)
                break;
            cout << "Ma nhan vien khong hop le. Thu lai.\n";
            ++attempts;
        }
        if (!okCode)
        {
            cout << "Nhap ma nhan vien that bai. Huy dang ky.\n";
            return;
        }
    }

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
        cout << "║      CAC CHUC NANG CUA KHACH HANG     ║\n";
        cout << "╠═══════════════════════════════════════╣\n";
        cout << "║  Xin chao: " << user->getFullName() << endl;
        cout << "╠═══════════════════════════════════════╣\n";
        cout << "║  1. Quan ly xe cua toi                ║\n";
        cout << "║  2. Dat cho truoc                     ║\n";
        cout << "║  3. Xem bang gia gui xe               ║\n";
        cout << "║  4. Xem lich su gui xe                ║\n";
        cout << "║  5. Quan ly tai khoan                 ║\n";
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
                manager.customerViewPricing();
                break;
            case 4:
                viewCustomerHistory(manager);
                break;
            case 5:
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
        cout << "========== QUAN LY XE CUA TOI ==========\n";
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
                string plate;
                while (true)
                {
                    cout << "\nBien so xe: ";
                    getline(cin, plate);

                    // Kiểm tra định dạng bằng regex
                    if (!Utils::isValidLicensePlate(plate))
                    {
                        cout << "Bien so khong hop le (dinh dang: 12A-3456 hoac 12A34567). Vui long nhap lai!\n";
                        continue;
                    }

                    // Kiểm tra trùng trong danh sách

                    auto existing = manager.getVehicleByPlate(plate);
                    if (existing != nullptr)
                    {
                        cout << "Bien so da ton tai. Vui long nhap lai!\n";
                        continue;
                    }

                    // Nếu hợp lệ và không trùng thì thoát vòng lặp
                    break;
                }

                // Sau khi biển số hợp lệ, nhập các thông tin khác
                int typeChoice;
                string brand, model, color;

                cout << "Loai xe (1-Xe may, 2-O to, 3-Xe dap dien): ";
                cin >> typeChoice;
                cin.ignore();
                cout << "Hang xe: ";
                getline(cin, brand);
                cout << "Model: ";
                getline(cin, model);
                cout << "Mau sac: ";
                getline(cin, color);

                // Đăng ký xe
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
                string vehicleId;
                shared_ptr<Vehicle> existingVehicle;

                while (true)
                {
                    cout << "Nhap Vehicle ID: ";
                    getline(cin, vehicleId);

                    existingVehicle = manager.getVehicle(vehicleId);
                    if (existingVehicle == nullptr)
                    {
                        cout << "Vehicle ID khong ton tai. Vui long nhap lai!\n";
                        continue;
                    }

                    break; // ID hợp lệ thì thoát vòng lặp
                }

                // Sau khi ID hợp lệ, nhập thông tin mới
                string brand, model, color;
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

                cout << "\n--- XOA XE ---\n";

                // Hiển thị danh sách xe của khách hàng
                auto vehicles = manager.getVehiclesByCustomer(
                    manager.getCurrentUser()->getUserId());

                if (vehicles.empty())
                {
                    cout << "Ban chua co xe nao de xoa.\n";
                    break;
                }

                cout << "Danh sach xe cua ban:\n\n";
                int count = 1;
                for (auto it = vehicles.begin(); it != vehicles.end(); ++it)
                {
                    cout << count++ << ". ";
                    cout << "Bien so: " << (*it)->getLicensePlate()
                         << " | " << (*it)->getTypeString()
                         << " | " << (*it)->getBrand() << " " << (*it)->getModel()
                         << " | ID: " << (*it)->getVehicleId() << "\n";
                }

                cout << "\nNhap Vehicle ID can xoa (hoac '0' de huy): ";
                getline(cin, vehicleId);

                if (vehicleId == "0")
                {
                    cout << "Da huy thao tac xoa.\n";
                    break;
                }

                // Tìm xe
                auto vehicle = manager.getVehicle(vehicleId);
                if (!vehicle)
                {
                    cout << "\n✗ Khong tim thay xe voi ID: " << vehicleId << "\n";
                    break;
                }

                // Kiểm tra quyền sở hữu
                if (vehicle->getCustomerId() != manager.getCurrentUser()->getUserId())
                {
                    cout << "\n✗ Xe nay khong thuoc ve ban!\n";
                    break;
                }

                // Hiển thị thông tin xe cần xóa
                cout << "\n--- THONG TIN XE CAN XOA ---\n";
                cout << "Vehicle ID:  " << vehicle->getVehicleId() << "\n";
                cout << "Bien so:     " << vehicle->getLicensePlate() << "\n";
                cout << "Loai:        " << vehicle->getTypeString() << "\n";
                cout << "Hang:        " << vehicle->getBrand() << " " << vehicle->getModel() << "\n";
                cout << "Mau:         " << vehicle->getColor() << "\n";
                cout << "----------------------------\n";
                cout << "\nBan co CHAC CHAN muon xoa xe nay khong?\n";
                cout << "Nhap 'YES' (viet hoa) de xac nhan, hoac bat ky gi khac de huy: ";

                string confirm;
                getline(cin, confirm);

                if (confirm != "YES")
                {
                    cout << "\n✓ Da huy thao tac xoa.\n";
                    break;
                }
                try
                {
                    if (manager.deleteVehicle(vehicleId))
                    {
                        cout << "\n✓✓✓ XOA XE THANH CONG! ✓✓✓\n";
                        cout << "Xe " << vehicle->getLicensePlate()
                             << " da duoc xoa khoi he thong.\n";
                    }
                    else
                    {
                        cout << "\n✗ Xoa xe that bai!\n";
                    }
                }
                catch (const exception &e)
                {
                    cout << "\n✗ Loi: " << e.what() << endl;
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
                cout << "\n--- DAT CHO MOI ---\n";
                string vehicleId;
                shared_ptr<Vehicle> vehicle;
                int hours = 0;

                while (true)
                {
                    cout << "Vehicle ID (nhap 0 de huy): ";
                    getline(cin, vehicleId);
                    if (vehicleId == "0")
                    {
                        cout << "Huy dat cho.\n";
                        break;
                    }
                    vehicle = manager.getVehicle(vehicleId);
                    if (vehicle)
                        break;
                    cout << "Vehicle ID khong ton tai. Thu lai hoac nhap 0 de huy.\n";
                }

                if (!vehicle)
                    break;

                cout << "Du kien den sau bao nhieu gio? ";
                if (!(cin >> hours))
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Nhap khong hop le\n";
                    break;
                }
                cin.ignore();

                time_t expectedArrival = time(nullptr) + static_cast<time_t>(hours) * 3600;

                // Lấy danh sách chỗ trống theo loại xe
                auto freeSlots = manager.getAvailableSlotsByType(vehicle->getType());

                // Hiển thị và thu id vào vector để chọn
                DoubleLinkedList<string> slotIds;
                int idxPrint = 0;
                freeSlots.forEach([&](const ParkingSlot &s)
                                  {
                    cout << ++idxPrint << ". " << s.getSlotNumber()
                         << " (ID: " << s.getSlotId() << ")\n";
                    slotIds.pushBack(s.getSlotId()); });

                if (slotIds.empty())
                {
                    cout << "Hien tai khong con cho trong cho loai xe nay.\n";
                    break;
                }

                cout << "Nhap so thu tu de chon cho (0 = de chu quan, auto assign): ";
                int choice;
                if (!(cin >> choice))
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Nhap khong hop le\n";
                    break;
                }
                cin.ignore();

                string chosenSlotId;
                if (choice > 0)
                {
                    int i = 1;
                    for (auto it = slotIds.begin(); it != slotIds.end(); ++it)
                    {
                        if (i == choice)
                        {
                            chosenSlotId = *it;
                            break;
                        }
                        ++i;
                    }
                }
                bool ok = manager.createBooking(
                    manager.getCurrentUser()->getUserId(), // giả sử có getter getCurrentUser()
                    vehicleId,
                    expectedArrival,
                    chosenSlotId);

                if (ok)
                    cout << "Dat cho thanh cong.\n";
                else
                    cout << "Dat cho that bai.\n";
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

void viewCustomerHistory(ParkingManager &manager)
{
    auto tickets = manager.getTicketsByCustomer(manager.getCurrentUser()->getUserId());

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
}

void customerAccountManagement(ParkingManager &manager)
{
    Utils::clearScreen();
    auto user = manager.getCurrentUser();
    Customer *customer = dynamic_cast<Customer *>(user.get());

    cout << "========== QUAN LY TAI KHOAN ==========\n";
    cout << "\n1. Xem thong tin tai khoan\n";
    cout << "2. Cap nhat thong tin\n";
    cout << "3. Doi mat khau\n";
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
            cout << "========== THONG TIN TAI KHOAN ==========\n\n";
            customer->displayInfo();
            cout << "Diem tich luy:  " << customer->getLoyaltyPoints() << " diem\n";
            cout << "\n=========================================\n";
            break;
        }
        case 2:
        {
            // CẬP NHẬT THÔNG TIN
            string fullName, phone, email;
            cout << "\n--- CAP NHAT THONG TIN ---\n";
            cout << "Ho ten hien tai: " << user->getFullName() << "\n";
            cout << "Ho ten moi (Enter de giu nguyen): ";
            getline(cin, fullName);
            if (fullName.empty())
                fullName = user->getFullName();

            cout << "So dien thoai hien tai: " << user->getPhoneNumber() << "\n";
            cout << "So dien thoai moi (Enter de giu nguyen): ";
            getline(cin, phone);
            if (phone.empty())
                phone = user->getPhoneNumber();

            cout << "Email hien tai: " << user->getEmail() << "\n";
            cout << "Email moi (Enter de giu nguyen): ";
            getline(cin, email);
            if (email.empty())
                email = user->getEmail();

            if (manager.updateCustomerInfo(user->getUserId(), fullName, phone, email))
            {
                cout << "\n✓ Cap nhat thanh cong!\n";
            }
            break;
        }
        case 3:
        {
            // ĐỔI MẬT KHẨU
            string oldPassword, newPassword, confirmPassword;

            cout << "\n--- DOI MAT KHAU ---\n";
            cout << "Mat khau hien tai: ";
            getline(cin, oldPassword);

            if (oldPassword != user->getPassword())
            {
                cout << "\n✗ Mat khau hien tai khong dung!\n";
                break;
            }

            cout << "Mat khau moi: ";
            getline(cin, newPassword);

            if (newPassword.length() < 6)
            {
                cout << "\n✗ Mat khau phai co it nhat 6 ky tu!\n";
                break;
            }

            cout << "Xac nhan mat khau moi: ";
            getline(cin, confirmPassword);

            if (newPassword != confirmPassword)
            {
                cout << "\n✗ Mat khau xac nhan khong khop!\n";
                break;
            }

            user->setPassword(newPassword);
            manager.saveAllData();
            cout << "\n✓ Doi mat khau thanh cong!\n";
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

void showAdminMenu(ParkingManager &manager)
{
    while (manager.isLoggedIn())
    {
        Utils::clearScreen();
        auto user = manager.getCurrentUser();
        cout << "╔═══════════════════════════════════════╗\n";
        cout << "║        CAC CHUC NANG CUA QUAN LY      ║\n";
        cout << "╠═══════════════════════════════════════╣\n";
        cout << "║  Xin chao: " << user->getFullName() << endl;
        cout << "╠═══════════════════════════════════════╣\n";
        cout << "║  1. Quan ly khach hang                ║\n";
        cout << "║  2. Quan ly xe                        ║\n";
        cout << "║  3. Quan ly cho do                    ║\n";
        cout << "║  4. Quan ly don dat cho               ║\n";
        cout << "║  5. Quan ly ve xe                     ║\n";
        cout << "║  6. Thong ke va bao cao               ║\n";
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
    cout << "========== QUAN LY KHACH HANG ==========\n";
    cout << "1. Xem tat ca khach hang\n";
    cout << "2. Cap nhat thong tin khach hang\n";
    cout << "3. Them khach hang moi\n";
    cout << "4. Xoa khach hang\n";
    cout << "5. Tim kiem khach hang\n";
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
            string customerId, fullName, phone, email;
            int loyaltyPoints;

            cout << "\n--- CAP NHAT KHACH HANG ---\n";
            cout << "Nhap Customer ID: ";
            getline(cin, customerId);

            // Hiển thị thông tin hiện tại
            Customer *customer = manager.getCustomer(customerId);
            if (!customer)
            {
                cout << "Khong tim thay khach hang voi ID: " << customerId << "\n";
                break;
            }

            cout << "\n--- THONG TIN HIEN TAI ---\n";
            customer->displayInfo();

            cout << "\n--- NHAP THONG TIN MOI ---\n";
            cout << "Ho ten moi (Enter de giu nguyen): ";
            getline(cin, fullName);
            if (fullName.empty())
                fullName = customer->getFullName();

            cout << "So dien thoai moi (Enter de giu nguyen): ";
            getline(cin, phone);
            if (phone.empty())
                phone = customer->getPhoneNumber();

            cout << "Email moi (Enter de giu nguyen): ";
            getline(cin, email);
            if (email.empty())
                email = customer->getEmail();

            cout << "Diem tich luy moi (hien tai: " << customer->getLoyaltyPoints() << "): ";
            string pointsStr;
            getline(cin, pointsStr);
            loyaltyPoints = pointsStr.empty() ? customer->getLoyaltyPoints() : stoi(pointsStr);

            if (manager.updateCustomerDetails(customerId, fullName, phone,
                                              email, loyaltyPoints))
            {
                cout << "\n✓ Cap nhat thanh cong!\n";
            }
            break;
        }
        case 3:
        {
            string username, password, fullName, phone, email;
            double initialBalance;

            cout << "\n========== THEM KHACH HANG MOI ==========\n";
            cout << "Username: ";
            getline(cin, username);
            cout << "Password: ";
            getline(cin, password);
            cout << "Ho ten: ";
            getline(cin, fullName);
            cout << "So dien thoai: ";
            getline(cin, phone);
            cout << "Email: ";
            getline(cin, email);
            cout << "So du ban dau (VND): ";
            cin >> initialBalance;
            cin.ignore();

            if (manager.addCustomer(username, password, fullName,
                                    phone, email))
            {
                cout << "\n✓ Them khach hang thanh cong!\n";
            }
            break;
        }

        case 4:
        {
            // XÓA KHÁCH HÀNG
            string customerId;

            cout << "\n========== XOA KHACH HANG ==========\n";
            cout << "Nhap Customer ID can xoa: ";
            getline(cin, customerId);

            // Hiển thị thông tin khách hàng
            Customer *customer = manager.getCustomer(customerId);
            if (!customer)
            {
                cout << "Khong tim thay khach hang voi ID: " << customerId << "\n";
                break;
            }

            cout << "\n--- THONG TIN KHACH HANG ---\n";
            customer->displayInfo();

            cout << "\n⚠ CANH BAO: Hanh dong nay khong the hoan tac!\n";
            cout << "Ban co chac chan muon xoa khach hang nay? (yes/no): ";
            string confirm;
            getline(cin, confirm);

            if (confirm == "yes" || confirm == "YES")
            {
                if (manager.deleteCustomer(customerId))
                {
                    cout << "\n✓ Xoa khach hang thanh cong!\n";
                }
            }
            else
            {
                cout << "\n✗ Da huy thao tac xoa.\n";
            }
            break;
        }
        case 5:
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
    cout << "========== QUAN LY BAI DO XE ==========\n";
    cout << "1. Xem tat ca cho do\n";
    cout << "2. Cap nhat trang thai cho do\n";
    cout << "3. Xem cho trong\n";
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
        case 2:
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
        case 3:
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
    cout << "========== QUAN LY DON DAT CHO ==========\n";
    cout << "1. Xem tat ca don dat cho\n";
    cout << "2. Xac nhan don dat cho\n";
    cout << "3. Huy don dat cho\n";
    cout << "4. Check-in don dat cho\n";
    cout << "5. Check-out don dat cho\n";
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
                cout << "\nXac nhan dat cho thanh cong!\n";
            }
            break;
        }
        case 3:
        {
            manager.cancelBooking();
            break;
        }
        case 4:
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
        case 5:
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
    cout << "1. Xem ta ca ve xe\n";
    cout << "2. Quan ly gia ve\n";
    cout << "3. Tim kiem ve xe\n";
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
            cout << "\n--- TAT CA VE XE ---\n";
            int count = 1;
            for (auto it = tickets.begin(); it != tickets.end(); ++it)
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
            manager.adminPriceManagement(manager);
            break;
        }
        case 3:
        {
            manager.advancedTicketSearch(manager);
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
        cout << "1. Bao cao hoat dong\n";
        cout << "2. Thong ke tong quat\n";
        cout << "3. Doanh thu theo khoang thoi gian\n";
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
                int subChoice;
                cout << "\n=== BAO CAO HOAT DONG ===\n";
                cout << "1. Theo ngay\n";
                cout << "2. Theo thang\n";
                cout << "0. Quay lai\n";
                cout << "Nhap lua chon: ";
                cin >> subChoice;
                cin.ignore();

                if (subChoice == 1)
                {
                    manager.generateDailyReport();
                }
                else if (subChoice == 2)
                {
                    manager.generateMonthlyReport();
                }
                else
                {
                    cout << "Quay lai menu chinh...\n";
                }
                break;
            }
            case 2:
            {
                cout << "\n========== THONG KE TONG QUAT ==========\n";
                cout << "Tong so khach hang: " << manager.getAllCustomers().size();
                cout << "Tong so xe dang ky: " << manager.getAllVehicles().size() << "\n";
                cout << "Tong so cho do: " << manager.getTotalSlots() << "\n";
                cout << "Tong so don dat cho: " << manager.getAllBookings().size() << "\n";
                cout << "Tong so ve xe: " << manager.getAllTickets().size() << "\n";
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