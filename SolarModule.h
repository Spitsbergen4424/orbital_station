#pragma once

#include "EnergyModule.h"

// Солнечная панель — стандартный энергетический модуль
class SolarModule : public EnergyModule {
public:
    SolarModule(const std::string& name, int durability, int energyConsumption,
                int capacity, int efficiency);

    std::unique_ptr<Module> clone() const override;  // копия панели
    std::string getType() const override;            // "SolarModule"
};
