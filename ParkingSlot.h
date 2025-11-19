#ifndef PARKING_SLOT_H
#define PARKING_SLOT_H

#include <string>
#include <iostream>
#include "Vehicle.h"

enum class SlotStatus
{
    AVAILABLE,
    OCCUPIED,
    RESERVED,
    MAINTENANCE
};

class ParkingSlot
{
private:
    std::string slotId;
    std::string slotNumber;  // Số vị trí (A01, B12, ...)
    VehicleType suitableFor; // Loại xe phù hợp
    SlotStatus status;
    std::string currentTicketId; // ID của ticket hiện tại (nếu đang sử dụng)

public:
    // Constructor
    ParkingSlot();
    ParkingSlot(const std::string &id, const std::string &number, VehicleType type);

    // Getters
    std::string getSlotId() const { return slotId; }
    std::string getSlotNumber() const { return slotNumber; }
    VehicleType getSuitableFor() const { return suitableFor; }
    SlotStatus getStatus() const { return status; }
    std::string getCurrentTicketId() const { return currentTicketId; }

    // Setters
    void setStatus(SlotStatus s) { status = s; }
    void setCurrentTicketId(const std::string &ticketId) { currentTicketId = ticketId; }

    // Methods
    bool isAvailable() const { return status == SlotStatus::AVAILABLE; }
    bool canAccommodate(VehicleType type) const;
    void occupy(const std::string &ticketId);
    void release();
    void reserve();
    void setMaintenance();

    void displayInfo() const;
    std::string toFileString() const;
    void fromFileString(const std::string &line);

    // Operator overloading
    friend std::ostream &operator<<(std::ostream &os, const ParkingSlot &slot);
    bool operator==(const ParkingSlot &other) const;

    // Static methods
    static std::string statusToString(SlotStatus status);
    static SlotStatus stringToStatus(const std::string &str);
};

#endif