#include "modules/ResearchModule.h"

ResearchModule::ResearchModule(const std::string& name, int durability, int energyConsumption,
                               int capacity, int efficiency)
    : Module(name, durability, energyConsumption, capacity, efficiency),
      researchBlocked_(false) {}

void ResearchModule::setResearchBlocked(bool blocked) noexcept {
    researchBlocked_ = blocked;
}

bool ResearchModule::isResearchBlocked() const noexcept {
    return researchBlocked_;
}
