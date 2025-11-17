#include "DataManager.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>

using namespace std;

// ==================== CONSTRUCTOR & DESTRUCTOR ====================

DataManager::DataManager(int slots) 
    : totalSlots(slots), occupiedSlots(0), nextTicketID(1000) {
    initializeSlots();
    initializeDefaultPricing();
}

DataManager::~DataManager() {
    // Giải phóng bộ nhớ
    managerList.for_each([](Manager*& m) { delete m; });
    customerList.for_each([](Customer*& c) { delete c; });
    vehicleList.for_each([](Vehicle*& v) { delete v; });
    slotList.for_each([](ParkingSlot*& s) { delete s; });
    ticketList.for_each([](ParkingTicket*& t) { delete t; });
}

// ==================== HELPER METHODS ====================

int DataManager::generateTicketID() {
    return ++nextTicketID;
}

string DataManager::generateTicketCode(int id) {
    stringstream ss;
    ss << "TK" << setfill('0') << setw(6) << id;
    return ss.str();
}

string DataManager::getCurrentDate() {
    time_t now = time(nullptr);
    struct tm* timeinfo = localtime(&now);
    char buffer[80];
    strftime(buffer, 80, "%Y-%m-%d", timeinfo);
    return string(buffer);
}

string DataManager::getCurrentTime() {
    time_t now = time(nullptr);
    struct tm* timeinfo = localtime(&now);
    char buffer[80];
    strftime(buffer, 80, "%H:%M:%S", timeinfo);
    return string(buffer);
}

// ==================== KHỞI TẠO HỆ THỐNG ====================

void DataManager::initializeSlots() {
    for (int i = 1; i <= totalSlots; i++) {
        ParkingSlot* slot = new ParkingSlot();
        slot->setSlotNumber(i);
        
        // Phân chia theo khu vực
        if (i <= 30) slot->setZone("A");
        else if (i <= 60) slot->setZone("B");
        else if (i <= 90) slot->setZone("C");
        else slot->setZone("D");
        
        slotList.push_back(slot);
    }
}

void DataManager::initializeDefaultPricing() {
    pricing.motorcycleHourly = 5000;
    pricing.motorcycleDailyMax = 50000;
    pricing.carHourly = 15000;
    pricing.carDailyMax = 150000;
    pricing.electricBikeHourly = 3000;
    pricing.electricBikeDailyMax = 30000;
}

void DataManager::initializeSampleData() {
    // Tạo Manager mẫu
    Manager* mgr1 = new Manager("MGR001", "admin", "admin123", 
                                "Nguyễn Văn A", "0901234567", 
                                "admin@parking.com", "Sáng", 15000000);
    addManager(mgr1);
    Manager* mgr2 = new Manager("MGR002", "manager2", "pass123",
        "Trần Thị B", "0907654321",
        "tranb@parking.com", "Chiều", 12000000);
addManager(mgr2);

// Tạo Customer mẫu
Customer* cust1 = new Customer("CUST001", "customer1", "pass123",
           "Lê Văn C", "0912345678",
           "levanc@gmail.com");
addCustomer(cust1);

Customer* cust2 = new Customer("CUST002", "customer2", "pass123",
           "Phạm Thị D", "0923456789",
           "phamd@gmail.com");
addCustomer(cust2);

// Tạo Vehicle mẫu
Vehicle* v1 = new Motorcycle("29A-12345", "Lê Văn C", "Đỏ");
registerVehicle(v1);

Vehicle* v2 = new Car("30B-67890", "Phạm Thị D", "Trắng", 5);
registerVehicle(v2);

Vehicle* v3 = new ElectricBike("29C-11111", "Lê Văn C", "Xanh");
registerVehicle(v3);
}

// ==================== NHÓM 1: QUẢN LÝ MANAGER ====================

void DataManager::addManager(Manager* mgr) {
Manager** existing = managerList.find_if([&](Manager* const& m) {
return m->getManagerID() == mgr->getManagerID();
});

if (existing) {
throw DuplicateException("Mã Manager đã tồn tại!");
}

managerList.push_back(mgr);
}

