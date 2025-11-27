#include <iostream>
#include <limits>
#include "ParkingManager.h"
#include "UI.h"
using namespace std;

UI ui;

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
        ui.showInfoMessage("Khoi tao du lieu");
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
        ui.showSuccessMessage("Da khoi tao 45 cho do xe!");
        Utils::pause();
    }

    while (true)
    {
        Utils::clearScreen();
        showMainMenu();

        int choice = ui.inputBoxInt("Nhap lua chon: ");

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
                cout << "          Cam on da su dung! Tam biet!\n";
                return 0;
            default:
                cout << "          Lua chon khong hop le!\n";
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
    ui.showMainMenu();
}

void handleLogin(ParkingManager &manager)
{
    Utils::clearScreen();
    ui.showLoginScreen();

    string username, password;

    while (true)
    {
        username = ui.inputBoxString("Nhap ten dang nhap (0 de huy): ");
        if (username == "0")
        {
            ui.showInfoMessage("Huy dang nhap. Tro ve menu chinh.");
            return;
        }
        password = ui.inputBoxString("Mat khau: ");
        if (manager.login(username, password))
        {
            ui.showSuccessMessage("Dang nhap thanh cong!");

            auto user = manager.getCurrentUser();
            if (user->getRole() == UserRole::CUSTOMER)
            {
                showCustomerMenu(manager);
            }
            else
            {
                showAdminMenu(manager);
            }
            break;
        }
        else
        {
            ui.showErrorMessage("Sai ten dang nhap hoac mat khau! Vui long nhap lai.");
        }
    }
}

void handleRegister(ParkingManager &manager)
{
    Utils::clearScreen();
    ui.showRegisterScreen();

    string username, password, fullName, phone, email;
    int roleChoice;
    string employeeID = "";

    while (true)
    {
        roleChoice = ui.inputBoxInt("Loai tai khoan (1-Khach hang, 2-Quan ly, 0-Huy): ");

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            ui.showErrorMessage("Nhap sai dinh dang! Thu lai.");
            continue;
        }

        if (roleChoice == 0)
            return;

        if (roleChoice == 1 || roleChoice == 2)
            break;
        ui.showErrorMessage("Lua chon khong hop le! Thu lai.");
    }
    if (roleChoice == 2)
    {
        const DoubleLinkedList<string> validCodes = []()
        {
            DoubleLinkedList<string> d;
            d.pushBack("EMP-001");
            d.pushBack("EMP-002");
            d.pushBack("EMP-ADMIN");
            return d;
        }();

        bool okCode = false;
        int attempts = 0;
        while (attempts < 3)
        {
            employeeID = ui.inputBoxString("Nhap ma nhan vien (0 de huy): ");
            if (employeeID == "0")
                return;

            for (auto it = validCodes.begin(); it != validCodes.end(); ++it)
            {
                if (*it == employeeID)
                {
                    okCode = true;
                    break;
                }
            }
            if (okCode)
                break;
            ui.showErrorMessage("Ma nhan vien khong hop le. Thu lai.");
            ++attempts;
        }
        if (!okCode)
        {
            ui.showErrorMessage("Nhap ma nhan vien that bai. Huy dang ky.");
            return;
        }
    }

    username = ui.inputBoxString("Ten dang nhap: ");

    while (true)
    {
        password = ui.inputBoxString("Mat khau moi: ");
        if (password.length() < 6)
        {
            ui.showErrorMessage("Mat khau phai co it nhat 6 ky tu!");
            continue;
        }

        break;
    }

    fullName = ui.inputBoxString("Ho va ten: ");

    while (true)
    {
        phone = ui.inputBoxString("So dien thoai: ");

        if (Utils::isValidPhoneNumber(phone))
            break;

        ui.showErrorMessage("So dien thoai khong hop le! Vui long nhap lai.");
    }

    while (true)
    {
        email = ui.inputBoxString("Email: ");

        if (Utils::isValidEmail(email))
            break;

        ui.showErrorMessage("Email khong hop le! Vui long nhap lai.");
    }

    UserRole role = (roleChoice == 2) ? UserRole::ADMIN : UserRole::CUSTOMER;

    try
    {
        // Truyền employeeID vào hàm registerUser
        if (manager.registerUser(username, password, fullName, phone, email, role, employeeID))
        {
            ui.showSuccessMessage("Dang ky thanh cong!");
        }
        else
        {
            ui.showErrorMessage("Dang ky that bai!");
        }
    }
    catch (const exception &e)
    {
        ui.showErrorMessage(e.what());
    }

    Utils::pause();
}

void showCustomerMenu(ParkingManager &manager)
{
    while (manager.isLoggedIn())
    {
        Utils::clearScreen();
        auto user = manager.getCurrentUser();
        ui.showCustomerMenu(user->getUsername());

        int choice = ui.inputBoxInt("Nhap lua chon: ");

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
                Utils::pause();
                break;
            case 4:
                viewCustomerHistory(manager);
                break;
            case 5:
                customerAccountManagement(manager);
                break;

            case 0:
                manager.logout();
                ui.showSuccessMessage("Da dang xuat!");
                return;
            default:
                ui.showErrorMessage("Lua chon khong hop le!");
                Utils::pause();
            }
        }
        catch (const exception &e)
        {
            ui.showErrorMessage(e.what());
            Utils::pause();
        }
    }
}

