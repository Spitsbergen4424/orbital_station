#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif

#include "factories/AdvancedModuleFactory.h"
#include "factories/StandardModuleFactory.h"
#include "modules/Module.h"
#include "station/Station.h"

namespace {

void setupConsole() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);  // русский текст в консоли Windows
#endif
}

void printHeader(const std::string& title) {
    std::cout << "\n========== " << title << " ==========\n";
}

// Обход станции константным итератором: станцию не изменяем
void printStation(const Station& station, const std::string& title) {
    std::cout << title << " (модулей " << station.size() << "/" << station.getMaxModules() << "):\n";
    for (ConstStationIterator it = station.getIterator(); it != station.end(); ++it) {
        std::cout << "  ";
        it->print();
    }
}

const void* address(const Module& module) {
    return static_cast<const void*>(&module);
}

// Сценарий 1: создание станции, добавление модулей через разные фабрики,
// вывод списка через итератор
void scenarioCreate() {
    printHeader("Сценарий 1: создание станции и модулей через разные фабрики");

    StandardModuleFactory standard;
    AdvancedModuleFactory advanced;
    Station station(5);

    station.addModule(standard.createResidentialModule("Каюта экипажа A"));
    station.addModule(advanced.createResidentialModule("Медицинский отсек"));
    station.addModule(standard.createEnergyModule("Солнечная панель"));
    station.addModule(advanced.createEnergyModule("Реактор"));
    station.addModule(standard.createResearchModule("Биолаборатория"));

    std::cout << "Список модулей через getIterator() и hasNext():\n";
    for (StationIterator it = station.getIterator(); it.hasNext(); ++it) {
        std::cout << "  ";
        it->print();
    }

    std::cout << "\nТот же список через range-based for (begin()/end()):\n";
    for (const Module& module : station) {
        std::cout << "  [ID " << module.getId() << "] " << module.getName()
                  << " — " << module.getType() << "\n";
    }
}

// Сценарий 2: копирование станции, изменение копии,
// проверка независимости оригинала
void scenarioCopy() {
    printHeader("Сценарий 2: копирование станции (глубокая копия)");

    StandardModuleFactory standard;
    Station original(4);
    original.addModule(standard.createResidentialModule("Каюта экипажа A"));
    original.addModule(standard.createEnergyModule("Солнечная панель"));
    original.addModule(standard.createResearchModule("Биолаборатория"));

    std::cout << "Адрес первого модуля оригинала: " << address(*original.begin()) << "\n";

    Station copy(original);  // копирующий конструктор: каждый модуль через clone()
    std::cout << "Адрес первого модуля копии:    " << address(*copy.begin())
              << " (объекты разные — копия глубокая)\n";

    std::cout << "\nИзменяем копию: переименование, повреждение, удаление, добавление\n";
    copy.begin()->setName("Каюта экипажа A (изменённая)");
    copy.begin()->damage(40);
    copy.removeByIndex(copy.size() - 1);
    copy.addModule(standard.createResearchModule("Доп. лаборатория"));

    printStation(copy, "Копия после изменений");

    // самоприсваивание не должно ничего сломать
    Station& sameStation = copy;
    copy = sameStation;
    std::cout << "Самоприсваивание прошло без ошибок (модулей: " << copy.size() << ")\n";

    // копирующее присваивание
    Station assigned(2);
    assigned = original;
    std::cout << "Копирующее присваивание: модулей " << assigned.size()
              << ", лимит " << assigned.getMaxModules() << "\n";

    const Module& origFirst = *original.begin();
    std::cout << "\nОригинал не изменился: имя \"" << origFirst.getName()
              << "\", прочность " << origFirst.getDurability().getValue()
              << "/" << origFirst.getDurability().getMaxValue()
              << ", модулей " << original.size() << "\n";
}

// Сценарий 3: перемещение станции через конструктор перемещения,
// проверка отсутствия копирования
void scenarioMove() {
    printHeader("Сценарий 3: перемещение станции");

    StandardModuleFactory standard;
    Station source(3);
    source.addModule(standard.createEnergyModule("Солнечная панель"));
    source.addModule(standard.createResearchModule("Биолаборатория"));

    const void* before = address(*source.begin());
    std::cout << "До перемещения: модулей " << source.size()
              << ", адрес первого модуля " << before << "\n";

    Station moved(std::move(source));  // перемещающий конструктор: вектор переезжает целиком
    std::cout << "После перемещения: модулей " << moved.size()
              << ", адрес первого модуля " << address(*moved.begin())
              << " (совпадает — копирования не было)\n";
    std::cout << "Источник после перемещения: модулей " << source.size()
              << " (станция пуста)\n";

    Station target(2);
    target = std::move(moved);  // присваивание перемещением
    std::cout << "После присваивания перемещением: модулей " << target.size() << "\n";
    printStation(target, "Станция после двух перемещений");
}

