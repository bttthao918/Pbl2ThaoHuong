#include "ParkingManager.h"
#include "Utils.h"
#include "Exception.h"     // THÊM
#include <fstream>
#include <algorithm>
#include <iomanip>

using namespace std;       // THÊM

ParkingManager::ParkingManager() : currentUser(nullptr)
{
    loadAllData();
}

ParkingManager::~ParkingManager()
{
    saveAllData();
}

/* ============================
        USER MANAGEMENT
   ============================ */

bool ParkingManager::registerUser(const string &username, const string &password,
                                  const string &fullName, const string &phone,
                                  const string &email, UserRole role)
{
    // Check username exists
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
            // Admin mới chỉ còn employeeID, không còn phòng ban + chức vụ
            newUser = make_shared<Admin>(userId, username, password,
                                         fullName, phone, email,
                                         "EMP0000"); // bạn có thể sửa lại generate employee code
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

/* ============================
          ADMIN MANAGEMENT
   ============================ */

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

/* ============================
        VEHICLE MANAGEMENT
   ============================ */

bool ParkingManager::registerVehicle(const string &licensePlate, VehicleType type,
                                     const string &brand, const string &model,
                                     const string &color, const string &customerId)
{
    auto existing = vehicles.find([&](const shared_ptr<Vehicle> &v)
                                  { return v->getLicensePlate() == licensePlate; });

    if (existing != nullptr)
        throw DuplicateException("Bien so xe da ton tai");

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
                                          customerId, brand, model, color,
                                          5, false);
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

DoubleLinkedList<shared_ptr<Vehicle>> ParkingManager::getVehiclesByCustomer(const string &customerId)
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
saveVehicles();

return removed;
}

/* ============================
PARKING SLOT MANAGEMENT
============================ */

bool ParkingManager::addParkingSlot(const string &slotNumber, VehicleType type)
{
auto existing = parkingSlots.find([&](const ParkingSlot &s)
       { return s.getSlotNumber() == slotNumber; });

if (existing != nullptr)
throw DuplicateException("So vi tri da ton tai");

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
slot->setMaintenance();
else
slot->setStatus(status);

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
saveSlots();

return removed;
}

/* ============================
BOOKING MANAGEMENT
============================ */

