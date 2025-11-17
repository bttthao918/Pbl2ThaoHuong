#include "Manager.h"
#include "DataManager.h"  // Hệ thống quản lý chung
#include <iostream>
#include <iomanip>

using namespace std;

// Constructor
Manager::Manager(string id, string user, string pass, string name,
                 string phone, string mail, string shift, double salary)
    : User(user, pass, name, phone, mail, "Manager"),
      managerID(id), shift(shift), salary(salary),
      totalCheckIns(0), totalCheckOuts(0), totalRevenue(0.0), 
      customersManaged(0) {
    time_t now = time(nullptr);
    struct tm* timeinfo = localtime(&now);
    char buffer[80];
    strftime(buffer, 80, "%Y-%m-%d", timeinfo);
    startDate = string(buffer);
}

// Override display
void Manager::display() {
    cout << "\n╔════════════════════════════════════════════╗\n";
    cout << "║          THÔNG TIN QUẢN LÝ                 ║\n";
    cout << "╠════════════════════════════════════════════╣\n";
    cout << "║ Mã NV:     " << setw(31) << left << managerID << "║\n";
    cout << "║ Họ tên:    " << setw(31) << left << fullName << "║\n";
    cout << "║ Username:  " << setw(31) << left << username << "║\n";
    cout << "║ SĐT:       " << setw(31) << left << phone << "║\n";
    cout << "║ Email:     " << setw(31) << left << email << "║\n";
    cout << "║ Ca làm:    " << setw(31) << left << shift << "║\n";
    cout << "║ Lương:     " << setw(31) << left << (to_string((long long)salary) + " VNĐ") << "║\n";
    cout << "║ Ngày vào:  " << setw(31) << left << startDate << "║\n";
    cout << "╚════════════════════════════════════════════╝\n";
}

// Menu của Manager
void Manager::showMenu() {
    int choice;
    DataManager* system = nullptr;  // Sẽ được truyền vào từ bên ngoài
    
    do {
        cout << "\n╔════════════════════════════════════════════╗\n";
        cout << "║       MENU QUẢN LÝ - " << setw(24) << left << fullName << "║\n";
        cout << "╠════════════════════════════════════════════╣\n";
        cout << "║ 0. QUẢN LÝ TÀI KHOẢN                       ║\n";
        cout << "║    1. Xem hồ sơ của tôi                    ║\n";
        cout << "║    2. Chỉnh sửa thông tin                  ║\n";
        cout << "║    3. Đổi mật khẩu                         ║\n";
        cout << "╠════════════════════════════════════════════╣\n";
        cout << "║ 1. QUẢN LÝ XE                              ║\n";
        cout << "║    4. Xe vào (Check-in)                    ║\n";
        cout << "║    5. Xe ra (Check-out)                    ║\n";
        cout << "║    6. Tìm kiếm xe                          ║\n";
        cout << "║    7. In vé gửi xe                         ║\n";
        cout << "║    8. Xem tất cả xe                        ║\n";
        cout << "╠════════════════════════════════════════════╣\n";
        cout << "║ 2. QUẢN LÝ CHỖ ĐỖ                         ║\n";
        cout << "║    9. Xem tất cả chỗ                       ║\n";
        cout << "║   10. Xem chỗ trống                        ║\n";
        cout << "║   11. Xem chỗ đã có xe                     ║\n";
        cout << "╠════════════════════════════════════════════╣\n";
        cout << "║ 3. QUẢN LÝ KHÁCH HÀNG                      ║\n";
        cout << "║   12. Xem tất cả khách hàng                ║\n";
        cout << "║   13. Tìm khách hàng                       ║\n";
        cout << "║   14. Xem chi tiết khách hàng              ║\n";
        cout << "║   15. Cập nhật thông tin khách hàng        ║\n";
        cout << "║   16. Kích hoạt/Vô hiệu hóa khách hàng     ║\n";
        cout << "╠════════════════════════════════════════════╣\n";
        cout << "║ 4. QUẢN LÝ GIÁ CƯỚC                        ║\n";
        cout << "║   17. Xem bảng giá hiện tại                ║\n";
        cout << "║   18. Cập nhật giá xe máy                  ║\n";
        cout << "║   19. Cập nhật giá ô tô                    ║\n";
        cout << "║   20. Cập nhật giá xe đạp điện             ║\n";
        cout << "╠════════════════════════════════════════════╣\n";
        cout << "║ 5. THỐNG KÊ & BÁO CÁO                      ║\n";
        cout << "║   21. Thống kê của tôi                     ║\n";
        cout << "║   22. Báo cáo ngày                         ║\n";
        cout << "║   23. Báo cáo tháng                        ║\n";
        cout << "║   24. Báo cáo doanh thu                    ║\n";
        cout << "╠════════════════════════════════════════════╣\n";
        cout << "║ 0. Đăng xuất                               ║\n";
        cout << "╚════════════════════════════════════════════╝\n";
        cout << "Chọn chức năng: ";
        cin >> choice;
        cin.ignore();
        
        // Xử lý menu...
        
    } while (choice != 0);
}