Manager* DataManager::findManager(string managerID) {
Manager** found = managerList.find_if([&](Manager* const& m) {
return m->getManagerID() == managerID;
});
return found ? *found : nullptr;
}

Manager* DataManager::loginManager(string username, string password) {
Manager** found = managerList.find_if([&](Manager* const& m) {
return m->getUsername() == username && m->checkPassword(password);
});
return found ? *found : nullptr;
}

void DataManager::displayAllManagers() {
if (managerList.is_empty()) {
cout << "Chưa có Manager nào trong hệ thống!\n";
return;
}

cout << "\n╔════════════════════════════════════════════════════════════════╗\n";
cout << "║                   DANH SÁCH QUẢN LÝ                            ║\n";
cout << "╚════════════════════════════════════════════════════════════════╝\n";

managerList.for_each([](Manager*& m) {
m->display();
cout << "────────────────────────────────────────────────────────────────\n";
});
}

DoubleLinkedList<Manager*>* DataManager::getAllManagers() {
    return &managerList;
}

// ==================== NHÓM 2: QUẢN LÝ CUSTOMER ====================

void DataManager::addCustomer(Customer* cust) {
    Customer** existing = customerList.find_if([&](Customer* const& c) {
        return c->getPhone() == cust->getPhone();
    });
    
    if (existing) {
        throw DuplicateException("Số điện thoại đã được đăng ký!");
    }
    
    customerList.push_back(cust);
}

Customer* DataManager::findCustomer(string customerID) {
    Customer** found = customerList.find_if([&](Customer* const& c) {
        return c->getCustomerID() == customerID;
    });
    return found ? *found : nullptr;
}

Customer* DataManager::findCustomerByPhone(string phone) {
    Customer** found = customerList.find_if([&](Customer* const& c) {
        return c->getPhone() == phone;
    });
    return found ? *found : nullptr;
}

Customer* DataManager::loginCustomer(string username, string password) {
    Customer** found = customerList.find_if([&](Customer* const& c) {
        return c->getUsername() == username && c->checkPassword(password);
    });
    return found ? *found : nullptr;
}

void DataManager::activateCustomer(string customerID) {
    Customer* cust = findCustomer(customerID);
    if (cust) {
        cust->setActive(true);
    } else {
        throw NotFoundException("Không tìm thấy khách hàng!");
    }
}

void DataManager::deactivateCustomer(string customerID) {
    Customer* cust = findCustomer(customerID);
    if (cust) {
        cust->setActive(false);
    } else {
        throw NotFoundException("Không tìm thấy khách hàng!");
    }
}

void DataManager::displayAllCustomers() {
    if (customerList.is_empty()) {
        cout << "Chưa có khách hàng nào!\n";
        return;
    }
    
    cout << "\n╔════════════════════════════════════════════════════════════════╗\n";
    cout << "║                   DANH SÁCH KHÁCH HÀNG                         ║\n";
    cout << "╚════════════════════════════════════════════════════════════════╝\n";
    
    customerList.for_each([](Customer*& c) {
        c->display();
        cout << "────────────────────────────────────────────────────────────────\n";
    });
}

DoubleLinkedList<Customer*>* DataManager::getAllCustomers() {
    return &customerList;
}

// ==================== NHÓM 3: QUẢN LÝ VEHICLE ====================

void DataManager::registerVehicle(Vehicle* vehicle) {
    Vehicle** existing = vehicleList.find_if([&](Vehicle* const& v) {
        return v->getLicensePlate() == vehicle->getLicensePlate();
    });
    
    if (existing) {
        throw DuplicateException("Biển số xe đã được đăng ký!");
    }
    
    vehicleList.push_back(vehicle);
}

Vehicle* DataManager::findVehicle(string licensePlate) {
    Vehicle** found = vehicleList.find_if([&](Vehicle* const& v) {
        return v->getLicensePlate() == licensePlate;
    });
    return found ? *found : nullptr;
}

DoubleLinkedList<Vehicle*> DataManager::searchVehicleByKeyword(string keyword) {
    // Chuyển keyword về chữ thường
    transform(keyword.begin(), keyword.end(), keyword.begin(), ::tolower);
    
    return vehicleList.filter([&](Vehicle* const& v) {
        string plate = v->getLicensePlate();
        string owner = v->getOwnerName();
        
        transform(plate.begin(), plate.end(), plate.begin(), ::tolower);
        transform(owner.begin(), owner.end(), owner.begin(), ::tolower);
        
        return plate.find(keyword) != string::npos || 
               owner.find(keyword) != string::npos;
    });
}

