#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

#include "Logger.h"
#include "ModuleFactory.h"
#include "Station.h"
#include "Tests.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif

namespace {

void setupConsole() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
}

void printHeader(const std::string& title) {
    std::cout << "\n========== " << title << " ==========\n";
}

const void* address(const Module& module) {
    return static_cast<const void*>(&module);
}

void equipStation(Station& station, const ModuleFactory& factory, const std::string& suffix) {
    station.addModule(factory.createResidentialModule("Жилой-" + suffix));
    station.addModule(factory.createEnergyModule("Энерго-" + suffix));
    station.addModule(factory.createResearchModule("Лаб-" + suffix));
}

void scenarioCreate(Station& station, const ModuleFactory& standard, const ModuleFactory& advanced) {
    printHeader("Сценарий 1: создание станции и модулей через фабрики");
    equipStation(station, standard, "1");
    equipStation(station, advanced, "2");

    std::cout << "\nСписок модулей (getIterator):\n";
    station.print(std::cout);

    std::cout << "\nКраткий список (range-based for):\n";
    for (const Module& module : station) {
        std::cout << "  " << module.getName() << " — " << module.getType() << ", " << module.getCategory() << '\n';
    }
}

void scenarioCopy(const Station& original, const ModuleFactory& factory) {
    printHeader("Сценарий 2: глубокое копирование станции");
    const int copiesBefore = Module::copiesMade();
    Station copy(original);
    std::cout << "Скопировано модулей: " << Module::copiesMade() - copiesBefore << '\n';
    std::cout << "Первый модуль: оригинал " << address(*original.begin()) << ", копия " << address(*copy.begin())
              << " (разные объекты)\n";

    std::cout << "\nИзменяем копию: переименование, повреждение, удаление и добавление модуля\n";
    copy.setName("Заря-копия");
    copy.at(0).setName("Жилой-1 (копия)");
    copy.at(0).takeDamage(30);
    copy.removeByIndex(copy.size() - 1);
    copy.addModule(factory.createResearchModule("Лаб-копия"));

    std::cout << "\nОригинал:\n";
    original.print(std::cout);
    std::cout << "\nКопия:\n";
    copy.print(std::cout);
    std::cout << "Оригинал не изменился: " << (original.at(0).getName() == "Жилой-1" ? "да" : "нет") << '\n';

    std::cout << "\nКопирующее присваивание:\n";
    Station reserve("Резерв", 2);
    reserve = original;
    std::cout << "Резервная станция: модулей " << reserve.size() << '/' << reserve.getMaxModules() << '\n';
    Station& sameStation = reserve;
    reserve = sameStation;
}

Station scenarioMove(Station& source) {
    printHeader("Сценарий 3: перемещение станции");
    const int copiesBefore = Module::copiesMade();
    const void* firstBefore = address(*source.begin());

    Station moved(std::move(source));
    std::cout << "Конструктор перемещения:\n"
              << "  модулей в новой станции: " << moved.size() << ", в исходной: " << source.size() << '\n'
              << "  адрес первого модуля: до " << firstBefore << ", после " << address(*moved.begin()) << '\n'
              << "  копирований модулей: " << Module::copiesMade() - copiesBefore << '\n';

    Station target("Пустая", 1);
    target = std::move(moved);
    std::cout << "Перемещающее присваивание:\n"
              << "  модулей: " << target.size() << '/' << target.getMaxModules() << ", в источнике: " << moved.size()
              << '\n'
              << "  адрес первого модуля: " << address(*target.begin()) << '\n'
              << "  копирований модулей: " << Module::copiesMade() - copiesBefore << '\n';
    return target;
}

void scenarioActions(Station& station) {
    printHeader("Сценарий 4: действия с модулями при обходе");
    std::cout << "Активируем все модули (range-based for):\n";
    for (Module& module : station) {
        module.activate();
    }
    std::cout << "Энергобаланс: " << station.energyBalance() << " кВт\n";

    std::cout << "\nМодули с эффективностью ниже 80% отправляем в ремонт (getIterator):\n";
    StationIterator it = station.getIterator();
    while (it.hasNext()) {
        Module& module = it.next();
        if (module.getEfficiency() < 80) {
            module.setState(ModuleState::Repair);
        }
    }
    std::cout << "Пробуем снова активировать все модули:\n";
    for (Module& module : station) {
        module.activate();
    }

    std::cout << "\nАвария: модуль на позиции 1 получает критическое повреждение\n";
    Module& damaged = station.at(1);
    damaged.takeDamage(damaged.durability().getMax());
    std::cout << "Энергобаланс после аварии: " << station.energyBalance() << " кВт\n";
    std::cout << "Ремонтируем и снова активируем:\n";
    damaged.repair(damaged.durability().getMax());
    damaged.activate();

    std::cout << "\nДеактивируем исследовательские модули ради экономии энергии:\n";
    for (Module& module : station) {
        if (module.getCategory() == "Исследовательский") {
            module.deactivate();
        }
    }

    std::cout << "\nПеремещаем первый модуль в конец станции:\n";
    station.moveModule(0, station.size() - 1);
    station.print(std::cout);

    std::cout << "\nСортировка по эффективности:\n";
    station.sortByEfficiency();
    station.print(std::cout);
}

void scenarioRemoval(Station& station, const ModuleFactory& factory) {
    printHeader("Сценарий 5: удаление модулей и ограничение количества");
    const int removedId = station.at(1).getId();
    std::cout << "Удаляем модуль по id " << removedId << " и модуль с индексом 0\n";
    station.removeById(removedId);
    station.removeByIndex(0);
    station.print(std::cout);

    std::cout << "\nПовторное удаление id " << removedId << ":\n";
    const bool removedAgain = station.removeById(removedId);
    std::cout << "Результат: " << (removedAgain ? "удалён" : "не найден") << '\n';
    try {
        station.removeByIndex(100);
    } catch (const std::out_of_range& error) {
        std::cout << "Ошибка: " << error.what() << '\n';
    }

    std::cout << "\nЗаполняем станцию до лимита " << station.getMaxModules() << ":\n";
    int number = 1;
    while (!station.isFull()) {
        station.addModule(factory.createResidentialModule("Доп-" + std::to_string(number++)));
    }
    try {
        station.addModule(factory.createResidentialModule("Лишний"));
    } catch (const std::length_error& error) {
        std::cout << "Ошибка: " << error.what() << '\n';
    }

    try {
        Station invalid("Ошибочная", 0);
    } catch (const std::invalid_argument& error) {
        std::cout << "Ошибка: " << error.what() << '\n';
    }

    std::cout << '\n';
    station.print(std::cout);
}

}

int main() {
    setupConsole();

    try {
        StandardModuleFactory standardFactory;
        AdvancedModuleFactory advancedFactory;

        Station station("Заря", 8);
        scenarioCreate(station, standardFactory, advancedFactory);
        scenarioCopy(station, advancedFactory);
        Station movedStation = scenarioMove(station);
        scenarioActions(movedStation);
        scenarioRemoval(movedStation, standardFactory);

        std::cout << "\nЗаписей в журнале: " << LoggingProxy::instance().recordsWritten() << '\n';
    } catch (const std::exception& error) {
        std::cout << "Непредвиденная ошибка: " << error.what() << '\n';
        return 1;
    }

    printHeader("Тестирование");
    return runTests() == 0 ? 0 : 1;
}
