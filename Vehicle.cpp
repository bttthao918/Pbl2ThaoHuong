#include "Vehicle.h"
#include <iostream>
using namespace std;

Vehicle::Vehicle(string id, string owner, string phone,
                 string entry, string slot, string ticket)
    : vehicleID(id), ownerName(owner), phoneNumber(phone),
      entryTime(entry), exitTime(""), parkingSlotID(slot),
      ticketID(ticket) {}

void Vehicle::setExitTime(string exit) {
    exitTime = exit;
}
