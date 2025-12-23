#include "UI.h"
#include <iostream>
#include <limits>
#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif
using namespace std;

// Print horizontal line with corners
void UI::printRow(const string &leftPart, const string &value, int totalWidth)
{
    int contentWidth = (totalWidth > 0) ? (totalWidth - 2) : (boxWidth - 2);
    size_t barPos = leftPart.find('|');
    if (barPos != string::npos)
    {
        string prefix = leftPart.substr(0, barPos + 1);
        string afterBar = leftPart.substr(barPos + 1);
        string content = afterBar + value;

        int contentLen = static_cast<int>(content.length());
        int padding = contentWidth - contentLen;
        if (padding < 0)
            padding = 0;

        cout << prefix << content << string(padding, ' ') << "|" << endl;
        return;
    }
    string content = leftPart + value;
    int rightPad = boxWidth - 3 - static_cast<int>(content.length());
    if (rightPad < 0)
        rightPad = 0;

    cout << string(leftPadding, ' ') << "| " << leftPart << value
         << string(rightPad, ' ') << "|" << endl;
}

void UI::printBorder(const int widths[], int count)
{
    cout << "          +";
    for (int i = 0; i < count; ++i)
    {
        cout << string(widths[i], '-') << "+";
    }
    cout << endl;
}

void UI::printHeader(const string headers[], const int widths[], int count)
{
    cout << "          |";
    for (int i = 0; i < count; ++i)
    {
        cout << " " << setw(widths[i] - 1) << left << headers[i] << "|";
    }
    cout << endl;
}

void UI::printTableRow(const string cols[], const int widths[], int count)
{
    cout << "          |";
    for (int i = 0; i < count; ++i)
    {
        cout << " " << setw(widths[i] - 1) << left << cols[i] << "|";
    }
    cout << endl;
}

void UI::printHorizontalLine(char left, char mid, char right)
{
    cout << string(leftPadding, ' ') << left;
    for (int i = 0; i < boxWidth - 2; i++)
        cout << mid;
    cout << right << endl;
}

// Print empty line inside box
void UI::printEmptyLine()
{
    cout << string(leftPadding, ' ') << "|" << string(boxWidth - 2, ' ') << "|" << endl;
}

// Print centered text inside box
void UI::printCenteredText(const string &text, const string &color)
{
    int textLen = text.length();
    int totalPad = boxWidth - 2 - textLen;
    int leftPad = totalPad / 2;
    int rightPad = totalPad - leftPad;

    cout << string(leftPadding, ' ') << "|" << string(leftPad, ' ');
    if (!color.empty())
        cout << color;
    cout << text;
    if (!color.empty())
        cout << Color::RESET;
    cout << string(rightPad, ' ') << "|" << endl;
}

string UI::inputBoxString(const string &label)
{
    string input;

    cout << string(leftPadding, ' ') << "+";
    cout << string(boxWidth - 2, '-') << "+\n";

    cout << string(leftPadding, ' ') << "| " << label;
    cout.flush();

    getline(cin, input);
    cout << "\033[1A"; 
    cout << "\033[K";  
    cout << string(leftPadding, ' ') << "| " << label << input;

    int usedSpace = 2 + label.length() + input.length() + 1; 
    int pad = boxWidth - usedSpace;
    if (pad < 0)
        pad = 0;

    cout << string(pad, ' ') << "|\n";

    cout << string(leftPadding, ' ') << "+";
    cout << string(boxWidth - 2, '-') << "+\n";

    return input;
}

int UI::inputBoxInt(const string &label)
{
    while (true)
    {
        string s = inputBoxString(label);

        try
        {
            return stoi(s);
        }
        catch (...)
        {
            showErrorMessage("Gia tri khong hop le! Hay nhap so nguyen.");
        }
    }
}

double UI::inputBoxDouble(const string &label)
{
    while (true)
    {
        string s = inputBoxString(label);

        try
        {
            return stod(s);
        }
        catch (...)
        {
            showErrorMessage("Gia tri khong hop le! Hay nhap so thuc.");
        }
    }
}

