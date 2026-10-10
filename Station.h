#pragma once

#include <cstddef>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

#include "Module.h"
#include "StationIterator.h"

class Station {
public:
    Station(const std::string& name, std::size_t maxModules);

    Station(const Station& other);
    Station& operator=(const Station& other);
    Station(Station&& other) noexcept;
    Station& operator=(Station&& other) noexcept;
    ~Station();

    void addModule(std::unique_ptr<Module> module);
    bool removeById(int id);
    void removeByIndex(std::size_t index);
    Module* findById(int id);
    const Module* findById(int id) const;
    Module& at(std::size_t index);
    const Module& at(std::size_t index) const;
    void moveModule(std::size_t fromIndex, std::size_t toIndex);
    void sortByEfficiency();

    StationIterator begin();
    StationIterator end();
    ConstStationIterator begin() const;
    ConstStationIterator end() const;
    StationIterator getIterator();
    ConstStationIterator getIterator() const;

    const std::string& getName() const;
    void setName(const std::string& name);
    std::size_t size() const;
    std::size_t getMaxModules() const;
    bool isFull() const;
    bool isEmpty() const;
    int energyBalance() const;
    void print(std::ostream& out) const;

private:
    static ModuleContainer cloneModules(const ModuleContainer& source);
    void checkIndex(std::size_t index) const;
    void reindex();

    std::string name_;
    ModuleContainer modules_;
    std::size_t maxModules_;
};
