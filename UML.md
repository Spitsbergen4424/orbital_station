
# UML — архитектура управления космической станцией

В этом документе представлены UML-диаграмма классов и схема взаимодействия паттернов проектирования, добавленных для управления космической станцией.

## 1. Диаграмма классов

```mermaid
classDiagram
direction TB

class StationFacade {
    -shared_ptr~StationMediator~ mediator
    -unique_ptr~Handler~ handlers
    +activateModule(index)
    +deactivateModule(index)
    +moveModule(from, to)
    +repairModule(index)
    +damageModule(index)
    +distributeEnergy(index, percent)
    +startResearch(index)
    +buildModule(type, name)
    +getStatus(index)
    +processInput(input)
    +executeCommand(command)
}

class Handler {
    <<abstract>>
    -string keyword
    -function action
    -unique_ptr~Handler~ next
    +handle(input) bool
    +setNext(handler)
}

class ActivateHandler
class DeactivateHandler
class MoveHandler
class RepairHandler
class DamageHandler
class EnergyHandler
class ResearchHandler
class BuildHandler
class StatusHandler
class HelpHandler
class ExitHandler

class Command {
    <<abstract>>
    #shared_ptr~StationMediator~ mediator
    +execute()*
}

class ActivateModuleCommand
class DeactivateModuleCommand
class MoveModuleCommand
class RepairModuleCommand
class DamageModuleCommand
class DistributeEnergyCommand
class StartResearchCommand
class BuildModuleCommand
class StatusCommand

class StationMediator {
    -CommandCenter center
    -ModuleFactory standardFactory
    -ModuleFactory advancedFactory
    +activateModule(index)
    +deactivateModule(index)
    +moveModule(from, to)
    +repairModule(index)
    +damageModule(index, amount)
    +distributeEnergy(index, percent)
    +requestEnergy(index, amount)
    +startResearch(index)
    +buildModule(type, name)
    +getStatus(index)
    +notifyDamage(index, amount)
    +notifyActivation(index)
    +notifyResearchComplete(index)
}

class CommandCenter {
    -double energy
    -double oxygen
    -int safetyLevel
    -Station modules
    +createModule(factory, category, name)
    +consumeEnergy(amount)
    +printStatistics()
    +flushPendingRemovals()
}

class Station {
    -vector~unique_ptr~Module~~ modules
    +addModule(module)
    +removeByIndex(index)
    +moveModule(from, to)
    +size()
}

class Module {
    <<abstract>>
    -int id
    -string name
    -Durability durability
    -EnergyConsumption energyConsumption
    -ModuleState state
    +activate()
    +deactivate()
    +damage(amount)
    +repair(amount)
    +getType()
}

class ResearchModule {
    -bool researchBlocked
    +isResearchBlocked()
    +setResearchBlocked(blocked)
}

class ModuleFactory {
    <<abstract>>
    +createResidentialModule(name)
    +createEnergyModule(name)
    +createResearchModule(name)
}

class StandardModuleFactory
class AdvancedModuleFactory

class LoggingProxy {
    +write(message)
    +globalLog(source, message)
}

StationFacade --> StationMediator : использует
StationFacade o-- Handler : цепочка обработчиков
Handler <|-- ActivateHandler
Handler <|-- DeactivateHandler
Handler <|-- MoveHandler
Handler <|-- RepairHandler
Handler <|-- DamageHandler
Handler <|-- EnergyHandler
Handler <|-- ResearchHandler
Handler <|-- BuildHandler
Handler <|-- StatusHandler
Handler <|-- HelpHandler
Handler <|-- ExitHandler

Handler --> Handler : следующий обработчик

Command <|-- ActivateModuleCommand
Command <|-- DeactivateModuleCommand
Command <|-- MoveModuleCommand
Command <|-- RepairModuleCommand
Command <|-- DamageModuleCommand
Command <|-- DistributeEnergyCommand
Command <|-- StartResearchCommand
Command <|-- BuildModuleCommand
Command <|-- StatusCommand

StationFacade ..> Command : создаёт и выполняет
Command --> StationMediator : передаёт запрос

StationMediator --> CommandCenter : координирует
StationMediator ..> Module : управляет

CommandCenter *-- Station
Station *-- Module : хранит

Module <|-- ResearchModule
ModuleFactory <|-- StandardModuleFactory
ModuleFactory <|-- AdvancedModuleFactory

CommandCenter ..> ModuleFactory : создание модулей

StationFacade ..> LoggingProxy : журналирование
CommandCenter ..> LoggingProxy : журналирование
```

