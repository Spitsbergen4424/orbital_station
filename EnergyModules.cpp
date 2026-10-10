#include "EnergyModules.h"

#include <stdexcept>
#include <utility>

#include "Logger.h"

EnergyModule::EnergyModule(const std::string& name, int efficiency, Durability durability,
                           EnergyConsumption energyConsumption, Capacity capacity, int nominalPower)
    : Module(name, efficiency, durability, energyConsumption, capacity), nominalPower_(nominalPower) {
    if (nominalPower < 0) {
        throw std::invalid_argument("Номинальная мощность не может быть отрицательной");
    }
}

std::string EnergyModule::getCategory() const {
    return "Энергетический";
}

std::string EnergyModule::getCapacityUnit() const {
    return "кВт·ч";
}

int EnergyModule::getPowerOutput() const {
    if (!isActive()) {
        return 0;
    }
    return nominalPower_ * getEfficiency() / 100;
}

std::string EnergyModule::describe() const {
    return Module::describe() + " | выработка " + std::to_string(getPowerOutput()) + '/' +
           std::to_string(nominalPower_) + " кВт";
}

int EnergyModule::getNominalPower() const {
    return nominalPower_;
}

SolarModule::SolarModule(const std::string& name, int panelArea)
    : EnergyModule(name, 70, Durability(80, 80), EnergyConsumption(1), Capacity(200), panelArea / 2),
      panelArea_(panelArea) {}

std::unique_ptr<Module> SolarModule::clone() const {
    return std::make_unique<SolarModule>(*this);
}

std::string SolarModule::getType() const {
    return "SolarModule";
}

std::string SolarModule::describe() const {
    return EnergyModule::describe() + " | панели " + std::to_string(panelArea_) + " м²";
}

int SolarModule::getPanelArea() const {
    return panelArea_;
}

ReactorModule::ReactorModule(const std::string& name, int nominalPower)
    : EnergyModule(name, 95, Durability(150, 150), EnergyConsumption(10), Capacity(500), nominalPower),
      core_(std::make_unique<ReactorCore>()) {}

ReactorModule::ReactorModule(const ReactorModule& other)
    : EnergyModule(other),
      core_(other.core_ ? std::make_unique<ReactorCore>(*other.core_) : nullptr) {}

ReactorModule& ReactorModule::operator=(const ReactorModule& other) {
    if (this != &other) {
        std::unique_ptr<ReactorCore> newCore =
            other.core_ ? std::make_unique<ReactorCore>(*other.core_) : nullptr;
        EnergyModule::operator=(other);
        core_ = std::move(newCore);
    }
    return *this;
}

ReactorModule::ReactorModule(ReactorModule&& other) noexcept
    : EnergyModule(std::move(other)), core_(std::move(other.core_)) {}

ReactorModule& ReactorModule::operator=(ReactorModule&& other) noexcept {
    if (this != &other) {
        EnergyModule::operator=(std::move(other));
        core_ = std::move(other.core_);
    }
    return *this;
}

ReactorModule::~ReactorModule() = default;

std::unique_ptr<Module> ReactorModule::clone() const {
    return std::make_unique<ReactorModule>(*this);
}

std::string ReactorModule::getType() const {
    return "ReactorModule";
}

std::string ReactorModule::describe() const {
    if (!core_) {
        return EnergyModule::describe() + " | активная зона отсутствует";
    }
    return EnergyModule::describe() + " | топливо " + std::to_string(core_->fuelLevel) + "%, " +
           std::to_string(core_->temperature) + " K";
}

int ReactorModule::getPowerOutput() const {
    if (!core_ || core_->fuelLevel == 0) {
        return 0;
    }
    return EnergyModule::getPowerOutput();
}

bool ReactorModule::hasCore() const {
    return core_ != nullptr;
}

int ReactorModule::getFuelLevel() const {
    return core_ ? core_->fuelLevel : 0;
}

void ReactorModule::burnFuel(int percent) {
    if (percent < 0) {
        throw std::invalid_argument("Расход топлива не может быть отрицательным");
    }
    ReactorCore& reactorCore = core();
    reactorCore.fuelLevel = percent >= reactorCore.fuelLevel ? 0 : reactorCore.fuelLevel - percent;
    logInfo(LogSource::MODULE, "Реактор " + label() + ": остаток топлива " +
                                   std::to_string(reactorCore.fuelLevel) + "%");
}

void ReactorModule::refuel() {
    core().fuelLevel = 100;
    logInfo(LogSource::MODULE, "Реактор " + label() + " заправлен");
}

ReactorCore& ReactorModule::core() {
    if (!core_) {
        throw std::logic_error("У реактора " + label() + " нет активной зоны (объект перемещён)");
    }
    return *core_;
}
