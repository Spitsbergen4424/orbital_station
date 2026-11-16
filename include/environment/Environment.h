#pragma once

#include "modules/Module.h"

// Общий интерфейс для любого космического окружения.
class Environment {
public:
    virtual ~Environment() = default;

    virtual void applyEffect(Module& module) const = 0;
    virtual const char* getName() const noexcept = 0;
    virtual double getEnergyModifier() const noexcept = 0;
    virtual double getDamageModifier() const noexcept = 0;
};