## 2. Последовательность взаимодействия паттернов

Ниже показано, как проходит пользовательская команда `activate 0`.

В этом процессе участвуют все четыре реализованных паттерна:

1. **Цепочка обязанностей** определяет тип введённой команды.
2. **Фасад** предоставляет единую точку входа и организует выполнение операции.
3. **Команда** представляет действие пользователя отдельным объектом.
4. **Посредник** передаёт запрос в командный центр и управляет нужным модулем.

```
Diagram
    User->>Main: activate 0
    Main->>Facade: processInput(input)

    Note over Facade,Handler: Цепочка обязанностей

    Facade->>Chain: handle(input)
    Chain->>Handler: handle(input)
    Handler->>Handler: Разбор команды и индекса

    Note over Handler,Facade: Фасад и команда

    Handler->>Facade: activateModule(0)
    Facade->>Command: создать команду
    Facade->>Command: executeCommand(command)
    Facade->>Logger: Записать выполнение команды
    Command->>Command: execute()

    Note over Command,Module: Посредник

    Command->>Mediator: activateModule(0)
    Mediator->>Center: Получить модуль
    Center-->>Mediator: Ссылка на Module
    Mediator->>Module: activate()
    Module-->>Mediator: Результат активации

    opt Модуль успешно активирован
        Mediator->>Logger: Записать событие
    end

    Mediator-->>Command: Результат операции
    Command-->>Facade: Завершение выполнения
    Facade-->>Main: Результат
    Main-->>User: Модуль активирован
```

## 3. Общая цепочка взаимодействия

```mermaid
flowchart TD
    A["Пользователь вводит команду"] --> B["main()"]
    B --> C["StationFacade"]
    C --> D["Chain of Responsibility"]

    D --> E{"Какой обработчик подходит?"}
    E --> F["ActivateHandler"]
    E --> G["BuildHandler"]
    E --> H["RepairHandler"]
    E --> I["EnergyHandler"]
    E --> J["ResearchHandler"]

    F --> K["StationFacade: метод операции"]
    G --> K
    H --> K
    I --> K
    J --> K

    K --> L["Создание объекта Command"]
    L --> M["executeCommand()"]
    M --> N["Command::execute()"]
    N --> O["StationMediator"]

    O --> P["CommandCenter"]
    P --> Q["Station"]
    Q --> R["Module"]

    O --> S["Координация энергии и ресурсов"]
    R --> T["Результат операции"]
    S --> T

    T --> U["LoggingProxy"]
    U --> V["Ответ пользователю"]
```

## 4. Роли паттернов

| Паттерн | Реализация | Назначение |
|---|---|---|
| Facade | `StationFacade` | Единый интерфейс управления |
| Chain of Responsibility | `Handler` и наследники | Разбор пользовательского ввода |
| Command | `Command` и наследники | Представление действий в виде объектов |
| Mediator | `StationMediator` | Координация взаимодействия компонентов |

## 5. Итог

Пользователь не обращается напрямую к модулям или командному центру.

Каждая команда проходит через обработчик пользовательского ввода, фасад и объект команды. Далее команда передаёт запрос посреднику, который взаимодействует с командным центром и нужными модулями.

Таким образом, четыре паттерна образуют единую цепочку, в которой каждый компонент отвечает за свою задачу.
