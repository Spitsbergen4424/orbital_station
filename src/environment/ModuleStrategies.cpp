#include "environment/ModuleStrategies.h"

#include <iostream>

#include "modules/EnergyModule.h"
#include "modules/ReactorModule.h"
#include "modules/ResidentialModule.h"
#include "modules/ResearchModule.h"

namespace {
void logStrategy(const Module& module, const char* description) {
    std::cout << "[STRATEGY] " << module.getType() << " \"" << module.getName()
              << "\": " << description << '\n';
}
}

EnergyModuleStrategy::EnergyModuleStrategy(StrategyMode mode) : mode_(mode) {}

void EnergyModuleStrategy::apply(Module& module) const {
    if (dynamic_cast<EnergyModule*>(&module) == nullptr ||
        dynamic_cast<ReactorModule*>(&module) != nullptr) {
        return;
    }

    if (mode_ == StrategyMode::DeepSpace) {
        module.applyEnergyConsumptionModifier(1.5);
        module.damage(2);
        logStrategy(module, "энергопотребление x1.5, радиационный урон 2");
    } else {
        module.damage(8);
        logStrategy(module, "удар обломками: урон прочности 8");
    }
}

CrewModuleStrategy::CrewModuleStrategy(StrategyMode mode) : mode_(mode) {}

void CrewModuleStrategy::apply(Module& module) const {
    if (dynamic_cast<ResidentialModule*>(&module) == nullptr) {
        return;
    }

    if (mode_ == StrategyMode::DeepSpace) {
        module.applyEnergyConsumptionModifier(1.5);
        module.damage(1);
        logStrategy(module, "энергопотребление x1.5, радиационный урон 1");
    } else {
        module.damage(8);
        logStrategy(module, "удар обломками: урон прочности 8");
    }
}

ResearchModuleStrategy::ResearchModuleStrategy(StrategyMode mode) : mode_(mode) {}

void ResearchModuleStrategy::apply(Module& module) const {
    auto* research = dynamic_cast<ResearchModule*>(&module);
    if (research == nullptr) {
        return;
    }

    if (mode_ == StrategyMode::DeepSpace) {
        module.applyEnergyConsumptionModifier(1.5);
        module.damage(4);
        logStrategy(module, "энергопотребление x1.5, радиационный урон 4");
    } else {
        research->setResearchBlocked(true);
        module.deactivate();
        module.damage(8);
        logStrategy(module, "исследования заблокированы, урон прочности 8");
    }
}

ReactorModuleStrategy::ReactorModuleStrategy(StrategyMode mode) : mode_(mode) {}

void ReactorModuleStrategy::apply(Module& module) const {
    if (dynamic_cast<ReactorModule*>(&module) == nullptr) {
        return;
    }

    if (mode_ == StrategyMode::DeepSpace) {
        module.applyEnergyConsumptionModifier(1.5);
        module.damage(2);
        logStrategy(module, "реактор: энергопотребление x1.5, радиационный урон 2");
    } else {
        module.damage(5);
        logStrategy(module, "реактор защищённее: урон прочности 5");
    }
}
