#pragma once
#include "observer/ModuleEvent.h"

class IObserver {
public:
    virtual ~IObserver() = default;
    virtual void onModuleEvent(const ModuleEvent& event) = 0;
};
