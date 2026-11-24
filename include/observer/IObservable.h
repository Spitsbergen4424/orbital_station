#pragma once
#include <algorithm>
#include <vector>
#include "observer/IObserver.h"

class IObservable {
public:
    IObservable() = default;
    // Подписчики не копируются: копия модуля не должна уведомлять старый CommandCenter.
    IObservable(const IObservable&) {}
    IObservable& operator=(const IObservable&) { observers_.clear(); return *this; }
    virtual ~IObservable() = default;
    virtual void subscribe(IObserver* observer) {
        if (observer && std::find(observers_.begin(), observers_.end(), observer) == observers_.end()) observers_.push_back(observer);
    }
    virtual void unsubscribe(IObserver* observer) {
        observers_.erase(std::remove(observers_.begin(), observers_.end(), observer), observers_.end());
    }
    virtual void notify(const ModuleEvent& event) {
        // Копия списка защищает обход от изменения подписок во время callback.
        const auto snapshot = observers_;
        for (IObserver* observer : snapshot)
            if (observer && std::find(observers_.begin(), observers_.end(), observer) != observers_.end()) observer->onModuleEvent(event);
    }
private:
    std::vector<IObserver*> observers_;
};
