#pragma once

#include <memory>
#include <vector>

#include "Module.h"

// Итератор по модулям станции: при разыменовании отдаёт сам модуль (Module&),
// а не unique_ptr, в котором модуль хранится
class StationIterator {
public:
    StationIterator(std::vector<std::unique_ptr<Module>>::iterator current,
                    std::vector<std::unique_ptr<Module>>::iterator end);

    Module& operator*() const;      // текущий модуль
    StationIterator& operator++();  // перейти к следующему модулю
    
    Module* operator->() const { return current_->get(); }  // доступ к полям текущего модуля
    bool operator==(const StationIterator& other) const;
    bool operator!=(const StationIterator& other) const;

    bool hasNext() const;  // false, когда модули закончились

private:
    std::vector<std::unique_ptr<Module>>::iterator current_;  // текущий модуль
    std::vector<std::unique_ptr<Module>>::iterator end_;      // позиция за последним модулем
};
