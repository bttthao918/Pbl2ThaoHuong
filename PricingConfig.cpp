#include "PricingConfig.h"
#include "UI.h"
#include <fstream>
#include <iostream>
#include <iomanip>
using namespace std;

extern UI ui;

PricingConfig *PricingConfig::instance = nullptr;

PricingConfig::PricingConfig()
{
    motorcyclePricePerHour = 5000.0;
    carStandardPricePerHour = 15000.0;
    carLuxuryPricePerHour = 20000.0;
    electricBikePricePerHour = 3000.0;
    minimumMinutes = 30;

    loadFromFile();
}

PricingConfig *PricingConfig::getInstance()
{
    if (instance == nullptr)
    {
        instance = new PricingConfig();
    }
    return instance;
}

void PricingConfig::setMotorcyclePrice(double price)
{
    if (price > 0)
    {
        motorcyclePricePerHour = price;
        saveToFile();
    }
}

void PricingConfig::setCarStandardPrice(double price)
{
    if (price > 0)
    {
        carStandardPricePerHour = price;
        saveToFile();
    }
}

void PricingConfig::setCarLuxuryPrice(double price)
{
    if (price > 0)
    {
        carLuxuryPricePerHour = price;
        saveToFile();
    }
}

void PricingConfig::setElectricBikePrice(double price)
{
    if (price > 0)
    {
        electricBikePricePerHour = price;
        saveToFile();
    }
}

void PricingConfig::setMinimumMinutes(int minutes)
{
    if (minutes > 0)
    {
        minimumMinutes = minutes;
        saveToFile();
    }
}

double PricingConfig::getPricePerHour(VehicleType type, bool isLuxury) const
{
    switch (type)
    {
    case VehicleType::MOTORCYCLE:
        return motorcyclePricePerHour;
    case VehicleType::CAR:
        return isLuxury ? carLuxuryPricePerHour : carStandardPricePerHour;
    case VehicleType::ELECTRIC_BIKE:
        return electricBikePricePerHour;
    default:
        return 0.0;
    }
}

void PricingConfig::loadFromFile()
{
    ifstream file(PRICING_FILE);
    if (!file.is_open())
    {
        saveToFile();
        return;
    }

    file >> motorcyclePricePerHour;
    file >> carStandardPricePerHour;
    file >> carLuxuryPricePerHour;
    file >> electricBikePricePerHour;
    file >> minimumMinutes;

    file.close();
}

void PricingConfig::saveToFile()
{
    ofstream file(PRICING_FILE);
    if (!file.is_open())
    {
        throw runtime_error("Khong the luu file pricing.txt");
    }

    file << motorcyclePricePerHour << endl;
    file << carStandardPricePerHour << endl;
    file << carLuxuryPricePerHour << endl;
    file << electricBikePricePerHour << endl;
    file << minimumMinutes << endl;

    file.close();
}

void PricingConfig::displayPricing() const
{
    cout << endl;
    ui.printHorizontalLine('+', '-', '+');
    ui.printEmptyLine();
    ui.printCenteredText("BANG GIA GUI XE", Color::CYAN);
    ui.printEmptyLine();
    ui.printHorizontalLine('+', '-', '+');
    ui.printEmptyLine();

    cout << fixed << setprecision(0);
    ui.printRow("          |  1. Xe may:         ", to_string((int)motorcyclePricePerHour) + " VND/gio");
    ui.printRow("          |  2. O to thuong:    ", to_string((int)carStandardPricePerHour) + " VND/gio");
    ui.printRow("          |  3. O to sang:      ", to_string((int)carLuxuryPricePerHour) + " VND/gio");
    ui.printRow("          |  4. Xe dap dien:    ", to_string((int)electricBikePricePerHour) + " VND/gio");
    ui.printEmptyLine();
    ui.printRow("          |  Thoi gian toi thieu: ", to_string(minimumMinutes) + " phut");
    ui.printEmptyLine();
    ui.printHorizontalLine('+', '-', '+');
}