void customerVehicleManagement(ParkingManager &manager)
{
    while (true)
    {
        Utils::clearScreen();
        ui.showVehicleManagementMenu();

        int choice = ui.inputBoxInt("Nhap lua chon: ");

        if (choice == 0)
            break;

        try
        {
            switch (choice)
            {
            case 1:
            {
                Utils::clearScreen();
                string plate;
                while (true)
                {
                    plate = ui.inputBoxString("Bien so xe: ");

                    if (!Utils::isValidLicensePlate(plate))
                    {
                        ui.showErrorMessage("Bien so khong hop le (dinh dang: 12A-3456 hoac 12A34567). Vui long nhap lai!");
                        continue;
                    }

                    auto existing = manager.getVehicleByPlate(plate);
                    if (existing != nullptr)
                    {
                        ui.showErrorMessage("Bien so da ton tai. Vui long nhap lai!");
                        continue;
                    }
                    break;
                }

                int typeChoice;
                string brand, model, color;

                typeChoice = ui.inputBoxInt("Loai xe (1-Xe may, 2-O to, 3-Xe dap dien): ");
                brand = ui.inputBoxString("Hang xe: ");
                model = ui.inputBoxString("Model: ");
                color = ui.inputBoxString("Mau sac: ");

                VehicleType type;
                int engineCapacity = 0, seatCount = 0, battery = 0, maxSpeed = 0;
                bool isLuxury = false;
                switch (typeChoice)
                {
                case 1:
                    type = VehicleType::MOTORCYCLE;
                    engineCapacity = ui.inputBoxInt("Phan khoi (cc): ");
                    break;
                case 2:
                    type = VehicleType::CAR;
                    seatCount = ui.inputBoxInt("So cho ngoi: ");
                    isLuxury = (ui.inputBoxInt("Xe sang? (1-Co, 0-Khong): ") == 1);
                    break;
                case 3:
                    type = VehicleType::ELECTRIC_BIKE;
                    battery = ui.inputBoxInt("Dung luong pin (mAh): ");
                    maxSpeed = ui.inputBoxInt("Toc do toi da (km/h): ");
                    break;
                default:
                    break;
                }

                if (manager.registerVehicle(plate, type, brand, model, color,
                                            manager.getCurrentUser()->getUserId(),
                                            engineCapacity, seatCount, isLuxury, battery, maxSpeed))
                {
                    auto v = manager.getVehicleByPlate(plate);
                    if (v)
                    {
                        if (type == VehicleType::MOTORCYCLE)
                        {
                            auto m = std::dynamic_pointer_cast<Motorcycle>(v);
                            if (m)
                                m->setEngineCapacity(engineCapacity);
                        }
                        else if (type == VehicleType::CAR)
                        {
                            auto c = std::dynamic_pointer_cast<Car>(v);
                            if (c)
                            {
                                c->setSeatCount(seatCount);
                                c->setIsLuxury(isLuxury);
                            }
                        }
                        else if (type == VehicleType::ELECTRIC_BIKE)
                        {
                            auto e = std::dynamic_pointer_cast<ElectricBike>(v);
                            if (e)
                            {
                                e->setBatteryCapacity(battery);
                                e->setMaxSpeed(maxSpeed);
                            }
                        }

                        // Luu thay doi neu can
                        manager.saveAllData();
                    }
                }
                ui.showSuccessMessage("Dang ky xe thanh cong!");
            }
            break;
            case 2:
            {
                Utils::clearScreen();
                auto vehicles = manager.getVehiclesByCustomer(
                    manager.getCurrentUser()->getUserId());

                ui.showReportHeader("DANH SACH XE CUA BAN");
                if (vehicles.empty())
                {
                    ui.showErrorMessage("Chua co xe nao duoc dang ky.");
                }
                else
                {
                    int widths[] = {16, 22, 19, 28};
                    string headers[] = {"Vehicle ID", "License Plate", "Type", "Brand & Model"};
                    ui.printBorder(widths, 4);
                    ui.printHeader(headers, widths, 4);
                    ui.printBorder(widths, 4);

                    for (auto it = vehicles.begin(); it != vehicles.end(); ++it)
                    {
                        (*it)->displayTableRow();
                    }

                    ui.printBorder(widths, 4);
                }
                break;
            }
            case 3:
            {
                Utils::clearScreen();
                string vehicleId;
                shared_ptr<Vehicle> existingVehicle;

                while (true)
                {
                    vehicleId = ui.inputBoxString("Nhap Vehicle ID: ");

                    existingVehicle = manager.getVehicle(vehicleId);
                    if (existingVehicle == nullptr)
                    {
                        ui.showErrorMessage("Vehicle ID khong ton tai. Vui long nhap lai!");
                        continue;
                    }

                    break;
                }

                string brand, model, color;
                brand = ui.inputBoxString("Hang xe moi (Enter de giu nguyen): ");
                if (brand.empty())
                    brand = existingVehicle->getBrand();
                model = ui.inputBoxString("Model moi (Enter de giu nguyen): ");
                if (model.empty())
                    model = existingVehicle->getModel();
                color = ui.inputBoxString("Mau sac moi (Enter de giu nguyen): ");
                if (color.empty())
                    color = existingVehicle->getColor();

                if (manager.updateVehicle(vehicleId, brand, model, color))
                {
                    ui.showSuccessMessage("Cap nhat thanh cong!");
                }
                break;
            }
            case 4:
            {
                Utils::clearScreen();
                string vehicleId;
                auto vehicles = manager.getVehiclesByCustomer(manager.getCurrentUser()->getUserId());

                if (vehicles.empty())
                {
                    ui.showErrorMessage("Ban chua co xe nao de xoa.");
                    break;
                }

                ui.showReportHeader("DANH SACH XE CUA BAN");
                int widths[] = {16, 22, 19, 28};
                string headers[] = {"Vehicle ID", "License Plate", "Type", "Brand & Model"};
                ui.printBorder(widths, 4);
                ui.printHeader(headers, widths, 4);
                ui.printBorder(widths, 4);

                for (auto it = vehicles.begin(); it != vehicles.end(); ++it)
                {
                    (*it)->displayTableRow();
                }

                ui.printBorder(widths, 4);
                vehicleId = ui.inputBoxString("Nhap Vehicle ID can xoa (hoac '0' de huy): ");

                if (vehicleId == "0")
                {
                    ui.showInfoMessage("Da huy thao tac xoa.");
                    break;
                }

                auto vehicle = manager.getVehicle(vehicleId);
                if (!vehicle)
                {
                    ui.showErrorMessage("Khong tim thay xe voi ID: " + vehicleId);
                    break;
                }

                if (vehicle->getCustomerId() != manager.getCurrentUser()->getUserId())
                {
                    ui.showErrorMessage("Xe nay khong thuoc ve ban!");
                    break;
                }

                ui.showReportHeader("THONG TIN XE CAN XOA");
                vehicle->displayInfo();
                string confirm = ui.inputBoxString("Ban co chac chan muon xoa xe nay? (yes/no): ");

                if (confirm == "yes" || confirm == "YES")
                {
                    try
                    {
                        if (manager.deleteVehicle(vehicleId))
                        {
                            ui.showSuccessMessage("Xoa xe thanh cong!");
                        }
                        else
                        {
                            ui.showErrorMessage("Xoa xe that bai!");
                        }
                    }
                    catch (const exception &e)
                    {
                        ui.showErrorMessage(e.what());
                    }
                }
                else
                {
                    ui.showInfoMessage("Da huy thao tac xoa.");
                }
                break;
            }
            default:
                ui.showErrorMessage("Lua chon khong hop le!");
            }
        }
        catch (const exception &e)
        {
            ui.showErrorMessage(e.what());
        }
        Utils::pause();
    }
}

