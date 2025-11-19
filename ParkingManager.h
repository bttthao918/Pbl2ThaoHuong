#ifndef PARKING_MANAGER_H
#define PARKING_MANAGER_H

#include "DoubleLinkedList.h"
#include "User.h"
#include "Customer.h"
#include "Admin.h"
#include "Vehicle.h"
#include "ParkingSlot.h"
#include "BookingTicket.h"
#include "Exception.h"      // THÊM

#include <string>
#include <memory>

using namespace std;        // THÊM

class ParkingManager
{
private:
    DoubleLinkedList<shared_ptr<User>> users;
    DoubleLinkedList<shared_ptr<Vehicle>> vehicles;
    DoubleLinkedList<ParkingSlot> parkingSlots;
    DoubleLinkedList<Booking> bookings;
    DoubleLinkedList<ParkingTicket> tickets;

    shared_ptr<User> currentUser;

    const string USERS_FILE = "users.txt";
    const string VEHICLES_FILE = "vehicles.txt";
    const string SLOTS_FILE = "slots.txt";
    const string BOOKINGS_FILE = "bookings.txt";
    const string TICKETS_FILE = "tickets.txt";

public:
    ParkingManager();
    ~ParkingManager();

    // User
    bool registerUser(const string &username, const string &password,
                      const string &fullName, const string &phone,
                      const string &email, UserRole role);

    bool login(const string &username, const string &password);
    void logout();

    shared_ptr<User> getCurrentUser() const { return currentUser; }
    bool isLoggedIn() const { return currentUser != nullptr; }

    Customer *getCustomer(const string &customerId);
    DoubleLinkedList<shared_ptr<User>> getAllCustomers();
    bool updateCustomerInfo(const string &customerId, const string &fullName,
                            const string &phone, const string &email);
    bool addBalance(const string &customerId, double amount);

    // Admin
    Admin *getAdmin(const string &adminId);
    DoubleLinkedList<shared_ptr<User>> getAllAdmins();

    // Vehicle
    bool registerVehicle(const string &licensePlate, VehicleType type,
                         const string &brand, const string &model,
                         const string &color, const string &customerId);

    shared_ptr<Vehicle> getVehicle(const string &vehicleId);
    shared_ptr<Vehicle> getVehicleByPlate(const string &plate);
    DoubleLinkedList<shared_ptr<Vehicle>> getVehiclesByCustomer(const string &customerId);
    DoubleLinkedList<shared_ptr<Vehicle>> getAllVehicles();
    bool updateVehicle(const string &vehicleId, const string &brand,
                       const string &model, const string &color);
    bool deleteVehicle(const string &vehicleId);

    // Parking slot
    bool addParkingSlot(const string &slotNumber, VehicleType type);
    ParkingSlot *getParkingSlot(const string &slotId);
    ParkingSlot *getParkingSlotByNumber(const string &slotNumber);
    DoubleLinkedList<ParkingSlot> getAvailableSlots(VehicleType type);
    DoubleLinkedList<ParkingSlot> getAllSlots();
    bool updateSlotStatus(const string &slotId, SlotStatus status);
    bool deleteParkingSlot(const string &slotId);

    // Booking
    bool createBooking(const string &customerId, const string &vehicleId,
                       time_t expectedArrival);
    Booking *getBooking(const string &bookingId);
    DoubleLinkedList<Booking> getBookingsByCustomer(const string &customerId);
    DoubleLinkedList<Booking> getAllBookings();
    bool confirmBooking(const string &bookingId);
    bool cancelBooking(const string &bookingId);

    // Ticket
    string checkIn(const string &customerId, const string &vehicleId,
                   const string &bookingId = "");
    bool checkOut(const string &ticketId);
    ParkingTicket *getTicket(const string &ticketId);
    DoubleLinkedList<ParkingTicket> getTicketsByCustomer(const string &customerId);
    DoubleLinkedList<ParkingTicket> getTicketsByVehicle(const string &vehicleId);
    DoubleLinkedList<ParkingTicket> getAllTickets();
    DoubleLinkedList<ParkingTicket> getActiveTickets();

    // Search
    DoubleLinkedList<shared_ptr<User>> searchUsers(const string &keyword);
    DoubleLinkedList<shared_ptr<Vehicle>> searchVehicles(const string &keyword);
    DoubleLinkedList<ParkingTicket> searchTickets(const string &keyword);

    // Sort
    void sortUsersByName();
    void sortVehiclesByPlate();
    void sortTicketsByTime();

    // Statistics
    int getTotalSlots() const;
    int getAvailableSlotCount(VehicleType type) const;
    int getOccupiedSlotCount() const;
    double getTotalRevenue() const;
    double getRevenueByPeriod(time_t startTime, time_t endTime) const;

    void generateDailyReport() const;
    void generateMonthlyReport() const;

    // Global load/save
    void saveAllData();
    void loadAllData();

private:
    // File helpers
    void loadUsers();
    void saveUsers();
    void loadVehicles();
    void saveVehicles();
    void loadSlots();
    void saveSlots();
    void loadBookings();
    void saveBookings();
    void loadTickets();
    void saveTickets();

    // ID helpers
    string generateUserId();
    string generateVehicleId();
    string generateSlotId();
    string generateBookingId();
    string generateTicketId();

    ParkingSlot *findAvailableSlot(VehicleType type);
};

#endif