void DataManager::displayAllVehicles() {
    if (vehicleList.is_empty()) {
        cout << "Chưa có phương tiện nào!\n";
        return;
    }
    
    cout << "\n╔════════════════════════════════════════════════════════════════╗\n";
    cout << "║                   DANH SÁCH PHƯƠNG TIỆN                        ║\n";
    cout << "╚════════════════════════════════════════════════════════════════╝\n";
    
    vehicleList.for_each([](Vehicle*& v) {
        v->display();
        cout << "────────────────────────────────────────────────────────────────\n";
    });
}

DoubleLinkedList<Vehicle*>* DataManager::getAllVehicles() {
    return &vehicleList;
}

// ==================== NHÓM 4: QUẢN LÝ PARKING SLOT ====================

ParkingSlot* DataManager::findEmptySlot() {
    ParkingSlot** slot = slotList.find_if([](ParkingSlot* const& s) {
        return !s->isOccupied();
    });
    return slot ? *slot : nullptr;
}

ParkingSlot* DataManager::findEmptySlotByType(string vehicleType) {
    ParkingSlot** slot = slotList.find_if([&](ParkingSlot* const& s) {
        return !s->isOccupied() && s->getVehicleType() == vehicleType;
    });
    
    // Nếu không tìm thấy slot theo loại, tìm slot trống bất kỳ
    if (!slot) {
        slot = slotList.find_if([](ParkingSlot* const& s) {
            return !s->isOccupied();
        });
    }
    
    return slot ? *slot : nullptr;
}
ParkingSlot* DataManager::findSlotByNumber(int slotNumber) {
    ParkingSlot** found = slotList.find_if([&](ParkingSlot* const& s) {
        return s->getSlotNumber() == slotNumber;
    });
    return found ? *found : nullptr;
}

void DataManager::occupySlot(int slotNumber, string licensePlate, string vehicleType) {
    ParkingSlot* slot = findSlotByNumber(slotNumber);
    if (!slot) {
        throw NotFoundException("Không tìm thấy chỗ đỗ!");
    }
    
    if (slot->isOccupied()) {
        throw InvalidDataException("Chỗ đỗ đã có xe!");
    }
    
    slot->occupy(licensePlate, vehicleType);
    occupiedSlots++;
}

void DataManager::releaseSlot(int slotNumber) {
    ParkingSlot* slot = findSlotByNumber(slotNumber);
    if (slot && slot->isOccupied()) {
        slot->release();
        occupiedSlots--;
    }
}

DoubleLinkedList<ParkingSlot*> DataManager::getEmptySlots() {
    return slotList.filter([](ParkingSlot* const& s) {
        return !s->isOccupied();
    });
}

DoubleLinkedList<ParkingSlot*> DataManager::getOccupiedSlots() {
    return slotList.filter([](ParkingSlot* const& s) {
        return s->isOccupied();
    });
}

void DataManager::displayAllSlots() {
    cout << "\n╔════════════════════════════════════════════════════════════════╗\n";
    cout << "║                   TRẠNG THÁI TẤT CẢ CHỖ ĐỖ                    ║\n";
    cout << "╚════════════════════════════════════════════════════════════════╝\n";
    
    slotList.for_each([](ParkingSlot*& s) {
        s->display();
    });
}

void DataManager::displayEmptySlots() {
    DoubleLinkedList<ParkingSlot*> emptySlots = getEmptySlots();
    
    cout << "\n╔════════════════════════════════════════════════════════════════╗\n";
    cout << "║                   CHỖ ĐỖ TRỐNG                                 ║\n";
    cout << "╚════════════════════════════════════════════════════════════════╝\n";
    cout << "Số chỗ trống: " << emptySlots.get_size() << "\n\n";
    
    emptySlots.for_each([](ParkingSlot*& s) {
        s->display();
    });
}

