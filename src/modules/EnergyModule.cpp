#include "modules/EnergyModule.h"

EnergyModule::EnergyModule(const std::string& name, int durability, int energyConsumption,
                           int capacity, int efficiency)
    : Module(name, durability, energyConsumption, capacity, efficiency) {}
