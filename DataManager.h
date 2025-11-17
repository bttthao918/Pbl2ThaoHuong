#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "DoubleLinkedList.h"
#include "User.h"
#include "Manager.h"
#include "Customer.h"
#include "Vehicle.h"
#include "Motorcycle.h"
#include "Car.h"
#include "ElectricBike.h"
#include "ParkingSlot.h"
#include "ParkingTicket.h"
#include "Exception.h"
#include <string>
#include <ctime>
#include <fstream>

using namespace std;

class DataManager {
private:
    // ==================== DANH SÁCH DỮ LIỆU ====================
    DoubleLinkedList<Manager*> managerList;
    DoubleLinkedList<Customer*> customerList;
    DoubleLinkedList<Vehicle*> vehicleList;
    DoubleLinkedList<ParkingSlot*> slotList;
    DoubleLinkedList<ParkingTicket*> ticketList;
    
    // ==================== THÔNG TIN HỆ THỐNG ====================
    int totalSlots;              // Tổng số chỗ đỗ
    int occupiedSlots;           // Số chỗ đã sử dụng
    int nextTicketID;            // ID vé tiếp theo
    
    // ==================== CẤU HÌNH GIÁ ====================
    struct PricingConfig {
        double motorcycleHourly;
        double motorcycleDailyMax;
        double carHourly;
        double carDailyMax;
        double electricBikeHourly;
        double electricBikeDailyMax;
    } pricing;
    
    // ==================== HELPER METHODS ====================
    int generateTicketID();
    string generateTicketCode(int id);
    string getCurrentDate();
    string getCurrentTime();
    
public:
    // ==================== CONSTRUCTOR & DESTRUCTOR ====================
    DataManager(int slots = 100);
    ~DataManager();
    
    // ==================== KHỞI TẠO HỆ THỐNG ====================
    void initializeSlots();
    void initializeDefaultPricing();
    void initializeSampleData();
    
    // ==================== NHÓM 1: QUẢN LÝ MANAGER ====================
    void addManager(Manager* mgr);
    Manager* findManager(string managerID);
    Manager* loginManager(string username, string password);
    void updateManager(string managerID, Manager* newData);
    void deleteManager(string managerID);
    DoubleLinkedList<Manager*>* getAllManagers();
    void displayAllManagers();
    
    // ==================== NHÓM 2: QUẢN LÝ CUSTOMER ====================
    void addCustomer(Customer* cust);
    Customer* findCustomer(string customerID);
    Customer* findCustomerByPhone(string phone);
    Customer* loginCustomer(string username, string password);
    void updateCustomer(string customerID, Customer* newData);
    void deleteCustomer(string customerID);
    void activateCustomer(string customerID);
    void deactivateCustomer(string customerID);
    void resetCustomerPassword(string customerID, string newPassword);
    DoubleLinkedList<Customer*>* getAllCustomers();
    void displayAllCustomers();
    
    // ==================== NHÓM 3: QUẢN LÝ VEHICLE ====================
    void registerVehicle(Vehicle* vehicle);
    Vehicle* findVehicle(string licensePlate);
    void updateVehicle(string licensePlate, Vehicle* newData);
    void deleteVehicle(string licensePlate);
    DoubleLinkedList<Vehicle*> searchVehicleByKeyword(string keyword);
    DoubleLinkedList<Vehicle*> getVehiclesByType(string type);
    DoubleLinkedList<Vehicle*>* getAllVehicles();
    void displayAllVehicles();
    void displayVehiclesByType(string type);
    
    // ==================== NHÓM 4: QUẢN LÝ PARKING SLOT ====================
    ParkingSlot* findEmptySlot();
    ParkingSlot* findEmptySlotByType(string vehicleType);
    ParkingSlot* findSlotByNumber(int slotNumber);
    void occupySlot(int slotNumber, string licensePlate, string vehicleType);
    void releaseSlot(int slotNumber);
    DoubleLinkedList<ParkingSlot*> getEmptySlots();
    DoubleLinkedList<ParkingSlot*> getOccupiedSlots();
    DoubleLinkedList<ParkingSlot*> getSlotsByZone(string zone);
    DoubleLinkedList<ParkingSlot*>* getAllSlots();
    void displayAllSlots();
    void displayEmptySlots();
    void displayOccupiedSlots();
    
    // ==================== NHÓM 5: CHECK-IN / CHECK-OUT ====================
    ParkingTicket* checkIn(string licensePlate, string customerPhone, Manager* performedBy);
    double checkOut(string ticketID, Manager* performedBy);
    ParkingTicket* findTicket(string ticketID);
    ParkingTicket* findActiveTicketByPlate(string licensePlate);
    void printTicket(ParkingTicket* ticket);
    void reprintTicket(string ticketID);
    
    // ==================== NHÓM 6: PRICING (GIÁ CƯỚC) ====================
    void updateMotorcyclePricing(double hourly, double dailyMax);
    void updateCarPricing(double hourly, double dailyMax);
    void updateElectricBikePricing(double hourly, double dailyMax);
    double calculateFee(ParkingTicket* ticket, Vehicle* vehicle);
    void displayCurrentPricing();
    PricingConfig getCurrentPricing();
    
    // ==================== NHÓM 7: THỐNG KÊ & BÁO CÁO ====================
    // Thống kê tổng quan
    int getTotalSlots();
    int getOccupiedSlots();
    int getAvailableSlots();
    double getOccupancyRate();
    int getTotalVehicles();
    int getTotalCustomers();
    int getTotalManagers();
    
    // Thống kê theo Manager
    void getManagerStatistics(string managerID, int& checkIns, int& checkOuts, double& revenue);
    void displayManagerStatistics(string managerID);
    
    // Thống kê theo thời gian
    double getDailyRevenue();
    double getMonthlyRevenue();
    int getDailyCheckIns();
    int getDailyCheckOuts();
    
    // Báo cáo
    void generateSystemReport();
    void generateDailyReport();
    void generateMonthlyReport();
    void generateRevenueReport();
    void generateCustomerReport();
    
    // Lịch sử
    DoubleLinkedList<ParkingTicket*> getCustomerHistory(string customerPhone);
    DoubleLinkedList<ParkingTicket*> getActiveTickets();
    DoubleLinkedList<ParkingTicket*> getCompletedTickets();
    DoubleLinkedList<ParkingTicket*>* getAllTickets();
    void displayActiveTickets();
    void displayAllTickets();
    
    // ==================== NHÓM 8: TÌM KIẾM ====================
    DoubleLinkedList<Manager*> searchManager(string keyword);
    DoubleLinkedList<Customer*> searchCustomer(string keyword);
    DoubleLinkedList<Vehicle*> searchVehicle(string keyword);
    DoubleLinkedList<ParkingTicket*> searchTicketByDate(string date);
    
    // ==================== NHÓM 9: LƯU/TẢI DỮ LIỆU ====================
    void saveAllData();
    void loadAllData();
    void saveManagers(string filename);
    void saveCustomers(string filename);
    void saveVehicles(string filename);
    void saveTickets(string filename);
    void loadManagers(string filename);
    void loadCustomers(string filename);
    void loadVehicles(string filename);
    void loadTickets(string filename);
    
    // ==================== NHÓM 10: TIỆN ÍCH ====================
    void clearAllData();
    void resetSystem();
    bool isSystemEmpty();
    void displaySystemInfo();
};

#endif