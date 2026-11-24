#pragma once

#include "modules/Module.h"

class ResearchModule : public Module {
public:
    ResearchModule(const std::string& name, int durability, int energyConsumption,
                   int capacity, int efficiency);

    void setResearchBlocked(bool blocked) noexcept;
    bool isResearchBlocked() const noexcept;

private:
    bool researchBlocked_ = false;
};
