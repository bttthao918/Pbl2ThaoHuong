#include "ParkingManager.h"
#include "UI.h"
#include "Utils.h"
#include "Exceptions.h"
#include "PricingConfig.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <limits>
using namespace std;

extern UI ui;

ParkingManager::ParkingManager() : currentUser(nullptr)
{
    loadAllData();
}

ParkingManager::~ParkingManager()
{
    saveAllData();
}

// ========== User Management ==========
bool ParkingManager::registerUser(const string &username, const string &password,
                                  const string &fullName, const string &phone,
                                  const string &email, UserRole role,
                                  const string &employeeID) // Thêm tham số này
{
    // Check if username already exists
    auto existing = users.find([&](const shared_ptr<User> &u)
                               { return u->getUsername() == username; });

    if (existing != nullptr)
    {
        throw DuplicateException("Ten dang nhap da ton tai");
    }

    try
    {
        string userId = generateUserId();
        shared_ptr<User> newUser;

        if (role == UserRole::CUSTOMER)
        {
            newUser = make_shared<Customer>(userId, username, password,
                                            fullName, phone, email);
        }
        else
        {
            // Sử dụng employeeID được truyền vào thay vì tự tạo
            if (employeeID.empty())
            {
                throw invalid_argument("Ma nhan vien khong duoc de trong");
            }

            newUser = make_shared<Admin>(userId, username, password,
                                         fullName, phone, email, employeeID);
        }

        users.pushBack(newUser);
        saveUsers();
        return true;
    }
    catch (const exception &e)
    {
        throw;
    }
}

bool ParkingManager::login(const string &username, const string &password)
{
    auto user = users.find([&](const shared_ptr<User> &u)
                           { return u->getUsername() == username && u->getPassword() == password; });

    if (user != nullptr)
    {
        currentUser = *user;
        return true;
    }
    return false;
}

void ParkingManager::logout()
{
    currentUser = nullptr;
}

Customer *ParkingManager::getCustomer(const string &customerId)
{
    auto user = users.find([&](const shared_ptr<User> &u)
                           { return u->getUserId() == customerId && u->getRole() == UserRole::CUSTOMER; });

    if (user != nullptr)
    {
        return dynamic_cast<Customer *>(user->get());
    }
    return nullptr;
}

DoubleLinkedList<shared_ptr<User>> ParkingManager::getAllCustomers()
{
    return users.filter([](const shared_ptr<User> &u)
                        { return u->getRole() == UserRole::CUSTOMER; });
}

bool ParkingManager::updateCustomerInfo(const string &customerId, const string &fullName,
                                        const string &phone, const string &email)
{
    Customer *customer = getCustomer(customerId);
    if (!customer)
        return false;

    try
    {
        customer->setFullName(fullName);
        customer->setPhoneNumber(phone);
        customer->setEmail(email);
        saveUsers();
        return true;
    }
    catch (const exception &e)
    {
        throw;
    }
}

bool ParkingManager::addCustomer(const string &username, const string &password,
                                 const string &fullName, const string &phone,
                                 const string &email)
{
    // Kiểm tra username đã tồn tại chưa
    auto existing = users.find([&](const shared_ptr<User> &u)
                               { return u->getUsername() == username; });

    if (existing != nullptr)
    {
        throw DuplicateException("Ten dang nhap da ton tai");
    }

    try
    {
        string userId = generateUserId();
        shared_ptr<Customer> newCustomer = make_shared<Customer>(
            userId, username, password, fullName, phone, email);

        users.pushBack(newCustomer);
        saveUsers();

        ui.showInfoMessage("Da tao Customer ID: " + userId);
        return true;
    }
    catch (const exception &e)
    {
        throw;
    }
}

bool ParkingManager::deleteCustomer(const string &customerId)
{
    // Kiểm tra khách hàng có xe đang đăng ký không
    auto customerVehicles = getVehiclesByCustomer(customerId);
    if (!customerVehicles.empty())
    {
        throw InvalidInputException("Khach hang van con xe dang ky. Vui long xoa xe truoc!");
    }

    // Kiểm tra khách hàng có booking đang active không
    auto customerBookings = getBookingsByCustomer(customerId);
    for (auto it = customerBookings.begin(); it != customerBookings.end(); ++it)
    {
        if (it->getStatus() == BookingStatus::PENDING ||
            it->getStatus() == BookingStatus::CONFIRMED)
        {
            throw InvalidInputException("Khach hang van con booking chua hoan thanh!");
        }
    }

    // Kiểm tra khách hàng có ticket đang active không
    auto customerTickets = getTicketsByCustomer(customerId);
    for (auto it = customerTickets.begin(); it != customerTickets.end(); ++it)
    {
        if (it->getStatus() == TicketStatus::ACTIVE)
        {
            throw InvalidInputException("Khach hang van con ve gui xe chua thanh toan!");
        }
    }

    // Xóa user
    bool removed = users.remove([&](const shared_ptr<User> &u)
                                { return u->getUserId() == customerId && u->getRole() == UserRole::CUSTOMER; });

    if (removed)
    {
        saveUsers();
        return true;
    }

    return false;
}

bool ParkingManager::updateCustomerDetails(const string &customerId,
                                           const string &fullName,
                                           const string &phone,
                                           const string &email,
                                           int loyaltyPoints)
{
    Customer *customer = getCustomer(customerId);
    if (!customer)
    {
        throw NotFoundException("Khong tim thay khach hang");
    }

    try
    {
        // Cập nhật thông tin cơ bản
        customer->setFullName(fullName);
        customer->setPhoneNumber(phone);
        customer->setEmail(email);

        while (customer->getLoyaltyPoints() < loyaltyPoints)
        {
            customer->addLoyaltyPoints(1);
        }

        saveUsers();
        return true;
    }
    catch (const exception &e)
    {
        throw;
    }
}

Admin *ParkingManager::getAdmin(const string &adminId)
{
    auto user = users.find([&](const shared_ptr<User> &u)
                           { return u->getUserId() == adminId && u->getRole() == UserRole::ADMIN; });

    if (user != nullptr)
    {
        return dynamic_cast<Admin *>(user->get());
    }
    return nullptr;
}

DoubleLinkedList<shared_ptr<User>> ParkingManager::getAllAdmins()
{
    return users.filter([](const shared_ptr<User> &u)
                        { return u->getRole() == UserRole::ADMIN; });
}

// ========== Vehicle Management ==========

