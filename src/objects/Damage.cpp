#include "objects/Damage.h"
const char* damageTypeName(DamageType type) noexcept {
    switch(type) { case DamageType::Radiation:return "радиационный"; case DamageType::Physical:return "физический"; case DamageType::Electrical:return "электрический"; default:return "неизвестный"; }
}