void UI::printInputLine(const string &label, const string &input)
{
    string text = label + input;
    int padR = boxWidth - 2 - text.length();
    if (padR < 0)
        padR = 0;

    cout << string(leftPadding, ' ') << "| "
         << text
         << string(padR, ' ')
         << "|" << endl;
}


string UI::getHiddenInput()
{
    string pwd;
#ifdef _WIN32
    char ch;
    while ((ch = _getch()) != '\r')
    {
        if (ch == '\b' && !pwd.empty())
        {
            pwd.pop_back();
            cout << "\b \b";
        }
        else if (ch != '\b')
        {
            pwd += ch;
            cout << '*';
        }
    }
    cout << endl;
#else
    termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    getline(cin, pwd);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif
    return pwd;
}

// ============== MAIN MENU ==============
void UI::showMainMenu()
{
    cout << endl
         << endl;
    printHorizontalLine('+', '-', '+');
    printEmptyLine();
    printCenteredText("HE THONG QUAN LY BAI DO XE", Color::CYAN);
    printEmptyLine();
    printHorizontalLine('+', '-', '+');
    printEmptyLine();
    printCenteredText("1. DANG NHAP");
    printEmptyLine();
    printCenteredText("2. DANG KY");
    printEmptyLine();
    printCenteredText("0. THOAT");
    printEmptyLine();
    printHorizontalLine('+', '-', '+');
}

// ============== LOGIN SCREEN ==============
void UI::showLoginScreen()
{
    cout << endl
         << endl;
    printHorizontalLine('+', '-', '+');
    printEmptyLine();
    printCenteredText("DANG NHAP", Color::CYAN);
    printEmptyLine();
    printHorizontalLine('+', '-', '+');
}

string UI::inputBoxPassword(const string &label)
{
    cout << string(leftPadding, ' ') << "+";
    cout << string(boxWidth - 2, '-') << "+\n";

    cout << string(leftPadding, ' ') << "| " << label;
    cout.flush();

    string pwd = getHiddenInput();

    cout << "\033[1A"; 
    cout << "\033[K"; 

    string masked(pwd.length(), '*');
    cout << string(leftPadding, ' ') << "| " << label << masked;

    int usedSpace = 2 + label.length() + masked.length() + 1;
    int pad = boxWidth - usedSpace;
    if (pad < 0)
        pad = 0;

    cout << string(pad, ' ') << "|\n";

    cout << string(leftPadding, ' ') << "+";
    cout << string(boxWidth - 2, '-') << "+\n";

    return pwd;
}

// ============== REGISTER SCREEN ==============
void UI::showRegisterScreen()
{
    cout << endl
         << endl;
    printHorizontalLine('+', '-', '+');
    printEmptyLine();
    printCenteredText("DANG KY TAI KHOAN", Color::CYAN);
    printEmptyLine();
    printHorizontalLine('+', '-', '+');
    printEmptyLine();
    printCenteredText("1. Khach hang");
    printEmptyLine();
    printCenteredText("2. Quan ly");
    printEmptyLine();
    printCenteredText("0. Quay lai");
    printEmptyLine();
    printHorizontalLine('+', '-', '+');
}

// ============== CUSTOMER MENU ==============
void UI::showCustomerMenu(const string &userName)
{
    cout << endl
         << endl;
    printHorizontalLine('+', '-', '+');
    printEmptyLine();
    printCenteredText("MENU KHACH HANG", Color::CYAN);
    printEmptyLine();
    printHorizontalLine('+', '-', '+');
    printRow("  Xin chao: ", userName);
    printHorizontalLine('+', '-', '+');
    printEmptyLine();
    printCenteredText("1. Quan ly xe cua toi");
    printEmptyLine();
    printCenteredText("2. Dat cho truoc");
    printEmptyLine();
    printCenteredText("3. Xem bang gia gui xe");
    printEmptyLine();
    printCenteredText("4. Xem lich su gui xe");
    printEmptyLine();
    printCenteredText("5. Quan ly tai khoan");
    printEmptyLine();
    printCenteredText("0. Dang xuat");
    printEmptyLine();
    printHorizontalLine('+', '-', '+');
}

