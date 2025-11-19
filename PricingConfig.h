#ifndef PRICING_CONFIG_H
#define PRICING_CONFIG_H

#include <string>
#include <map>
#include "Vehicle.h"

class PricingConfig
{
private:
    // Giá theo loại xe (VND/giờ)
    double motorcyclePricePerHour;
    double carStandardPricePerHour;
    double carLuxuryPricePerHour;
    double electricBikePricePerHour;

    // Thời gian tối thiểu (phút)
    int minimumMinutes;

    // File lưu trữ
    const std::string PRICING_FILE = "pricing.txt";

    // Singleton instance
    static PricingConfig *instance;

    // Private constructor
    PricingConfig();

public:
    // Singleton pattern
    static PricingConfig *getInstance();

    // Getters
    double getMotorcyclePrice() const { return motorcyclePricePerHour; }
    double getCarStandardPrice() const { return carStandardPricePerHour; }
    double getCarLuxuryPrice() const { return carLuxuryPricePerHour; }
    double getElectricBikePrice() const { return electricBikePricePerHour; }
    int getMinimumMinutes() const { return minimumMinutes; }

    // Setters
    void setMotorcyclePrice(double price);
    void setCarStandardPrice(double price);
    void setCarLuxuryPrice(double price);
    void setElectricBikePrice(double price);
    void setMinimumMinutes(int minutes);

    // Get price by vehicle type and luxury status
    double getPricePerHour(VehicleType type, bool isLuxury = false) const;

    // File operations
    void loadFromFile();
    void saveToFile();

    // Display
    void displayPricing() const;
};

#endif