bool ParkingManager::registerVehicle(const string &licensePlate, VehicleType type,
                                     const string &brand, const string &model,
                                     const string &color, const string &customerId,
                                     int engineCapacity, int seatCount,
                                     bool isLuxury, int battery, int maxSpeed)
{
    // Check if license plate already exists
    auto existing = vehicles.find([&](const shared_ptr<Vehicle> &v)
                                  { return v->getLicensePlate() == licensePlate; });

    if (existing != nullptr)
    {
        throw DuplicateException("Bien so xe da ton tai");
    }

    try
    {
        // Tạo ID dễ nhớ dựa trên thông tin khách hàng
        string vehicleId = generateVehicleId(customerId);
        shared_ptr<Vehicle> newVehicle;

        switch (type)
        {
        case VehicleType::MOTORCYCLE:
        {
            int ec = (engineCapacity > 0) ? engineCapacity : 150;
            newVehicle = make_shared<Motorcycle>(vehicleId, licensePlate,
                                                 customerId, brand, model, color, ec);
            break;
        }
        case VehicleType::CAR:
        {
            int sc = (seatCount > 0) ? seatCount : 5;
            newVehicle = make_shared<Car>(vehicleId, licensePlate,
                                          customerId, brand, model, color, sc, isLuxury);
            break;
        }
        case VehicleType::ELECTRIC_BIKE:
        {
            int b = (battery > 0) ? battery : 10000;
            int ms = (maxSpeed > 0) ? maxSpeed : 45;
            newVehicle = make_shared<ElectricBike>(vehicleId, licensePlate,
                                                   customerId, brand, model, color,
                                                   b, ms);
            break;
        }
        default:
            throw invalid_argument("Loai xe khong hop le");
        }

        vehicles.pushBack(newVehicle);
        saveVehicles();

        // In ra Vehicle ID để khách hàng biết
        ui.showInfoMessage("Da tao Vehicle ID: " + vehicleId);

        return true;
    }
    catch (const exception &e)
    {
        throw;
    }
}

shared_ptr<Vehicle> ParkingManager::getVehicle(const string &vehicleId)
{
    auto vehicle = vehicles.find([&](const shared_ptr<Vehicle> &v)
                                 { return v->getVehicleId() == vehicleId; });

    return vehicle ? *vehicle : nullptr;
}

shared_ptr<Vehicle> ParkingManager::getVehicleByPlate(const string &plate)
{
    auto vehicle = vehicles.find([&](const shared_ptr<Vehicle> &v)
                                 { return v->getLicensePlate() == plate; });

    return vehicle ? *vehicle : nullptr;
}

DoubleLinkedList<shared_ptr<Vehicle>> ParkingManager::getVehiclesByCustomer(
    const string &customerId)
{
    return vehicles.filter([&](const shared_ptr<Vehicle> &v)
                           { return v->getCustomerId() == customerId; });
}

DoubleLinkedList<shared_ptr<Vehicle>> ParkingManager::getAllVehicles()
{
    return vehicles;
}

bool ParkingManager::updateVehicle(const string &vehicleId, const string &brand,
                                   const string &model, const string &color)
{
    auto vehicle = getVehicle(vehicleId);
    if (!vehicle)
        return false;

    vehicle->setBrand(brand);
    vehicle->setModel(model);
    vehicle->setColor(color);
    saveVehicles();
    return true;
}

bool ParkingManager::deleteVehicle(const string &vehicleId)
{
    bool removed = vehicles.remove([&](const shared_ptr<Vehicle> &v)
                                   { return v->getVehicleId() == vehicleId; });

    if (removed)
    {
        saveVehicles();
    }
    return removed;
}

// ========== Parking Slot Management ==========
bool ParkingManager::addParkingSlot(const string &slotNumber, VehicleType type)
{
    auto existing = parkingSlots.find([&](const ParkingSlot &s)
                                      { return s.getSlotNumber() == slotNumber; });

    if (existing != nullptr)
    {
        throw DuplicateException("So vi tri da ton tai");
    }

    string slotId = generateSlotId();
    ParkingSlot newSlot(slotId, slotNumber, type);
    parkingSlots.pushBack(newSlot);
    saveSlots();
    return true;
}

ParkingSlot *ParkingManager::getParkingSlot(const string &slotId)
{
    return parkingSlots.find([&](const ParkingSlot &s)
                             { return s.getSlotId() == slotId; });
}

ParkingSlot *ParkingManager::getParkingSlotByNumber(const string &slotNumber)
{
    return parkingSlots.find([&](const ParkingSlot &s)
                             { return s.getSlotNumber() == slotNumber; });
}

DoubleLinkedList<ParkingSlot> ParkingManager::getAvailableSlots(VehicleType type)
{
    return parkingSlots.filter([&](const ParkingSlot &s)
                               { return s.isAvailable() && s.canAccommodate(type); });
}

DoubleLinkedList<ParkingSlot> ParkingManager::getAllSlots()
{
    return parkingSlots;
}

DoubleLinkedList<ParkingSlot> ParkingManager::getAvailableSlotsByType(VehicleType type)
{
    DoubleLinkedList<ParkingSlot> result;
    parkingSlots.forEach([&](const ParkingSlot &slot)
                         {
        if (slot.getSuitableFor() == type && slot.getStatus() == SlotStatus::AVAILABLE)
        {
            result.pushBack(slot);
        } });
    return result;
}

bool ParkingManager::updateSlotStatus(const string &slotId, SlotStatus status)
{
    ParkingSlot *slot = getParkingSlot(slotId);
    if (!slot)
        return false;

    try
    {
        if (status == SlotStatus::MAINTENANCE)
        {
            slot->setMaintenance();
        }
        else
        {
            slot->setStatus(status);
        }
        saveSlots();
        return true;
    }
    catch (const exception &e)
    {
        throw;
    }
}

bool ParkingManager::deleteParkingSlot(const string &slotId)
{
    bool removed = parkingSlots.remove([&](const ParkingSlot &s)
                                       { return s.getSlotId() == slotId; });

    if (removed)
    {
        saveSlots();
    }
    return removed;
}

