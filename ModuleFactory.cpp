#include "ModuleFactory.h"

#include "Logger.h"

namespace {

void logCreated(const ModuleFactory& factory, const Module& module) {
    logInfo(LogSource::FACTORY, factory.getFamilyName() + ": создан " + module.getType() + ' ' +
                                    module.label() + " (" + module.getCategory() + ")");
}

}

std::unique_ptr<ResidentialModule> StandardModuleFactory::createResidentialModule(const std::string& name) const {
    auto module = std::make_unique<CrewCabin>(name);
    logCreated(*this, *module);
    return module;
}

std::unique_ptr<EnergyModule> StandardModuleFactory::createEnergyModule(const std::string& name) const {
    auto module = std::make_unique<SolarModule>(name);
    logCreated(*this, *module);
    return module;
}

std::unique_ptr<ResearchModule> StandardModuleFactory::createResearchModule(const std::string& name) const {
    auto module = std::make_unique<BiologyLab>(name);
    logCreated(*this, *module);
    return module;
}

std::string StandardModuleFactory::getFamilyName() const {
    return "StandardModuleFactory";
}

std::unique_ptr<ResidentialModule> AdvancedModuleFactory::createResidentialModule(const std::string& name) const {
    auto module = std::make_unique<MedicalCabin>(name);
    logCreated(*this, *module);
    return module;
}

std::unique_ptr<EnergyModule> AdvancedModuleFactory::createEnergyModule(const std::string& name) const {
    auto module = std::make_unique<ReactorModule>(name);
    logCreated(*this, *module);
    return module;
}

std::unique_ptr<ResearchModule> AdvancedModuleFactory::createResearchModule(const std::string& name) const {
    auto module = std::make_unique<PhysicsLab>(name);
    logCreated(*this, *module);
    return module;
}

std::string AdvancedModuleFactory::getFamilyName() const {
    return "AdvancedModuleFactory";
}
