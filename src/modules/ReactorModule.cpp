#include "modules/ReactorModule.h"

ReactorModule::ReactorModule(const std::string& name, int durability, int energyConsumption,
                        int capacity, int efficiency)
    : EnergyModule (name, durability, energyConsumption, capacity, efficiency) {}

std::unique_ptr<Module> ReactorModule::clone() const {
    return std::make_unique<ReactorModule>(*this);
}

std::string ReactorModule::getType() const {
    return "ReactorModule";
}