// ========== Booking Management ==========
bool ParkingManager::createBooking(const string &customerId, const string &vehicleId,
                                   time_t expectedArrival, const string &slotId)
{
    // Verify customer and vehicle exist

    if (!getCustomer(customerId))
        return false;
    auto vehicle = getVehicle(vehicleId);
    if (!vehicle)
        return false;

    ParkingSlot *slotPtr = nullptr;
    if (!slotId.empty())
    {
        slotPtr = getParkingSlot(slotId);
        if (!slotPtr)
            return false;
        if (slotPtr->getStatus() != SlotStatus::AVAILABLE)
            return false;
        if (!slotPtr->canAccommodate(vehicle->getType()))
            return false;
    }
    else
    {
        slotPtr = findAvailableSlot(vehicle->getType());
        if (!slotPtr)
            return false;
    }

    // create booking and attach slotId
    string bookingId = generateBookingId();
    Booking newBooking(bookingId, customerId, vehicleId, expectedArrival);
    newBooking.setSlotId(slotPtr->getSlotId()); // ensure Booking has setSlotId
    newBooking.confirm();

    // reserve slot
    slotPtr->reserve();

    bookings.pushBack(newBooking);
    saveBookings();
    saveSlots();
    return true;
}

Booking *ParkingManager::getBooking(const string &bookingId)
{
    return bookings.find([&](const Booking &b)
                         { return b.getBookingId() == bookingId; });
}

DoubleLinkedList<Booking> ParkingManager::getBookingsByCustomer(const string &customerId)
{
    return bookings.filter([&](const Booking &b)
                           { return b.getCustomerId() == customerId; });
}

DoubleLinkedList<Booking> ParkingManager::getAllBookings()
{
    return bookings;
}

bool ParkingManager::confirmBooking(const string &bookingId)
{
    Booking *booking = getBooking(bookingId);
    if (!booking)
        return false;

    try
    {
        booking->confirm();
        saveBookings();
        return true;
    }
    catch (const exception &e)
    {
        throw;
    }
}

void ParkingManager::cancelBooking()
{
    // KIỂM TRA: Nếu là customer thì chỉ lấy booking của họ
    DoubleLinkedList<Booking> bookingsToShow;

    if (currentUser && currentUser->getRole() == UserRole::CUSTOMER)
    {
        // Khách hàng chỉ thấy booking của mình
        bookingsToShow = getBookingsByCustomer(currentUser->getUserId());
    }
    else
    {
        // Admin thấy tất cả
        bookingsToShow = getAllBookings();
    }

    if (bookingsToShow.empty())
    {
        ui.showInfoMessage("Khong co don dat cho nao.");
        return;
    }

    // Hiển thị danh sách booking
    ui.showReportHeader("DANH SACH DON DAT CHO");
    for (auto it = bookingsToShow.begin(); it != bookingsToShow.end(); ++it)
    {
        it->displayInfo();
    }
    // Người dùng nhập ID
    string bookingId = ui.inputBoxString("Nhap Booking ID can huy (0 de thoat): ");

    if (bookingId == "0")
    {
        ui.showInfoMessage("Da huy thao tac.");
        return;
    }

    Booking *booking = getBooking(bookingId);
    if (!booking)
    {
        ui.showErrorMessage("Khong tim thay don dat cho voi ID: " + bookingId);
        return;
    }

    // KIỂM TRA QUYỀN SỞ HỮU: Customer chỉ được hủy booking của mình
    if (currentUser && currentUser->getRole() == UserRole::CUSTOMER)
    {
        if (booking->getCustomerId() != currentUser->getUserId())
        {
            ui.showErrorMessage("Ban khong co quyen huy booking nay!");
            return;
        }
    }

    // Chặn hủy khi trạng thái không hợp lệ
    if (booking->getStatus() == BookingStatus::CANCELLED)
    {
        ui.showErrorMessage("Don dat cho da bi huy truoc do.");
        return;
    }
    if (booking->getStatus() == BookingStatus::COMPLETED)
    {
        ui.showErrorMessage("Don dat cho da hoan thanh, khong the huy.");
        return;
    }
    // Nếu booking đã liên kết ticket (xe da check-in), không cho hủy
    if (!booking->getTicketId().empty())
    {
        ui.showErrorMessage("Don dat cho da co ticket lien quan, khong the huy.");
        return;
    }

    // Hiển thị chi tiết để xác nhận
    ui.showReportHeader("THONG TIN DON DAT CHO");
    booking->displayInfo();

    // Xác nhận
    string choice = ui.inputBoxString("Ban co chac muon huy don dat cho nay? (y/n): ");

    if (choice == "y" || choice == "Y")
    {
        try
        {
            // Thực hiện hủy trạng thái booking
            booking->cancel();
            saveBookings();

            // Nếu booking giữ 1 slot ở trạng thái RESERVED thì giải phóng slot
            string slotId = booking->getSlotId();
            if (!slotId.empty())
            {
                ParkingSlot *slot = getParkingSlot(slotId);
                if (slot && slot->getStatus() == SlotStatus::RESERVED)
                {
                    slot->setStatus(SlotStatus::AVAILABLE);
                    saveSlots();
                }
            }

            ui.showSuccessMessage("Huy don dat cho thanh cong.");
        }
        catch (const exception &e)
        {
            ui.showErrorMessage("Loi khi huy don dat cho: " + string(e.what()));
        }
    }
    else
    {
        ui.showInfoMessage("Da huy thao tac.");
    }
}

// ========== Ticket Management ==========
ParkingSlot *ParkingManager::findAvailableSlot(VehicleType type)
{
    return parkingSlots.find([&](const ParkingSlot &s)
                             { return s.isAvailable() && s.canAccommodate(type); });
}

string ParkingManager::checkIn(const string &customerId, const string &vehicleId, const string &bookingId, const string &slotId)
{
    Customer *customer = getCustomer(customerId);
    auto vehicle = getVehicle(vehicleId);
    if (!customer || !vehicle)
    {
        throw NotFoundException("Khach hang hoac xe khong ton tai");
    }

    ParkingSlot *slot = nullptr;

    if (!bookingId.empty())
    {
        Booking *booking = getBooking(bookingId);
        if (!booking || booking->getStatus() != BookingStatus::CONFIRMED)
        {
            throw InvalidInputException("Booking khong hop le hoac chua xac nhan");
        }
        slot = getParkingSlot(booking->getSlotId());
        if (!slot || (slot->getStatus() != SlotStatus::AVAILABLE && slot->getStatus() != SlotStatus::RESERVED))
        {
            throw InvalidInputException("Slot trong booking khong hop le");
        }
    }
    else
    {
        if (!slotId.empty())
        {
            slot = getParkingSlot(slotId);
            if (!slot || slot->getStatus() != SlotStatus::AVAILABLE || !slot->canAccommodate(vehicle->getType()))
            {
                throw InvalidInputException("Slot khong hop le hoac da duoc su dung");
            }
        }
        else
        {
            slot = findAvailableSlot(vehicle->getType());
            if (!slot)
            {
                throw NotFoundException("Khong con cho do");
            }
        }
    }

    string ticketId = generateTicketId();
    ParkingTicket newTicket(ticketId, customerId, vehicleId, slot->getSlotId());

    if (!bookingId.empty())
    {
        Booking *booking = getBooking(bookingId);
        newTicket.setBookingId(bookingId);
        booking->setTicketId(ticketId);
        booking->complete();
        saveBookings();
    }

    slot->occupy(ticketId);
    tickets.pushBack(newTicket);
    saveSlots();
    saveTickets();

    return ticketId;
}

