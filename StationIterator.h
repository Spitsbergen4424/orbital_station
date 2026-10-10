#pragma once

#include <cstddef>
#include <iterator>
#include <memory>
#include <vector>

#include "Module.h"

using ModuleContainer = std::vector<std::unique_ptr<Module>>;

class StationIterator {
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = Module;
    using difference_type = std::ptrdiff_t;
    using pointer = Module*;
    using reference = Module&;

    StationIterator() = default;
    StationIterator(ModuleContainer::iterator current, ModuleContainer::iterator end);

    reference operator*() const;
    pointer operator->() const;
    StationIterator& operator++();
    StationIterator operator++(int);
    bool operator==(const StationIterator& other) const;
    bool operator!=(const StationIterator& other) const;

    bool hasNext() const;
    reference next();
    reference current() const;

    ModuleContainer::iterator base() const;

private:
    ModuleContainer::iterator current_{};
    ModuleContainer::iterator end_{};
};

class ConstStationIterator {
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = Module;
    using difference_type = std::ptrdiff_t;
    using pointer = const Module*;
    using reference = const Module&;

    ConstStationIterator() = default;
    ConstStationIterator(ModuleContainer::const_iterator current, ModuleContainer::const_iterator end);

    reference operator*() const;
    pointer operator->() const;
    ConstStationIterator& operator++();
    ConstStationIterator operator++(int);
    bool operator==(const ConstStationIterator& other) const;
    bool operator!=(const ConstStationIterator& other) const;

    bool hasNext() const;
    reference next();
    reference current() const;

private:
    ModuleContainer::const_iterator current_{};
    ModuleContainer::const_iterator end_{};
};
