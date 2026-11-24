#include "modules/Module.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <utility>
#include <chrono>
#include <chrono>

#include "station/Station.h"

int Module::nextId_ = 1;  // нумерация модулей начинается с 1

Module::Module(const std::string& name, int durability, int energyConsumption,
               int capacity, int efficiency)
    : id_(nextId_++),
      name_(name),
      efficiency_(0),
      state_(ModuleState::Off),  // новый модуль всегда выключен
      durability_(durability),
      energyConsumption_(energyConsumption),
      baseEnergyConsumption_(energyConsumption),
      capacity_(capacity) {
    setEfficiency(efficiency);
}

int Module::getId() const {
    return id_;
}

const std::string& Module::getName() const {
    return name_;
}

int Module::getEfficiency() const {
    return efficiency_;
}

ModuleState Module::getState() const {
    return state_;
}

std::string Module::getStateName() const {
    switch (state_) {
        case ModuleState::Off:
            return "выключен";
        case ModuleState::On:
            return "включён";
        case ModuleState::Repair:
            return "ремонт";
        case ModuleState::Emergency:
            return "авария";
    }
    return "неизвестно";
}

const Durability& Module::getDurability() const {
    return durability_;
}

const EnergyConsumption& Module::getEnergyConsumption() const {
    return energyConsumption_;
}

const Capacity& Module::getCapacity() const {
    return capacity_;
}

void Module::setName(const std::string& name) {
    name_ = name;
}

void Module::applyEnergyConsumptionModifier(double modifier) {
    if (!std::isfinite(modifier) || modifier < 0.0) {
        return;
    }
    const int modified = static_cast<int>(baseEnergyConsumption_ * modifier + 0.5);
    energyConsumption_.setValue(modified);
}

void Module::resetEnergyConsumption() noexcept {
    energyConsumption_.setValue(baseEnergyConsumption_);
}


void Module::setTypeInfo(std::shared_ptr<const ModuleTypeInfo> info) noexcept { typeInfo_ = std::move(info); }
std::shared_ptr<const ModuleTypeInfo> Module::getTypeInfo() const noexcept { return typeInfo_; }
void Module::multiplyCharacteristics(double modifier) {
    if (!std::isfinite(modifier) || modifier < 0.0) return;
    setEfficiency(static_cast<int>(std::lround(efficiency_ * modifier)));
    notify({ModuleEventType::Modified, id_, 0, std::chrono::system_clock::now()});
}


void Module::setEfficiency(int efficiency) {
    // держим эффективность в диапазоне от MIN_EFFICIENCY до MAX_EFFICIENCY
    efficiency_ = std::clamp(efficiency, MIN_EFFICIENCY, MAX_EFFICIENCY);
}

bool Module::activate() {
    // включить сломанный или аварийный модуль нельзя
    if (state_ == ModuleState::Emergency || durability_.getValue() <= 0) {
        return false;
    }
    state_ = ModuleState::On;
    notify({ModuleEventType::Activated, id_, 0, std::chrono::system_clock::now()});
    return true;
}

void Module::deactivate() {
    if (state_ == ModuleState::Emergency) return;
    state_ = ModuleState::Off;
    notify({ModuleEventType::Deactivated, id_, 0, std::chrono::system_clock::now()});
}

void Module::setState(ModuleState newState) {
    const ModuleState previous = state_;
    state_ = newState;
    if (previous != ModuleState::On && newState == ModuleState::On)
        notify({ModuleEventType::Activated, id_, 0, std::chrono::system_clock::now()});
    else if (previous == ModuleState::On && newState == ModuleState::Off)
        notify({ModuleEventType::Deactivated, id_, 0, std::chrono::system_clock::now()});
    if (previous != ModuleState::Emergency && newState == ModuleState::Emergency)
        notify({ModuleEventType::Destroyed, id_, 0, std::chrono::system_clock::now()});
}

void Module::damage(int amount) {
    if (amount <= 0) {
        return;
    }
    durability_.decrease(amount);
    notify({ModuleEventType::Damaged, id_, amount, std::chrono::system_clock::now()});
    if (durability_.getValue() <= 0 && state_ != ModuleState::Emergency) {
        state_ = ModuleState::Emergency;
        notify({ModuleEventType::Destroyed, id_, amount, std::chrono::system_clock::now()});
    }
}

void Module::repair(int amount) {
    if (amount <= 0) {
        return;
    }
    durability_.repair(amount);
    if (state_ == ModuleState::Emergency) {
        // из аварии модуль выходит в состояние "ремонт", а не сразу во "включён"
        state_ = ModuleState::Repair;
    }
}

void Module::moveToIndex(Station& station, std::size_t newIndex) {
    int currentIndex = station.indexOf(id_);
    if (currentIndex >= 0) {
        station.moveModule(static_cast<std::size_t>(currentIndex), newIndex);
    }
}

void Module::print() const {
    std::cout << "[ID " << id_ << "] " << name_ << " — тип " << getType()
              << "; состояние: " << getStateName()
              << "; прочность " << durability_.getValue() << "/" << durability_.getMaxValue()
              << "; энергопотребление " << energyConsumption_.getValue() << " кВт"
              << "; вместимость " << capacity_.getValue()
              << "; эффективность " << efficiency_ << "%\n";
}