void customerBookingManagement(ParkingManager &manager)
{
    while (true)
    {
        Utils::clearScreen();
        ui.showBookingManagementMenu();

        int choice = ui.inputBoxInt("Nhap lua chon: ");

        if (choice == 0)
            break;

        try
        {
            switch (choice)
            {

            case 1:
            {
                Utils::clearScreen();
                ui.showReportHeader("DAT CHO MOI");

                string vehicleId;
                shared_ptr<Vehicle> vehicle;
                int hours = -1;

                // 1. Nhập Vehicle ID
                while (true)
                {
                    vehicleId = ui.inputBoxString("Vehicle ID (nhap 0 de huy): ");
                    if (vehicleId == "0")
                    {
                        ui.showInfoMessage("Huy dat cho.");
                        break;
                    }

                    vehicle = manager.getVehicle(vehicleId);
                    if (vehicle)
                        break;

                    ui.showErrorMessage("Vehicle ID khong ton tai. Thu lai hoac nhap 0 de huy.");
                }

                if (!vehicle)
                    break; // Thoát nếu không có xe

                // 2. Nhập số giờ dự kiến
                hours = ui.inputBoxInt("Du kien den sau bao nhieu gio? (>=0): ");
                if (hours < 0)
                {
                    ui.showErrorMessage("Nhap khong hop le!");
                    break;
                }

                time_t expectedArrival = time(nullptr) + static_cast<time_t>(hours) * 3600;

                // 3. Lấy danh sách chỗ trống
                auto freeSlots = manager.getAvailableSlotsByType(vehicle->getType());
                DoubleLinkedList<string> slotIds;

                ui.showReportHeader("DANH SACH CHO TRONG");
                int index = 1;
                for (auto it = freeSlots.begin(); it != freeSlots.end(); ++it)
                {
                    it->displayTableRow();
                    slotIds.pushBack(it->getSlotId());
                    ++index;
                }

                ui.printHorizontalLine('+', '-', '+');

                if (slotIds.empty())
                {
                    ui.showErrorMessage("Hien tai khong con cho trong cho loai xe nay.");
                    break;
                }

                // 4. Người dùng chọn chỗ
                int choice = ui.inputBoxInt("Nhap so thu tu de chon cho (0 = chon tu dong): ");
                if (choice < 0 || choice > slotIds.size())
                {
                    ui.showErrorMessage("Lua chon khong hop le!");
                    break;
                }

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
                // Nếu choice == 0, chosenSlotId sẽ để trống -> manager tự chọn slot

                // 5. Tạo booking
                bool ok = manager.createBooking(
                    manager.getCurrentUser()->getUserId(),
                    vehicleId,
                    expectedArrival,
                    chosenSlotId);

                if (ok)
                    ui.showSuccessMessage("Dat cho thanh cong.");
                else
                    ui.showErrorMessage("Dat cho that bai.");

                break;
            }
            case 2:
            {
                Utils::clearScreen();

                auto bookings = manager.getBookingsByCustomer(
                    manager.getCurrentUser()->getUserId());

                ui.showReportHeader("DANH SACH DAT CHO");
                if (bookings.empty())
                {
                    ui.showInfoMessage("Chua co dat cho nao.");
                }
                else
                {
                    int widths[] = {23, 15, 29, 18};
                    string headers[] = {"Booking ID", "Vehicle ID", "Check-in", "Status"};
                    ui.printBorder(widths, 4);
                    ui.printHeader(headers, widths, 4);
                    ui.printBorder(widths, 4);

                    for (auto it = bookings.begin(); it != bookings.end(); ++it)
                    {
                        it->displayTableRow();
                    }
                    ui.printBorder(widths, 4);
                }
                break;
            }
            case 3:
            {
                Utils::clearScreen();
                ui.showReportHeader("HUY DAT CHO");
                manager.cancelBooking();
                break;
            }
            default:
                ui.showErrorMessage("Lua chon khong hop le!");
            }
        }
        catch (const exception &e)
        {
            ui.showErrorMessage(e.what());
        }

        Utils::pause();
    }
}