void UI::showVehicleManagementMenu()
{
    cout << endl
         << endl;
    printHorizontalLine('+', '-', '+');
    printEmptyLine();
    printCenteredText("QUAN LY XE CUA TOI", Color::CYAN);
    printEmptyLine();
    printHorizontalLine('+', '-', '+');
    printEmptyLine();
    printCenteredText("1. Dang ky xe moi");
    printEmptyLine();
    printCenteredText("2. Xem danh sach xe");
    printEmptyLine();
    printCenteredText("3. Cap nhat thong tin xe");
    printEmptyLine();
    printCenteredText("4. Xoa xe");
    printEmptyLine();
    printCenteredText("0. Quay lai");
    printEmptyLine();
    printHorizontalLine('+', '-', '+');
}

void UI::showBookingManagementMenu()
{
    cout << endl
         << endl;
    printHorizontalLine('+', '-', '+');
    printEmptyLine();
    printCenteredText("DAT CHO TRUOC", Color::CYAN);
    printEmptyLine();
    printHorizontalLine('+', '-', '+');
    printEmptyLine();
    printCenteredText("1. Dat cho moi");
    printEmptyLine();
    printCenteredText("2. Xem danh sach dat cho");
    printEmptyLine();
    printCenteredText("3. Huy dat cho");
    printEmptyLine();
    printCenteredText("0. Quay lai");
    printEmptyLine();
    printHorizontalLine('+', '-', '+');
}

void UI::showAccountManagementMenu()
{
    cout << endl
         << endl;
    printHorizontalLine('+', '-', '+');
    printEmptyLine();
    printCenteredText("QUAN LY TAI KHOAN", Color::CYAN);
    printEmptyLine();
    printHorizontalLine('+', '-', '+');
    printEmptyLine();
    printCenteredText("1. Xem thong tin tai khoan");
    printEmptyLine();
    printCenteredText("2. Cap nhat thong tin");
    printEmptyLine();
    printCenteredText("3. Doi mat khau");
    printEmptyLine();
    printCenteredText("0. Quay lai");
    printEmptyLine();
    printHorizontalLine('+', '-', '+');
}

// ============== ADMIN MENU ==============
void UI::showAdminMenu(const string &userName)
{
    cout << endl
         << endl;
    printHorizontalLine('+', '-', '+');
    printEmptyLine();
    printCenteredText("MENU QUAN LY", Color::CYAN);
    printEmptyLine();
    printHorizontalLine('+', '-', '+');
    printRow("  Xin chao: ", userName);
    printHorizontalLine('+', '-', '+');
    printEmptyLine();
    printCenteredText("1. Quan ly khach hang");
    printEmptyLine();
    printCenteredText("2. Quan ly xe");
    printEmptyLine();
    printCenteredText("3. Quan ly cho do");
    printEmptyLine();
    printCenteredText("4. Quan ly don dat cho");
    printEmptyLine();
    printCenteredText("5. Quan ly ve xe");
    printEmptyLine();
    printCenteredText("6. Thong ke va bao cao");
    printEmptyLine();
    printCenteredText("0. Dang xuat");
    printEmptyLine();
    printHorizontalLine('+', '-', '+');
}

