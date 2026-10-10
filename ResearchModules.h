#pragma once

#include <memory>
#include <string>

#include "Module.h"

class ResearchModule : public Module {
public:
    std::string getCategory() const override;
    std::string getCapacityUnit() const override;

protected:
    using Module::Module;
};

class BiologyLab : public ResearchModule {
public:
    explicit BiologyLab(const std::string& name, int samples = 30);

    std::unique_ptr<Module> clone() const override;
    std::string getType() const override;
};

class PhysicsLab : public ResearchModule {
public:
    explicit PhysicsLab(const std::string& name, int samples = 10);

    std::unique_ptr<Module> clone() const override;
    std::string getType() const override;
};
