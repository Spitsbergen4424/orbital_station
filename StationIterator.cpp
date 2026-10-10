#include "StationIterator.h"

#include <stdexcept>

StationIterator::StationIterator(ModuleContainer::iterator current, ModuleContainer::iterator end)
    : current_(current), end_(end) {}

StationIterator::reference StationIterator::operator*() const {
    return **current_;
}

StationIterator::pointer StationIterator::operator->() const {
    return current_->get();
}

StationIterator& StationIterator::operator++() {
    ++current_;
    return *this;
}

StationIterator StationIterator::operator++(int) {
    StationIterator previous = *this;
    ++current_;
    return previous;
}

bool StationIterator::operator==(const StationIterator& other) const {
    return current_ == other.current_;
}

bool StationIterator::operator!=(const StationIterator& other) const {
    return !(*this == other);
}

bool StationIterator::hasNext() const {
    return current_ != end_;
}

StationIterator::reference StationIterator::next() {
    if (!hasNext()) {
        throw std::out_of_range("StationIterator: модулей больше нет");
    }
    Module& module = **current_;
    ++current_;
    return module;
}

StationIterator::reference StationIterator::current() const {
    if (!hasNext()) {
        throw std::out_of_range("StationIterator: итератор за последним модулем");
    }
    return **current_;
}

ModuleContainer::iterator StationIterator::base() const {
    return current_;
}

ConstStationIterator::ConstStationIterator(ModuleContainer::const_iterator current,
                                           ModuleContainer::const_iterator end)
    : current_(current), end_(end) {}

ConstStationIterator::reference ConstStationIterator::operator*() const {
    return **current_;
}

ConstStationIterator::pointer ConstStationIterator::operator->() const {
    return current_->get();
}

ConstStationIterator& ConstStationIterator::operator++() {
    ++current_;
    return *this;
}

ConstStationIterator ConstStationIterator::operator++(int) {
    ConstStationIterator previous = *this;
    ++current_;
    return previous;
}

bool ConstStationIterator::operator==(const ConstStationIterator& other) const {
    return current_ == other.current_;
}

bool ConstStationIterator::operator!=(const ConstStationIterator& other) const {
    return !(*this == other);
}

bool ConstStationIterator::hasNext() const {
    return current_ != end_;
}

ConstStationIterator::reference ConstStationIterator::next() {
    if (!hasNext()) {
        throw std::out_of_range("ConstStationIterator: модулей больше нет");
    }
    const Module& module = **current_;
    ++current_;
    return module;
}

ConstStationIterator::reference ConstStationIterator::current() const {
    if (!hasNext()) {
        throw std::out_of_range("ConstStationIterator: итератор за последним модулем");
    }
    return **current_;
}
