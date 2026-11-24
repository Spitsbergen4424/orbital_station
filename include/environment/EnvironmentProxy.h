#pragma once
#include <memory>
#include <string>
#include <unordered_set>
#include "environment/Environment.h"
class EnvironmentProxy final : public Environment {
public:
    explicit EnvironmentProxy(std::shared_ptr<Environment> realEnvironment);
    void applyEffect(Module& module) const override;
    const char* getName() const noexcept override;
    double getEnergyModifier() const noexcept override;
    double getDamageModifier() const noexcept override;
private:
    std::string cacheKey(const Module& module) const;
    std::shared_ptr<Environment> realEnvironment_;
    mutable std::unordered_set<std::string> appliedEffects_;
};
