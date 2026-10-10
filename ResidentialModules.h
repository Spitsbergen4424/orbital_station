#pragma once

#include <memory>
#include <string>

#include "Module.h"

class ResidentialModule : public Module {
public:
    std::string getCategory() const override;
    std::string getCapacityUnit() const override;

protected:
    using Module::Module;
};

class CrewCabin : public ResidentialModule {
public:
    explicit CrewCabin(const std::string& name, int berths = 4);

    std::unique_ptr<Module> clone() const override;
    std::string getType() const override;
};

class MedicalCabin : public ResidentialModule {
public:
    explicit MedicalCabin(const std::string& name, int beds = 2);

    std::unique_ptr<Module> clone() const override;
    std::string getType() const override;
};