bool ParkingManager::checkOut(const string &ticketId)
{
    ParkingTicket *ticket = getTicket(ticketId);
    if (!ticket || !ticket->isActive())
    {
        throw NotFoundException("Ticket khong ton tai hoac da thanh toan");
    }

    // Get vehicle to calculate fee
    auto vehicle = getVehicle(ticket->getVehicleId());
    if (!vehicle)
    {
        throw NotFoundException("Khong tim thay thong tin xe");
    }

    // Calculate fee
    long long duration = ticket->getParkingDuration();
    double fee = vehicle->calculateParkingFee(duration);

    // Get customer and deduct balance
    Customer *customer = getCustomer(ticket->getCustomerId());
    if (!customer)
    {
        throw NotFoundException("Khong tim thay khach hang");
    }

    try
    {
        customer->addLoyaltyPoints(static_cast<int>(fee / 1000)); // 1 point per 1000 VND

        // Update ticket
        ticket->checkOut(fee);

        // Release slot
        ParkingSlot *slot = getParkingSlot(ticket->getSlotId());
        if (slot)
        {
            slot->release();
        }

        saveUsers();
        saveSlots();
        saveTickets();

        return true;
    }
    catch (const exception &e)
    {
        throw;
    }
}

ParkingTicket *ParkingManager::getTicket(const string &ticketId)
{
    return tickets.find([&](const ParkingTicket &t)
                        { return t.getTicketId() == ticketId; });
}

DoubleLinkedList<ParkingTicket> ParkingManager::getAllTickets()
{
    return tickets;
}

DoubleLinkedList<ParkingTicket> ParkingManager::getTicketsByCustomer(const string &customerId)
{
    return tickets.filter([&](const ParkingTicket &t)
                          { return t.getCustomerId() == customerId; });
}

DoubleLinkedList<ParkingTicket> ParkingManager::getTicketsByVehicle(const string &vehicleId)
{
    return tickets.filter([&](const ParkingTicket &t)
                          { return t.getVehicleId() == vehicleId; });
}

DoubleLinkedList<ParkingTicket> ParkingManager::getTicketsByStatus(
    TicketStatus status)
{

    return tickets.filter([&](const ParkingTicket &t)
                          { return t.getStatus() == status; });
}

void ParkingManager::displayTicketDetail(ParkingManager &manager, const string &ticketId)
{
    ParkingTicket *ticket = manager.getTicket(ticketId);
    if (!ticket)
    {
        ui.showErrorMessage("Khong tim thay ve voi ID: " + ticketId);
        return;
    }

    cout << endl;

    // ===== HEADER =====
    ui.printHorizontalLine('+', '-', '+');
    ui.printEmptyLine();
    ui.printCenteredText("THONG TIN CHI TIET VE", Color::CYAN);
    ui.printEmptyLine();
    ui.printHorizontalLine('+', '-', '+');

    // ===== THONG TIN VE =====
    ui.printRow("  Ticket ID:     ", ticket->getTicketId());
    ui.printRow("  Trang thai:    ", ParkingTicket::statusToString(ticket->getStatus()));
    ui.printHorizontalLine('+', '-', '+');

    // ===== KHACH HANG =====
    if (auto customer = manager.getCustomer(ticket->getCustomerId()))
    {
        ui.printCenteredText("KHACH HANG", Color::CYAN);
        ui.printRow("  ID:            ", customer->getUserId());
        ui.printRow("  Ho ten:        ", customer->getFullName());
        ui.printRow("  So dien thoai: ", customer->getPhoneNumber());
        ui.printHorizontalLine('+', '-', '+');
    }

    // ===== XE =====
    if (auto vehicle = manager.getVehicle(ticket->getVehicleId()))
    {
        ui.printCenteredText("THONG TIN XE", Color::CYAN);
        ui.printRow("  Bien so:       ", vehicle->getLicensePlate());
        ui.printRow("  Loai xe:       ", vehicle->getTypeString());
        ui.printRow("  Hang:          ", vehicle->getBrand() + " " + vehicle->getModel());
        ui.printHorizontalLine('+', '-', '+');
    }

    // ===== CHO DO =====
    if (auto slot = manager.getParkingSlot(ticket->getSlotId()))
    {
        ui.printCenteredText("VI TRI DO", Color::CYAN);
        ui.printRow("  So vi tri:     ", slot->getSlotNumber());
        ui.printRow("  Khu vuc:       ", slot->getSlotNumber().substr(0, 1));
        ui.printHorizontalLine('+', '-', '+');
    }

    // ===== THOI GIAN =====
    ui.printCenteredText("THOI GIAN", Color::CYAN);
    ui.printRow("  Check-in:      ", Utils::timeToString(ticket->getCheckInTime()));

    auto vehicle = manager.getVehicle(ticket->getVehicleId());

    if (ticket->getStatus() == TicketStatus::ACTIVE)
    {
        long long mins = ticket->getParkingDuration();
        ui.printRow("  Thoi gian gui: ", to_string(mins) + " phut (dang tinh...)");

        if (vehicle)
        {
            double fee = vehicle->calculateParkingFee(mins);
            ostringstream oss;
            oss << fixed << setprecision(0) << "~" << fee << " VND";
            ui.printRow("  Phi du kien:   ", oss.str());
        }
    }
    else if (ticket->getStatus() == TicketStatus::PAID)
    {
        ui.printRow("  Check-out:     ", Utils::timeToString(ticket->getCheckOutTime()));
        ui.printRow("  Thoi gian gui: ", to_string(ticket->getParkingDuration()) + " phut");

        ui.printHorizontalLine('+', '-', '+');
        ui.printCenteredText("PHI GUI XE", Color::CYAN);

        ostringstream oss;
        oss << fixed << setprecision(0) << ticket->getFee() << " VND";
        ui.printRow("  Tong cong:     ", oss.str());
    }

    ui.printHorizontalLine('+', '-', '+');
}

