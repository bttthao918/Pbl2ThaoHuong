#include "PricingConfig.h"
#include <fstream>
#include <iostream>
#include <iomanip>

// Initialize static member
PricingConfig *PricingConfig::instance = nullptr;

PricingConfig::PricingConfig()
{
    // Giá mặc định
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
    std::ifstream file(PRICING_FILE);
    if (!file.is_open())
    {
        // File chưa tồn tại, sử dụng giá mặc định
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
    std::ofstream file(PRICING_FILE);
    if (!file.is_open())
    {
        throw std::runtime_error("Khong the luu file pricing.txt");
    }

    file << motorcyclePricePerHour << std::endl;
    file << carStandardPricePerHour << std::endl;
    file << carLuxuryPricePerHour << std::endl;
    file << electricBikePricePerHour << std::endl;
    file << minimumMinutes << std::endl;

    file.close();
}

void PricingConfig::displayPricing() const
{
    std::cout << "\n========== BANG GIA GUI XE ==========\n";
    std::cout << std::fixed << std::setprecision(0);
    std::cout << "1. Xe may:           " << motorcyclePricePerHour << " VND/gio\n";
    std::cout << "2. O to thuong:      " << carStandardPricePerHour << " VND/gio\n";
    std::cout << "3. O to sang:        " << carLuxuryPricePerHour << " VND/gio\n";
    std::cout << "4. Xe dap dien:      " << electricBikePricePerHour << " VND/gio\n";
    std::cout << "\nThoi gian toi thieu: " << minimumMinutes << " phut\n";
    std::cout << "======================================\n";
}