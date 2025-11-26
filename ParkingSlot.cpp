#include "ParkingSlot.h"
#include "Exceptions.h"
#include "UI.h"
#include <sstream>
#include <iomanip>
using namespace std;

extern UI ui;

ParkingSlot::ParkingSlot() : status(SlotStatus::AVAILABLE),
                             suitableFor(VehicleType::MOTORCYCLE) {}

ParkingSlot::ParkingSlot(const string &id, const string &number, VehicleType type)
    : slotId(id), slotNumber(number), suitableFor(type),
      status(SlotStatus::AVAILABLE), currentTicketId("") {}

bool ParkingSlot::canAccommodate(VehicleType type) const
{
    // Xe ô tô chỉ vào chỗ ô tô
    // Xe máy và xe đạp điện có thể vào cả chỗ xe máy và xe đạp điện
    if (type == VehicleType::CAR)
    {
        return suitableFor == VehicleType::CAR;
    }
    return suitableFor != VehicleType::CAR;
}

void ParkingSlot::occupy(const string &ticketId)
{
    if (status != SlotStatus::AVAILABLE && status != SlotStatus::RESERVED)
    {
        throw InvalidInputException("Cho do khong kha dung");
    }
    status = SlotStatus::OCCUPIED;
    currentTicketId = ticketId;
}

void ParkingSlot::release()
{
    status = SlotStatus::AVAILABLE;
    currentTicketId = "";
}

void ParkingSlot::reserve()
{
    if (status != SlotStatus::AVAILABLE)
    {
        throw InvalidInputException("Cho do khong the dat truoc");
    }
    status = SlotStatus::RESERVED;
}

void ParkingSlot::setMaintenance()
{
    if (status == SlotStatus::OCCUPIED)
    {
        throw InvalidInputException("Khong the bao tri khi dang co xe");
    }
    status = SlotStatus::MAINTENANCE;
    currentTicketId = "";
}

void ParkingSlot::displayInfo() const
{
    ui.printHorizontalLine('+', '-', '+');
    ui.printRow("          | Slot ID:        ", slotId);
    ui.printRow("          | So vi tri:      ", slotNumber);
    ui.printRow("          | Loai xe:        ", Vehicle::vehicleTypeToString(suitableFor));
    ui.printRow("          | Trang thai:     ", statusToString(status));
    if (!currentTicketId.empty())
    {
        ui.printRow("          | Ticket hien tai: ", currentTicketId);
    }
    ui.printHorizontalLine('+', '-', '+');
}

void ParkingSlot::displayTableRow() const
{
    // int widths[] = {22, 26, 20, 17};
    cout << "          | " << setw(20) << left << slotId
         << " | " << setw(24) << left << slotNumber
         << " | " << setw(18) << left << Vehicle::vehicleTypeToString(suitableFor)
         << " | " << setw(15) << left << statusToString(status)
         << " |" << endl;
}

string ParkingSlot::toFileString() const
{
    ostringstream oss;
    oss << slotId << "|" << slotNumber << "|"
        << Vehicle::vehicleTypeToString(suitableFor) << "|"
        << statusToString(status) << "|" << currentTicketId;
    return oss.str();
}

void ParkingSlot::fromFileString(const string &line)
{
    istringstream iss(line);
    string typeStr, statusStr;

    getline(iss, slotId, '|');
    getline(iss, slotNumber, '|');
    getline(iss, typeStr, '|');
    suitableFor = Vehicle::stringToVehicleType(typeStr);
    getline(iss, statusStr, '|');
    status = stringToStatus(statusStr);
    getline(iss, currentTicketId);
}

ostream &operator<<(ostream &os, const ParkingSlot &slot)
{
    os << "Slot " << slot.slotNumber << " (" << slot.statusToString(slot.status) << ")";
    return os;
}

bool ParkingSlot::operator==(const ParkingSlot &other) const
{
    return slotId == other.slotId;
}

string ParkingSlot::statusToString(SlotStatus status)
{
    switch (status)
    {
    case SlotStatus::AVAILABLE:
        return "AVAILABLE";
    case SlotStatus::OCCUPIED:
        return "OCCUPIED";
    case SlotStatus::RESERVED:
        return "RESERVED";
    case SlotStatus::MAINTENANCE:
        return "MAINTENANCE";
    default:
        return "UNKNOWN";
    }
}

SlotStatus ParkingSlot::stringToStatus(const string &str)
{
    if (str == "AVAILABLE")
        return SlotStatus::AVAILABLE;
    if (str == "OCCUPIED")
        return SlotStatus::OCCUPIED;
    if (str == "RESERVED")
        return SlotStatus::RESERVED;
    if (str == "MAINTENANCE")
        return SlotStatus::MAINTENANCE;
    return SlotStatus::AVAILABLE;
}