#pragma once

#include <memory>
#include <vector>

#include "modules/Module.h"
#include "station/StationIterator.h"

class Station {
public:
    Station(std::size_t maxModules);

    // Правило пяти
    Station(const Station& other);
    Station& operator=(const Station& other);
    Station(Station&& other) noexcept;
    Station& operator=(Station&& other) noexcept;
    ~Station();

    bool addModule(std::unique_ptr<Module> module);
    bool removeById(int id);
    bool removeByIndex(std::size_t index);
    Module* findById(int id);
    int indexOf(int id) const;

    bool moveModule(std::size_t fromIndex, std::size_t toIndex); 
    void sortByEfficiency(); 

    std::size_t size() const;
    std::size_t getMaxModules() const;


    StationIterator begin();
    StationIterator end();
    StationIterator getIterator();  


    ConstStationIterator begin() const;
    ConstStationIterator end() const;
    ConstStationIterator getIterator() const;

private:
    std::vector<std::unique_ptr<Module>> modules_;  
    std::size_t maxModules_;
};
