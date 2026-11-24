#pragma once
#include "environment/Environment.h"

class AsteroidField final : public Environment {
public:
    void applyEffect(Module& module) const override;
    const char* getName() const noexcept override;
    double getEnergyModifier() const noexcept override;
    double getDamageModifier() const noexcept override;
};
