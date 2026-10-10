#include "Station.h"

#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <utility>

#include "Logger.h"

namespace {

std::string quoted(const std::string& text) {
    return "«" + text + "»";
}

}

Station::Station(const std::string& name, std::size_t maxModules) : name_(name), maxModules_(maxModules) {
    if (maxModules_ == 0) {
        throw std::invalid_argument("Станция " + quoted(name_) +
                                    ": максимальное число модулей должно быть положительным");
    }
    logInfo(LogSource::STATION, "Создана станция " + quoted(name_) + ", лимит модулей: " +
                                    std::to_string(maxModules_));
}

Station::Station(const Station& other)
    : name_(other.name_), modules_(cloneModules(other.modules_)), maxModules_(other.maxModules_) {
    logInfo(LogSource::STATION, "Станция " + quoted(name_) + " скопирована конструктором копирования (модулей: " +
                                    std::to_string(modules_.size()) + ")");
}

Station& Station::operator=(const Station& other) {
    if (this == &other) {
        logWarning(LogSource::STATION, "Самоприсваивание станции " + quoted(name_) + " пропущено");
        return *this;
    }
    ModuleContainer copy = cloneModules(other.modules_);
    name_ = other.name_;
    maxModules_ = other.maxModules_;
    modules_ = std::move(copy);
    logInfo(LogSource::STATION, "Станции присвоена копия " + quoted(name_) + " (модулей: " +
                                    std::to_string(modules_.size()) + ")");
    return *this;
}

Station::Station(Station&& other) noexcept
    : name_(std::move(other.name_)), modules_(std::move(other.modules_)), maxModules_(other.maxModules_) {
    other.name_.clear();
    other.modules_.clear();
    logInfo(LogSource::STATION, "Станция " + quoted(name_) + " перемещена конструктором перемещения (модулей: " +
                                    std::to_string(modules_.size()) + ")");
}

Station& Station::operator=(Station&& other) noexcept {
    if (this != &other) {
        name_ = std::move(other.name_);
        modules_ = std::move(other.modules_);
        maxModules_ = other.maxModules_;
        other.name_.clear();
        other.modules_.clear();
        logInfo(LogSource::STATION, "Станции присвоена перемещением " + quoted(name_) + " (модулей: " +
                                        std::to_string(modules_.size()) + ")");
    }
    return *this;
}

Station::~Station() = default;

void Station::addModule(std::unique_ptr<Module> module) {
    if (!module) {
        throw std::invalid_argument("Нельзя добавить пустой модуль");
    }
    if (isFull()) {
        logError(LogSource::STATION, "Модуль " + module->label() + " не добавлен: станция " + quoted(name_) +
                                         " заполнена (" + std::to_string(maxModules_) + ")");
        throw std::length_error("Станция " + quoted(name_) + " заполнена: лимит " +
                                std::to_string(maxModules_) + " модулей");
    }
    const std::string label = module->label();
    module->moveToIndex(modules_.size());
    modules_.push_back(std::move(module));
    logInfo(LogSource::STATION, "Модуль " + label + " добавлен на станцию " + quoted(name_) + " (" +
                                    std::to_string(modules_.size()) + '/' + std::to_string(maxModules_) + ")");
}

bool Station::removeById(int id) {
    auto it = std::find_if(modules_.begin(), modules_.end(),
                           [id](const std::unique_ptr<Module>& module) { return module->getId() == id; });
    if (it == modules_.end()) {
        logWarning(LogSource::STATION, "Модуль #" + std::to_string(id) + " не найден на станции " + quoted(name_));
        return false;
    }
    const std::string label = (*it)->label();
    modules_.erase(it);
    reindex();
    logInfo(LogSource::STATION, "Модуль " + label + " удалён по id");
    return true;
}

void Station::removeByIndex(std::size_t index) {
    checkIndex(index);
    const std::string label = modules_[index]->label();
    modules_.erase(modules_.begin() + static_cast<std::ptrdiff_t>(index));
    reindex();
    logInfo(LogSource::STATION, "Модуль " + label + " удалён по индексу " + std::to_string(index));
}