void viewCustomerHistory(ParkingManager &manager)
{
    Utils::clearScreen();
    auto tickets = manager.getTicketsByCustomer(manager.getCurrentUser()->getUserId());

    ui.showReportHeader("LICH SU GUI XE");
    if (tickets.empty())
    {
        ui.showInfoMessage("Chua co lich su gui xe.");
    }
    else
    {
        int widths[] = {17, 13, 17, 21, 16};
        string headers[] = {"Ticket ID", "Vehicle ID", "Slot", "Check-in", "Trang thai"};

        ui.printBorder(widths, 5);
        ui.printHeader(headers, widths, 5);
        ui.printBorder(widths, 5);

        for (auto it = tickets.begin(); it != tickets.end(); ++it)
        {
            it->displayTableRow();
        }

        ui.printBorder(widths, 5);
    }
    Utils::pause();
}

void customerAccountManagement(ParkingManager &manager)
{
    while (true)
    {
        Utils::clearScreen();
        ui.showAccountManagementMenu();
        auto user = manager.getCurrentUser();
        Customer *customer = dynamic_cast<Customer *>(user.get());

        int choice = ui.inputBoxInt("Nhap lua chon: ");
        if (choice == 0)
            break;

        try
        {
            switch (choice)
            {
            case 1:
            {
                Utils::clearScreen();
                ui.showReportHeader("THONG TIN TAI KHOAN");
                customer->displayInfo();
                break;
            }
            case 2:
            {
                Utils::clearScreen();
                string fullName, phone, email;
                ui.showReportHeader("CAP NHAT THONG TIN");
                string availName = ui.inputBoxString("Ho ten hien tai: " + user->getFullName());
                fullName = ui.inputBoxString("Ho ten moi (Enter de giu nguyen): ");
                if (fullName.empty())
                    fullName = user->getFullName();

                string availPhone = ui.inputBoxString("SDT hien tai: " + user->getPhoneNumber());
                phone = ui.inputBoxString("So dien thoai moi (Enter de giu nguyen): ");
                if (phone.empty())
                    phone = user->getPhoneNumber();

                string availEmail = ui.inputBoxString("Email hien tai: " + user->getEmail());
                email = ui.inputBoxString("Email moi (Enter de giu nguyen): ");
                if (email.empty())
                    email = user->getEmail();

                if (manager.updateCustomerInfo(user->getUserId(), fullName, phone, email))
                {
                    ui.showSuccessMessage("Cap nhat thanh cong!");
                }
                break;
            }
            case 3:
            {
                Utils::clearScreen();
                string oldPassword, newPassword, confirmPassword;
                int attempts = 0;
                ui.showReportHeader("DOI MAT KHAU");
                oldPassword = ui.inputBoxString("Mat khau hien tai: ");

                while (attempts < 3)
                {
                    if (oldPassword == user->getPassword())
                    {
                        break;
                    }
                    attempts++;
                    ui.showErrorMessage("Mat khau hien tai khong dung! Vui long thu lai.");
                    oldPassword = ui.inputBoxString("Mat khau hien tai: ");
                }
                if (attempts == 3)
                {
                    ui.showErrorMessage("Nhap sai qua 3 lan! Da thoat!");
                    break;
                }
                while (true)
                {
                    newPassword = ui.inputBoxString("Mat khau: ");

                    if (newPassword.length() < 6)
                    {
                        ui.showErrorMessage("Mat khau phai co it nhat 6 ky tu!");
                        continue;
                    }

                    break;
                }

                confirmPassword = ui.inputBoxString("Xac nhan mat khau moi: ");

                if (newPassword != confirmPassword)
                {
                    ui.showErrorMessage("Mat khau xac nhan khong khop!");
                    break;
                }

                user->setPassword(newPassword);
                manager.saveAllData();
                ui.showSuccessMessage("Doi mat khau thanh cong!");
                break;
            }
            default:
                ui.showErrorMessage("Lua chon khong hop le!");
            }
        }
        catch (const exception &e)
        {
            ui.showErrorMessage(e.what());
        }

        Utils::pause();
    }
}