void DataManager::displayOccupiedSlots() {
    DoubleLinkedList<ParkingSlot*> occupiedSlots = getOccupiedSlots();
    
    cout << "\n╔════════════════════════════════════════════════════════════════╗\n";
    cout << "║                   CHỖ ĐÃ CÓ XE                                 ║\n";
    cout << "╚════════════════════════════════════════════════════════════════╝\n";
    cout << "Số chỗ đã dùng: " << occupiedSlots.get_size() << "\n\n";
    
    occupiedSlots.for_each([](ParkingSlot*& s) {
        s->display();
    });
}

DoubleLinkedList<ParkingSlot*>* DataManager::getAllSlots() {
    return &slotList;
}

// ==================== NHÓM 5: CHECK-IN / CHECK-OUT ====================

ParkingTicket* DataManager::checkIn(string licensePlate, string customerPhone, Manager* performedBy) {
    // 1. Kiểm tra xe đã đăng ký chưa
    Vehicle* vehicle = findVehicle(licensePlate);
    if (!vehicle) {
        throw NotFoundException("Xe chưa được đăng ký trong hệ thống!");
    }
    
    // 2. Kiểm tra xe đã trong bãi chưa
    ParkingTicket* existingTicket = findActiveTicketByPlate(licensePlate);
    if (existingTicket) {
        throw DuplicateException("Xe đang gửi trong bãi!");
    }
    
    // 3. Kiểm tra khách hàng
    Customer* customer = findCustomerByPhone(customerPhone);
    if (!customer) {
        throw NotFoundException("Khách hàng chưa đăng ký! Vui lòng đăng ký trước.");
    }
    
    if (!customer->isActive()) {
        throw InvalidDataException("Tài khoản khách hàng đã bị vô hiệu hóa!");
    }
    
    // 4. Tìm chỗ trống
    ParkingSlot* slot = findEmptySlotByType(vehicle->getTypeName());
    if (!slot) {
        throw FullCapacityException("Bãi đỗ đã đầy!");
    }
    
    // 5. Tạo vé mới
    int ticketID = generateTicketID();
    string ticketCode = generateTicketCode(ticketID);
    
    ParkingTicket* ticket = new ParkingTicket();
    ticket->setTicketID(ticketCode);
    ticket->setLicensePlate(licensePlate);
    ticket->setCustomerPhone(customerPhone);
    ticket->setCustomerName(customer->getFullName());
    ticket->setVehicleType(vehicle->getTypeName());
    ticket->setSlotNumber(slot->getSlotNumber());
    ticket->setCheckInTime(time(nullptr));
    ticket->setManagerID(performedBy->getManagerID());
    ticket->setManagerName(performedBy->getFullName());
    
    // 6. Cập nhật slot
    occupySlot(slot->getSlotNumber(), licensePlate, vehicle->getTypeName());
    
    // 7. Lưu vé
    ticketList.push_back(ticket);
    
    // 8. Cập nhật thống kê Manager
    performedBy->incrementCheckIns();
    performedBy->incrementCustomersManaged();
    
    return ticket;
}

double DataManager::checkOut(string ticketID, Manager* performedBy) {
    // 1. Tìm vé
    ParkingTicket* ticket = findTicket(ticketID);
    if (!ticket) {
        throw NotFoundException("Không tìm thấy vé!");
    }
    if (ticket->isCheckedOut()) {
        throw InvalidDataException("Vé đã được thanh toán!");
    }
    
    // 2. Tìm xe
    Vehicle* vehicle = findVehicle(ticket->getLicensePlate());
    if (!vehicle) {
        throw NotFoundException("Không tìm thấy thông tin xe!");
    }
    
    // 3. Tính phí
    double fee = calculateFee(ticket, vehicle);
    
    // 4. Cập nhật vé
    ticket->setCheckOutTime(time(nullptr));
    ticket->setFee(fee);
    ticket->setCheckedOut(true);
    
    // 5. Giải phóng slot
    releaseSlot(ticket->getSlotNumber());
    
    // 6. Cập nhật thống kê Manager
    performedBy->incrementCheckOuts();
    performedBy->addRevenue(fee);
    
    return fee;
}

