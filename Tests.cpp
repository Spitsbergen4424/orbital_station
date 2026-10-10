#include "Tests.h"

#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "Logger.h"
#include "ModuleFactory.h"
#include "Station.h"

namespace {

int passed = 0;
int failed = 0;

void check(bool condition, const std::string& description) {
    if (condition) {
        ++passed;
        std::cout << "  [OK]   " << description << '\n';
    } else {
        ++failed;
        std::cout << "  [FAIL] " << description << '\n';
    }
}

template <typename Exception, typename Action>
void checkThrows(Action action, const std::string& description) {
    bool thrown = false;
    try {
        action();
    } catch (const Exception&) {
        thrown = true;
    } catch (...) {
    }
    check(thrown, description);
}

void section(const std::string& title) {
    std::cout << "\n-- " << title << '\n';
}

Station makeStation(const std::string& name, std::size_t maxModules) {
    Station station(name, maxModules);
    StandardModuleFactory standard;
    AdvancedModuleFactory advanced;
    station.addModule(standard.createResidentialModule("Каюта"));
    station.addModule(advanced.createEnergyModule("Реактор"));
    station.addModule(standard.createResearchModule("Биолаб"));
    return station;
}

void testAttributes() {
    section("Классы-атрибуты");
    checkThrows<std::invalid_argument>([] { Durability d(-1); }, "Durability(-1) бросает исключение");
    checkThrows<std::invalid_argument>([] { Durability d(150, 100); }, "Durability выше максимума бросает исключение");
    Durability durability(50, 100);
    durability.decrease(80);
    check(durability.getValue() == 0 && durability.isBroken(), "decrease не опускает прочность ниже 0");
    durability.repair(500);
    check(durability.getValue() == 100 && durability.isFull(), "repair не поднимает прочность выше максимума");
    checkThrows<std::invalid_argument>([&durability] { durability.setValue(101); }, "Durability::setValue выше максимума");
    checkThrows<std::invalid_argument>([&durability] { durability.decrease(-5); }, "Durability::decrease с отрицательным аргументом");

    checkThrows<std::invalid_argument>([] { EnergyConsumption e(-5); }, "EnergyConsumption(-5) бросает исключение");
    EnergyConsumption energy(10);
    energy.increase(5);
    check(energy.getValue() == 15, "EnergyConsumption::increase");
    energy.decrease(100);
    check(energy.getValue() == 0, "EnergyConsumption::decrease не опускает ниже 0");

    checkThrows<std::invalid_argument>([] { Capacity c(-1); }, "Capacity(-1) бросает исключение");
    Capacity capacity(4);
    checkThrows<std::invalid_argument>([&capacity] { capacity.setValue(-2); }, "Capacity::setValue(-2) бросает исключение");
    check(capacity.getValue() == 4, "после ошибки значение Capacity не изменилось");
    check(Durability().getValue() == Durability::kDefaultMax && EnergyConsumption().getValue() == 0 &&
              Capacity().getValue() == 0,
          "конструкторы по умолчанию");
}

void testModules() {
    section("Модули");
    CrewCabin first("Каюта-1");
    CrewCabin second("Каюта-2");
    check(first.getId() != second.getId(), "модули получают уникальные id");
    checkThrows<std::invalid_argument>([&first] { first.setEfficiency(101); }, "эффективность выше 100 запрещена");
    checkThrows<std::invalid_argument>([] { BiologyLab lab(""); }, "пустое имя модуля запрещено");

    check(first.activate() && first.isActive(), "активация модуля во включённом состоянии");
    first.setState(ModuleState::Repair);
    check(!first.isActive(), "переход в ремонт деактивирует модуль");
    check(!first.activate(), "модуль в ремонте нельзя активировать");

    first.takeDamage(1000);
    check(first.getState() == ModuleState::Emergency, "нулевая прочность переводит модуль в аварию");
    first.repair(1000);
    check(first.getState() == ModuleState::Enabled, "полный ремонт возвращает модуль во включённое состояние");

    SolarModule solar("Солнце", 40);
    check(solar.getPowerOutput() == 0, "неактивный энергомодуль не вырабатывает энергию");
    solar.activate();
    check(solar.getPowerOutput() == 20 * 70 / 100, "выработка = номинал × эффективность");
}

void testFactories() {
    section("Абстрактная фабрика");
    StandardModuleFactory standard;
    AdvancedModuleFactory advanced;
    const ModuleFactory& standardFactory = standard;
    const ModuleFactory& advancedFactory = advanced;

    check(standardFactory.createResidentialModule("A")->getType() == "CrewCabin", "Standard: жилой модуль — CrewCabin");
    check(standardFactory.createEnergyModule("B")->getType() == "SolarModule", "Standard: энергомодуль — SolarModule");
    check(standardFactory.createResearchModule("C")->getType() == "BiologyLab", "Standard: лаборатория — BiologyLab");
    check(advancedFactory.createResidentialModule("D")->getType() == "MedicalCabin", "Advanced: жилой модуль — MedicalCabin");
    check(advancedFactory.createEnergyModule("E")->getType() == "ReactorModule", "Advanced: энергомодуль — ReactorModule");
    check(advancedFactory.createResearchModule("F")->getType() == "PhysicsLab", "Advanced: лаборатория — PhysicsLab");
    check(advancedFactory.createEnergyModule("G")->getCategory() == "Энергетический", "категория энергомодуля");
}

void testStationRuleOfFive() {
    section("Правило пяти для Station");
    Station original = makeStation("Оригинал", 5);

    Station copy(original);
    check(copy.size() == original.size(), "копия содержит столько же модулей");
    check(&*copy.begin() != &*original.begin(), "модули копии — отдельные объекты (глубокая копия)");
    check(copy.begin()->getId() == original.begin()->getId(), "модули копии сохраняют id");
    copy.begin()->setName("Изменённая");
    copy.removeByIndex(1);
    check(original.begin()->getName() == "Каюта" && original.size() == 3, "изменения копии не влияют на оригинал");

    Station assigned("Другая", 1);
    assigned = original;
    check(assigned.size() == 3 && assigned.getMaxModules() == 5, "копирующее присваивание переносит модули и лимит");
    Station& alias = assigned;
    assigned = alias;
    check(assigned.size() == 3, "самоприсваивание безопасно");

    const int copiesBefore = Module::copiesMade();
    const Module* firstModule = &*original.begin();
    Station moved(std::move(original));
    check(Module::copiesMade() == copiesBefore, "конструктор перемещения не копирует модули");
    check(&*moved.begin() == firstModule, "после перемещения модули остались теми же объектами");
    check(original.isEmpty(), "исходная станция после перемещения пуста");

    Station target("Цель", 2);
    target = std::move(moved);
    check(Module::copiesMade() == copiesBefore, "перемещающее присваивание не копирует модули");
    check(target.size() == 3 && moved.isEmpty() && &*target.begin() == firstModule,
          "перемещающее присваивание передаёт владение модулями");
}

void testReactorRuleOfFive() {
    section("Правило пяти для ReactorModule (владеет активной зоной)");
    ReactorModule original("Реактор-А");
    ReactorModule copy(original);
    copy.burnFuel(40);
    check(original.getFuelLevel() == 100 && copy.getFuelLevel() == 60, "копия реактора имеет свою активную зону");

    ReactorModule assigned("Реактор-Б");
    assigned = copy;
    assigned.burnFuel(10);
    check(copy.getFuelLevel() == 60 && assigned.getFuelLevel() == 50, "копирующее присваивание реактора глубокое");

    ReactorModule moved(std::move(copy));
    check(moved.getFuelLevel() == 60 && !copy.hasCore(), "перемещение забирает активную зону");

    ReactorModule target("Реактор-В");
    target = std::move(moved);
    check(target.getFuelLevel() == 60 && !moved.hasCore(), "перемещающее присваивание забирает активную зону");

    std::unique_ptr<Module> cloned = target.clone();
    check(cloned->getType() == "ReactorModule" && cloned.get() != &target, "clone() создаёт новый объект того же типа");
}

void testIterator() {
    section("Итератор");
    Station station = makeStation("Итератор", 5);

    std::size_t count = 0;
    for (Module& module : station) {
        module.activate();
        ++count;
    }
    check(count == station.size(), "range-based for обходит все модули");

    std::vector<int> ids;
    StationIterator it = station.getIterator();
    while (it.hasNext()) {
        ids.push_back(it.next().getId());
    }
    check(ids.size() == 3 && ids[0] == station.at(0).getId() && ids[2] == station.at(2).getId(),
          "getIterator() обходит модули по порядку");
    checkThrows<std::out_of_range>([&it] { it.next(); }, "next() после последнего модуля бросает исключение");

    const Station& constStation = station;
    int active = 0;
    for (const Module& module : constStation) {
        active += module.isActive() ? 1 : 0;
    }
    check(active == 3, "константный итератор (begin() const / end() const)");

    StationIterator postfix = station.begin();
    StationIterator previous = postfix++;
    check(previous == station.begin() && postfix != station.begin(), "постфиксный ++ возвращает старое значение");
}

void testStationOperations() {
    section("Операции станции");
    checkThrows<std::invalid_argument>([] { Station station("Пустая", 0); }, "станция с лимитом 0 запрещена");

    Station station = makeStation("Операции", 3);
    check(station.isFull(), "станция заполнена после трёх модулей");
    checkThrows<std::length_error>([&station] { station.addModule(std::make_unique<CrewCabin>("Лишняя")); },
                                   "добавление сверх лимита бросает исключение");
    checkThrows<std::invalid_argument>([&station] { station.addModule(nullptr); }, "нельзя добавить nullptr");

    const int reactorId = station.at(1).getId();
    check(station.findById(reactorId) == &station.at(1), "findById находит модуль");
    check(station.findById(-1) == nullptr, "findById возвращает nullptr для неизвестного id");

    const int firstId = station.at(0).getId();
    station.moveModule(0, 2);
    check(station.at(2).getId() == firstId && station.at(2).getPosition() == 2, "moveModule переставляет модуль и обновляет позицию");
    station.moveModule(2, 0);
    check(station.at(0).getId() == firstId, "moveModule в обратную сторону");
    checkThrows<std::out_of_range>([&station] { station.moveModule(0, 10); }, "moveModule с неверным индексом");

    station.sortByEfficiency();
    bool sorted = true;
    for (std::size_t i = 1; i < station.size(); ++i) {
        sorted = sorted && station.at(i - 1).getEfficiency() >= station.at(i).getEfficiency();
    }
    check(sorted, "sortByEfficiency упорядочивает по убыванию эффективности");

    check(station.removeById(reactorId) && station.size() == 2, "removeById удаляет модуль");
    check(!station.removeById(reactorId), "повторный removeById возвращает false");
    checkThrows<std::out_of_range>([&station] { station.removeByIndex(5); }, "removeByIndex с неверным индексом");
    station.removeByIndex(0);
    check(station.size() == 1 && station.at(0).getPosition() == 0, "removeByIndex удаляет модуль и пересчитывает позиции");
}

}

int runTests() {
    passed = 0;
    failed = 0;
    LoggingProxy& logger = LoggingProxy::instance();
    const bool wasEnabled = logger.isEnabled();
    logger.setEnabled(false);

    testAttributes();
    testModules();
    testFactories();
    testStationRuleOfFive();
    testReactorRuleOfFive();
    testIterator();
    testStationOperations();

    logger.setEnabled(wasEnabled);
    std::cout << "\nИтого: пройдено " << passed << ", провалено " << failed << '\n';
    return failed;
}
