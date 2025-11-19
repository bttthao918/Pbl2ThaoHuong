#include "ParkingManager.h"
#include "Utils.h"
#include <fstream>
#include <algorithm>
#include <iomanip>

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

bool ParkingManager::addBalance(const string &customerId, double amount)
{
    Customer *customer = getCustomer(customerId);
    if (!customer)
        return false;

    try
    {
        customer->addBalance(amount);
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
        string vehicleId = generateVehicleId();
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
                                   time_t expectedArrival)
{
    // Verify customer and vehicle exist
    if (!getCustomer(customerId) || !getVehicle(vehicleId))
    {
        throw NotFoundException("Khach hang hoac xe khong ton tai");
    }

    string bookingId = generateBookingId();
    Booking newBooking(bookingId, customerId, vehicleId, expectedArrival);
    newBooking.confirm();

    bookings.pushBack(newBooking);
    saveBookings();
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

bool ParkingManager::cancelBooking(const string &bookingId)
{
    Booking *booking = getBooking(bookingId);
    if (!booking)
        return false;

    try
    {
        booking->cancel();
        saveBookings();
        return true;
    }
    catch (const exception &e)
    {
        throw;
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
        customer->deductBalance(fee);
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

DoubleLinkedList<ParkingTicket> ParkingManager::getAllTickets()
{
    return tickets;
}

DoubleLinkedList<ParkingTicket> ParkingManager::getActiveTickets()
{
    return tickets.filter([](const ParkingTicket &t)
                          { return t.isActive(); });
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

void ParkingManager::sortTicketsByTime()
{
    tickets.sort([](const ParkingTicket &a, const ParkingTicket &b)
                 {
                     return a.getCheckInTime() > b.getCheckInTime(); // Newest first
                 });
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
    ifstream file(USERS_FILE);
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
            string roleStr;

            // Read first 7 fields to determine role
            vector<string> fields;
            string field;
            while (getline(iss, field, '|'))
            {
                fields.push_back(field);
                if (fields.size() == 7)
                    break;
            }

            if (fields.size() < 7)
                continue;

            shared_ptr<User> user;
            if (fields[6] == "CUSTOMER")
            {
                user = make_shared<Customer>();
            }
            else
            {
                user = make_shared<Admin>();
            }

            user->fromFileString(line);
            users.pushBack(user);
        }
        catch (const exception &e)
        {
            cerr << "Loi khi doc user: " << e.what() << endl;
        }
    }
    file.close();
}

void ParkingManager::saveUsers()
{
    ofstream file(USERS_FILE);
    if (!file.is_open())
    {
        throw runtime_error("Khong the mo file users.txt");
    }

    for (auto it = users.begin(); it != users.end(); ++it)
    {
        file << (*it)->toFileString() << endl;
    }
    file.close();
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

string ParkingManager::generateVehicleId()
{
    return Utils::generateID("V");
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