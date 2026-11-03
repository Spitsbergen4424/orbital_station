#include "factories/AdvancedModuleFactory.h"

#include "modules/MedicalCabin.h"
#include "modules/PhysicsLab.h"
#include "modules/ReactorModule.h"

// Улучшенные модули: прочнее и эффективнее, но дороже в обслуживании
std::unique_ptr<Module> AdvancedModuleFactory::createResidentialModule(const std::string& name) const {
    return std::make_unique<MedicalCabin>(name, 120, 7, 8, 85);
}

std::unique_ptr<Module> AdvancedModuleFactory::createEnergyModule(const std::string& name) const {
    return std::make_unique<ReactorModule>(name, 150, 10, 500, 95);
}

std::unique_ptr<Module> AdvancedModuleFactory::createResearchModule(const std::string& name) const {
    return std::make_unique<PhysicsLab>(name, 110, 12, 30, 90);
}