bool ParkingManager::createBooking(const string &customerId, const string &vehicleId,
    time_t expectedArrival)
{
if (!getCustomer(customerId) || !getVehicle(vehicleId))
throw NotFoundException("Khach hang hoac xe khong ton tai");

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

/* ============================
TICKET / CHECK-IN / OUT
============================ */

ParkingSlot *ParkingManager::findAvailableSlot(VehicleType type)
{
return parkingSlots.find([&](const ParkingSlot &s)
{ return s.isAvailable() && s.canAccommodate(type); });
}

string ParkingManager::checkIn(const string &customerId, const string &vehicleId,
const string &bookingId)
{
Customer *customer = getCustomer(customerId);
auto vehicle = getVehicle(vehicleId);

if (!customer || !vehicle)
throw NotFoundException("Khach hang hoac xe khong ton tai");

ParkingSlot *slot = findAvailableSlot(vehicle->getType());
if (!slot)
throw NotFoundException("Khong con cho do");

string ticketId = generateTicketId();
ParkingTicket newTicket(ticketId, customerId, vehicleId, slot->getSlotId());

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
throw NotFoundException("Ticket khong ton tai hoac da thanh toan");

auto vehicle = getVehicle(ticket->getVehicleId());
if (!vehicle)
throw NotFoundException("Khong tim thay thong tin xe");

long long duration = ticket->getParkingDuration();
double fee = vehicle->calculateParkingFee(duration);

Customer *customer = getCustomer(ticket->getCustomerId());
if (!customer)
throw NotFoundException("Khong tim thay khach hang");

try
{
customer->deductBalance(fee);
customer->addLoyaltyPoints(static_cast<int>(fee / 1000));

ticket->checkOut(fee);

ParkingSlot *slot = getParkingSlot(ticket->getSlotId());
if (slot)
slot->release();

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
/* ============================
             SEARCH
   ============================ */

   DoubleLinkedList<shared_ptr<User>> ParkingManager::searchUsers(const string &keyword)
   {
       string key = Utils::toLower(keyword);
   
       return users.filter([&](const shared_ptr<User> &u)
                           {
                               return Utils::toLower(u->getFullName()).find(key) != string::npos ||
                                      Utils::toLower(u->getUsername()).find(key) != string::npos ||
                                      Utils::toLower(u->getPhoneNumber()).find(key) != string::npos;
                           });
   }
   
   DoubleLinkedList<shared_ptr<Vehicle>> ParkingManager::searchVehicles(const string &keyword)
   {
       string key = Utils::toLower(keyword);
   
       return vehicles.filter([&](const shared_ptr<Vehicle> &v)
                              {
                                  return Utils::toLower(v->getLicensePlate()).find(key) != string::npos ||
                                         Utils::toLower(v->getBrand()).find(key) != string::npos ||
                                         Utils::toLower(v->getModel()).find(key) != string::npos;
                              });
   }
   
   DoubleLinkedList<ParkingTicket> ParkingManager::searchTickets(const string &keyword)
   {
       string key = Utils::toLower(keyword);
   
       return tickets.filter([&](const ParkingTicket &t)
                             {
                                 return Utils::toLower(t.getTicketId()).find(key) != string::npos ||
                                        Utils::toLower(t.getVehicleId()).find(key) != string::npos ||
                                        Utils::toLower(t.getCustomerId()).find(key) != string::npos;
                             });
   }
   
   /* ============================
                 SORT
      ============================ */
   
   void ParkingManager::sortUsersByName()
   {
       users.sort([](const shared_ptr<User> &a, const shared_ptr<User> &b)
                  {
                      return Utils::toLower(a->getFullName()) <
                             Utils::toLower(b->getFullName());
                  });
   }
   
   void ParkingManager::sortVehiclesByPlate()
   {
       vehicles.sort([](const shared_ptr<Vehicle> &a, const shared_ptr<Vehicle> &b)
                     {
                         return Utils::toLower(a->getLicensePlate()) <
                                Utils::toLower(b->getLicensePlate());
                     });
   }
   
   void ParkingManager::sortTicketsByTime()
   {
       tickets.sort([](const ParkingTicket &a, const ParkingTicket &b)
                    {
                        return a.getEntryTime() < b.getEntryTime();
                    });
   }
   
   /* ============================
               STATISTICS
      ============================ */
   
   int ParkingManager::getTotalSlots() const
   {
       return parkingSlots.size();
   }
   
   int ParkingManager::getAvailableSlotCount(VehicleType type) const
   {
       int count = 0;
   
       parkingSlots.forEach([&](const ParkingSlot &s)
                            {
                                if (s.isAvailable() && s.canAccommodate(type))
                                    count++;
                            });
   
       return count;
   }
   
   int ParkingManager::getOccupiedSlotCount() const
   {
       int count = 0;
   
       parkingSlots.forEach([&](const ParkingSlot &s)
                            {
                                if (!s.isAvailable())
                                    count++;
                            });
   
       return count;
   }
   
   double ParkingManager::getTotalRevenue() const
   {
       double total = 0;
   
       tickets.forEach([&](const ParkingTicket &t)
                       {
                           if (!t.isActive())
                               total += t.getFee();
                       });
   
       return total;
   }
   
   double ParkingManager::getRevenueByPeriod(time_t startTime, time_t endTime) const
   {
       double total = 0;
   
       tickets.forEach([&](const ParkingTicket &t)
                       {
                           if (!t.isActive() &&
                               t.getExitTime() >= startTime &&
                               t.getExitTime() <= endTime)
                           {
                               total += t.getFee();
                           }
                       });
   
       return total;
   }
   
   void ParkingManager::generateDailyReport() const
   {
       time_t now = time(nullptr);
       struct tm *timeInfo = localtime(&now);
   
       timeInfo->tm_hour = 0;
       timeInfo->tm_min = 0;
       timeInfo->tm_sec = 0;
   
       time_t startOfDay = mktime(timeInfo);
       time_t endOfDay = startOfDay + 86400;
   
       double revenue = getRevenueByPeriod(startOfDay, endOfDay);
   
       cout << "===== BAO CAO NGAY =====" << endl;
       cout << "Tong doanh thu trong ngay: " << revenue << " VND" << endl;
   }
   
   void ParkingManager::generateMonthlyReport() const
   {
       time_t now = time(nullptr);
       struct tm *timeInfo = localtime(&now);
   
       timeInfo->tm_mday = 1;
       timeInfo->tm_hour = 0;
       timeInfo->tm_min = 0;
       timeInfo->tm_sec = 0;
   
       time_t startOfMonth = mktime(timeInfo);
   
       // Cộng 1 tháng
       timeInfo->tm_mon += 1;
       time_t endOfMonth = mktime(timeInfo);
   
       double revenue = getRevenueByPeriod(startOfMonth, endOfMonth);
   
       cout << "===== BAO CAO THANG =====" << endl;
       cout << "Tong doanh thu trong thang: " << revenue << " VND" << endl;
   }
/* ============================
            LOAD / SAVE
   ============================ */

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
   
   /* -------- USERS -------- */
   
   void ParkingManager::saveUsers()
   {
       ofstream out(USERS_FILE);
       if (!out)
           return;
   
       users.forEach([&](const shared_ptr<User> &u)
                     { out << u->toFileString() << endl; });
   
       out.close();
   }
   
   void ParkingManager::loadUsers()
   {
       ifstream in(USERS_FILE);
       if (!in)
           return;
   
       string line;
       while (getline(in, line))
       {
           vector<string> parts;
           string temp;
           stringstream ss(line);
   
           while (getline(ss, temp, '|'))
               parts.push_back(temp);
   
           if (parts.size() < 7)
               continue;
   
           string id = parts[0];
           string uname = parts[1];
           string pwd = parts[2];
           string name = parts[3];
           string phone = parts[4];
           string email = parts[5];
           string role = parts[6];
   
           shared_ptr<User> user;
   
           if (role == "CUSTOMER")
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
   
       in.close();
   }
   
   /* -------- VEHICLES -------- */
   
   void ParkingManager::saveVehicles()
   {
       ofstream out(VEHICLES_FILE);
       if (!out)
           return;
   
       vehicles.forEach([&](const shared_ptr<Vehicle> &v)
                        { out << v->toFileString() << endl; });
   
       out.close();
   }
   
   void ParkingManager::loadVehicles()
   {
       ifstream in(VEHICLES_FILE);
       if (!in)
           return;
   
       string line;
       while (getline(in, line))
       {
           shared_ptr<Vehicle> v = Vehicle::createFromString(line);
           if (v)
               vehicles.pushBack(v);
       }
   
       in.close();
   }
   
   /* -------- SLOTS -------- */
   
   void ParkingManager::saveSlots()
   {
       ofstream out(SLOTS_FILE);
       if (!out)
           return;
   
       parkingSlots.forEach([&](const ParkingSlot &s)
                            { out << s.toFileString() << endl; });
   
       out.close();
   }
   
   void ParkingManager::loadSlots()
   {
       ifstream in(SLOTS_FILE);
       if (!in)
           return;
   
       string line;
       while (getline(in, line))
       {
           ParkingSlot slot;
           slot.fromFileString(line);
           parkingSlots.pushBack(slot);
       }
   
       in.close();
   }
   
   /* -------- BOOKINGS -------- */
   
   void ParkingManager::saveBookings()
   {
       ofstream out(BOOKINGS_FILE);
       if (!out)
           return;
   
       bookings.forEach([&](const Booking &b)
                        { out << b.toFileString() << endl; });
   
       out.close();
   }
   
   void ParkingManager::loadBookings()
   {
       ifstream in(BOOKINGS_FILE);
       if (!in)
           return;
   
       string line;
       while (getline(in, line))
       {
           Booking b;
           b.fromFileString(line);
           bookings.pushBack(b);
       }
   
       in.close();
   }
   
   /* -------- TICKETS -------- */
   
   void ParkingManager::saveTickets()
   {
       ofstream out(TICKETS_FILE);
       if (!out)
           return;
   
       tickets.forEach([&](const ParkingTicket &t)
                       { out << t.toFileString() << endl; });
   
       out.close();
   }
   
   void ParkingManager::loadTickets()
   {
       ifstream in(TICKETS_FILE);
       if (!in)
           return;
   
       string line;
       while (getline(in, line))
       {
           ParkingTicket t;
           t.fromFileString(line);
           tickets.pushBack(t);
       }
   
       in.close();
   }
   
   /* ============================
             GENERATE ID
      ============================ */
   
   string ParkingManager::generateUserId()
   {
       return "USR" + Utils::generateID("");
   }
   
   string ParkingManager::generateVehicleId()
   {
       return "VHC" + Utils::generateID("");
   }
   
   string ParkingManager::generateSlotId()
   {
       return "SLT" + Utils::generateID("");
   }
   
   string ParkingManager::generateBookingId()
   {
       return "BKG" + Utils::generateID("");
   }
   
   string ParkingManager::generateTicketId()
   {
       return "TKT" + Utils::generateID("");
   }
      