#pragma once
#include "objects/ExternalObject.h"
class MeteorShower final : public ExternalObject { public: explicit MeteorShower(int strength=15); void interact(CommandCenter&,Module&) override; };