ParkingTicket* DataManager::findTicket(string ticketID) {
    ParkingTicket** found = ticketList.find_if([&](ParkingTicket* const& t) {
        return t->getTicketID() == ticketID;
    });
    return found ? *found : nullptr;
}

ParkingTicket* DataManager::findActiveTicketByPlate(string licensePlate) {
    ParkingTicket** found = ticketList.find_if([&](ParkingTicket* const& t) {
        return t->getLicensePlate() == licensePlate && !t->isCheckedOut();
    });
    return found ? *found : nullptr;
}

void DataManager::printTicket(ParkingTicket* ticket) {
    cout << "\n╔══════════════════════════════════════════════════════════════╗\n";
    cout << "║              VÉ GỬI XE - PARKING TICKET                      ║\n";
    cout << "╠══════════════════════════════════════════════════════════════╣\n";
    ticket->display();
    cout << "╚══════════════════════════════════════════════════════════════╝\n";
}

void DataManager::reprintTicket(string ticketID) {
    ParkingTicket* ticket = findTicket(ticketID);
    if (ticket) {
        printTicket(ticket);
    } else {
        throw NotFoundException("Không tìm thấy vé!");
    }
}

// ==================== NHÓM 6: PRICING ====================

void DataManager::updateMotorcyclePricing(double hourly, double dailyMax) {
    pricing.motorcycleHourly = hourly;
    pricing.motorcycleDailyMax = dailyMax;
}

void DataManager::updateCarPricing(double hourly, double dailyMax) {
    pricing.carHourly = hourly;
    pricing.carDailyMax = dailyMax;
}

void DataManager::updateElectricBikePricing(double hourly, double dailyMax) {
    pricing.electricBikeHourly = hourly;
    pricing.electricBikeDailyMax = dailyMax;
}
double DataManager::calculateFee(ParkingTicket* ticket, Vehicle* vehicle) {
    // If manager already adjusted the fee on this ticket, use it
    if (ticket->wasAdjusted() && ticket->getFee() > 0) {
        return ticket->getFee();
    }

    int hours = ticket->calculateParkingHours();
    double hourlyRate = 0, dailyMax = 0;
    
    string type = vehicle->getTypeName();
    if (type == "Motorcycle" || type == "Xe máy") {
        hourlyRate = pricing.motorcycleHourly;
        dailyMax = pricing.motorcycleDailyMax;
    } else if (type == "Car" || type == "Ô tô") {
        hourlyRate = pricing.carHourly;
        dailyMax = pricing.carDailyMax;
    } else {
        hourlyRate = pricing.electricBikeHourly;
        dailyMax = pricing.electricBikeDailyMax;
    }
    
    double fee = hours * hourlyRate;
    
    // Tính theo ngày nếu quá 24h
    if (hours >= 24) {
        int days = hours / 24;
        int remainingHours = hours % 24;
        fee = (days * dailyMax) + (remainingHours * hourlyRate);
    }
    
    // Giới hạn phí tối đa trong ngày
    if (fee > dailyMax && hours < 24) {
        fee = dailyMax;
    }
    
    return fee;
}

void DataManager::displayCurrentPricing() {
    cout << "\n╔════════════════════════════════════════════════════════════════╗\n";
    cout << "║                    BẢNG GIÁ HIỆN TẠI                           ║\n";
    cout << "╠════════════════════════════════════════════════════════════════╣\n";
    cout << "║ XE MÁY:                                                        ║\n";
    cout << "║   - Theo giờ:  " << setw(44) << left << (to_string((int)pricing.motorcycleHourly) + " VNĐ/giờ") << "║\n";
    cout << "║   - Tối đa/ngày: " << setw(41) << left << (to_string((int)pricing.motorcycleDailyMax) + " VNĐ") << "║\n";
    cout << "╠════════════════════════════════════════════════════════════════╣\n";
    cout << "║ Ô TÔ:                                                          ║\n";
    cout << "║   - Theo giờ:  " << setw(44) << left << (to_string((int)pricing.carHourly) + " VNĐ/giờ") << "║\n";
    cout << "║   - Tối đa/ngày: " << setw(41) << left << (to_string((int)pricing.carDailyMax) + " VNĐ") << "║\n";
    cout << "╠════════════════════════════════════════════════════════════════╣\n";
    cout << "║ XE ĐẠP ĐIỆN:                                                   ║\n";
    cout << "║   - Theo giờ:  " << setw(44) << left << (to_string((int)pricing.electricBikeHourly) + " VNĐ/giờ") << "║\n";
    cout << "║   - Tối đa/ngày: " << setw(41) << left << (to_string((int)pricing.electricBikeDailyMax) + " VNĐ") << "║\n";
    cout << "╚════════════════════════════════════════════════════════════════╝\n";
}

