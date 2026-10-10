#pragma once

#include <memory>
#include <string>

#include "Module.h"

class EnergyModule : public Module {
public:
    std::string getCategory() const override;
    std::string getCapacityUnit() const override;
    int getPowerOutput() const override;
    std::string describe() const override;

    int getNominalPower() const;

protected:
    EnergyModule(const std::string& name, int efficiency, Durability durability,
                 EnergyConsumption energyConsumption, Capacity capacity, int nominalPower);

private:
    int nominalPower_;
};

class SolarModule : public EnergyModule {
public:
    explicit SolarModule(const std::string& name, int panelArea = 40);

    std::unique_ptr<Module> clone() const override;
    std::string getType() const override;
    std::string describe() const override;

    int getPanelArea() const;

private:
    int panelArea_;
};

struct ReactorCore {
    int fuelLevel = 100;
    int temperature = 300;
};

class ReactorModule : public EnergyModule {
public:
    explicit ReactorModule(const std::string& name, int nominalPower = 120);

    ReactorModule(const ReactorModule& other);
    ReactorModule& operator=(const ReactorModule& other);
    ReactorModule(ReactorModule&& other) noexcept;
    ReactorModule& operator=(ReactorModule&& other) noexcept;
    ~ReactorModule() override;

    std::unique_ptr<Module> clone() const override;
    std::string getType() const override;
    std::string describe() const override;
    int getPowerOutput() const override;

    bool hasCore() const;
    int getFuelLevel() const;
    void burnFuel(int percent);
    void refuel();

private:
    ReactorCore& core();

    std::unique_ptr<ReactorCore> core_;
};
