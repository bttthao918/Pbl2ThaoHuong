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
    std::string slotNumber;
    VehicleType suitableFor;
    SlotStatus status;
    std::string currentTicketId;

public:
    ParkingSlot();
    ParkingSlot(const std::string &id, const std::string &number, VehicleType type);

    std::string getSlotId() const { return slotId; }
    std::string getSlotNumber() const { return slotNumber; }
    VehicleType getSuitableFor() const { return suitableFor; }
    SlotStatus getStatus() const { return status; }
    std::string getCurrentTicketId() const { return currentTicketId; }

    void setStatus(SlotStatus s) { status = s; }
    void setCurrentTicketId(const std::string &ticketId) { currentTicketId = ticketId; }

    bool isAvailable() const { return status == SlotStatus::AVAILABLE; }
    bool canAccommodate(VehicleType type) const;
    void occupy(const std::string &ticketId);
    void release();
    void reserve();
    void setMaintenance();

    void displayInfo() const;
    void displayTableRow() const;
    std::string toFileString() const;
    void fromFileString(const std::string &line);

    friend std::ostream &operator<<(std::ostream &os, const ParkingSlot &slot);
    bool operator==(const ParkingSlot &other) const;

    static std::string statusToString(SlotStatus status);
    static SlotStatus stringToStatus(const std::string &str);
};

#endif