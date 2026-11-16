#include "environment/EarthOrbit.h"
#include "environment/DeepSpace.h"
#include "environment/AsteroidField.h"

#include <iostream>
#include <memory>

#include "environment/ModuleStrategies.h"
#include "modules/ReactorModule.h"
#include "modules/ResidentialModule.h"
#include "modules/ResearchModule.h"

namespace {
void clearResearchBlock(Module& module) {
    if (auto* research = dynamic_cast<ResearchModule*>(&module)) {
        research->setResearchBlocked(false);
    }
}
}

void EarthOrbit::applyEffect(Module& module) const {
    module.resetEnergyConsumption();
    // При выходе из астероидного поля снимаем временную блокировку исследований.
    clearResearchBlock(module);
    std::cout << "[ENV] Орбита Земли: нейтральное влияние на модуль \""
              << module.getName() << "\"\n";
}

const char* EarthOrbit::getName() const noexcept { return "Орбита Земли"; }
double EarthOrbit::getEnergyModifier() const noexcept { return 1.0; }
double EarthOrbit::getDamageModifier() const noexcept { return 1.0; }

void DeepSpace::applyEffect(Module& module) const {
    clearResearchBlock(module);

    // ReactorModule наследуется от EnergyModule, поэтому проверяем реактор
    // раньше общей категории энергетических модулей.
    if (dynamic_cast<ReactorModule*>(&module) != nullptr) {
        ReactorModuleStrategy(StrategyMode::DeepSpace).apply(module);
    } else if (dynamic_cast<ResearchModule*>(&module) != nullptr) {
        ResearchModuleStrategy(StrategyMode::DeepSpace).apply(module);
    } else if (dynamic_cast<ResidentialModule*>(&module) != nullptr) {
        CrewModuleStrategy(StrategyMode::DeepSpace).apply(module);
    } else if (dynamic_cast<EnergyModule*>(&module) != nullptr) {
        EnergyModuleStrategy(StrategyMode::DeepSpace).apply(module);
    } else {
        std::cout << "[ENV] Глубокий космос: неизвестный тип модуля — "
                     "специальная стратегия не назначена\n";
    }
}

const char* DeepSpace::getName() const noexcept { return "Глубокий космос"; }
double DeepSpace::getEnergyModifier() const noexcept { return 1.5; }
double DeepSpace::getDamageModifier() const noexcept { return 1.5; }

void AsteroidField::applyEffect(Module& module) const {
    module.resetEnergyConsumption();
    if (dynamic_cast<ReactorModule*>(&module) != nullptr) {
        ReactorModuleStrategy(StrategyMode::AsteroidField).apply(module);
    } else if (dynamic_cast<ResearchModule*>(&module) != nullptr) {
        ResearchModuleStrategy(StrategyMode::AsteroidField).apply(module);
    } else if (dynamic_cast<ResidentialModule*>(&module) != nullptr) {
        CrewModuleStrategy(StrategyMode::AsteroidField).apply(module);
    } else if (dynamic_cast<EnergyModule*>(&module) != nullptr) {
        EnergyModuleStrategy(StrategyMode::AsteroidField).apply(module);
    } else {
        module.damage(8);
        std::cout << "[ENV] Астероидное поле: общий урон прочности 8\n";
    }
}

const char* AsteroidField::getName() const noexcept { return "Астероидное поле"; }
double AsteroidField::getEnergyModifier() const noexcept { return 1.0; }
double AsteroidField::getDamageModifier() const noexcept { return 1.25; }