// ===== NHÓM 0: QUẢN LÝ TÀI KHOẢN =====

void Manager::viewMyProfile() {
    display();
    
    cout << "\n╔════════════════════════════════════════════╗\n";
    cout << "║          THỐNG KÊ CÔNG VIỆC                ║\n";
    cout << "╠════════════════════════════════════════════╣\n";
    cout << "║ Tổng xe vào:     " << setw(25) << left << totalCheckIns << "║\n";
    cout << "║ Tổng xe ra:      " << setw(25) << left << totalCheckOuts << "║\n";
    cout << "║ Doanh thu:       " << setw(25) << left << (to_string((long long)totalRevenue) + " VNĐ") << "║\n";
    cout << "║ Khách quản lý:   " << setw(25) << left << customersManaged << "║\n";
    cout << "╚════════════════════════════════════════════╝\n";
}

void Manager::editMyProfile() {
    int choice;
    cout << "\n=== CHỈNH SỬA THÔNG TIN ===\n";
    cout << "1. Đổi họ tên\n";
    cout << "2. Đổi số điện thoại\n";
    cout << "3. Đổi email\n";
    cout << "4. Đổi ca làm\n";
    cout << "0. Quay lại\n";
    cout << "Chọn: ";
    cin >> choice;
    cin.ignore();
    
    string newValue;
    switch(choice) {
        case 1:
            cout << "Họ tên mới: ";
            getline(cin, newValue);
            updateMyFullName(newValue);
            cout << "✓ Cập nhật thành công!\n";
            break;
        case 2:
            cout << "SĐT mới: ";
            getline(cin, newValue);
            updateMyPhone(newValue);
            cout << "✓ Cập nhật thành công!\n";
            break;
        case 3:
            cout << "Email mới: ";
            getline(cin, newValue);
            updateMyEmail(newValue);
            cout << "✓ Cập nhật thành công!\n";
            break;
        case 4:
            cout << "Ca làm mới (Sáng/Chiều/Tối): ";
            getline(cin, newValue);
            setShift(newValue);
            cout << "✓ Cập nhật thành công!\n";
            break;
    }
}

void Manager::updateMyFullName(string newName) {
    fullName = newName;
}

void Manager::updateMyPhone(string newPhone) {
    phone = newPhone;
}

void Manager::updateMyEmail(string newEmail) {
    email = newEmail;
}

void Manager::changeMyPassword() {
    string oldPass, newPass, confirmPass;
    
    cout << "Mật khẩu cũ: ";
    getline(cin, oldPass);
    
    if (!checkPassword(oldPass)) {
        cout << "❌ Mật khẩu cũ không đúng!\n";
        return;
    }
    
    cout << "Mật khẩu mới: ";
    getline(cin, newPass);
    
    cout << "Xác nhận mật khẩu: ";
    getline(cin, confirmPass);
    
    if (newPass != confirmPass) {
        cout << "❌ Mật khẩu xác nhận không khớp!\n";
        return;
    }
    
    setPassword(newPass);
    cout << "✓ Đổi mật khẩu thành công!\n";
}

// ===== NHÓM 1: QUẢN LÝ XE =====

void Manager::checkInVehicle(Vehicle* vehicle, ParkingTicket* ticket) {
    // Method này sẽ được gọi từ ParkingSystem
    // Logic thực tế nằm trong ParkingSystem::checkInVehicle()
    cout << "✓ Check-in thành công cho xe: " << vehicle->getLicensePlate() << "\n";
    incrementCheckIns();
    incrementCustomersManaged();
}

void Manager::checkOutVehicle(string vehicleID) {
    // Method này sẽ được gọi từ ParkingSystem
    // Logic thực tế nằm trong ParkingSystem::checkOutVehicle()
    cout << "✓ Check-out thành công!\n";
    incrementCheckOuts();
}

void Manager::searchVehicle(string keyword) {
    // Gọi ParkingSystem để tìm kiếm
    cout << "Đang tìm kiếm xe với từ khóa: " << keyword << "\n";
}

void Manager::printTicket(string ticketID) {
    // In vé
    cout << "In vé số: " << ticketID << "\n";
}

void Manager::reprintTicket(string ticketID) {
    // In lại vé
    cout << "In lại vé số: " << ticketID << "\n";
}

void Manager::viewAllVehicles() {
    cout << "\n=== DANH SÁCH TẤT CẢ XE ===\n";
    // Gọi ParkingSystem để lấy danh sách
}

void Manager::viewVehiclesByType(string type) {
    cout << "\n=== DANH SÁCH XE LOẠI: " << type << " ===\n";
    // Gọi ParkingSystem để lấy danh sách theo loại
}

// ===== NHÓM 2: QUẢN LÝ SLOT =====

void Manager::manageSlots() {
    cout << "\n=== QUẢN LÝ CHỖ ĐỖ XE ===\n";
}

