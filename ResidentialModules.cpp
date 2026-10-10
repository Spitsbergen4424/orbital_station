#include "ResidentialModules.h"

std::string ResidentialModule::getCategory() const {
    return "Жилой";
}

std::string ResidentialModule::getCapacityUnit() const {
    return "чел.";
}

CrewCabin::CrewCabin(const std::string& name, int berths)
    : ResidentialModule(name, 85, Durability(100), EnergyConsumption(5), Capacity(berths)) {}

std::unique_ptr<Module> CrewCabin::clone() const {
    return std::make_unique<CrewCabin>(*this);
}

std::string CrewCabin::getType() const {
    return "CrewCabin";
}

MedicalCabin::MedicalCabin(const std::string& name, int beds)
    : ResidentialModule(name, 90, Durability(120, 120), EnergyConsumption(12), Capacity(beds)) {}

std::unique_ptr<Module> MedicalCabin::clone() const {
    return std::make_unique<MedicalCabin>(*this);
}

std::string MedicalCabin::getType() const {
    return "MedicalCabin";
}
