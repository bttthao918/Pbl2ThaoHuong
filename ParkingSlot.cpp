#include "ParkingSlot.h"
#include "Exceptions.h"
#include <sstream>

ParkingSlot::ParkingSlot() : status(SlotStatus::AVAILABLE),
                             suitableFor(VehicleType::MOTORCYCLE) {}

ParkingSlot::ParkingSlot(const std::string &id, const std::string &number, VehicleType type)
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

void ParkingSlot::occupy(const std::string &ticketId)
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
    std::cout << "Slot ID: " << slotId << std::endl;
    std::cout << "So vi tri: " << slotNumber << std::endl;
    std::cout << "Loai xe: " << Vehicle::vehicleTypeToString(suitableFor) << std::endl;
    std::cout << "Trang thai: " << statusToString(status) << std::endl;
    if (!currentTicketId.empty())
    {
        std::cout << "Ticket hien tai: " << currentTicketId << std::endl;
    }
}

std::string ParkingSlot::toFileString() const
{
    std::ostringstream oss;
    oss << slotId << "|" << slotNumber << "|"
        << Vehicle::vehicleTypeToString(suitableFor) << "|"
        << statusToString(status) << "|" << currentTicketId;
    return oss.str();
}

void ParkingSlot::fromFileString(const std::string &line)
{
    std::istringstream iss(line);
    std::string typeStr, statusStr;

    std::getline(iss, slotId, '|');
    std::getline(iss, slotNumber, '|');
    std::getline(iss, typeStr, '|');
    suitableFor = Vehicle::stringToVehicleType(typeStr);
    std::getline(iss, statusStr, '|');
    status = stringToStatus(statusStr);
    std::getline(iss, currentTicketId);
}

std::ostream &operator<<(std::ostream &os, const ParkingSlot &slot)
{
    os << "Slot " << slot.slotNumber << " (" << slot.statusToString(slot.status) << ")";
    return os;
}

bool ParkingSlot::operator==(const ParkingSlot &other) const
{
    return slotId == other.slotId;
}

std::string ParkingSlot::statusToString(SlotStatus status)
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

SlotStatus ParkingSlot::stringToStatus(const std::string &str)
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