void showAdminMenu(ParkingManager &manager)
{
    while (manager.isLoggedIn())
    {
        Utils::clearScreen();
        ui.showAdminMenu(manager.getCurrentUser()->getFullName());
        int choice = ui.inputBoxInt("Nhap lua chon: ");

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
                ui.showSuccessMessage("Da dang xuat!");
                return;
            default:
                ui.showErrorMessage("Lua chon khong hop le!");
                Utils::pause();
            }
        }
        catch (const exception &e)
        {
            ui.showErrorMessage(e.what());
            Utils::pause();
        }
    }
}

void adminUserManagement(ParkingManager &manager)
{
    while (true)
    {
        Utils::clearScreen();
        ui.showUserManagementMenu();

        int choice = ui.inputBoxInt("Nhap lua chon: ");

        try
        {
            switch (choice)
            {
            case 1:
            {
                Utils::clearScreen();
                auto customers = manager.getAllCustomers();
                ui.showReportHeader("TAT CA KHACH HANG");
                if (customers.empty())
                {
                    ui.showInfoMessage("Chua co khach hang nao.");
                }
                else
                {
                    int widths[] = {20, 25, 15, 25};
                    string headers[] = {"Customer ID", "Full Name", "Phone Number", "Email"};
                    ui.printBorder(widths, 4);
                    ui.printHeader(headers, widths, 4);
                    ui.printBorder(widths, 4);

                    for (auto it = customers.begin(); it != customers.end(); ++it)
                    {
                        (*it)->displayTableRow();
                    }

                    ui.printBorder(widths, 4);
                }
                break;
            }
            case 2:
            {
                Utils::clearScreen();
                string customerId, fullName, phone, email;
                int loyaltyPoints;

                ui.showReportHeader("CAP NHAT KHACH HANG");
                customerId = ui.inputBoxString("Nhap Customer ID: ");
                Customer *customer = manager.getCustomer(customerId);
                if (!customer)
                {
                    ui.showErrorMessage("Khong tim thay khach hang voi ID: " + customerId);
                    break;
                }

                ui.showReportHeader("THONG TIN HIEN TAI");
                customer->displayInfo();

                ui.showReportHeader("NHAP THONG TIN MOI");
                fullName = ui.inputBoxString("Ho ten moi (Enter de giu nguyen): ");
                if (fullName.empty())
                    fullName = customer->getFullName();

                phone = ui.inputBoxString("So dien thoai moi (Enter de giu nguyen): ");
                if (phone.empty())
                    phone = customer->getPhoneNumber();

                email = ui.inputBoxString("Email moi (Enter de giu nguyen): ");
                if (email.empty())
                    email = customer->getEmail();

                loyaltyPoints = ui.inputBoxInt("Diem tich luy moi (hien tai: " + to_string(customer->getLoyaltyPoints()) + "): ");

                if (manager.updateCustomerDetails(customerId, fullName, phone,
                                                  email, loyaltyPoints))
                {
                    ui.showSuccessMessage("Cap nhat thanh cong!");
                }
                break;
            }
            case 3:
            {
                Utils::clearScreen();
                string username, password, fullName, phone, email;

                ui.showReportHeader("THEM KHACH HANG MOI");
                username = ui.inputBoxString("Username: ");
                password = ui.inputBoxString("Password: ");
                fullName = ui.inputBoxString("Ho ten: ");
                phone = ui.inputBoxString("So dien thoai: ");
                email = ui.inputBoxString("Email: ");

                if (manager.addCustomer(username, password, fullName,
                                        phone, email))
                {
                    ui.showSuccessMessage("Them khach hang thanh cong!");
                }
                break;
            }

            case 4:
            {
                Utils::clearScreen();
                string customerId;

                ui.showReportHeader("XOA KHACH HANG");
                customerId = ui.inputBoxString("Nhap Customer ID can xoa: ");

                Customer *customer = manager.getCustomer(customerId);
                if (!customer)
                {
                    ui.showErrorMessage("Khong tim thay khach hang voi ID: " + customerId);
                    break;
                }

                ui.showReportHeader("THONG TIN KHACH HANG");
                customer->displayInfo();
                string confirm = ui.inputBoxString("Ban co chac chan muon xoa khach hang nay? (yes/no): ");

                if (confirm == "yes" || confirm == "YES")
                {
                    if (manager.deleteCustomer(customerId))
                    {
                        ui.showSuccessMessage("Xoa khach hang thanh cong!");
                    }
                }
                else
                {
                    ui.showErrorMessage("Da huy thao tac xoa.");
                }
                break;
            }
            case 5:
            {
                while (true)
                {
                    Utils::clearScreen();
                    string keyword;
                    ui.showReportHeader("TIM KIEM NGUOI DUNG");
                    keyword = ui.inputBoxString("Nhap tu khoa tim kiem (Nhan Enter de thoat): ");

                    if (keyword.empty())
                    {
                        ui.showInfoMessage("Thoat tim kiem.");
                        break;
                    }
                    auto results = manager.searchUsers(keyword);
                    ui.showReportHeader("KET QUA TIM KIEM");
                    if (results.empty())
                    {
                        ui.showInfoMessage("Khong tim thay ket qua nao.");
                    }
                    else
                    {
                        int widths[] = {20, 25, 15, 25};
                        string headers[] = {"Customer ID", "Full Name", "Phone Number", "Email"};

                        ui.printBorder(widths, 4);
                        ui.printHeader(headers, widths, 4);
                        ui.printBorder(widths, 4);

                        for (auto it = results.begin(); it != results.end(); ++it)
                        {
                            (*it)->displayTableRow();
                        }

                        ui.printBorder(widths, 4);
                    }

                    Utils::pause();
                }
                break;
            }
            case 0:
                return;
            }
        }
        catch (const exception &e)
        {
            cout << "Loi: " << e.what() << endl;
        }
        Utils::pause();
    }
}
void adminVehicleManagement(ParkingManager &manager)
{
    while (true)
    {
        Utils::clearScreen();
        ui.showAdminVehicleMenu();
        int choice = ui.inputBoxInt("Nhap lua chon: ");

        if (choice == 0)
            return;
        try
        {
            switch (choice)
            {
            case 1:
            {
                Utils::clearScreen();
                auto vehicles = manager.getAllVehicles();
                ui.showReportHeader("TAT CA XE");
                if (vehicles.empty())
                {
                    ui.showInfoMessage("Chua co xe nao.");
                }
                else
                {
                    int widths[] = {16, 22, 19, 28};
                    string headers[] = {"Vehicle ID", "License Plate", "Type", "Brand & Model"};
                    ui.printBorder(widths, 4);
                    ui.printHeader(headers, widths, 4);
                    ui.printBorder(widths, 4);

                    for (auto it = vehicles.begin(); it != vehicles.end(); ++it)
                    {
                        (*it)->displayTableRow();
                    }

                    ui.printBorder(widths, 4);
                }
                break;
            }
            case 2:
            {
                Utils::clearScreen();
                string keyword = ui.inputBoxString("Nhap tu khoa tim kiem (bien so, hang xe, model): ");

                auto results = manager.searchVehicles(keyword);
                ui.showReportHeader("KET QUA TIM KIEM");
                if (results.empty())
                {
                    ui.showInfoMessage("Khong tim thay ket qua nao.");
                }
                else
                {
                    int widths[] = {16, 22, 19, 28};
                    string headers[] = {"Vehicle ID", "License Plate", "Type", "Brand & Model"};
                    ui.printBorder(widths, 4);
                    ui.printHeader(headers, widths, 4);
                    ui.printBorder(widths, 4);

                    for (auto it = results.begin(); it != results.end(); ++it)
                    {
                        (*it)->displayTableRow();
                    }

                    ui.printBorder(widths, 4);
                }
                break;
            }
            }
        }
        catch (const exception &e)
        {
            ui.showErrorMessage(e.what());
        }
        Utils::pause();
    }
}

