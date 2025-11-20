#include "ParkingManager.h"
#include "Utils.h"
#include "Exceptions.h"
#include "PricingConfig.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <limits>
using namespace std;

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
                                  const string &email, UserRole role)
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
            string employeeID = "EMP" + userId.substr(3);

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

        cout << "✓ Da tao Customer ID: " << userId << endl;
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
                                     const string &color, const string &customerId)
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
            newVehicle = make_shared<Motorcycle>(vehicleId, licensePlate,
                                                 customerId, brand, model, color, 150);
            break;
        case VehicleType::CAR:
            newVehicle = make_shared<Car>(vehicleId, licensePlate,
                                          customerId, brand, model, color, 5, false);
            break;
        case VehicleType::ELECTRIC_BIKE:
            newVehicle = make_shared<ElectricBike>(vehicleId, licensePlate,
                                                   customerId, brand, model, color,
                                                   10000, 45);
            break;
        }

        vehicles.pushBack(newVehicle);
        saveVehicles();

        // In ra Vehicle ID để khách hàng biết
        cout << "Da tao Vehicle ID: " << vehicleId << endl;

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
    // Lấy tất cả booking
    auto allBookings = getAllBookings();
    if (allBookings.empty())
    {
        cout << "Khong có don dat cho nao.\n";
        return;
    }

    // Hiển thị danh sách booking
    cout << "Danh sach tat ca don dat cho:\n";
    for (const auto &booking : allBookings)
    {
        cout << "-------------------------\n";
        booking.displayInfo();
    }

    // Người dùng nhập ID
    cout << "\nNhap ID don dat cho ban muon huy: ";
    string bookingId;
    cin >> bookingId;

    Booking *booking = getBooking(bookingId);
    if (!booking)
    {
        cout << "Khong tim thay don dat cho voi ID: " << bookingId << endl;
        return;
    }

    // Hiển thị chi tiết booking để xác nhận
    cout << "\nThông tin booking bạn chọn:\n";
    booking->displayInfo();

    // Xác nhận
    cout << "\nBạn có chắc chắn muốn hủy booking này? (y/n): ";
    char choice;
    cin >> choice;

    if (choice == 'y' || choice == 'Y')
    {
        try
        {
            booking->cancel();
            saveBookings();
            cout << "Hủy booking thành công.\n";
        }
        catch (const exception &e)
        {
            cout << "Lỗi khi hủy booking: " << e.what() << endl;
        }
    }
    else
    {
        cout << "Đã hủy thao tác.\n";
    }
}

// ========== Ticket Management ==========
ParkingSlot *ParkingManager::findAvailableSlot(VehicleType type)
{
    return parkingSlots.find([&](const ParkingSlot &s)
                             { return s.isAvailable() && s.canAccommodate(type); });
}

string ParkingManager::checkIn(const string &customerId, const string &vehicleId,
                               const string &bookingId)
{
    // Verify customer and vehicle
    Customer *customer = getCustomer(customerId);
    auto vehicle = getVehicle(vehicleId);

    if (!customer || !vehicle)
    {
        throw NotFoundException("Khach hang hoac xe khong ton tai");
    }

    // Find available slot
    ParkingSlot *slot = findAvailableSlot(vehicle->getType());
    if (!slot)
    {
        throw NotFoundException("Khong con cho do");
    }

    // Create ticket
    string ticketId = generateTicketId();
    ParkingTicket newTicket(ticketId, customerId, vehicleId, slot->getSlotId());

    // If has booking, link it
    if (!bookingId.empty())
    {
        Booking *booking = getBooking(bookingId);
        if (booking)
        {
            newTicket.setBookingId(bookingId);
            booking->setTicketId(ticketId);
            booking->complete();
            saveBookings();
        }
    }

    // Occupy slot
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
        cout << "Khong tim thay ticket!\n";
        return;
    }

    cout << "\n╔════════════════════════════════════════════╗\n";
    cout << "║         THONG TIN CHI TIET VE             ║\n";
    cout << "╚════════════════════════════════════════════╝\n\n";

    // Thông tin ticket
    cout << "🎫 THONG TIN VE:\n";
    cout << "   Ticket ID:      " << ticket->getTicketId() << "\n";
    cout << "   Trang thai:     " << ParkingTicket::statusToString(ticket->getStatus()) << "\n";

    // Thông tin khách hàng
    cout << "\n👤 KHACH HANG:\n";
    Customer *customer = manager.getCustomer(ticket->getCustomerId());
    if (customer)
    {
        cout << "   ID:             " << customer->getUserId() << "\n";
        cout << "   Ho ten:         " << customer->getFullName() << "\n";
        cout << "   So dien thoai:  " << customer->getPhoneNumber() << "\n";
    }

    // Thông tin xe
    cout << "\n🚗 THONG TIN XE:\n";
    auto vehicle = manager.getVehicle(ticket->getVehicleId());
    if (vehicle)
    {
        cout << "   Bien so:        " << vehicle->getLicensePlate() << "\n";
        cout << "   Loai xe:        " << vehicle->getTypeString() << "\n";
        cout << "   Hang:           " << vehicle->getBrand() << " " << vehicle->getModel() << "\n";
    }

    // Thông tin chỗ đỗ
    cout << "\n🅿 VI TRI DO:\n";
    auto slot = manager.getParkingSlot(ticket->getSlotId());
    if (slot)
    {
        cout << "   So vi tri:      " << slot->getSlotNumber() << "\n";
        cout << "   Khu vuc:        " << slot->getSlotNumber().substr(0, 1) << "\n";
    }

    // Thông tin booking (nếu có)
    if (!ticket->getBookingId().empty())
    {
        cout << "\n📅 BOOKING:\n";
        Booking *booking = manager.getBooking(ticket->getBookingId());
        if (booking)
        {
            cout << "   Booking ID:     " << booking->getBookingId() << "\n";
            cout << "   Thoi gian dat:  " << Utils::timeToString(booking->getBookingTime()) << "\n";
        }
    }

    // Thông tin thời gian và phí
    cout << "\n⏰ THOI GIAN:\n";
    cout << "   Check-in:       " << Utils::timeToString(ticket->getCheckInTime()) << "\n";

    if (ticket->getStatus() == TicketStatus::ACTIVE)
    {
        long long currentDuration = ticket->getParkingDuration();
        cout << "   Thoi gian gui:  " << currentDuration << " phut (dang tinh...)\n";

        // Tính phí dự kiến
        if (vehicle)
        {
            double estimatedFee = vehicle->calculateParkingFee(currentDuration);
            cout << "   Phi du kien:    ~" << fixed << setprecision(0)
                 << estimatedFee << " VND\n";
        }
    }
    else if (ticket->getStatus() == TicketStatus::PAID)
    {
        cout << "   Check-out:      " << Utils::timeToString(ticket->getCheckOutTime()) << "\n";
        cout << "   Thoi gian gui:  " << ticket->getParkingDuration() << " phut\n";
        cout << "\n💰 PHI:\n";
        cout << "   Tong cong:      " << fixed << setprecision(0)
             << ticket->getFee() << " VND\n";
    }

    cout << "\n════════════════════════════════════════════\n";
}

