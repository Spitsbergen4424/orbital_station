#pragma once

#include <memory>

#include "environment/Environment.h"

// Proxy проверяет допустимость воздействия, затем делегирует работу
// реальному объекту Environment. Владение окружением разделяется через shared_ptr.
class EnvironmentProxy final : public Environment {
public:
    explicit EnvironmentProxy(std::shared_ptr<Environment> realEnvironment);

    void applyEffect(Module& module) const override;
    const char* getName() const noexcept override;
    double getEnergyModifier() const noexcept override;
    double getDamageModifier() const noexcept override;

private:
    std::shared_ptr<Environment> realEnvironment_;
};