void ParkingManager::advancedTicketSearch(ParkingManager &manager)
{
    while (true)
    {
        Utils::clearScreen();
        ui.showTicketSearchMenu();

        int choice = ui.inputBoxInt("Nhap lua chon: ");
        if (choice > 4)
        {
            ui.showErrorMessage("Lua chon khong hop le. Vui long thu lai.");
            continue;
        }

        DoubleLinkedList<ParkingTicket> results;

        switch (choice)
        {
        case 1:
        {
            Utils::clearScreen();
            string ticketId = ui.inputBoxString("Nhap Ticket ID: ");
            displayTicketDetail(manager, ticketId);
            Utils::pause();
            continue;
        }
        case 2:
        {
            Utils::clearScreen();
            string plate = ui.inputBoxString("Nhap bien so xe: ");

            auto vehicle = manager.getVehicleByPlate(plate);
            if (!vehicle)
            {
                ui.showErrorMessage("Khong tim thay xe voi bien so: " + plate);
                Utils::pause();
                continue;
            }

            results = manager.getTicketsByVehicle(vehicle->getVehicleId());
            break;
        }
        case 3:
        {
            Utils::clearScreen();
            string customerId = ui.inputBoxString("Nhap Customer ID: ");

            if (!manager.getCustomer(customerId))
            {
                ui.showErrorMessage("Khong tim thay khach hang voi ID: " + customerId);
                Utils::pause();
                continue;
            }

            results = manager.getTicketsByCustomer(customerId);
            break;
        }
        case 4:
        {
            Utils::clearScreen();
            ui.showStatusSelectMenu();
            int statusChoice = ui.inputBoxInt("Nhap lua chon: ");

            TicketStatus status;
            switch (statusChoice)
            {
            case 1:
                status = TicketStatus::ACTIVE;
                break;
            case 2:
                status = TicketStatus::PAID;
                break;
            case 3:
                status = TicketStatus::CANCELLED;
                break;
            default:
                ui.showErrorMessage("Lua chon trang thai khong hop le.");
                Utils::pause();
                continue;
            }

            results = manager.getTicketsByStatus(status);
            if (results.empty())
            {
                ui.showErrorMessage("Khong tim thay ve voi trang thai duoc chon.");
                Utils::pause();
                continue;
            }
            break;
        }
        case 0:
            return;
        }

        // Hiển thị kết quả
        ui.showReportHeader("KET QUA TIM KIEM");
        ui.showInfoMessage("Tim thay: " + to_string(results.size()) + " ve\n\n");

        if (results.empty())
        {
            ui.showErrorMessage("Khong co ket qua.\n");
        }
        else
        {
            int count = 1;
            ui.printHorizontalLine('+', '-', '+');
            for (auto &t : results)
            {
                ostringstream val;
                // Ticket id + các thông tin bổ sung
                val << t.getTicketId();

                auto vehicle = manager.getVehicle(t.getVehicleId());
                if (vehicle)
                    val << " | Bien so: " << vehicle->getLicensePlate();

                val << " | " << Utils::timeToString(t.getCheckInTime());
                val << " | " << ParkingTicket::statusToString(t.getStatus());

                if (t.getStatus() == TicketStatus::PAID)
                {
                    ostringstream feeStr;
                    feeStr << fixed << setprecision(0) << t.getFee() << " VND";
                    val << " | " << feeStr.str();
                }

                ui.printRow("          |  " + to_string(count++) + ". ID: ", val.str());
            }

            ui.printHorizontalLine('+', '-', '+');
            Utils::pause();
        }
    }
}

void ParkingManager::adminPriceManagement(ParkingManager &manager)
{
    PricingConfig *pricing = PricingConfig::getInstance();

    while (true)
    {
        Utils::clearScreen();
        ui.showReportHeader("QUAN LY GIA VE");

        // Hiển thị bảng giá hiện tại
        pricing->displayPricing();

        ui.showPriceChangeMenu();
        int choice = ui.inputBoxInt("Nhap lua chon: ");

        if (choice == 0)
            return;

        try
        {
            double newPrice;
            int newMinutes;
            std::string confirm;

            switch (choice)
            {
            case 1:
            {
                double old = pricing->getMotorcyclePrice();
                newPrice = ui.inputBoxDouble("Nhap gia moi cho xe may (VND/gio): ");
                confirm = ui.inputBoxString("Ban co chac muon cap nhat gia xe may tu " + std::to_string((long long)old) + " -> " + std::to_string((long long)newPrice) + " ? (y/n): ");
                if (confirm == "y" || confirm == "Y")
                {
                    pricing->setMotorcyclePrice(newPrice);
                    ui.showSuccessMessage("Cap nhat gia thanh cong!");
                }
                else
                {
                    ui.showInfoMessage("Da huy thao tac.");
                }
                break;
            }

            case 2:
            {
                double old = pricing->getCarStandardPrice();
                newPrice = ui.inputBoxDouble("Nhap gia moi cho o to thuong (VND/gio): ");
                confirm = ui.inputBoxString("Ban co chac muon cap nhat gia o to thuong tu " + std::to_string((long long)old) + " -> " + std::to_string((long long)newPrice) + " ? (y/n): ");
                if (confirm == "y" || confirm == "Y")
                {
                    pricing->setCarStandardPrice(newPrice);
                    ui.showSuccessMessage("Cap nhat gia thanh cong!");
                }
                else
                {
                    ui.showInfoMessage("Da huy thao tac.");
                }
                break;
            }

            case 3:
            {
                double old = pricing->getCarLuxuryPrice();
                newPrice = ui.inputBoxDouble("Nhap gia moi cho o to sang (VND/gio): ");
                confirm = ui.inputBoxString("Ban co chac muon cap nhat gia o to sang tu " + std::to_string((long long)old) + " -> " + std::to_string((long long)newPrice) + " ? (y/n): ");
                if (confirm == "y" || confirm == "Y")
                {
                    pricing->setCarLuxuryPrice(newPrice);
                    ui.showSuccessMessage("Cap nhat gia thanh cong!");
                }
                else
                {
                    ui.showInfoMessage("Da huy thao tac.");
                }
                break;
            }

            case 4:
            {
                double old = pricing->getElectricBikePrice();
                newPrice = ui.inputBoxDouble("Nhap gia moi cho xe dap dien (VND/gio): ");
                confirm = ui.inputBoxString("Ban co chac muon cap nhat gia xe dap dien tu " + std::to_string((long long)old) + " -> " + std::to_string((long long)newPrice) + " ? (y/n): ");
                if (confirm == "y" || confirm == "Y")
                {
                    pricing->setElectricBikePrice(newPrice);
                    ui.showSuccessMessage("Cap nhat gia thanh cong!");
                }
                else
                {
                    ui.showInfoMessage("Da huy thao tac.");
                }
                break;
            }

            case 5:
            {
                int old = pricing->getMinimumMinutes();
                newMinutes = ui.inputBoxInt("Nhap thoi gian toi thieu moi (phut): ");
                confirm = ui.inputBoxString("Ban co chac muon cap nhat thoi gian toi thieu tu " + std::to_string(old) + " -> " + std::to_string(newMinutes) + " phut? (y/n): ");
                if (confirm == "y" || confirm == "Y")
                {
                    pricing->setMinimumMinutes(newMinutes);
                    ui.showSuccessMessage("Cap nhat gia thanh cong!");
                }
                else
                {
                    ui.showInfoMessage("Da huy thao tac.");
                }
                break;
            }

            default:
                ui.showErrorMessage("Lua chon khong hop le!");
            }
        }
        catch (const std::exception &e)
        {
            ui.showErrorMessage(std::string("Loi: ") + e.what());
        }
        Utils::pause();
    }
}