void adminSlotManagement(ParkingManager &manager)
{
    while (true)
    {
        Utils::clearScreen();
        ui.showSlotManagementMenu();
        int choice = ui.inputBoxInt("Nhap lua chon: ");

        if (choice == 0)
            return;

        try
        {
            switch (choice)
            {
            case 1:
            {
                Utils::clearScreen();
                auto slots = manager.getAllSlots();
                ui.showReportHeader("TAT CA CHO DO");
                if (slots.empty())
                {
                    ui.showInfoMessage("Chua co cho do nao.");
                }
                else
                {
                    int widths[] = {22, 26, 20, 17};
                    string headers[] = {"Slot ID", "Number of Positions", "Vehicle Type", "Status"};
                    ui.printBorder(widths, 4);
                    ui.printHeader(headers, widths, 4);
                    ui.printBorder(widths, 4);

                    for (auto it = slots.begin(); it != slots.end(); ++it)
                    {
                        it->displayTableRow();
                    }

                    ui.printBorder(widths, 4);
                    ui.printRow("          | Tong: ", to_string(slots.size()) + " cho");
                    ui.printBorder(widths, 4);
                }
                break;
            }
            case 2:
            {
                Utils::clearScreen();
                string slotId;
                int statusChoice;

                slotId = ui.inputBoxString("Slot ID: ");
                cout << "          | "
                     << setw(19) << left << "0-AVAILABLE"
                     << " | " << setw(19) << left << "1-OCCUPIED"
                     << " | " << setw(19) << left << "2-RESERVED"
                     << " | " << setw(20) << left << "3-MAINTENANCE"
                     << " |" << endl;

                statusChoice = ui.inputBoxInt("Trang thai moi: ");

                SlotStatus status = static_cast<SlotStatus>(statusChoice);
                if (manager.updateSlotStatus(slotId, status))
                    ui.showSuccessMessage("Cap nhat thanh cong!");
                else
                    ui.showErrorMessage("Cap nhat that bai!");
                break;
            }
            case 3:
            {
                Utils::clearScreen();
                ui.showReportHeader("CHO DO CON TRONG");
                ui.printAvailableSlots(
                    manager.getAvailableSlotCount(VehicleType::MOTORCYCLE),
                    manager.getAvailableSlotCount(VehicleType::CAR),
                    manager.getAvailableSlotCount(VehicleType::ELECTRIC_BIKE));
                break;
            }
            }
        }
        catch (const exception &e)
        {
            ui.showErrorMessage(e.what());
        }
        Utils::pause();
    }
}

