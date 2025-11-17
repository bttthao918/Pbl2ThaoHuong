#include "Manager.h"
#include "DataManager.h"
#include <iostream>

using namespace std;

// Implement adjustTicketPrice: tìm vé, chỉnh phí và thông báo
void Manager::adjustTicketPrice(const string& ticketID, double newFee, DataManager* dm, const string& note) {
	if (!dm) {
		cout << "DataManager không tồn tại!" << endl;
		return;
	}

	ParkingTicket* ticket = dm->findTicket(ticketID);
	if (!ticket) {
		cout << "Không tìm thấy vé: " << ticketID << endl;
		return;
	}

	ticket->managerAdjustFee(newFee, this->getManagerID(), note);
	cout << "Đã cập nhật phí vé " << ticketID << " thành " << newFee << " (bởi Manager " << this->getManagerID() << ")\n";
}
