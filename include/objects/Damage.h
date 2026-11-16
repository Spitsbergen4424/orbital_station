#pragma once
#include <string>
enum class DamageType { Radiation, Physical, Electrical, Unknown };
struct Damage { int amount = 0; DamageType type = DamageType::Unknown; };
const char* damageTypeName(DamageType type) noexcept;