void ParkingManager::customerViewPricing()
{
    Utils::clearScreen();
    PricingConfig *pricing = PricingConfig::getInstance();
    pricing->displayPricing();

    ui.printHorizontalLine('+', '-', '+');
    ui.printRow("          | ", "                                    LUU Y                                          ");
    ui.printHorizontalLine('+', '-', '+');
    ui.printRow("          | ", " - Phi duoc tinh theo gio, lam tron len                                            ");
    ui.printRow("          | ", " - Thoi gian toi thieu: " + to_string(pricing->getMinimumMinutes()) + " phut");
    ui.printRow("          | ", " - Vi du: Gui 45 phut = 1 gio                                                      ");
    ui.printHorizontalLine('+', '-', '+');
}

// ========== Search & Filter ==========
DoubleLinkedList<shared_ptr<User>> ParkingManager::searchUsers(const string &keyword)
{
    string lowerKeyword = Utils::toLower(keyword);
    return users.filter([&](const shared_ptr<User> &u)
                        { return Utils::toLower(u->getFullName()).find(lowerKeyword) != string::npos ||
                                 Utils::toLower(u->getUsername()).find(lowerKeyword) != string::npos ||
                                 u->getPhoneNumber().find(keyword) != string::npos ||
                                 Utils::toLower(u->getEmail()).find(lowerKeyword) != string::npos; });
}

DoubleLinkedList<shared_ptr<Vehicle>> ParkingManager::searchVehicles(const string &keyword)
{
    string lowerKeyword = Utils::toLower(keyword);
    return vehicles.filter([&](const shared_ptr<Vehicle> &v)
                           { return Utils::toLower(v->getLicensePlate()).find(lowerKeyword) != string::npos ||
                                    Utils::toLower(v->getBrand()).find(lowerKeyword) != string::npos ||
                                    Utils::toLower(v->getModel()).find(lowerKeyword) != string::npos; });
}

DoubleLinkedList<ParkingTicket> ParkingManager::searchTickets(const string &keyword)
{
    string lowerKeyword = Utils::toLower(keyword);
    return tickets.filter([&](const ParkingTicket &t)
                          { return t.getTicketId().find(keyword) != string::npos ||
                                   t.getCustomerId().find(keyword) != string::npos ||
                                   t.getVehicleId().find(keyword) != string::npos; });
}

// ========== Sort ==========
void ParkingManager::sortUsersByName()
{
    users.sort([](const shared_ptr<User> &a, const shared_ptr<User> &b)
               { return a->getFullName() < b->getFullName(); });
}

void ParkingManager::sortVehiclesByPlate()
{
    vehicles.sort([](const shared_ptr<Vehicle> &a, const shared_ptr<Vehicle> &b)
                  { return a->getLicensePlate() < b->getLicensePlate(); });
}

// ========== Statistics & Reports ==========
int ParkingManager::getTotalSlots() const
{
    return parkingSlots.size();
}

int ParkingManager::getAvailableSlotCount(VehicleType type) const
{
    int count = 0;
    for (auto it = parkingSlots.begin(); it != parkingSlots.end(); ++it)
    {
        if (it->isAvailable() && it->canAccommodate(type))
        {
            count++;
        }
    }
    return count;
}

int ParkingManager::getOccupiedSlotCount() const
{
    int count = 0;
    for (auto it = parkingSlots.begin(); it != parkingSlots.end(); ++it)
    {
        if (it->getStatus() == SlotStatus::OCCUPIED)
        {
            count++;
        }
    }
    return count;
}

double ParkingManager::getTotalRevenue() const
{
    double total = 0.0;
    for (auto it = tickets.begin(); it != tickets.end(); ++it)
    {
        if (it->getStatus() == TicketStatus::PAID)
        {
            total += it->getFee();
        }
    }
    return total;
}

double ParkingManager::getRevenueByPeriod(time_t startTime, time_t endTime) const
{
    double total = 0.0;
    for (auto it = tickets.begin(); it != tickets.end(); ++it)
    {
        if (it->getStatus() == TicketStatus::PAID &&
            it->getCheckOutTime() >= startTime &&
            it->getCheckOutTime() <= endTime)
        {
            total += it->getFee();
        }
    }
    return total;
}

