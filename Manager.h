#ifndef MANAGER_H
#define MANAGER_H

#include "User.h"
#include "Vehicle.h"
#include "ParkingTicket.h"
#include <string>
using namespace std;

class Manager : public User {
private:
    string managerID;
    string shift;
    double salary;
    string startDate;
    int totalCheckIns;
    int totalCheckOuts;
    double totalRevenue;
    int customersManaged;

public:
    // Constructor
    Manager(string id, string user, string pass, string name,
            string phone, string mail, string shift, double salary);

    // Override phương thức ảo
    void display() override;
    string getUserType() override { return "Manager"; }
    void showMenu() override;

    // ===== NHÓM 0: QUẢN LÝ TÀI KHOẢN =====
    void viewMyProfile();
    void editMyProfile();
    void updateMyFullName(string newName);
    void updateMyPhone(string newPhone);
    void updateMyEmail(string newEmail);
    void changeMyPassword();

    // ===== NHÓM 1: QUẢN LÝ XE =====
    void checkInVehicle(Vehicle *vehicle, ParkingTicket *ticket);
    void checkOutVehicle(string vehicleID);
    void searchVehicle(string keyword);
    void printTicket(string ticketID);
    void reprintTicket(string ticketID);
    void viewAllVehicles();
    void viewVehiclesByType(string type);

    // ===== NHÓM 2: QUẢN LÝ SLOT =====
    void manageSlots();
    void viewAllSlots();
    void viewEmptySlots();
    void viewOccupiedSlots();
    void viewSlotsByZone(string zone);

    // ===== NHÓM 3: QUẢN LÝ KHÁCH HÀNG =====
    void viewAllCustomers();
    void searchCustomer(string keyword);
    void viewCustomerDetails(string customerID);
    void updateCustomerInfo(string customerID);
    void activateCustomer(string customerID);
    void deactivateCustomer(string customerID);
    void resetCustomerPassword(string customerID);
    void adjustCustomerMembership(string customerID, string newLevel);
    void viewCustomerHistory(string customerID);
    void addNoteToCustomer(string customerID, string note);

    // ===== NHÓM 4: GIÁ CƯỚC =====
    void viewCurrentPricing();
    void updateMotorcyclePrice(double hourlyRate, double dailyMax);
    void updateCarPrice(double hourlyRate, double dailyMax);
    void updateElectricBikePrice(double hourlyRate, double dailyMax);
    void updateDiscountRates();
    void viewPricingHistory();
    void applySpecialPromotion(string promoName, double discountPercent,
                               string startDate, string endDate);

    // ===== NHÓM 5: THỐNG KÊ =====
    void viewMyStatistics();
    void viewDailyReport();
    void viewMonthlyReport();
    void viewRevenueReport();
    void viewCustomerReport();
    void exportReport(string filename);

    // Getters
    string getManagerID() const { return managerID; }
    string getShift() const { return shift; }
    void setShift(string s) { shift = s; }
    int getTotalCheckIns() const { return totalCheckIns; }
    int getTotalCheckOuts() const { return totalCheckOuts; }
    double getTotalRevenue() const { return totalRevenue; }
    int getCustomersManaged() const { return customersManaged; }

    // Thống kê
    void incrementCheckIns() { totalCheckIns++; }
    void incrementCheckOuts() { totalCheckOuts++; }
    void addRevenue(double amount) { totalRevenue += amount; }
    void incrementCustomersManaged() { customersManaged++; }
};

#endif
