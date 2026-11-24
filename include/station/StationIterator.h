#pragma once

#include <iterator>
#include <memory>
#include <vector>

#include "modules/Module.h"

using ModuleContainer = std::vector<std::unique_ptr<Module>>;

// Итератор по модулям станции: при разыменовании отдаёт сам модуль (Module&),
// а не unique_ptr, в котором модуль хранится
class StationIterator {
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = Module;
    using difference_type = std::ptrdiff_t;
    using pointer = Module*;
    using reference = Module&;

    StationIterator() = default;
    StationIterator(ModuleContainer::iterator current, ModuleContainer::iterator end);

    Module& operator*() const;      // текущий модуль
    StationIterator& operator++();  // перейти к следующему модулю

    Module* operator->() const { return current_->get(); }  // доступ к полям текущего модуля
    bool operator==(const StationIterator& other) const;
    bool operator!=(const StationIterator& other) const;

    bool hasNext() const;  // false, когда модули закончились

    ModuleContainer::iterator base() const;  // лежащий в основе итератор вектора

private:
    ModuleContainer::iterator current_;  // текущий модуль
    ModuleContainer::iterator end_;      // позиция за последним модулем
};

// Константный итератор: для обхода станции, которую нельзя изменять
class ConstStationIterator {
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = Module;
    using difference_type = std::ptrdiff_t;
    using pointer = const Module*;
    using reference = const Module&;

    ConstStationIterator() = default;
    ConstStationIterator(ModuleContainer::const_iterator current, ModuleContainer::const_iterator end);

    const Module& operator*() const;      // текущий модуль (только чтение)
    ConstStationIterator& operator++();   // перейти к следующему модулю

    const Module* operator->() const { return current_->get(); }  // доступ к полям текущего модуля
    bool operator==(const ConstStationIterator& other) const;
    bool operator!=(const ConstStationIterator& other) const;

    bool hasNext() const;  // false, когда модули закончились

private:
    ModuleContainer::const_iterator current_;  // текущий модуль
    ModuleContainer::const_iterator end_;      // позиция за последним модулем
};