void ParkingManager::generateDailyReport() const
{
    int boxWidth = 90;
    int leftPadding = 10;

    auto printLine = [&](char left, char mid, char right)
    {
        cout << string(leftPadding, ' ') << left;
        for (int i = 0; i < boxWidth - 2; i++)
            cout << mid;
        cout << right << endl;
    };

    auto printEmpty = [&]()
    {
        cout << string(leftPadding, ' ') << "|" << string(boxWidth - 2, ' ') << "|" << endl;
    };

    auto printCenter = [&](const string &text, bool cyan = false)
    {
        int pad = (boxWidth - 2 - text.length()) / 2;
        int padR = boxWidth - 2 - text.length() - pad;
        cout << string(leftPadding, ' ') << "|" << string(pad, ' ');
        if (cyan)
            cout << "\033[36m";
        cout << text;
        if (cyan)
            cout << "\033[0m";
        cout << string(padR, ' ') << "|" << endl;
    };

    auto printLeft = [&](const string &label, const string &value)
    {
        string content = label + value;
        int padR = boxWidth - 3 - content.length();
        if (padR < 0)
            padR = 0;
        cout << string(leftPadding, ' ') << "| " << content << string(padR, ' ') << "|" << endl;
    };

    cout << endl;
    printLine('+', '-', '+');
    printEmpty();
    printCenter("BAO CAO NGAY " + Utils::getCurrentDateTime(), true);
    printEmpty();
    printLine('+', '-', '+');

    // Slot statistics
    printCenter("THONG KE CHO DO", true);
    printLeft("  Tong so cho:         ", to_string(getTotalSlots()));
    printLeft("  Dang su dung:        ", to_string(getOccupiedSlotCount()));
    printLeft("  Trong (Xe may):      ", to_string(getAvailableSlotCount(VehicleType::MOTORCYCLE)));
    printLeft("  Trong (O to):        ", to_string(getAvailableSlotCount(VehicleType::CAR)));
    printLeft("  Trong (Xe dap dien): ", to_string(getAvailableSlotCount(VehicleType::ELECTRIC_BIKE)));
    printLine('+', '-', '+');

    // Revenue
    time_t now = time(nullptr);
    time_t startOfDay = now - (now % 86400);
    double dailyRevenue = getRevenueByPeriod(startOfDay, now);

    printCenter("DOANH THU", true);
    ostringstream oss1, oss2;
    oss1 << fixed << setprecision(0) << dailyRevenue << " VND";
    oss2 << fixed << setprecision(0) << getTotalRevenue() << " VND";
    printLeft("  Hom nay:             ", oss1.str());
    printLeft("  Tong doanh thu:      ", oss2.str());
    printLine('+', '-', '+');

    // Active tickets
    int activeCount = 0;
    for (auto it = tickets.begin(); it != tickets.end(); ++it)
    {
        if (it->isActive())
            activeCount++;
    }

    printCenter("VE HIEN TAI", true);
    printLeft("  Dang hoat dong:      ", to_string(activeCount));
    printLeft("  Tong da tao:         ", to_string(tickets.size()));
    printLine('+', '-', '+');
}

void ParkingManager::generateMonthlyReport() const
{
    int boxWidth = 90;
    int leftPadding = 10;

    auto printLine = [&](char left, char mid, char right)
    {
        cout << string(leftPadding, ' ') << left;
        for (int i = 0; i < boxWidth - 2; i++)
            cout << mid;
        cout << right << endl;
    };

    auto printEmpty = [&]()
    {
        cout << string(leftPadding, ' ') << "|" << string(boxWidth - 2, ' ') << "|" << endl;
    };

    auto printCenter = [&](const string &text, bool cyan = false)
    {
        int pad = (boxWidth - 2 - text.length()) / 2;
        int padR = boxWidth - 2 - text.length() - pad;
        cout << string(leftPadding, ' ') << "|" << string(pad, ' ');
        if (cyan)
            cout << "\033[36m";
        cout << text;
        if (cyan)
            cout << "\033[0m";
        cout << string(padR, ' ') << "|" << endl;
    };

    auto printLeft = [&](const string &label, const string &value)
    {
        string content = label + value;
        int padR = boxWidth - 3 - content.length();
        if (padR < 0)
            padR = 0;
        cout << string(leftPadding, ' ') << "| " << content << string(padR, ' ') << "|" << endl;
    };

    time_t now = time(nullptr);
    struct tm *timeinfo = localtime(&now);
    timeinfo->tm_mday = 1;
    timeinfo->tm_hour = 0;
    timeinfo->tm_min = 0;
    timeinfo->tm_sec = 0;
    time_t startOfMonth = mktime(timeinfo);

    double monthlyRevenue = getRevenueByPeriod(startOfMonth, now);

    cout << endl;
    printLine('+', '-', '+');
    printEmpty();
    printCenter("BAO CAO THANG", true);
    printEmpty();
    printLine('+', '-', '+');

    ostringstream oss;
    oss << fixed << setprecision(0) << monthlyRevenue << " VND";
    printLeft("  Doanh thu thang nay: ", oss.str());
    printLine('+', '-', '+');

    // Count tickets by type
    int motorcycleTickets = 0, carTickets = 0, electricBikeTickets = 0;

    for (auto it = tickets.begin(); it != tickets.end(); ++it)
    {
        if (it->getCheckInTime() >= startOfMonth)
        {
            auto vehicle = const_cast<ParkingManager *>(this)->getVehicle(it->getVehicleId());
            if (vehicle)
            {
                switch (vehicle->getType())
                {
                case VehicleType::MOTORCYCLE:
                    motorcycleTickets++;
                    break;
                case VehicleType::CAR:
                    carTickets++;
                    break;
                case VehicleType::ELECTRIC_BIKE:
                    electricBikeTickets++;
                    break;
                }
            }
        }
    }

    printCenter("THONG KE THEO LOAI XE", true);
    printLeft("  Xe may:              ", to_string(motorcycleTickets) + " luot");
    printLeft("  O to:                ", to_string(carTickets) + " luot");
    printLeft("  Xe dap dien:         ", to_string(electricBikeTickets) + " luot");
    printLine('+', '-', '+');
}

// ========== File Operations ==========
void ParkingManager::saveAllData()
{
    saveUsers();
    saveVehicles();
    saveSlots();
    saveBookings();
    saveTickets();
}

void ParkingManager::loadAllData()
{
    loadUsers();
    loadVehicles();
    loadSlots();
    loadBookings();
    loadTickets();
}

void ParkingManager::loadUsers()
{
    users.clear();

    {
        ifstream inCust(CUSTOMERS_FILE);
        string line;
        if (inCust.is_open())
        {
            while (getline(inCust, line))
            {
                if (line.empty())
                    continue;
                try
                {
                    auto c = make_shared<Customer>();
                    c->fromFileString(line);
                    users.pushBack(c);
                }
                catch (const exception &e)
                {
                    cerr << "Loi khi doc customer: " << e.what() << endl;
                }
            }
            inCust.close();
        }
    }

    {
        ifstream inAdmin(ADMINS_FILE);
        string line;
        if (inAdmin.is_open())
        {
            while (getline(inAdmin, line))
            {
                if (line.empty())
                    continue;
                try
                {
                    auto a = make_shared<Admin>();
                    a->fromFileString(line);
                    users.pushBack(a);
                }
                catch (const exception &e)
                {
                    cerr << "Loi khi doc admin: " << e.what() << endl;
                }
            }
            inAdmin.close();
        }
    }
}

