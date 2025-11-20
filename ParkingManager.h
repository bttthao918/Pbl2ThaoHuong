#ifndef PARKING_MANAGER_H
#define PARKING_MANAGER_H

#include "DoubleLinkedList.h"
#include "Vehicle.h"
#include "User.h"
#include "Admin.h"
#include "Customer.h"
#include "Car.h"
#include "Motorcycle.h"
#include "ElectricBike.h"
#include "Utils.h"
#include "ParkingSlot.h"
#include "Booking.h"
#include "ParkingTicket.h"
#include <string>
#include <memory>

class ParkingManager
{
private:
    DoubleLinkedList<shared_ptr<User>> users;
    DoubleLinkedList<shared_ptr<Vehicle>> vehicles;
    DoubleLinkedList<ParkingSlot> parkingSlots;
    DoubleLinkedList<Booking> bookings;
    DoubleLinkedList<ParkingTicket> tickets;

    shared_ptr<User> currentUser; // User đang đăng nhập

    // File paths
    const string CUSTOMERS_FILE = "customers.txt";
    const string ADMINS_FILE = "admins.txt";
    const string VEHICLES_FILE = "vehicles.txt";
    const string SLOTS_FILE = "slots.txt";
    const string BOOKINGS_FILE = "bookings.txt";
    const string TICKETS_FILE = "tickets.txt";

public:
    // Constructor & Destructor
    ParkingManager();
    ~ParkingManager();

    // User management
    bool registerUser(const string &username, const string &password,
                      const string &fullName, const string &phone,
                      const string &email, UserRole role);
    bool login(const string &username, const string &password);
    void logout();
    shared_ptr<User> getCurrentUser() const { return currentUser; }
    bool isLoggedIn() const { return currentUser != nullptr; }

    // Customer specific
    Customer *getCustomer(const string &customerId);
    DoubleLinkedList<shared_ptr<User>> getAllCustomers();
    bool updateCustomerInfo(const string &customerId, const string &fullName,
                            const string &phone, const string &email);
    // Admin specific
    bool addCustomer(const string &username, const string &password,
                     const string &fullName, const string &phone,
                     const string &email);
    bool deleteCustomer(const string &customerId);
    bool updateCustomerDetails(const string &customerId,
                               const string &fullName,
                               const string &phone,
                               const string &email,
                               int loyaltyPoints);
    Admin *getAdmin(const string &adminId);
    DoubleLinkedList<shared_ptr<User>> getAllAdmins();

    // Vehicle management
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

    // Parking slot management
    bool addParkingSlot(const string &slotNumber, VehicleType type);
    ParkingSlot *getParkingSlot(const string &slotId);
    ParkingSlot *getParkingSlotByNumber(const string &slotNumber);
    DoubleLinkedList<ParkingSlot> getAvailableSlots(VehicleType type);
    DoubleLinkedList<ParkingSlot> getAvailableSlotsByType(VehicleType type);
    DoubleLinkedList<ParkingSlot> getAllSlots();
    bool updateSlotStatus(const string &slotId, SlotStatus status);
    bool deleteParkingSlot(const string &slotId);

    // Booking management
    bool createBooking(const string &customerId,
                       const string &vehicleId,
                       time_t expectedArrival,
                       const string &slotId = "");
    Booking *getBooking(const string &bookingId);
    DoubleLinkedList<Booking> getBookingsByCustomer(const string &customerId);
    DoubleLinkedList<Booking> getAllBookings();
    bool confirmBooking(const string &bookingId);
    void cancelBooking();

    // Ticket management (Check-in/Check-out)
    string checkIn(const string &customerId, const string &vehicleId,
                   const string &bookingId = ""); // Return ticketId
    bool checkOut(const string &ticketId);
    ParkingTicket *getTicket(const string &ticketId);
    DoubleLinkedList<ParkingTicket> getAllTickets();
    DoubleLinkedList<ParkingTicket> getTicketsByCustomer(const string &customerId);
    DoubleLinkedList<ParkingTicket> getTicketsByVehicle(const string &vehicleId);
    DoubleLinkedList<ParkingTicket> getTicketsByStatus(TicketStatus status);
    void displayTicketDetail(ParkingManager &manager, const string &ticketId);
    void advancedTicketSearch(ParkingManager &manager);
    void adminPriceManagement(ParkingManager &manager);
    void customerViewPricing();

    // Search & Filter
    DoubleLinkedList<shared_ptr<User>> searchUsers(const string &keyword);
    DoubleLinkedList<shared_ptr<Vehicle>> searchVehicles(const string &keyword);
    DoubleLinkedList<ParkingTicket> searchTickets(const string &keyword);

    // Sort
    void sortUsersByName();
    void sortVehiclesByPlate();

    // Statistics & Reports
    int getTotalSlots() const;
    int getAvailableSlotCount(VehicleType type) const;
    int getOccupiedSlotCount() const;
    double getTotalRevenue() const;
    double getRevenueByPeriod(time_t startTime, time_t endTime) const;
    void generateDailyReport() const;
    void generateMonthlyReport() const;

    // File operations
    void saveAllData();
    void loadAllData();

private:
    // Helper functions
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

    string generateUserId();
    string generateVehicleId(const string &customerId);
    string generateSlotId();
    string generateBookingId();
    string generateTicketId();

    ParkingSlot *findAvailableSlot(VehicleType type);
};

#endif