PricingConfig DataManager::getCurrentPricing() {
    return pricing;
}

// ==================== NHÓM 7: THỐNG KÊ & BÁO CÁO ====================

int DataManager::getTotalSlots() {
    return totalSlots;
}

int DataManager::getOccupiedSlots() {
    return occupiedSlots;
}

int DataManager::getAvailableSlots() {
    return totalSlots - occupiedSlots;
}

double DataManager::getOccupancyRate() {
    return (double)occupiedSlots / totalSlots * 100.0;
}

int DataManager::getTotalVehicles() {
    return vehicleList.get_size();
}

int DataManager::getTotalCustomers() {
    return customerList.get_size();
}

int DataManager::getTotalManagers() {
    return managerList.get_size();
}

void DataManager::getManagerStatistics(string managerID, int& checkIns, int& checkOuts, double& revenue) {
    Manager* mgr = findManager(managerID);
    if (mgr) {
        checkIns = mgr->getTotalCheckIns();
        checkOuts = mgr->getTotalCheckOuts();
        revenue = mgr->getTotalRevenue();
    }
}

void DataManager::displayManagerStatistics(string managerID) {
    Manager* mgr = findManager(managerID);
    if (!mgr) {
        throw NotFoundException("Không tìm thấy Manager!");
    }
    
    mgr->viewMyStatistics();
}

void DataManager::generateSystemReport() {
    cout << "\n╔════════════════════════════════════════════════════════════════╗\n";
    cout << "║                  BÁO CÁO TỔNG QUAN HỆ THỐNG                    ║\n";
    cout << "╠════════════════════════════════════════════════════════════════╣\n";
    cout << "║ Tổng số chỗ đỗ:        " << setw(39) << left << totalSlots << "║\n";
    cout << "║ Đang sử dụng:          " << setw(39) << left << occupiedSlots << "║\n";
    cout << "║ Còn trống:             " << setw(39) << left << getAvailableSlots() << "║\n";
    cout << "║ Tỷ lệ lấp đầy:         " << setw(39) << left << (to_string((int)getOccupancyRate()) + "%") << "║\n";
    cout << "╠════════════════════════════════════════════════════════════════╣\n";
    cout << "║ Tổng phương tiện:      " << setw(39) << left << getTotalVehicles() << "║\n";
    cout << "║ Tổng khách hàng:       " << setw(39) << left << getTotalCustomers() << "║\n";
    cout << "║ Tổng nhân viên:        " << setw(39) << left << getTotalManagers() << "║\n";
    cout << "║ Tổng vé đã tạo:        " << setw(39) << left << ticketList.get_size() << "║\n";
    cout << "╚════════════════════════════════════════════════════════════════╝\n";
}

void DataManager::generateDailyReport() {
    cout << "\n╔════════════════════════════════════════════════════════════════╗\n";
    cout << "║                      BÁO CÁO NGÀY                              ║\n";
    cout << "║                  " << getCurrentDate() << "                          ║\n";
    cout << "╚════════════════════════════════════════════════════════════════╝\n";
    generateSystemReport();
}

DoubleLinkedList<ParkingTicket*> DataManager::getActiveTickets() {
    return ticketList.filter([](ParkingTicket* const& t) {
        return !t->isCheckedOut();
    });
}

DoubleLinkedList<ParkingTicket*> DataManager::getCompletedTickets() {
    return ticketList.filter([](ParkingTicket* const& t) {
        return t->isCheckedOut();
    });
}

void DataManager::displayActiveTickets() {
    DoubleLinkedList<ParkingTicket*> activeTickets = getActiveTickets();
    
    cout << "\n