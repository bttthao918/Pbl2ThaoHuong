#ifndef PARKING_SLOT_H
#define PARKING_SLOT_H

#include <string>
#include <iostream>
#include "Vehicle.h"

using namespace std;

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
    string slotId;
    string slotNumber;       // Số vị trí (A01, B12, ...)
    VehicleType suitableFor; // Loại xe phù hợp
    SlotStatus status;
    string currentTicketId; // ID của ticket hiện tại (nếu đang sử dụng)

public:
    // Constructor
    ParkingSlot();
    ParkingSlot(const string &id, const string &number, VehicleType type);

    // Getters
    string getSlotId() const { return slotId; }
    string getSlotNumber() const { return slotNumber; }
    VehicleType getSuitableFor() const { return suitableFor; }
    SlotStatus getStatus() const { return status; }
    string getCurrentTicketId() const { return currentTicketId; }

    // Setters
    void setStatus(SlotStatus s) { status = s; }
    void setCurrentTicketId(const string &ticketId) { currentTicketId = ticketId; }

    // Methods
    bool isAvailable() const { return status == SlotStatus::AVAILABLE; }
    bool canAccommodate(VehicleType type) const;
    void occupy(const string &ticketId);
    void release();
    void reserve();
    void setMaintenance();

    void displayInfo() const;
    string toFileString() const;
    void fromFileString(const string &line);

    // Operator overloading
    friend ostream &operator<<(ostream &os, const ParkingSlot &slot);
    bool operator==(const ParkingSlot &other) const;

    // Static methods
    static string statusToString(SlotStatus status);
    static SlotStatus stringToStatus(const string &str);
};

#endif