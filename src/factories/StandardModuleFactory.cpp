#include "factories/StandardModuleFactory.h"

#include "modules/BiologyLab.h"
#include "modules/CrewCabin.h"
#include "modules/SolarModule.h"

// Стандартные модули: дешевле и проще, но менее эффективны
std::unique_ptr<Module> StandardModuleFactory::createResidentialModule(const std::string& name) const {
    return std::make_unique<CrewCabin>(name, 100, 5, 6, 75);
}

std::unique_ptr<Module> StandardModuleFactory::createEnergyModule(const std::string& name) const {
    return std::make_unique<SolarModule>(name, 80, 2, 100, 60);
}

std::unique_ptr<Module> StandardModuleFactory::createResearchModule(const std::string& name) const {
    return std::make_unique<BiologyLab>(name, 90, 8, 20, 70);
}
