#include "modules/CrewCabin.h"

CrewCabin::CrewCabin(const std::string& name, int durability, int energyConsumption,
                        int capacity, int efficiency)
    : ResidentialModule (name, durability, energyConsumption, capacity, efficiency) {}

std::unique_ptr<Module> CrewCabin::clone() const {
    return std::make_unique<CrewCabin>(*this);
}

std::string CrewCabin::getType() const {
    return "CrewCabin";
}