const Module* Station::findById(int id) const {
    auto it = std::find_if(modules_.cbegin(), modules_.cend(),
                           [id](const std::unique_ptr<Module>& module) { return module->getId() == id; });
    return it != modules_.cend() ? it->get() : nullptr;
}

Module* Station::findById(int id) {
    return const_cast<Module*>(static_cast<const Station&>(*this).findById(id));
}

const Module& Station::at(std::size_t index) const {
    checkIndex(index);
    return *modules_[index];
}

Module& Station::at(std::size_t index) {
    checkIndex(index);
    return *modules_[index];
}

void Station::moveModule(std::size_t fromIndex, std::size_t toIndex) {
    checkIndex(fromIndex);
    checkIndex(toIndex);
    if (fromIndex == toIndex) {
        return;
    }
    const auto first = modules_.begin();
    const auto from = static_cast<std::ptrdiff_t>(fromIndex);
    const auto to = static_cast<std::ptrdiff_t>(toIndex);
    if (fromIndex < toIndex) {
        std::rotate(first + from, first + from + 1, first + to + 1);
    } else {
        std::rotate(first + to, first + from, first + from + 1);
    }
    reindex();
    logInfo(LogSource::STATION, "Модуль " + modules_[toIndex]->label() + " перемещён: позиция " +
                                    std::to_string(fromIndex) + " → " + std::to_string(toIndex));
}

void Station::sortByEfficiency() {
    for (StationIterator current = begin(); current != end(); ++current) {
        StationIterator best = current;
        for (StationIterator candidate = std::next(current); candidate != end(); ++candidate) {
            if (candidate->getEfficiency() > best->getEfficiency()) {
                best = candidate;
            }
        }
        if (best != current) {
            std::iter_swap(current.base(), best.base());
        }
    }
    reindex();
    logInfo(LogSource::STATION, "Модули станции " + quoted(name_) + " отсортированы по эффективности");
}

StationIterator Station::begin() {
    return StationIterator(modules_.begin(), modules_.end());
}

StationIterator Station::end() {
    return StationIterator(modules_.end(), modules_.end());
}

ConstStationIterator Station::begin() const {
    return ConstStationIterator(modules_.cbegin(), modules_.cend());
}

ConstStationIterator Station::end() const {
    return ConstStationIterator(modules_.cend(), modules_.cend());
}

StationIterator Station::getIterator() {
    return begin();
}

ConstStationIterator Station::getIterator() const {
    return begin();
}

const std::string& Station::getName() const {
    return name_;
}

void Station::setName(const std::string& name) {
    logInfo(LogSource::STATION, "Станция " + quoted(name_) + " переименована в " + quoted(name));
    name_ = name;
}

std::size_t Station::size() const {
    return modules_.size();
}

std::size_t Station::getMaxModules() const {
    return maxModules_;
}

bool Station::isFull() const {
    return modules_.size() >= maxModules_;
}

bool Station::isEmpty() const {
    return modules_.empty();
}

int Station::energyBalance() const {
    int balance = 0;
    for (const Module& module : *this) {
        if (module.isActive()) {
            balance += module.getPowerOutput() - module.energyConsumption().getValue();
        }
    }
    return balance;
}

void Station::print(std::ostream& out) const {
    out << "Станция " << quoted(name_) << ": модулей " << modules_.size() << '/' << maxModules_
        << ", энергобаланс " << energyBalance() << " кВт\n";
    ConstStationIterator it = getIterator();
    while (it.hasNext()) {
        const Module& module = it.next();
        out << "  [" << module.getPosition() << "] " << module.describe() << '\n';
    }
}

ModuleContainer Station::cloneModules(const ModuleContainer& source) {
    ModuleContainer copy;
    copy.reserve(source.size());
    for (const auto& module : source) {
        copy.push_back(module->clone());
    }
    return copy;
}

void Station::checkIndex(std::size_t index) const {
    if (index >= modules_.size()) {
        throw std::out_of_range("Станция " + quoted(name_) + ": индекс " + std::to_string(index) +
                                " вне диапазона [0, " + std::to_string(modules_.size()) + ")");
    }
}

void Station::reindex() {
    for (std::size_t i = 0; i < modules_.size(); ++i) {
        modules_[i]->moveToIndex(i);
    }
}
