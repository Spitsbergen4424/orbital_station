#include "modules/SolarModule.h"

SolarModule::SolarModule(const std::string& name, int durability, int energyConsumption,
                         int capacity, int efficiency)
    : EnergyModule (name, durability, energyConsumption, capacity, efficiency) {}

std::unique_ptr<Module> SolarModule::clone() const {
    return std::make_unique<SolarModule>(*this);
}

std::string SolarModule::getType() const {
    return "SolarModule";
}