// Сценарий 4: итерация по модулям, активация, деактивация, смена состояния,
// перемещение модуля внутри станции
void scenarioStates() {
    printHeader("Сценарий 4: состояния модулей, перемещение и сортировка");

    StandardModuleFactory standard;
    AdvancedModuleFactory advanced;
    Station station(4);

    std::unique_ptr<Module> solar = standard.createEnergyModule("Солнечная панель");
    std::unique_ptr<Module> reactor = advanced.createEnergyModule("Реактор");
    Module& solarRef = *solar;
    Module& reactorRef = *reactor;
    station.addModule(std::move(solar));    // эффективность 60
    station.addModule(std::move(reactor));  // эффективность 95
    station.addModule(standard.createResidentialModule("Каюта экипажа"));   // эффективность 75
    station.addModule(standard.createResearchModule("Биолаборатория"));     // эффективность 70

    std::cout << "Включаем реактор:\n";
    std::cout << "  activate(): " << (reactorRef.activate() ? "успех" : "отказ") << "\n";
    std::cout << "  ";
    reactorRef.print();

    std::cout << "\nВыключаем солнечную панель:\n";
    solarRef.deactivate();
    std::cout << "  ";
    solarRef.print();

    std::cout << "\nПовреждаем реактор на 150 (прочность станет 0 — авария):\n";
    reactorRef.damage(150);
    std::cout << "  ";
    reactorRef.print();
    std::cout << "  activate() в состоянии аварии: "
              << (reactorRef.activate() ? "успех" : "отказ — модуль в аварии") << "\n";

    std::cout << "\nРемонтируем реактор на 200 (из аварии — в ремонт):\n";
    reactorRef.repair(200);
    std::cout << "  ";
    reactorRef.print();

    std::cout << "\nМеняем состояние реактора напрямую (setState(On)):\n";
    reactorRef.setState(ModuleState::On);
    std::cout << "  ";
    reactorRef.print();

    std::cout << "\nПеремещаем реактор в начало станции (moveToIndex):\n";
    reactorRef.moveToIndex(station, 0);
    printStation(station, "После перемещения");

    std::cout << "\nСортировка по убыванию эффективности (sortByEfficiency):\n";
    station.sortByEfficiency();
    printStation(station, "После сортировки");
}

// Сценарий 5: удаление модулей по ID и индексу,
// проверка ограничения максимального количества
void scenarioRemove() {
    printHeader("Сценарий 5: удаление модулей и ограничение количества");

    StandardModuleFactory standard;
    Station station(3);

    std::unique_ptr<Module> first = standard.createResidentialModule("Каюта экипажа");
    std::unique_ptr<Module> second = standard.createEnergyModule("Солнечная панель");
    std::unique_ptr<Module> third = standard.createResearchModule("Биолаборатория");
    int secondId = second->getId();
    station.addModule(std::move(first));
    station.addModule(std::move(second));
    station.addModule(std::move(third));
    printStation(station, "Станция заполнена до лимита");

    std::cout << "removeById(" << secondId << "): "
              << (station.removeById(secondId) ? "модуль удалён" : "модуль не найден") << "\n";
    std::cout << "removeById(999): "
              << (station.removeById(999) ? "модуль удалён" : "модуль не найден") << "\n";
    std::cout << "removeByIndex(0): "
              << (station.removeByIndex(0) ? "модуль удалён" : "индекс вне диапазона") << "\n";
    std::cout << "removeByIndex(10): "
              << (station.removeByIndex(10) ? "модуль удалён" : "индекс вне диапазона") << "\n";
    printStation(station, "После удалений");

    std::cout << "Заполняем станцию снова до лимита:\n";
    while (station.addModule(standard.createResearchModule("Лаборатория"))) {
        std::cout << "  модуль добавлен (" << station.size() << "/"
                  << station.getMaxModules() << ")\n";
    }
    std::cout << "addModule сверх лимита: "
              << (station.addModule(standard.createResearchModule("Лишний модуль"))
                      ? "модуль добавлен"
                      : "отказ — достигнут лимит модулей")
              << "\n";
    std::cout << "Итог: модулей " << station.size() << "/" << station.getMaxModules() << "\n";
}

}  // namespace

int main() {
    setupConsole();

    try {
        // конструктор станции с нулевым лимитом должен отклоняться
        try {
            Station bad(0);
            std::cout << "Ошибка: Station(0) не выбросил исключение\n";
        } catch (const std::invalid_argument& e) {
            std::cout << "Проверка Station(0): отклонён — " << e.what() << "\n";
        }

        scenarioCreate();
        scenarioCopy();
        scenarioMove();
        scenarioStates();
        scenarioRemove();

        std::cout << "\nВсе сценарии выполнены успешно.\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Непредвиденная ошибка: " << e.what() << "\n";
        return 1;
    }
}
