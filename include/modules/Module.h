#pragma once

#include <cstddef>
#include <memory>
#include <string>

#include "attributes/Capacity.h"
#include "attributes/Durability.h"
#include "attributes/EnergyConsumption.h"

class Station; 

enum class ModuleState {
    Off,        // выключен
    On,         // включён
    Repair,     // ремонт
    Emergency   // авария
};

class Module {
public:
    static constexpr int MIN_EFFICIENCY = 0;
    static constexpr int MAX_EFFICIENCY = 100;

    Module(const std::string& name, int durability, int energyConsumption,
        int capacity, int efficiency): baseEnergyConsumption_(energyConsumption) {};
    virtual ~Module() {}

    virtual std::unique_ptr<Module> clone() const = 0;
    virtual std::string getType() const = 0;

    void applyEnergyConsumptionModifier(double modifier);
    void resetEnergyConsumption() noexcept;

    int getId() const;
    const std::string& getName() const;
    int getEfficiency() const;
    ModuleState getState() const;
    std::string getStateName() const;
    const Durability& getDurability() const;
    const EnergyConsumption& getEnergyConsumption() const;
    const Capacity& getCapacity() const;

    void setName(const std::string& name);
    void setEfficiency(int efficiency);

    bool activate();
    void deactivate();
    void setState(ModuleState newState);
    void damage(int amount);
    void repair(int amount);
    void moveToIndex(Station& station, std::size_t newIndex);

    void print() const;

private:
    static int nextId_;

    int id_;
    std::string name_;
    int efficiency_;
    ModuleState state_;
    Durability durability_;
    EnergyConsumption energyConsumption_;
    Capacity capacity_;
    int baseEnergyConsumption_;
};
