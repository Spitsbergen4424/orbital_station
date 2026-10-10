#include "Module.h"

#include <sstream>
#include <stdexcept>

#include "Logger.h"

int Module::nextId_ = 1;
int Module::copiesMade_ = 0;

namespace {

int checkedEfficiency(int efficiency) {
    if (efficiency < 0 || efficiency > 100) {
        throw std::invalid_argument("Эффективность должна быть в диапазоне 0–100, получено " +
                                    std::to_string(efficiency));
    }
    return efficiency;
}

const std::string& checkedName(const std::string& name) {
    if (name.empty()) {
        throw std::invalid_argument("Имя модуля не может быть пустым");
    }
    return name;
}

}

std::string toString(ModuleState state) {
    switch (state) {
    case ModuleState::Enabled:   return "включён";
    case ModuleState::Repair:    return "ремонт";
    case ModuleState::Emergency: return "авария";
    }
    return "неизвестно";
}

Module::Module(const std::string& name, int efficiency, Durability durability,
               EnergyConsumption energyConsumption, Capacity capacity)
    : id_(0),
      name_(checkedName(name)),
      efficiency_(checkedEfficiency(efficiency)),
      durability_(durability),
      energyConsumption_(energyConsumption),
      capacity_(capacity) {
    id_ = nextId_++;
}

Module::Module(const Module& other)
    : id_(other.id_),
      name_(other.name_),
      efficiency_(other.efficiency_),
      state_(other.state_),
      active_(other.active_),
      position_(other.position_),
      durability_(other.durability_),
      energyConsumption_(other.energyConsumption_),
      capacity_(other.capacity_) {
    ++copiesMade_;
    logInfo(LogSource::MODULE, "Модуль " + label() + " скопирован");
}

Module& Module::operator=(const Module& other) {
    if (this != &other) {
        id_ = other.id_;
        name_ = other.name_;
        efficiency_ = other.efficiency_;
        state_ = other.state_;
        active_ = other.active_;
        position_ = other.position_;
        durability_ = other.durability_;
        energyConsumption_ = other.energyConsumption_;
        capacity_ = other.capacity_;
        ++copiesMade_;
        logInfo(LogSource::MODULE, "Модулю присвоена копия " + other.label());
    }
    return *this;
}

Module::Module(Module&& other) noexcept = default;

Module& Module::operator=(Module&& other) noexcept = default;

int Module::getPowerOutput() const {
    return 0;
}

std::string Module::describe() const {
    std::ostringstream out;
    out << label() << ' ' << getType() << " (" << getCategory() << ')'
        << " | эфф. " << efficiency_ << '%'
        << " | прочн. " << durability_.getValue() << '/' << durability_.getMax()
        << " | потр. " << energyConsumption_.getValue() << " кВт"
        << " | вмест. " << capacity_.getValue() << ' ' << getCapacityUnit()
        << " | " << toString(state_) << (active_ ? ", активен" : ", не активен");
    return out.str();
}

bool Module::activate() {
    if (state_ != ModuleState::Enabled) {
        logWarning(LogSource::MODULE, "Модуль " + label() + " нельзя активировать: состояние «" +
                                          toString(state_) + "»");
        return false;
    }
    if (!active_) {
        active_ = true;
        logInfo(LogSource::MODULE, "Модуль " + label() + " активирован");
    }
    return true;
}

void Module::deactivate() {
    if (active_) {
        active_ = false;
        logInfo(LogSource::MODULE, "Модуль " + label() + " деактивирован");
    }
}

void Module::setState(ModuleState newState) {
    if (newState == state_) {
        return;
    }
    logInfo(LogSource::MODULE, "Модуль " + label() + ": состояние «" + toString(state_) + "» → «" +
                                   toString(newState) + "»");
    state_ = newState;
    if (state_ != ModuleState::Enabled) {
        deactivate();
    }
}

void Module::takeDamage(int amount) {
    durability_.decrease(amount);
    logWarning(LogSource::MODULE, "Модуль " + label() + " повреждён на " + std::to_string(amount) +
                                      ", прочность " + std::to_string(durability_.getValue()) + '/' +
                                      std::to_string(durability_.getMax()));
    if (durability_.isBroken()) {
        setState(ModuleState::Emergency);
    }
}

void Module::repair(int amount) {
    durability_.repair(amount);
    logInfo(LogSource::MODULE, "Модуль " + label() + " отремонтирован на " + std::to_string(amount) +
                                   ", прочность " + std::to_string(durability_.getValue()) + '/' +
                                   std::to_string(durability_.getMax()));
    if (durability_.isFull() && state_ != ModuleState::Enabled) {
        setState(ModuleState::Enabled);
    }
}

int Module::getId() const {
    return id_;
}

const std::string& Module::getName() const {
    return name_;
}

void Module::setName(const std::string& name) {
    name_ = checkedName(name);
}

std::string Module::label() const {
    return '#' + std::to_string(id_) + " «" + name_ + "»";
}

int Module::getEfficiency() const {
    return efficiency_;
}

void Module::setEfficiency(int efficiency) {
    efficiency_ = checkedEfficiency(efficiency);
}

ModuleState Module::getState() const {
    return state_;
}

bool Module::isActive() const {
    return active_;
}

std::size_t Module::getPosition() const {
    return position_;
}

Durability& Module::durability() {
    return durability_;
}

const Durability& Module::durability() const {
    return durability_;
}

EnergyConsumption& Module::energyConsumption() {
    return energyConsumption_;
}

const EnergyConsumption& Module::energyConsumption() const {
    return energyConsumption_;
}

Capacity& Module::capacity() {
    return capacity_;
}

const Capacity& Module::capacity() const {
    return capacity_;
}

int Module::copiesMade() {
    return copiesMade_;
}

void Module::moveToIndex(std::size_t newIndex) {
    position_ = newIndex;
}