void Manager::viewAllSlots() {
    cout << "\n=== TẤT CẢ CHỖ ĐỖ ===\n";
}

void Manager::viewEmptySlots() {
    cout << "\n=== CHỖ ĐỖ TRỐNG ===\n";
}

void Manager::viewOccupiedSlots() {
    cout << "\n=== CHỖ ĐÃ CÓ XE ===\n";
}

void Manager::viewSlotsByZone(string zone) {
    cout << "\n=== CHỖ ĐỖ KHU VỰC: " << zone << " ===\n";
}

// ===== NHÓM 3: QUẢN LÝ KHÁCH HÀNG =====

void Manager::viewAllCustomers() {
    cout << "\n=== DANH SÁCH KHÁCH HÀNG ===\n";
}

void Manager::searchCustomer(string keyword) {
    cout << "Tìm kiếm khách hàng: " << keyword << "\n";
}

void Manager::viewCustomerDetails(string customerID) {
    cout << "Chi tiết khách hàng: " << customerID << "\n";
}

void Manager::updateCustomerInfo(string customerID) {
    cout << "Cập nhật thông tin khách: " << customerID << "\n";
}

void Manager::activateCustomer(string customerID) {
    cout << "✓ Kích hoạt khách hàng: " << customerID << "\n";
}

void Manager::deactivateCustomer(string customerID) {
    cout << "✓ Vô hiệu hóa khách hàng: " << customerID << "\n";
}

void Manager::resetCustomerPassword(string customerID) {
    cout << "✓ Reset mật khẩu cho khách: " << customerID << "\n";
}

void Manager::adjustCustomerMembership(string customerID, string newLevel) {
    cout << "✓ Điều chỉnh membership: " << customerID << " -> " << newLevel << "\n";
}

void Manager::viewCustomerHistory(string customerID) {
    cout << "\n=== LỊCH SỬ GỬI XE - " << customerID << " ===\n";
}

void Manager::addNoteToCustomer(string customerID, string note) {
    cout << "✓ Đã thêm ghi chú cho khách: " << customerID << "\n";
}

// ===== NHÓM 4: GIÁ CƯỚC =====

void Manager::viewCurrentPricing() {
    cout << "\n╔════════════════════════════════════════════╗\n";
    cout << "║          BẢNG GIÁ HIỆN TẠI                 ║\n";
    cout << "╠════════════════════════════════════════════╣\n";
    cout << "║ XE MÁY:                                    ║\n";
    cout << "║   - Theo giờ: 5,000 VNĐ/giờ                ║\n";
    cout << "║   - Theo ngày: 50,000 VNĐ/ngày             ║\n";
    cout << "╠════════════════════════════════════════════╣\n";
    cout << "║ Ô TÔ:                                      ║\n";
    cout << "║   - Theo giờ: 15,000 VNĐ/giờ               ║\n";
    cout << "║   - Theo ngày: 150,000 VNĐ/ngày            ║\n";
    cout << "╠════════════════════════════════════════════╣\n";
    cout << "║ XE ĐẠP ĐIỆN:                               ║\n";
    cout << "║   - Theo giờ: 3,000 VNĐ/giờ                ║\n";
    cout << "║   - Theo ngày: 30,000 VNĐ/ngày             ║\n";
    cout << "╚════════════════════════════════════════════╝\n";
}

void Manager::updateMotorcyclePrice(double hourlyRate, double dailyMax) {
    cout << "✓ Cập nhật giá xe máy: " << hourlyRate << " VNĐ/h, " 
         << dailyMax << " VNĐ/ngày\n";
}

void Manager::updateCarPrice(double hourlyRate, double dailyMax) {
    cout << "✓ Cập nhật giá ô tô: " << hourlyRate << " VNĐ/h, " 
         << dailyMax << " VNĐ/ngày\n";
}

void Manager::updateElectricBikePrice(double hourlyRate, double dailyMax) {
    cout << "✓ Cập nhật giá xe đạp điện: " << hourlyRate << " VNĐ/h, " 
         << dailyMax << " VNĐ/ngày\n";
}

// ===== NHÓM 5: THỐNG KÊ =====

void Manager::viewMyStatistics() {
    viewMyProfile();
}

void Manager::viewDailyReport() {
    cout << "\n=== BÁO CÁO NGÀY ===\n";
    // Gọi ParkingSystem::generateDailyReport()
}

void Manager::viewMonthlyReport() {
    cout << "\n=== BÁO CÁO THÁNG ===\n";
}

void Manager::viewRevenueReport() {
    cout << "\n=== BÁO CÁO DOANH THU ===\n";
    cout << "Doanh thu của tôi: " << totalRevenue << " VNĐ\n";
}

void Manager::viewCustomerReport() {
    cout << "\n=== BÁO CÁO KHÁCH HÀNG ===\n";
}

void Manager::exportReport(string filename) {
    cout << "✓ Xuất báo cáo ra file: " << filename << "\n";
}