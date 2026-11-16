#pragma once

#include <cstddef>
#include <memory>
#include <string>

#include "attributes/Capacity.h"
#include "attributes/Durability.h"
#include "attributes/EnergyConsumption.h"
#include "observer/IObservable.h"
#include "metadata/ModuleTypeInfo.h"
#include "observer/IObservable.h"
#include "metadata/ModuleTypeInfo.h"

class Station; 

enum class ModuleState {
    Off,        // выключен
    On,         // включён
    Repair,     // ремонт
    Emergency   // авария
};

class Module : public IObservable {
public:
    static constexpr int MIN_EFFICIENCY = 0;
    static constexpr int MAX_EFFICIENCY = 100;

    Module(const std::string& name, int durability, int energyConsumption,
           int capacity, int efficiency);
    virtual ~Module() {}

    virtual std::unique_ptr<Module> clone() const = 0;
    virtual std::string getType() const = 0;
    virtual bool consumesOxygen() const noexcept { return false; }

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

    // Окружение меняет текущее потребление относительно исходного значения.
    void applyEnergyConsumptionModifier(double modifier);
    void resetEnergyConsumption() noexcept;
    void setTypeInfo(std::shared_ptr<const ModuleTypeInfo> info) noexcept;
    std::shared_ptr<const ModuleTypeInfo> getTypeInfo() const noexcept;
    void multiplyCharacteristics(double modifier);

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
    int baseEnergyConsumption_;
    Capacity capacity_;
    std::shared_ptr<const ModuleTypeInfo> typeInfo_;
};