void ParkingManager::saveUsers()
{
    ofstream outCust(CUSTOMERS_FILE, ios::trunc);
    ofstream outAdmin(ADMINS_FILE, ios::trunc);

    if (!outCust.is_open() || !outAdmin.is_open())
    {
        throw runtime_error("Khong the mo file users (customers/admins)");
    }

    for (auto it = users.begin(); it != users.end(); ++it)
    {
        auto u = *it;
        if (!u)
            continue;
        if (u->getRole() == UserRole::CUSTOMER)
        {
            auto c = dynamic_pointer_cast<Customer>(u);
            if (c)
                outCust << c->toFileString() << '\n';
        }
        else if (u->getRole() == UserRole::ADMIN)
        {
            auto a = dynamic_pointer_cast<Admin>(u);
            if (a)
                outAdmin << a->toFileString() << '\n';
        }
    }

    outCust.close();
    outAdmin.close();
}

void ParkingManager::loadVehicles()
{
    ifstream file(VEHICLES_FILE);
    if (!file.is_open())
        return;

    string line;
    while (getline(file, line))
    {
        if (line.empty())
            continue;

        try
        {
            istringstream iss(line);
            vector<string> fields;
            string field;

            while (getline(iss, field, '|'))
            {
                fields.push_back(field);
                if (fields.size() == 4)
                    break;
            }

            if (fields.size() < 4)
                continue;

            VehicleType type = Vehicle::stringToVehicleType(fields[3]);
            shared_ptr<Vehicle> vehicle;

            switch (type)
            {
            case VehicleType::MOTORCYCLE:
                vehicle = make_shared<Motorcycle>();
                break;
            case VehicleType::CAR:
                vehicle = make_shared<Car>();
                break;
            case VehicleType::ELECTRIC_BIKE:
                vehicle = make_shared<ElectricBike>();
                break;
            }

            vehicle->fromFileString(line);
            vehicles.pushBack(vehicle);
        }
        catch (const exception &e)
        {
            cerr << "Loi khi doc vehicle: " << e.what() << endl;
        }
    }
    file.close();
}

void ParkingManager::saveVehicles()
{
    ofstream file(VEHICLES_FILE);
    if (!file.is_open())
    {
        throw runtime_error("Khong the mo file vehicles.txt");
    }

    for (auto it = vehicles.begin(); it != vehicles.end(); ++it)
    {
        file << (*it)->toFileString() << endl;
    }
    file.close();
}

void ParkingManager::loadSlots()
{
    ifstream file(SLOTS_FILE);
    if (!file.is_open())
        return;

    string line;
    while (getline(file, line))
    {
        if (line.empty())
            continue;

        try
        {
            ParkingSlot slot;
            slot.fromFileString(line);
            parkingSlots.pushBack(slot);
        }
        catch (const exception &e)
        {
            cerr << "Loi khi doc slot: " << e.what() << endl;
        }
    }
    file.close();
}

void ParkingManager::saveSlots()
{
    ofstream file(SLOTS_FILE);
    if (!file.is_open())
    {
        throw runtime_error("Khong the mo file slots.txt");
    }

    for (auto it = parkingSlots.begin(); it != parkingSlots.end(); ++it)
    {
        file << it->toFileString() << endl;
    }
    file.close();
}

void ParkingManager::loadBookings()
{
    ifstream file(BOOKINGS_FILE);
    if (!file.is_open())
        return;

    string line;
    while (getline(file, line))
    {
        if (line.empty())
            continue;

        try
        {
            Booking booking;
            booking.fromFileString(line);
            bookings.pushBack(booking);
        }
        catch (const exception &e)
        {
            cerr << "Loi khi doc booking: " << e.what() << endl;
        }
    }
    file.close();
}

void ParkingManager::saveBookings()
{
    ofstream file(BOOKINGS_FILE);
    if (!file.is_open())
    {
        throw runtime_error("Khong the mo file bookings.txt");
    }

    for (auto it = bookings.begin(); it != bookings.end(); ++it)
    {
        file << it->toFileString() << endl;
    }
    file.close();
}

void ParkingManager::loadTickets()
{
    ifstream file(TICKETS_FILE);
    if (!file.is_open())
        return;

    string line;
    while (getline(file, line))
    {
        if (line.empty())
            continue;

        try
        {
            ParkingTicket ticket;
            ticket.fromFileString(line);
            tickets.pushBack(ticket);
        }
        catch (const exception &e)
        {
            cerr << "Loi khi doc ticket: " << e.what() << endl;
        }
    }
    file.close();
}

void ParkingManager::saveTickets()
{
    ofstream file(TICKETS_FILE);
    if (!file.is_open())
    {
        throw runtime_error("Khong the mo file tickets.txt");
    }

    for (auto it = tickets.begin(); it != tickets.end(); ++it)
    {
        file << it->toFileString() << endl;
    }
    file.close();
}

// ========== Helper Functions ==========

string ParkingManager::generateUserId()
{
    return Utils::generateID("U");
}

string ParkingManager::generateVehicleId(const string &customerId)
{
    // Lấy thông tin khách hàng
    Customer *customer = getCustomer(customerId);
    if (!customer)
    {
        // Fallback nếu không tìm thấy customer
        return Utils::generateID("V");
    }

    // Đếm số xe hiện tại của khách hàng
    int vehicleCount = 0;
    for (auto it = vehicles.begin(); it != vehicles.end(); ++it)
    {
        if ((*it)->getCustomerId() == customerId)
        {
            vehicleCount++;
        }
    }
    vehicleCount++; // Xe mới này

    // Tạo ID dễ nhớ
    return Utils::generateReadableVehicleID(
        customer->getFullName(),
        customer->getPhoneNumber(),
        vehicleCount);
}

string ParkingManager::generateSlotId()
{
    return Utils::generateID("S");
}

string ParkingManager::generateBookingId()
{
    return Utils::generateID("B");
}

string ParkingManager::generateTicketId()
{
    return Utils::generateID("T");
}