void adminBookingManagement(ParkingManager &manager)
{
    while (true)
    {
        Utils::clearScreen();
        ui.showAdminBookingMenu();
        int choice = ui.inputBoxInt("Nhap lua chon: ");

        if (choice == 0)
            return;

        try
        {
            switch (choice)
            {
            case 1:
            {
                Utils::clearScreen();
                auto bookings = manager.getAllBookings();
                ui.showReportHeader("TAT CA DON DAT CHO");
                if (bookings.empty())
                {
                    ui.showInfoMessage("Chua co don dat cho nao.");
                }
                else
                {
                    int widths[] = {23, 15, 29, 18};
                    string headers[] = {"Booking ID", "Vehicle ID", "Check-in", "Status"};
                    ui.printBorder(widths, 4);
                    ui.printHeader(headers, widths, 4);
                    ui.printBorder(widths, 4);

                    for (auto it = bookings.begin(); it != bookings.end(); ++it)
                    {
                        it->displayTableRow();
                    }

                    ui.printBorder(widths, 4);
                }
                break;
            }
            case 2:
            {
                Utils::clearScreen();
                manager.cancelBooking();
                break;
            }
            case 3:
            {

                Utils::clearScreen();
                ui.showReportHeader("CHECK-IN");
                string vehicleId = ui.inputBoxString("Vehicle ID: ");
                string bookingId = ui.inputBoxString("Booking ID (bo trong neu khong dat truoc): ");

                string slotId;
                if (bookingId.empty())
                {
                    slotId = ui.inputBoxString("Nhap Slot ID (bo trong neu muon tu dong): ");
                }

                // --- CHANGED: determine correct customerId ---
                string customerId;
                if (!bookingId.empty())
                {
                    Booking *bk = manager.getBooking(bookingId);
                    if (!bk)
                    {
                        ui.showErrorMessage("Booking ID khong ton tai!");
                        break;
                    }
                    customerId = bk->getCustomerId(); // use booking's customer
                }
                else
                {
                    // No booking: admin must provide customer id (or we could try to infer from vehicle)
                    customerId = ui.inputBoxString("Nhap Customer ID (de tiep tuc): ");
                    if (customerId.empty())
                    {
                        ui.showErrorMessage("Customer ID bat buoc khi khong su dung booking!");
                        break;
                    }
                }

                string ticketId = manager.checkIn(customerId, vehicleId, bookingId, slotId);

                auto ticket = manager.getTicket(ticketId);
                ui.showSuccessMessage("Check-in thanh cong!");

                ui.printRow("          | ", "Ticket ID: " + ticketId);
                ui.printHorizontalLine('+', '-', '+');

                break;
            }

            case 4:
            {
                Utils::clearScreen();
                ui.showReportHeader("CHECK-OUT");
                string ticketId = ui.inputBoxString("Nhap Ticket ID: ");

                if (manager.checkOut(ticketId))
                {
                    auto ticket = manager.getTicket(ticketId);
                    ui.showSuccessMessage("Check-out thanh cong!");
                    ticket->displayInfo(); // In vé đầy đủ
                }
                break;
            }

            default:
                ui.showInfoMessage("Lua chon khong hop le. Vui long nhap lai.");
                break;
            }
        }
        catch (const exception &e)
        {
            ui.showErrorMessage(e.what());
        }
        Utils::pause();
    }
}

