#pragma once

#include <cstddef>
#include <memory>
#include <string>

#include "Capacity.h"
#include "Durability.h"
#include "EnergyConsumption.h"

enum class ModuleState { Enabled, Repair, Emergency };

std::string toString(ModuleState state);

class Station;

class Module {
public:
    virtual ~Module() = default;

    virtual std::unique_ptr<Module> clone() const = 0;
    virtual std::string getType() const = 0;
    virtual std::string getCategory() const = 0;
    virtual std::string getCapacityUnit() const = 0;
    virtual int getPowerOutput() const;
    virtual std::string describe() const;

    bool activate();
    void deactivate();
    void setState(ModuleState newState);
    void takeDamage(int amount);
    void repair(int amount);

    int getId() const;
    const std::string& getName() const;
    void setName(const std::string& name);
    std::string label() const;
    int getEfficiency() const;
    void setEfficiency(int efficiency);
    ModuleState getState() const;
    bool isActive() const;
    std::size_t getPosition() const;

    Durability& durability();
    const Durability& durability() const;
    EnergyConsumption& energyConsumption();
    const EnergyConsumption& energyConsumption() const;
    Capacity& capacity();
    const Capacity& capacity() const;

    static int copiesMade();

protected:
    Module(const std::string& name, int efficiency, Durability durability,
           EnergyConsumption energyConsumption, Capacity capacity);

    Module(const Module& other);
    Module& operator=(const Module& other);
    Module(Module&& other) noexcept;
    Module& operator=(Module&& other) noexcept;

private:
    friend class Station;
    void moveToIndex(std::size_t newIndex);

    static int nextId_;
    static int copiesMade_;

    int id_;
    std::string name_;
    int efficiency_;
    ModuleState state_ = ModuleState::Enabled;
    bool active_ = false;
    std::size_t position_ = 0;
    Durability durability_;
    EnergyConsumption energyConsumption_;
    Capacity capacity_;
};