void UI::showUserManagementMenu()
{
    cout << endl
         << endl;
    printHorizontalLine('+', '-', '+');
    printEmptyLine();
    printCenteredText("QUAN LY KHACH HANG", Color::CYAN);
    printEmptyLine();
    printHorizontalLine('+', '-', '+');
    printEmptyLine();
    printCenteredText("1. Xem tat ca khach hang");
    printEmptyLine();
    printCenteredText("2. Cap nhat thong tin khach hang");
    printEmptyLine();
    printCenteredText("3. Them khach hang moi");
    printEmptyLine();
    printCenteredText("4. Xoa khach hang");
    printEmptyLine();
    printCenteredText("5. Tim kiem khach hang");
    printEmptyLine();
    printCenteredText("0. Quay lai");
    printEmptyLine();
    printHorizontalLine('+', '-', '+');
}

void UI::showAdminVehicleMenu()
{
    cout << endl
         << endl;
    printHorizontalLine('+', '-', '+');
    printEmptyLine();
    printCenteredText("QUAN LY XE", Color::CYAN);
    printEmptyLine();
    printHorizontalLine('+', '-', '+');
    printEmptyLine();
    printCenteredText("1. Xem tat ca xe");
    printEmptyLine();
    printCenteredText("2. Tim kiem xe");
    printEmptyLine();
    printCenteredText("0. Quay lai");
    printEmptyLine();
    printHorizontalLine('+', '-', '+');
}

void UI::showSlotManagementMenu()
{
    cout << endl
         << endl;
    printHorizontalLine('+', '-', '+');
    printEmptyLine();
    printCenteredText("QUAN LY BAI DO XE", Color::CYAN);
    printEmptyLine();
    printHorizontalLine('+', '-', '+');
    printEmptyLine();
    printCenteredText("1. Xem tat ca cho do");
    printEmptyLine();
    printCenteredText("2. Cap nhat trang thai cho do");
    printEmptyLine();
    printCenteredText("3. Xem cho trong");
    printEmptyLine();
    printCenteredText("0. Quay lai");
    printEmptyLine();
    printHorizontalLine('+', '-', '+');
}

void UI::showAdminBookingMenu()
{
    cout << endl
         << endl;
    printHorizontalLine('+', '-', '+');
    printEmptyLine();
    printCenteredText("QUAN LY DON DAT CHO", Color::CYAN);
    printEmptyLine();
    printHorizontalLine('+', '-', '+');
    printEmptyLine();
    printCenteredText("1. Xem tat ca don dat cho");
    printEmptyLine();
    printCenteredText("2. Huy don dat cho");
    printEmptyLine();
    printCenteredText("3. Check-in don dat cho");
    printEmptyLine();
    printCenteredText("4. Check-out don dat cho");
    printEmptyLine();
    printCenteredText("0. Quay lai");
    printEmptyLine();
    printHorizontalLine('+', '-', '+');
}

void UI::showTicketManagementMenu()
{
    cout << endl
         << endl;
    printHorizontalLine('+', '-', '+');
    printEmptyLine();
    printCenteredText("QUAN LY VE XE", Color::CYAN);
    printEmptyLine();
    printHorizontalLine('+', '-', '+');
    printEmptyLine();
    printCenteredText("1. Xem tat ca ve xe");
    printEmptyLine();
    printCenteredText("2. Quan ly gia ve");
    printEmptyLine();
    printCenteredText("3. Tim kiem ve xe");
    printEmptyLine();
    printCenteredText("0. Quay lai");
    printEmptyLine();
    printHorizontalLine('+', '-', '+');
}

void UI::showStatisticsMenu()
{
    cout << endl
         << endl;
    printHorizontalLine('+', '-', '+');
    printEmptyLine();
    printCenteredText("THONG KE & BAO CAO", Color::CYAN);
    printEmptyLine();
    printHorizontalLine('+', '-', '+');
    printEmptyLine();
    printCenteredText("1. Bao cao hoat dong");
    printEmptyLine();
    printCenteredText("2. Thong ke tong quat");
    printEmptyLine();
    printCenteredText("3. Doanh thu theo khoang thoi gian");
    printEmptyLine();
    printCenteredText("0. Quay lai");
    printEmptyLine();
    printHorizontalLine('+', '-', '+');
}

