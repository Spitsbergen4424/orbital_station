#include "station/StationIterator.h"

// ---------- StationIterator ----------

StationIterator::StationIterator(ModuleContainer::iterator current, ModuleContainer::iterator end)
    : current_(current), end_(end) {}

Module& StationIterator::operator*() const {
    return **current_;  // отдаём сам модуль, а не unique_ptr
}

StationIterator& StationIterator::operator++() {
    ++current_;
    return *this;
}

bool StationIterator::operator==(const StationIterator& other) const {
    return current_ == other.current_;
}

bool StationIterator::operator!=(const StationIterator& other) const {
    return current_ != other.current_;
}

bool StationIterator::hasNext() const {
    return current_ != end_;
}

ModuleContainer::iterator StationIterator::base() const {
    return current_;
}

// ---------- ConstStationIterator ----------

ConstStationIterator::ConstStationIterator(ModuleContainer::const_iterator current,
                                           ModuleContainer::const_iterator end)
    : current_(current), end_(end) {}

const Module& ConstStationIterator::operator*() const {
    return **current_;  // отдаём сам модуль, а не unique_ptr
}

ConstStationIterator& ConstStationIterator::operator++() {
    ++current_;
    return *this;
}

bool ConstStationIterator::operator==(const ConstStationIterator& other) const {
    return current_ == other.current_;
}

bool ConstStationIterator::operator!=(const ConstStationIterator& other) const {
    return current_ != other.current_;
}

bool ConstStationIterator::hasNext() const {
    return current_ != end_;
}
