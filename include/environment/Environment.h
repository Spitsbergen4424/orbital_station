#pragma once

#include "modules/Module.h"

// Единый контракт для окружений станции. Модуль передаётся по ссылке:
// окружение изменяет существующий объект и не копирует его.
class Environment {
public:
    virtual ~Environment() = default;

    virtual void applyEffect(Module& module) const = 0;
    virtual const char* getName() const noexcept = 0;
    // Значение 0 означает отсутствие дополнительного влияния;
    // для DeepSpace значение 1.5 означает множитель энергопотребления.
    virtual double getEnergyModifier() const noexcept = 0;
    virtual double getDamageModifier() const noexcept = 0;
};