// ============== MESSAGES ==============
void UI::showSuccessMessage(const string &msg)
{
    cout << endl;
    cout << string(leftPadding, ' ') << Color::GREEN << "[SUCCESS] " << msg << Color::RESET << endl;
}

void UI::showErrorMessage(const string &msg)
{
    cout << endl;
    cout << string(leftPadding, ' ') << Color::RED << "[ERROR] " << msg << Color::RESET << endl;
}

void UI::showInfoMessage(const string &msg)
{
    cout << endl;
    cout << string(leftPadding, ' ') << Color::YELLOW << "[INFO] " << msg << Color::RESET << endl;
}

void UI::showPricingTable(double motorcycle, double carStd, double carLux, double eBike, int minMins)
{
    cout << endl;
    printHorizontalLine('+', '-', '+');
    printEmptyLine();
    printCenteredText("BANG GIA GUI XE", Color::CYAN);
    printEmptyLine();
    printHorizontalLine('+', '-', '+');
    printRow("  1. Xe may:        ", to_string((int)motorcycle) + " VND/gio");
    printRow("  2. O to thuong:   ", to_string((int)carStd) + " VND/gio");
    printRow("  3. O to sang:     ", to_string((int)carLux) + " VND/gio");
    printRow("  4. Xe dap dien:   ", to_string((int)eBike) + " VND/gio");
    printEmptyLine();
    printRow("  Thoi gian toi thieu: ", to_string(minMins) + " phut");
    printHorizontalLine('+', '-', '+');
}

void UI::showReportHeader(const string &title)
{
    cout << endl;
    printHorizontalLine('+', '-', '+');
    printEmptyLine();
    printCenteredText(title, Color::CYAN);
    printEmptyLine();
    printHorizontalLine('+', '-', '+');
}

void UI::printAvailableSlots(int motorCount, int carCount, int electricCount)
{
    auto printSlot = [](const string &label, int count)
    {
        string text = label + ": " + to_string(count) + " cho";
        int padding = 90 - 2 - text.length();
        if (padding < 0)
            padding = 0;
        cout << "          |" << text << string(padding, ' ') << "|" << endl;
    };

    printSlot("  Xe may", motorCount);
    printSlot("  O to", carCount);
    printSlot("  Xe dap dien", electricCount);

    cout << "          +---------------------------------------------------------------------------------------+" << endl;
}

// Menu nhỏ

void UI::showReportDetailMenu()
{
    printHorizontalLine('+', '-', '+');
    printCenteredText("1. Theo ngay");
    printCenteredText("2. Theo thang");
    printCenteredText("0. Quay lai");
    printHorizontalLine('+', '-', '+');
}

void UI::showStatusSelectMenu()
{
    printHorizontalLine('+', '-', '+');
    printCenteredText("Chon trang thai:");
    printCenteredText("1. ACTIVE (dang gui)");
    printCenteredText("2. PAID (da thanh toan)");
    printCenteredText("3. CANCELLED (da huy)");
    printHorizontalLine('+', '-', '+');
}

void UI::showPriceChangeMenu()
{
    printHorizontalLine('+', '-', '+');
    printCenteredText("Thay doi gia dich vu");
    printCenteredText("1. Thay doi gia xe may");
    printCenteredText("2. Thay doi gia o to thuong");
    printCenteredText("3. Thay doi gia o to sang");
    printCenteredText("4. Thay doi gia xe dap dien");
    printCenteredText("5. Thay doi thoi gian toi thieu");
    printCenteredText("0. Quay lai");
    printHorizontalLine('+', '-', '+');
}

void UI::showTicketSearchMenu()
{
    printHorizontalLine('+', '-', '+');
    printCenteredText("1. Tim theo ID ve xe");
    printCenteredText("2. Tim theo bien so xe");
    printCenteredText("3. Tim theo ID khach hang");
    printCenteredText("4. Tim theo trang thai");
    printCenteredText("0. Quay lai");
    printHorizontalLine('+', '-', '+');
}
