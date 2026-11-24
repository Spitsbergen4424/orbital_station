#include "mediator/StationMediator.h"

#include <iostream>

#include "modules/ResearchModule.h"

StationMediator::StationMediator(
    CommandCenter& center,
    std::shared_ptr<ModuleFactory> standardFactory,
    std::shared_ptr<ModuleFactory> advancedFactory
)
    : center_(center),
      standardFactory_(standardFactory),
      advancedFactory_(advancedFactory) {
}

Module* StationMediator::getModule(int index) {
    if (index < 0 ||
        index >= static_cast<int>(center_.moduleCount())) {
        return nullptr;
    }

    auto it = center_.modules().begin();

    for (int i = 0; i < index; ++i) {
        ++it;
    }

    return &(*it);
}

void StationMediator::activateModule(int index) {
    Module* module = getModule(index);

    if (!module) {
        std::cout << "Модуль не найден.\n";
        return;
    }

    if (module->activate()) {
        std::cout << "Модуль " << module->getName()
                  << " активирован.\n";
    } else {
        std::cout << "Не удалось активировать модуль.\n";
    }
}

void StationMediator::deactivateModule(int index) {
    Module* module = getModule(index);

    if (!module) {
        std::cout << "Модуль не найден.\n";
        return;
    }

    module->deactivate();
    std::cout << "Модуль выключен.\n";
}

void StationMediator::moveModule(int from, int to) {
    if (from < 0 || to < 0 ||
        !center_.modules().moveModule(from, to)) {
        std::cout << "Неверные индексы модулей.\n";
        return;
    }

    std::cout << "Модуль перемещён.\n";
}

void StationMediator::repairModule(int index) {
    Module* module = getModule(index);

    if (!module) {
        std::cout << "Модуль не найден.\n";
        return;
    }

    int missing =
        module->getDurability().getMaxValue() -
        module->getDurability().getValue();

    double cost = missing;

    if (missing == 0) {
        std::cout << "Модуль не нуждается в ремонте.\n";
        return;
    }

    if (center_.energy() < cost) {
        std::cout << "Недостаточно энергии для ремонта.\n";
        return;
    }

    center_.consumeEnergy(cost);
    module->repair(missing);

    std::cout << "Модуль отремонтирован.\n";
}

void StationMediator::distributeEnergy(int index, int percent) {
    Module* module = getModule(index);

    if (!module || percent < 0 || percent > 100) {
        std::cout << "Неверные параметры распределения энергии.\n";
        return;
    }

    if (energyDistribution_.size() < center_.moduleCount()) {
        energyDistribution_.resize(center_.moduleCount(), 0);
    }

    double amount = center_.energy() * percent / 100.0;

    energyDistribution_[index] = amount;

    std::cout << "Модулю " << module->getName()
              << " выделено " << amount
              << " единиц энергии.\n";
}

bool StationMediator::requestEnergy(int index, double amount) {
    Module* module = getModule(index);

    if (!module || amount <= 0) {
        return false;
    }

    if (center_.energy() < amount) {
        std::cout << "Недостаточно энергии.\n";
        return false;
    }

    center_.consumeEnergy(amount);

    return true;
}

void StationMediator::startResearch(int index) {
    Module* module = getModule(index);

    if (!module) {
        std::cout << "Модуль не найден.\n";
        return;
    }

    auto* research = dynamic_cast<ResearchModule*>(module);

    if (!research) {
        std::cout << "Этот модуль не предназначен для исследований.\n";
        return;
    }

    if (research->isResearchBlocked()) {
        std::cout << "Исследования временно заблокированы окружением.\n";
        return;
    }

    if (module->getState() == ModuleState::Emergency) {
        std::cout << "Аварийный модуль не может проводить исследования.\n";
        return;
    }

    if (!module->activate()) {
        std::cout << "Не удалось включить лабораторию.\n";
        return;
    }

    double cost = module->getEnergyConsumption().getValue();

    if (!requestEnergy(index, cost)) {
        module->deactivate();
        return;
    }

    std::cout << "Исследование завершено.\n";
    std::cout << "Эффективность: "
              << module->getEfficiency() << "%\n";
}

void StationMediator::buildModule(
    const std::string& type,
    const std::string& name
) {
    ModuleFactory* factory = standardFactory_.get();
    ModuleCategory category;

    if (type == "crew" || type == "medical") {
        category = ModuleCategory::Residential;

        if (type == "medical") {
            factory = advancedFactory_.get();
        }
    }
    else if (type == "solar" || type == "reactor") {
        category = ModuleCategory::Energy;

        if (type == "reactor") {
            factory = advancedFactory_.get();
        }
    }
    else if (type == "biology" || type == "physics") {
        category = ModuleCategory::Research;

        if (type == "physics") {
            factory = advancedFactory_.get();
        }
    }
    else {
        std::cout << "Неизвестный тип модуля.\n";
        return;
    }

    if (center_.createModule(*factory, category, name, 20)) {
        energyDistribution_.resize(center_.moduleCount(), 0);
        std::cout << "Модуль успешно построен.\n";
    }
    else {
        std::cout << "Не удалось построить модуль. "
                     "Проверьте энергию и свободное место.\n";
    }
}

void StationMediator::getStatus(int index) const {
    if (index == -1) {
        center_.printStatistics();

        int i = 0;

        for (const Module& module : center_.modules()) {
            std::cout << i++ << ": ";
            module.print();
        }

        return;
    }

    if (index < 0 ||
        index >= static_cast<int>(center_.moduleCount())) {
        std::cout << "Модуль не найден.\n";
        return;
    }

    auto it = center_.modules().begin();

    for (int i = 0; i < index; ++i) {
        ++it;
    }

    it->print();
}

void StationMediator::damageModule(int index, int amount) {
    Module* module = getModule(index);

    if (!module || amount <= 0) {
        std::cout << "Неверные параметры повреждения.\n";
        return;
    }

    module->damage(amount);

    center_.flushPendingRemovals();

    std::cout << "Модуль получил повреждения.\n";
}