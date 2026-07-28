# Solo builder roadmap

Този документ е за вариант, в който собственикът прави проекта сам.

Целта е да стане реално, но без да рискуваш колата от първия опит.

## Най-важното правило

Не започвай с вързване към фарове или към копчето.

Редът е:

1. На маса.
2. Read-only в колата.
3. Измерване на копчето.
4. Light switch emulator на маса.
5. Едва после контролирано включване в колата.

## Какво трябва да научиш първо

Минимално:

- как се мери напрежение с multimeter;
- как се мери continuity;
- какво е pull-up/pull-down;
- какво е voltage divider;
- как работи relay / opto-MOS / analog switch;
- какво е CAN-H/CAN-L;
- какво е fuse и защо трябва да е близо до захранването;
- как се прави crimp правилно;
- какво е common ground.

Не е нужно да станеш автоелектроник, но трябва да можеш да не вържеш 12V към грешен ESP32 pin.

## Какво да купиш първо

Не започвай веднага с custom PCB.

Първо купи bench kit:

- ESP32 dev board;
- CAN transceiver module, 3.3V compatible;
- 12V to 5V buck module;
- VEML7700/BH1750/TSL2591 light sensor module;
- multimeter;
- breadboard или screw-terminal proto board;
- 12V bench supply с current limit или малък fused 12V adapter;
- комплект резистори;
- LEDs;
- малки signal relays или opto-MOS само за bench;
- jumper wires;
- fuse holder + 0.5-2A fuses;
- USB cable;
- basic soldering tools.

Този комплект няма да е излишен. Той ще се използва за firmware, BLE, sensor, CAN logging и output tests.

## Какво да не купуваш още

Не купувай финално:

- custom PCB;
- automotive connectors;
- output relays за колата;
- adapter harness;
- скъпи CAN tools;
- готова кутия за монтаж.

Тези неща идват след измерване на копчето.

## Phase 0: setup на компютъра

Инсталирай:

- VS Code;
- PlatformIO;
- Git;
- KiCad, когато стигнем до PCB;
- OP-COM software на Windows лаптопа.

Първо целта е firmware да се компилира и тестовете да минават.

```sh
cd firmware
make test
```

## Phase 1: ESP32 на маса

Цел:

- ESP32 boot;
- Serial log;
- LED blink;
- четене на config;
- basic BLE service.

Няма 12V от колата. Няма CAN. Няма фарове.

Успех:

- виждаш serial output;
- можеш да се свържеш с телефона през BLE scanner app;
- можеш да прочетеш dummy setting.

## Phase 2: light sensor на маса

Цел:

- ESP32 чете ambient sensor;
- виждаш raw value;
- покриваш сензора и стойността пада/се променя;
- светиш с фенер и стойността се променя.

Успех:

- можеш да определиш `dark_threshold`;
- можеш да определиш `light_threshold`;
- няма нестабилно скачане.

## Phase 3: BLE app/settings

Първо може без custom app:

- използвай generic BLE scanner app;
- expose GATT characteristics;
- read/write settings.

После може да се прави истинско phone app.

Първи настройки:

- module enabled;
- Welcome lights;
- IGN/ACC lights;
- Goodbye lights;
- durations;
- thresholds.

## Phase 4: CAN на маса

Цел:

- ESP32 + CAN transceiver работи;
- можеш да логваш CAN frames от test source или втори CAN adapter.

Ако нямаш втори CAN tool, тази фаза може да е ограничена.

Важно:

- CAN TX трябва да може да се изключва;
- първият тест в колата е RX/read-only.

## Phase 5: read-only в колата

Това е първият контакт с колата.

Позволено:

- захранване през fuse;
- GND;
- CAN-H/CAN-L;
- high impedance measurement inputs, ако са защитени.

Забранено:

- output към копчето;
- output към фарове;
- output към BCM/UEC/REC;
- CAN TX, ако не е нужно.

Цел:

- модулът не прави нищо, само слуша;
- няма грешки;
- няма странно поведение;
- не пада акумулаторът;
- виждаш ignition/engine-running/RPM, ако CAN го дава.

## Phase 6: измерване на копчето

Това е задължително преди PCB output stage.

Мери се:

- `0`;
- `AUTO`;
- `габарити`;
- `къси`;
- fog positions;
- dimmer.

Записваш таблица:

| Позиция | Pin A | Pin B | Pin C | Resistance | Voltage | Notes |
|---|---|---|---|---|---|---|

Без тази таблица не се проектира output към копчето.

## Phase 7: light switch emulator на маса

След като знаем как работи копчето:

- правим малка тестова схема;
- емулираме само една позиция, примерно `къси`;
- проверяваме fail-safe;
- проверяваме, че output-ът остава open при reset/power loss.

Не се тества първо в колата.

## Phase 8: Rev A PCB

Едва тук има смисъл от custom PCB.

Rev A трябва да включва:

- ESP32 module;
- protected 12V power input;
- CAN transceiver с TX disable jumper;
- protected switch inputs;
- configurable output area;
- light sensor connector;
- programming header;
- test pads;
- fuse strategy;
- fail-safe output strategy.

Преди поръчка на PCB:

- схемата се review-ва;
- connector pinout-ът се проверява;
- clearances се проверяват;
- power input protection се проверява;
- output stage се проверява.

## Phase 9: първи реален output тест в колата

Първо само:

```text
AUTO + engine running -> manual low beam emulation
```

Без:

- Welcome;
- Goodbye;
- fog;
- GID;
- сложни app функции.

Проверяваш:

- ксенонът не пали на контакт;
- ксенонът пали след engine-running;
- level test се вижда;
- dimming работи;
- ръчните позиции работят;
- fog/high beam работят както заводски;
- няма check-control грешки.

## Phase 10: добавяне на функции

След като основното работи стабилно:

1. IGN/ACC lights.
2. Welcome lights.
3. Goodbye lights.
4. BLE calibration.
5. Sleep current optimization.
6. Future: GID overlay.

## Daily safety checklist

Преди да караш с модула включен:

- manual `къси` работят;
- manual `габарити` работят;
- модулът може да се disable-не;
- fuse е правилен;
- няма loose wires;
- няма загряване;
- модулът не дърпа ток при заключена кола;
- няма check-control грешки.

## Кога да спреш

Спираш веднага, ако:

- не си сигурен кой кабел е кой;
- има миризма на загряване;
- фаровете мигат;
- CD30/GID се рестартира;
- OP-COM показва странни грешки;
- ръчните светлини не работят;
- fuse гори;
- ESP32 се рестартира при палене.

## Реалистична препоръка

Ти можеш да направиш проекта, но не започвай от най-трудното.

Започни с:

1. ESP32 + BLE на маса.
2. Light sensor.
3. Config logic.
4. Read-only CAN.
5. Измерване на копчето.

Платката идва след това. Ако направиш PCB преди измерването, шансът е да се наложи да я преработваш.
