#include "environment/EarthOrbit.h"
#include "environment/DeepSpace.h"
#include "environment/AsteroidField.h"

#include "logging/LoggingProxy.h"

#include "environment/ModuleStrategies.h"
#include "modules/EnergyModule.h"
#include "modules/ReactorModule.h"
#include "modules/ResidentialModule.h"
#include "modules/ResearchModule.h"

namespace {
void resetTemporaryResearchRestriction(Module& module) {
    if (auto* research = dynamic_cast<ResearchModule*>(&module)) {
        research->setResearchBlocked(false);
    }
}

void applyByModuleFamily(Module& module, StrategyMode mode) {
    // Проверка реактора обязательно раньше EnergyModule: это наследник EnergyModule.
    if (dynamic_cast<ReactorModule*>(&module) != nullptr) {
        ReactorModuleStrategy(mode).apply(module);
    } else if (dynamic_cast<ResearchModule*>(&module) != nullptr) {
        ResearchModuleStrategy(mode).apply(module);
    } else if (dynamic_cast<ResidentialModule*>(&module) != nullptr) {
        CrewModuleStrategy(mode).apply(module);
    } else if (dynamic_cast<EnergyModule*>(&module) != nullptr) {
        EnergyModuleStrategy(mode).apply(module);
    } else {
        // Для неизвестного в текущей иерархии типа сохраняем безопасное поведение.
        module.damage(mode == StrategyMode::DeepSpace ? 2 : 8);
        LoggingProxy::globalLog("ENV", "Для неизвестного семейства применён общий урон");
    }
}
}

void EarthOrbit::applyEffect(Module& module) const {
    module.resetEnergyConsumption();
    resetTemporaryResearchRestriction(module);
    LoggingProxy::globalLog("ENV", std::string("Орбита Земли: нейтральное влияние на модуль ") + module.getName());
}
const char* EarthOrbit::getName() const noexcept { return "Орбита Земли"; }
double EarthOrbit::getEnergyModifier() const noexcept { return 0.0; }
double EarthOrbit::getDamageModifier() const noexcept { return 0.0; }

void DeepSpace::applyEffect(Module& module) const {
    resetTemporaryResearchRestriction(module);
    applyByModuleFamily(module, StrategyMode::DeepSpace);
}
const char* DeepSpace::getName() const noexcept { return "Глубокий космос"; }
double DeepSpace::getEnergyModifier() const noexcept { return 1.5; }
double DeepSpace::getDamageModifier() const noexcept { return 1.0; }

void AsteroidField::applyEffect(Module& module) const {
    module.resetEnergyConsumption();
    applyByModuleFamily(module, StrategyMode::AsteroidField);
}
const char* AsteroidField::getName() const noexcept { return "Астероидное поле"; }
double AsteroidField::getEnergyModifier() const noexcept { return 0.0; }
double AsteroidField::getDamageModifier() const noexcept { return 1.25; }