void ParkingManager::advancedTicketSearch(ParkingManager &manager)
{
    Utils::clearScreen();
    cout << "1. Tim theo ID ve xe\n";
    cout << "2. Tim theo bien so xe\n";
    cout << "3. Tim theo ID khach hang\n";
    cout << "0. Tim theo trang thai\n";
    cout << "0. Quay lai\n";

    int choice;
    cout << "\nNhap lua chon: ";
    cin >> choice;
    cin.ignore();

    DoubleLinkedList<ParkingTicket> results;

    switch (choice)
    {
    case 1:
    {
        string ticketId;
        cout << "Nhap Ticket ID: ";
        getline(cin, ticketId);

        displayTicketDetail(manager, ticketId);
        Utils::pause();
        return;
    }
    case 2:
    {
        string plate;
        cout << "Nhap bien so xe: ";
        getline(cin, plate);

        auto vehicle = manager.getVehicleByPlate(plate);
        if (vehicle)
        {
            results = manager.getTicketsByVehicle(vehicle->getVehicleId());
        }
        break;
    }
    case 3:
    {
        string customerId;
        cout << "Nhap Customer ID: ";
        getline(cin, customerId);

        results = manager.getTicketsByCustomer(customerId);
        break;
    }
    case 4:
    {
        cout << "Chon trang thai:\n";
        cout << "1. ACTIVE (dang gui)\n";
        cout << "2. PAID (da thanh toan)\n";
        cout << "3. CANCELLED (da huy)\n";

        int statusChoice;
        cin >> statusChoice;
        cin.ignore();

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
            return;
        }

        results = manager.getTicketsByStatus(status);
        break;
    }
    default:
        return;
    }

    // Hiển thị kết quả
    cout << "\n========== KET QUA TIM KIEM ==========\n";
    cout << "Tim thay: " << results.size() << " ve\n\n";

    if (results.empty())
    {
        cout << "Khong co ket qua.\n";
    }
    else
    {
        int count = 1;
        for (auto it = results.begin(); it != results.end(); ++it)
        {
            cout << count++ << ". ";
            cout << "ID: " << it->getTicketId();

            auto vehicle = manager.getVehicle(it->getVehicleId());
            if (vehicle)
            {
                cout << " | Bien so: " << vehicle->getLicensePlate();
            }

            cout << " | " << Utils::timeToString(it->getCheckInTime());
            cout << " | " << ParkingTicket::statusToString(it->getStatus());

            if (it->getStatus() == TicketStatus::PAID)
            {
                cout << " | " << fixed << setprecision(0)
                     << it->getFee() << " VND";
            }

            cout << "\n";
        }
    }

    Utils::pause();
}

