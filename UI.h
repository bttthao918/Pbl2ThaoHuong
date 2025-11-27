#ifndef UI_H
#define UI_H

#include <iostream>
#include <string>
#include <iomanip>
#ifdef _WIN32
#include <conio.h>
#include <iostream>
#else
#include <termios.h>
#include <unistd.h>
#endif
#include <vector>
#include <memory>
#include "ParkingManager.h"
#include "Vehicle.h"
#include "Booking.h"
#include "ParkingTicket.h"
#include "User.h"

namespace Color
{
    const std::string RESET = "\033[0m";
    const std::string CYAN = "\033[36m";
    const std::string GREEN = "\033[32m";
    const std::string RED = "\033[31m";
    const std::string YELLOW = "\033[33m";
    const std::string BLUE = "\033[34m";
    const std::string MAGENTA = "\033[35m";
    const std::string WHITE = "\033[37m";
}

class UI
{
private:
    int boxWidth = 90;
    int leftPadding = 10;

public:
    UI() {}

    void printHorizontalLine(char left, char mid, char right);
    void printEmptyLine();
    void printCenteredText(const std::string &text, const std::string &color = "");

    std::string getHiddenInput();
    std::string inputBoxPassword(const std::string &label);
    std::string inputBoxString(const std::string &label);
    int inputBoxInt(const std::string &label);
    double inputBoxDouble(const std::string &label);

    void printRow(const std::string &leftPart, const std::string &value, int totalWidth = 90);
    void printInputLine(const std::string &label, const std::string &input);
    void printBorder(const int widths[], int count);
    void printHeader(const std::string headers[], const int widths[], int count);
    void printTableRow(const std::string cols[], const int widths[], int count);
    void showMainMenu();
    void showLoginScreen();
    void showRegisterScreen();

    void showCustomerMenu(const std::string &userName);
    void showVehicleManagementMenu();
    void showBookingManagementMenu();
    void showAccountManagementMenu();

    void showAdminMenu(const std::string &userName);
    void showUserManagementMenu();
    void showAdminVehicleMenu();
    void showSlotManagementMenu();
    void showAdminBookingMenu();
    void showTicketManagementMenu();
    void showStatisticsMenu();

    void showSuccessMessage(const std::string &msg);
    void showErrorMessage(const std::string &msg);
    void showInfoMessage(const std::string &msg);

    void showPricingTable(double motorcycle, double carStd, double carLux, double eBike, int minMins);
    void showTicketDetail(const std::string &title);
    void showReportHeader(const std::string &title);
    void showTableHeader(const std::vector<std::string> &headers);

    void printAvailableSlots(int motorCount, int carCount, int electricCount);
    void showReportDetailMenu();
    void showStatusSelectMenu();
    void showPriceChangeMenu();
    void showTicketSearchMenu();
};

#endif