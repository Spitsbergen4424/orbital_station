#pragma once

#include <memory>
#include <string>

#include "Module.h"

// Абстрактная фабрика модулей: создаёт модули трёх категорий.
// Код, который пользуется фабрикой, не знает, какие именно классы она создаёт.
class ModuleFactory {
public:
    virtual ~ModuleFactory() {}

    virtual std::unique_ptr<Module> createResidentialModule(const std::string& name) const = 0;
    virtual std::unique_ptr<Module> createEnergyModule(const std::string& name) const = 0;
    virtual std::unique_ptr<Module> createResearchModule(const std::string& name) const = 0;
};
