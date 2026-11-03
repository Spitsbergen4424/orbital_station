#include "station/Station.h"

#include <algorithm>
#include <stdexcept>
#include <utility>

Station::Station(std::size_t maxModules) : modules_(), maxModules_(maxModules) {
    if (maxModules_ == 0) {
        throw std::invalid_argument("Максимальное число модулей должно быть положительным");
    }
}

// Копирующий конструктор: глубокая копия — каждый модуль копируется через clone()
Station::Station(const Station& other) : modules_(), maxModules_(other.maxModules_) {
    modules_.reserve(other.modules_.size());
    for (const std::unique_ptr<Module>& module : other.modules_) {
        modules_.push_back(module->clone());
    }
}

// Копирующее присваивание: с проверкой на самоприсваивание
Station& Station::operator=(const Station& other) {
    if (this != &other) {
        // Сначала копируем в отдельный буфер: присваивание не сломает станцию,
        // если clone() одного из модулей бросит исключение
        std::vector<std::unique_ptr<Module>> copy;
        copy.reserve(other.modules_.size());
        for (const std::unique_ptr<Module>& module : other.modules_) {
            copy.push_back(module->clone());
        }
        modules_ = std::move(copy);
        maxModules_ = other.maxModules_;
    }
    return *this;
}

// Перемещающий конструктор: забирает вектор модулей у other, копирования нет
Station::Station(Station&& other) noexcept
    : modules_(std::move(other.modules_)), maxModules_(other.maxModules_) {
    other.modules_.clear();  // оставляем other в корректном пустом состоянии
}

// Присваивание перемещением: с проверкой на самоприсваивание
Station& Station::operator=(Station&& other) noexcept {
    if (this != &other) {
        modules_ = std::move(other.modules_);
        maxModules_ = other.maxModules_;
        other.modules_.clear();
    }
    return *this;
}

// Деструктор: vector сам удалит все unique_ptr и, значит, сами модули
Station::~Station() = default;

bool Station::addModule(std::unique_ptr<Module> module) {
    if (!module) {
        return false;  // пустой указатель добавлять нельзя
    }
    if (modules_.size() >= maxModules_) {
        return false;  // превышен лимит модулей
    }
    modules_.push_back(std::move(module));  // станция становится владельцем модуля
    return true;
}

bool Station::removeById(int id) {
    for (auto it = modules_.begin(); it != modules_.end(); ++it) {
        if ((*it)->getId() == id) {
            modules_.erase(it);
            return true;
        }
    }
    return false;  // модуля с таким id нет
}

bool Station::removeByIndex(std::size_t index) {
    if (index >= modules_.size()) {
        return false;  // индекс за пределами вектора
    }
    modules_.erase(modules_.begin() + static_cast<std::ptrdiff_t>(index));
    return true;
}

Module* Station::findById(int id) {
    for (std::unique_ptr<Module>& module : modules_) {
        if (module->getId() == id) {
            return module.get();
        }
    }
    return nullptr;  // модуля с таким id нет
}

int Station::indexOf(int id) const {
    for (std::size_t i = 0; i < modules_.size(); ++i) {
        if (modules_[i]->getId() == id) {
            return static_cast<int>(i);
        }
    }
    return -1;  // модуля с таким id нет
}

bool Station::moveModule(std::size_t fromIndex, std::size_t toIndex) {
    if (fromIndex >= modules_.size() || toIndex >= modules_.size()) {
        return false;  // индексы за пределами вектора
    }
    if (fromIndex == toIndex) {
        return true;  // перемещать некуда
    }
    // rotate сохраняет порядок остальных модулей: модуль "вставляется" на новое место
    auto first = modules_.begin();
    auto from = static_cast<std::ptrdiff_t>(fromIndex);
    auto to = static_cast<std::ptrdiff_t>(toIndex);
    if (fromIndex < toIndex) {
        std::rotate(first + from, first + from + 1, first + to + 1);
    } else {
        std::rotate(first + to, first + from, first + from + 1);
    }
    return true;
}

// Сортировка по убыванию эффективности с использованием итератора (сортировка выбором)
void Station::sortByEfficiency() {
    for (StationIterator current = begin(); current != end(); ++current) {
        StationIterator best = current;
        for (StationIterator candidate = current; candidate != end(); ++candidate) {
            if (candidate->getEfficiency() > best->getEfficiency()) {
                best = candidate;
            }
        }
        if (best != current) {
            // меняем местами сами unique_ptr, а не модули
            std::iter_swap(current.base(), best.base());
        }
    }
}

std::size_t Station::size() const {
    return modules_.size();
}

std::size_t Station::getMaxModules() const {
    return maxModules_;
}

StationIterator Station::begin() {
    return StationIterator(modules_.begin(), modules_.end());
}

StationIterator Station::end() {
    return StationIterator(modules_.end(), modules_.end());
}

StationIterator Station::getIterator() {
    return begin();
}

ConstStationIterator Station::begin() const {
    return ConstStationIterator(modules_.cbegin(), modules_.cend());
}

ConstStationIterator Station::end() const {
    return ConstStationIterator(modules_.cend(), modules_.cend());
}

ConstStationIterator Station::getIterator() const {
    return begin();
}
