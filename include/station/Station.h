#pragma once

#include <cstddef>
#include <memory>
#include <vector>

#include "modules/Module.h"
#include "station/StationIterator.h"

// Орбитальная станция — контейнер модулей с ограничением на их количество
class Station {
public:
    Station(std::size_t maxModules);  // std::invalid_argument, если maxModules == 0

    // Правило пяти
    Station(const Station& other);                 // копия: каждый модуль копируется через clone()
    Station& operator=(const Station& other);      // присваивание копированием
    Station(Station&& other) noexcept;             // перемещение: забирает модули у other
    Station& operator=(Station&& other) noexcept;  // присваивание перемещением
    ~Station();

    bool addModule(std::unique_ptr<Module> module);  // false, если станция заполнена
    bool removeById(int id);                         // false, если модуля с таким id нет
    bool removeByIndex(std::size_t index);           // false, если индекс за пределами
    Module* findById(int id);                        // nullptr, если модуля с таким id нет
    int indexOf(int id) const;                       // -1, если модуля с таким id нет

    bool moveModule(std::size_t fromIndex, std::size_t toIndex);  // переставить модуль
    void sortByEfficiency();                                      // по убыванию эффективности

    std::size_t size() const;
    std::size_t getMaxModules() const;

    // Обход модулей: for (Module& module : station)
    StationIterator begin();
    StationIterator end();
    StationIterator getIterator();  // для обхода через hasNext()

    // Обход без изменения станции: for (const Module& module : station)
    ConstStationIterator begin() const;
    ConstStationIterator end() const;
    ConstStationIterator getIterator() const;

private:
    std::vector<std::unique_ptr<Module>> modules_;  // станция владеет модулями
    std::size_t maxModules_;
};
