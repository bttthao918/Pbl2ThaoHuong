#include "User.h"
#include <string>
#include "DoubleLinkedList.h"


class Customer : public User {
    private:
        string customerID;           // Mã khách hàng (CUS001, CUS002,...)
        int totalVisits;             // Tổng số lần gửi xe
        double totalSpent;           // Tổng chi tiêu
        string membershipLevel;      // Hạng thành viên
        double discountRate;         // Tỷ lệ giảm giá (%)
        DoubleLinkedList<string> vehicleIDs;  // Danh sách biển số xe của khách
        string lastVisit;            // Lần gửi xe gần nhất
        
    public:
        // Constructor
        Customer(string id, string user, string pass, string name,
                 string phone, string mail);
        
        // Override phương thức ảo
        void display() override;
        string getUserType() override { return "Customer"; }
        void showMenu() override;
        
        // Phương thức riêng của Customer
        void registerVehicle(string vehicleID);      // Đăng ký xe mới
        void removeVehicle(string vehicleID);        // Xóa xe
        void viewMyVehicles();                       // Xem xe của mình
        void viewMyHistory();                        // Xem lịch sử gửi xe
        void checkVehicleStatus(string vehicleID);   // Kiểm tra xe đang đỗ
        double applyDiscount(double originalFee);    // Áp dụng giảm giá
        void updateMembership();                     // Cập nhật hạng thành viên
        void viewMyProfile();                        // Xem thông tin cá nhân
        void viewNotes();                            // Xem ghi chú
        
        // Getters & Setters
        string getCustomerID() const { return customerID; }
        string getMembershipLevel() const { return membershipLevel; }
        int getTotalVisits() const { return totalVisits; }
        double getTotalSpent() const { return totalSpent; }
        double getDiscountRate() const { return discountRate; }
        string getLastVisit() const { return lastVisit; }
        
        void setMembershipLevel(string level) { membershipLevel = level; }
        void setDiscountRate(double rate) { discountRate = rate; }
        
        // Cập nhật sau mỗi lần gửi xe
        void addVisit(double fee);
    };