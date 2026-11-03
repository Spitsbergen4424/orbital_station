#pragma once

#include <cstddef>
#include <memory>
#include <string>

#include "attributes/Capacity.h"
#include "attributes/Durability.h"
#include "attributes/EnergyConsumption.h"

class Station;  // объявление станции: полное описание понадобится только в Module.cpp

// Состояния модуля
enum class ModuleState {
    Off,        // выключен
    On,         // включён
    Repair,     // ремонт
    Emergency   // авария
};

// Абстрактный базовый класс модуля станции
class Module {
public:
    static constexpr int MIN_EFFICIENCY = 0;
    static constexpr int MAX_EFFICIENCY = 100;

    Module(const std::string& name, int durability, int energyConsumption,
           int capacity, int efficiency);
    virtual ~Module() {}

    // Чисто виртуальные методы: их реализуют наследники
    virtual std::unique_ptr<Module> clone() const = 0;  // копия модуля
    virtual std::string getType() const = 0;            // "CrewCabin", "SolarModule", ...

    int getId() const;
    const std::string& getName() const;
    int getEfficiency() const;
    ModuleState getState() const;
    std::string getStateName() const;  // "выключен", "включён", "ремонт", "авария"
    const Durability& getDurability() const;
    const EnergyConsumption& getEnergyConsumption() const;
    const Capacity& getCapacity() const;

    void setName(const std::string& name);
    void setEfficiency(int efficiency);  // от 0 до 100

    bool activate();                      // включить; false, если модуль в аварии или сломан
    void deactivate();                    // выключить
    void setState(ModuleState newState);  // задать состояние
    void damage(int amount);              // урон; при прочности 0 модуль в аварии
    void repair(int amount);              // ремонт; из аварии модуль переходит в "ремонт"
    void moveToIndex(Station& station, std::size_t newIndex);  // переставить модуль на станции

    void print() const;

private:
    static int nextId_;  // id для следующего созданного модуля

    int id_;
    std::string name_;
    int efficiency_;
    ModuleState state_;
    Durability durability_;
    EnergyConsumption energyConsumption_;
    Capacity capacity_;
};
