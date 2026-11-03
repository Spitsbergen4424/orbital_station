#include "modules/ResearchModule.h"

ResearchModule::ResearchModule(const std::string& name, int durability, int energyConsumption,
                               int capacity, int efficiency)
    : Module(name, durability, energyConsumption, capacity, efficiency) {}