void adminTicketManagement(ParkingManager &manager)
{
    while (true)
    {
        Utils::clearScreen();
        ui.showTicketManagementMenu();

        int choice = ui.inputBoxInt("Nhap lua chon: ");

        if (choice == 0)
            return;

        try
        {
            switch (choice)
            {
            case 1:
            {
                Utils::clearScreen();
                auto tickets = manager.getAllTickets();
                ui.showReportHeader("TAT CA VE XE");
                if (tickets.empty())
                {
                    ui.showInfoMessage("Chua co ve nao.");
                }
                else
                {
                    int widths[] = {17, 13, 17, 21, 16};
                    string headers[] = {"Ticket ID", "Vehicle ID", "Slot", "Check-in", "Status"};
                    ui.printBorder(widths, 5);
                    ui.printHeader(headers, widths, 5);
                    ui.printBorder(widths, 5);

                    for (auto it = tickets.begin(); it != tickets.end(); ++it)
                    {
                        it->displayTableRow();
                    }

                    ui.printBorder(widths, 5);
                }
                break;
            }
            case 2:
            {
                Utils::clearScreen();
                manager.adminPriceManagement(manager);
                break;
            }
            case 3:
            {
                Utils::clearScreen();
                manager.advancedTicketSearch(manager);
                break;
            }
            }
        }
        catch (const exception &e)
        {
            ui.showErrorMessage(e.what());
        }
        Utils::pause();
    }
}

void adminStatistics(ParkingManager &manager)
{
    while (true)
    {
        Utils::clearScreen();
        ui.showStatisticsMenu();

        int choice = ui.inputBoxInt("Nhap lua chon: ");

        if (choice == 0)
            return;

        try
        {
            switch (choice)
            {
            case 1:
            {
                Utils::clearScreen();
                int subChoice = -1;
                while (true)
                {
                    if (subChoice == 0)
                        break;
                    Utils::clearScreen();
                    ui.showReportHeader("BAO CAO HOAT DONG");
                    ui.showReportDetailMenu();
                    subChoice = ui.inputBoxInt("Nhap lua chon: ");
                    if (subChoice == 0)
                        break;

                    else if (subChoice == 1)
                    {
                        Utils::clearScreen();
                        manager.generateDailyReport();
                        Utils::pause();
                    }
                    else if (subChoice == 2)
                    {
                        Utils::clearScreen();
                        manager.generateMonthlyReport();
                        Utils::pause();
                    }
                    else
                    {
                        ui.showInfoMessage("Quay lai menu chinh.");
                    }
                }
                break;
            }
            case 2:
            {
                Utils::clearScreen();
                ui.showReportHeader("THONG KE TONG QUAT");
                ui.printHorizontalLine('+', '-', '+');
                ui.printRow("          | ", "Tong so khach hang: " + to_string(manager.getAllCustomers().size()));
                ui.printRow("          | ", "Tong so xe dang ky: " + to_string(manager.getAllVehicles().size()));
                ui.printRow("          | ", "Tong so cho do:     " + to_string(manager.getTotalSlots()));
                ui.printRow("          | ", "Tong so booking:    " + to_string(manager.getAllBookings().size()));
                ui.printRow("          | ", "Tong so ve xe:      " + to_string(manager.getAllTickets().size()));
                ui.printRow("          | ", "Tong doanh thu:     " + to_string(static_cast<long long>(manager.getTotalRevenue())) + " VND");
                ui.printHorizontalLine('+', '-', '+');
                Utils::pause();
                break;
            }
            case 3:
            {
                Utils::clearScreen();
                ui.showReportHeader("DOANH THU THEO KHOANG THOI GIAN");
                int startHours = ui.inputBoxInt("Thoi gian bat dau (so gio truoc tinh tu bay gio): ");
                int endHours = ui.inputBoxInt("Thoi gian ket thuc (so gio truoc tinh tu bay gio): ");
                time_t now = time(nullptr);
                time_t startTime = now - (startHours * 3600);
                time_t endTime = now - (endHours * 3600);

                double revenue = manager.getRevenueByPeriod(startTime, endTime);
                ui.printHorizontalLine('+', '-', '+');
                ui.printRow("          | ", "Doanh thu trong khoang thoi gian: " + to_string(static_cast<long long>(revenue)) + " VND");
                ui.printHorizontalLine('+', '-', '+');

                Utils::pause();
                break;
            }
            default:
                ui.showInfoMessage("Lua chon khong hop le!\n");
            }
        }
        catch (const exception &e)
        {
            ui.showErrorMessage(e.what());
        }
    }
}