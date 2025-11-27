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
#include <vector>
#include <memory>
#include <iostream>
using std::shared_ptr;
using std::string;
using namespace std;

class ParkingManager
{
private:
    DoubleLinkedList<shared_ptr<User>> users;
    DoubleLinkedList<shared_ptr<Vehicle>> vehicles;
    DoubleLinkedList<ParkingSlot> parkingSlots;
    DoubleLinkedList<Booking> bookings;
    DoubleLinkedList<ParkingTicket> tickets;

    std::shared_ptr<User> currentUser;

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
                      const string &email, UserRole role,
                      const string &employeeID = ""); // Thêm tham số mới
    bool login(const std::string &username, const std::string &password);
    void logout();
    std::shared_ptr<User> getCurrentUser() const { return currentUser; }
    bool isLoggedIn() const { return currentUser != nullptr; }

    // Customer specific
    Customer *getCustomer(const std::string &customerId);
    DoubleLinkedList<std::shared_ptr<User>> getAllCustomers();
    bool updateCustomerInfo(const std::string &customerId, const std::string &fullName,
                            const std::string &phone, const std::string &email);
    // Admin specific
    bool addCustomer(const std::string &username, const std::string &password,
                     const std::string &fullName, const std::string &phone,
                     const std::string &email);
    bool deleteCustomer(const std::string &customerId);
    bool updateCustomerDetails(const std::string &customerId,
                               const std::string &fullName,
                               const std::string &phone,
                               const std::string &email,
                               int loyaltyPoints);
    Admin *getAdmin(const std::string &adminId);
    DoubleLinkedList<std::shared_ptr<User>> getAllAdmins();

    // Vehicle management
    bool registerVehicle(const string &licensePlate, VehicleType type,
                         const string &brand, const string &model,
                         const string &color, const string &customerId,
                         int engineCapacity, int seatCount,
                         bool isLuxury, int battery, int maxSpeed);
    std::shared_ptr<Vehicle> getVehicle(const std::string &vehicleId); // chỉ trả về 1 đối tượng ko cần DLL
    std::shared_ptr<Vehicle> getVehicleByPlate(const std::string &plate);
    DoubleLinkedList<std::shared_ptr<Vehicle>> getVehiclesByCustomer(const std::string &customerId);
    DoubleLinkedList<std::shared_ptr<Vehicle>> getAllVehicles();
    bool updateVehicle(const std::string &vehicleId, const std::string &brand,
                       const std::string &model, const std::string &color);
    bool deleteVehicle(const std::string &vehicleId);

    // Parking slot management
    bool addParkingSlot(const std::string &slotNumber, VehicleType type);
    ParkingSlot *getParkingSlot(const std::string &slotId);
    ParkingSlot *getParkingSlotByNumber(const std::string &slotNumber);
    DoubleLinkedList<ParkingSlot> getAvailableSlots(VehicleType type);
    DoubleLinkedList<ParkingSlot> getAvailableSlotsByType(VehicleType type);
    DoubleLinkedList<ParkingSlot> getAllSlots();
    bool updateSlotStatus(const std::string &slotId, SlotStatus status);
    bool deleteParkingSlot(const std::string &slotId);

    // Booking management
    bool createBooking(const std::string &customerId,
                       const std::string &vehicleId,
                       time_t expectedArrival,
                       const std::string &slotId = "");
    Booking *getBooking(const std::string &bookingId);
    DoubleLinkedList<Booking> getBookingsByCustomer(const std::string &customerId);
    DoubleLinkedList<Booking> getAllBookings();
    bool confirmBooking(const std::string &bookingId);
    void cancelBooking();

    // Ticket management (Check-in/Check-out)
    std::string checkIn(const std::string &customerId, const std::string &vehicleId,
                        const std::string &bookingId = "", const std::string &slotId = ""); // Return ticketId
    bool checkOut(const std::string &ticketId);
    ParkingTicket *getTicket(const std::string &ticketId);
    DoubleLinkedList<ParkingTicket> getAllTickets();
    DoubleLinkedList<ParkingTicket> getTicketsByCustomer(const std::string &customerId);
    DoubleLinkedList<ParkingTicket> getTicketsByVehicle(const std::string &vehicleId);
    DoubleLinkedList<ParkingTicket> getTicketsByStatus(TicketStatus status);
    void displayTicketDetail(ParkingManager &manager, const std::string &ticketId);
    void advancedTicketSearch(ParkingManager &manager);
    void adminPriceManagement(ParkingManager &manager);
    void customerViewPricing();

    // Search & Filter
    DoubleLinkedList<std::shared_ptr<User>> searchUsers(const std::string &keyword);
    DoubleLinkedList<std::shared_ptr<Vehicle>> searchVehicles(const std::string &keyword);
    DoubleLinkedList<ParkingTicket> searchTickets(const std::string &keyword);

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

    std::string generateUserId();
    std::string generateVehicleId(const std::string &customerId);
    std::string generateSlotId();
    std::string generateBookingId();
    std::string generateTicketId();

    ParkingSlot *findAvailableSlot(VehicleType type);
};

#endif