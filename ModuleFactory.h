#pragma once

#include <memory>
#include <string>

#include "EnergyModules.h"
#include "ResearchModules.h"
#include "ResidentialModules.h"

class ModuleFactory {
public:
    virtual ~ModuleFactory() = default;

    virtual std::unique_ptr<ResidentialModule> createResidentialModule(const std::string& name) const = 0;
    virtual std::unique_ptr<EnergyModule> createEnergyModule(const std::string& name) const = 0;
    virtual std::unique_ptr<ResearchModule> createResearchModule(const std::string& name) const = 0;

    virtual std::string getFamilyName() const = 0;
};

class StandardModuleFactory : public ModuleFactory {
public:
    std::unique_ptr<ResidentialModule> createResidentialModule(const std::string& name) const override;
    std::unique_ptr<EnergyModule> createEnergyModule(const std::string& name) const override;
    std::unique_ptr<ResearchModule> createResearchModule(const std::string& name) const override;
    std::string getFamilyName() const override;
};

class AdvancedModuleFactory : public ModuleFactory {
public:
    std::unique_ptr<ResidentialModule> createResidentialModule(const std::string& name) const override;
    std::unique_ptr<EnergyModule> createEnergyModule(const std::string& name) const override;
    std::unique_ptr<ResearchModule> createResearchModule(const std::string& name) const override;
    std::string getFamilyName() const override;
};
