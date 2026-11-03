#pragma once

#include "modules/EnergyModule.h"

// Реактор — улучшенный энергетический модуль
class ReactorModule : public EnergyModule {
public:
    ReactorModule(const std::string& name, int durability, int energyConsumption,
                  int capacity, int efficiency);

    std::unique_ptr<Module> clone() const override;  // копия реактора
    std::string getType() const override;            // "ReactorModule"
};