void ParkingManager::adminPriceManagement(ParkingManager &manager)
{
    PricingConfig *pricing = PricingConfig::getInstance();

    while (true)
    {
        Utils::clearScreen();
        cout << "========== QUAN LY GIA VE ==========\n";

        // Hiển thị bảng giá hiện tại
        pricing->displayPricing();

        cout << "\n1. Thay doi gia xe may\n";
        cout << "2. Thay doi gia o to thuong\n";
        cout << "3. Thay doi gia o to sang\n";
        cout << "4. Thay doi gia xe dap dien\n";
        cout << "5. Thay doi thoi gian toi thieu\n";
        cout << "0. Quay lai\n";

        int choice;
        cout << "\nNhap lua chon: ";
        cin >> choice;
        cin.ignore();

        if (choice == 0)
            break;

        try
        {
            double newPrice;
            int newMinutes;

            switch (choice)
            {
            case 1:
                cout << "Nhap gia moi cho xe may (VND/gio): ";
                cin >> newPrice;
                cin.ignore();
                pricing->setMotorcyclePrice(newPrice);
                cout << "\n✓ Cap nhat gia thanh cong!\n";
                break;

            case 2:
                cout << "Nhap gia moi cho o to thuong (VND/gio): ";
                cin >> newPrice;
                cin.ignore();
                pricing->setCarStandardPrice(newPrice);
                cout << "\n✓ Cap nhat gia thanh cong!\n";
                break;

            case 3:
                cout << "Nhap gia moi cho o to sang (VND/gio): ";
                cin >> newPrice;
                cin.ignore();
                pricing->setCarLuxuryPrice(newPrice);
                cout << "\n✓ Cap nhat gia thanh cong!\n";
                break;

            case 4:
                cout << "Nhap gia moi cho xe dap dien (VND/gio): ";
                cin >> newPrice;
                cin.ignore();
                pricing->setElectricBikePrice(newPrice);
                cout << "\n✓ Cap nhat gia thanh cong!\n";
                break;

            case 5:
                cout << "Nhap thoi gian toi thieu moi (phut): ";
                cin >> newMinutes;
                cin.ignore();
                pricing->setMinimumMinutes(newMinutes);
                cout << "\n✓ Cap nhat thanh cong!\n";
                break;

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

void ParkingManager::customerViewPricing()
{
    Utils::clearScreen();
    PricingConfig *pricing = PricingConfig::getInstance();
    pricing->displayPricing();

    cout << "\nLUU Y:\n";
    cout << "- Phi duoc tinh theo gio, lam tron len\n";
    cout << "- Thoi gian toi thieu: " << pricing->getMinimumMinutes() << " phut\n";
    cout << "- Vi du: Gui 45 phut = 1 gio\n";
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
    cout << "\n========== BAO CAO NGAY " << Utils::getCurrentDateTime() << " ==========\n";

    // Slot statistics
    cout << "\n1. THONG KE CHO DO:\n";
    cout << "   - Tong so cho: " << getTotalSlots() << endl;
    cout << "   - Dang su dung: " << getOccupiedSlotCount() << endl;
    cout << "   - Con trong (Xe may): " << getAvailableSlotCount(VehicleType::MOTORCYCLE) << endl;
    cout << "   - Con trong (O to): " << getAvailableSlotCount(VehicleType::CAR) << endl;
    cout << "   - Con trong (Xe dap dien): " << getAvailableSlotCount(VehicleType::ELECTRIC_BIKE) << endl;

    // Revenue statistics
    time_t now = time(nullptr);
    time_t startOfDay = now - (now % 86400); // Start of current day
    double dailyRevenue = getRevenueByPeriod(startOfDay, now);

    cout << "\n2. DOANH THU:\n";
    cout << "   - Doanh thu hom nay: " << fixed << setprecision(2)
         << dailyRevenue << " VND" << endl;
    cout << "   - Tong doanh thu: " << getTotalRevenue() << " VND" << endl;

    // Active tickets
    int activeCount = 0;
    for (auto it = tickets.begin(); it != tickets.end(); ++it)
    {
        if (it->isActive())
            activeCount++;
    }

    cout << "\n3. VE HIEN TAI:\n";
    cout << "   - So ve dang hoat dong: " << activeCount << endl;
    cout << "   - Tong so ve da tao: " << tickets.size() << endl;

    cout << "\n=======================================================\n";
}

void ParkingManager::generateMonthlyReport() const
{
    cout << "\n========== BAO CAO THANG ==========\n";

    time_t now = time(nullptr);
    struct tm *timeinfo = localtime(&now);
    timeinfo->tm_mday = 1;
    timeinfo->tm_hour = 0;
    timeinfo->tm_min = 0;
    timeinfo->tm_sec = 0;
    time_t startOfMonth = mktime(timeinfo);

    double monthlyRevenue = getRevenueByPeriod(startOfMonth, now);

    cout << "Doanh thu thang nay: " << fixed << setprecision(2)
         << monthlyRevenue << " VND" << endl;

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

    cout << "\nThong ke theo loai xe:\n";
    cout << "   - Xe may: " << motorcycleTickets << " luot\n";
    cout << "   - O to: " << carTickets << " luot\n";
    cout << "   - Xe dap dien: " << electricBikeTickets << " luot\n";

    cout << "\n====================================\n";
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