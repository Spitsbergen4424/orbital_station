#pragma once

#include "factories/ModuleFactory.h"

class AdvancedModuleFactory : public ModuleFactory {
public:
    virtual std::unique_ptr<Module> createResidentialModule(const std::string& name) const override;
    virtual std::unique_ptr<Module> createEnergyModule(const std::string& name) const override;
    virtual std::unique_ptr<Module> createResearchModule(const std::string& name) const override;
};