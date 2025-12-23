#ifndef PRICING_CONFIG_H
#define PRICING_CONFIG_H

#include <string>
#include <map>
#include "Vehicle.h"

class PricingConfig
{
private:
    
    double motorcyclePricePerHour;
    double carStandardPricePerHour;
    double carLuxuryPricePerHour;
    double electricBikePricePerHour;

    int minimumMinutes;

    const std::string PRICING_FILE = "pricing.txt";

    static PricingConfig *instance;

    PricingConfig();

public:

    static PricingConfig *getInstance();

    
    double getMotorcyclePrice() const { return motorcyclePricePerHour; }
    double getCarStandardPrice() const { return carStandardPricePerHour; }
    double getCarLuxuryPrice() const { return carLuxuryPricePerHour; }
    double getElectricBikePrice() const { return electricBikePricePerHour; }
    int getMinimumMinutes() const { return minimumMinutes; }

  
    void setMotorcyclePrice(double price);
    void setCarStandardPrice(double price);
    void setCarLuxuryPrice(double price);
    void setElectricBikePrice(double price);
    void setMinimumMinutes(int minutes);

    double getPricePerHour(VehicleType type, bool isLuxury = false) const;

    void loadFromFile();
    void saveToFile();

    void displayPricing() const;
};

#endif