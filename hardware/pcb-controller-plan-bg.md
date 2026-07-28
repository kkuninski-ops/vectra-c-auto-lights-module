# PCB и controller план - Rev A

Този документ описва практична първа платка за проекта.

Целта на Rev A не е веднага да е финален automotive product, а стабилен controller, който:

- може да се монтира чисто в колата;
- може да работи с Bluetooth/BLE app;
- може да слуша MS-CAN;
- може да чете светлинен сензор;
- може да емулира копчето за фаровете, след като буксата е измерена;
- има fail-safe поведение, ако модулът се изключи или забие.

## Най-важен принцип

Rev A платката не трябва да захранва директно:

- D1S xenon ballasts;
- sidelights;
- fog lights;
- rear lights.

Платката трябва да командва заводската система чрез light switch emulation, ако измерването на копчето го позволява.

## Controller

Препоръчителен controller за Rev A:

- `ESP32-WROOM-32E` или сходен ESP32 module;
- 4 MB flash е достатъчно;
- external antenna вариант `ESP32-WROOM-32UE` е опция, ако модулът е скрит дълбоко зад таблото и BLE сигналът е слаб.

Причини:

- има Bluetooth/BLE;
- има достатъчно GPIO;
- има TWAI/CAN controller вътре, нужен е само CAN transceiver;
- firmware scaffold-ът в проекта вече е ESP32-oriented.

Не препоръчвам bare ESP32 chip за първа платка. Module вариантът е по-лесен и по-надежден за производство.

## Основни блокове на платката

### 1. Power input

Вход:

- `VBAT_IN` от колата през отделен предпазител;
- `GND`.

Нужни защити:

- fuse 0.5-2A;
- reverse polarity protection;
- automotive TVS diode;
- input LC/RC filtering;
- bulk capacitor;
- buck converter;
- 3.3V rail за ESP32 и логика.

Добър клас power design:

- automotive buck converter с wide input;
- low quiescent current;
- enable pin;
- защита от spikes при палене.

За Rev A може да се използва готов качествен automotive buck module, но PCB дизайнът трябва да има място за правилен buck вариант.

### 2. ESP32 core

Платката трябва да има:

- ESP32 module;
- EN/reset circuit;
- boot button или boot pads;
- UART programming header: `3V3`, `GND`, `TX`, `RX`, `EN`, `BOOT`;
- status LED;
- reserved GPIO test pads.

### 3. CAN / MS-CAN

Нужен е 3.3V CAN transceiver:

- `TCAN332`, `TCAN332G`, `TCAN330`, `SN65HVD230` клас;
- CAN-H/CAN-L към MS-CAN;
- ESD protection на CAN линиите;
- optional 120 ohm termination, но по подразбиране не се поставя, защото колата вече има termination;
- jumper/solder bridge за TX enable.

За първи тест:

- CAN RX работи;
- CAN TX е изключен с jumper/solder bridge;
- модулът само слуша.

Това намалява риска да смути CD30/GID/BC.

### 4. Light switch interface

Това е най-важният и най-несигурен блок.

Преди измерване на копчето не трябва да се фиксира окончателен output design.

Платката трябва да има configurable output area с footprints за:

- signal relays;
- opto-MOS / solid-state relays;
- analog switches;
- resistor ladder emulation;
- series resistors;
- protection resistors;
- test pads.

Изходите трябва да са normally-open / fail-safe.

При изключен модул:

- оригиналното копче работи;
- няма backfeed към BCM/UEC/REC;
- няма активен xenon output;
- няма блокирани ръчни светлини.

### 5. Light switch sensing

Платката трябва да може да чете:

- `AUTO` позиция;
- manual `0`;
- sidelights;
- low beam;
- fog request, ако е достъпно;
- dimmer signal, ако е полезно.

Входовете трябва да са защитени:

- series resistor;
- voltage divider за 12V signals;
- RC filter;
- clamp/ESD protection;
- high impedance, за да не товарят заводската схема.

### 6. Ambient light sensor

Нужен е connector за външен светлинен сензор:

- `3V3`;
- `GND`;
- `SDA`;
- `SCL`;
- optional interrupt pin.

Подходящи sensor-и:

- `VEML7700`;
- `BH1750`;
- `TSL2591`.

Сензорът е по-добре да е на малка отделна платка, изведена към горната част на таблото или основата на предното стъкло.

### 7. Optional discrete inputs

Добре е Rev A да има резервни protected inputs:

- ignition sense;
- engine-running analog fallback;
- battery voltage sense;
- door/unlock fallback, ако CAN не даде надежден сигнал.

Тези входове не трябва да се използват, ако същото може да се чете чисто по CAN.

### 8. Outputs / indicators

Минимум:

- status LED на платката;
- optional small buzzer не е препоръчителен;
- optional service button за reset/pairing.

Не трябва да има потребителски бутон, нужен за ежедневна употреба. Настройките трябва да са през BLE app.

## Конектори

Препоръчителна структура:

### Main connector

- `VBAT_IN`;
- `GND`;
- `CAN_H`;
- `CAN_L`;
- `IGN_SENSE` optional;
- `AUX_IN_1`;
- `AUX_IN_2`;
- `AUX_OUT_DISABLE` или service pin.

### Light switch connector

- 8-12 pin connector към adapter harness за копчето;
- всички switch lines минават през платката само ако fail-safe bypass е решен;
- по-добре adapter harness, не рязане на заводската инсталация.

### Sensor connector

- 4-5 pin към ambient sensor.

### Programming/service header

- UART;
- 3.3V;
- GND;
- EN;
- BOOT.

## BLE app функции, които Rev A отключва

Първата версия с BLE може да отключи:

### Конфигурация

- module enabled/disabled;
- auto lights enabled/disabled;
- Welcome lights: off / sidelights / low beam / fog / combination;
- Welcome duration;
- Welcome only dark;
- IGN/ACC lights: off / sidelights / low beam / fog / combination;
- DRL start delay;
- Goodbye lights;
- Goodbye duration;
- Goodbye only dark;
- dark threshold;
- light threshold;
- brightness policy.

### Status

- current mode: Idle / Welcome / IGN/ACC / DRL / Goodbye;
- switch position;
- engine-running detected;
- ambient light raw;
- day/night state;
- battery voltage;
- current light command;
- last decision reason;
- fault flags.

### Calibration

- read current ambient value;
- set dark threshold from current value;
- set light threshold from current value;
- test output only when engine is off and safe conditions are met.

## Функции, които Rev A трябва да отключи първо

### Phase 1: bench

- ESP32 boot;
- BLE settings read/write;
- ambient sensor read;
- config save/load;
- firmware tests.

### Phase 2: car read-only

- read MS-CAN;
- detect ignition/engine-running, ако CAN го позволява;
- read battery voltage;
- log switch state, ако е вързано само high impedance;
- no outputs active.

### Phase 3: light switch emulator test

- само на маса или с ограничен test harness;
- proof, че output stage емулира нужната позиция;
- fail-safe test.

### Phase 4: first real AUTO function

- `AUTO + engine running -> manual low beam emulation`;
- xenon не пали на контакт;
- xenon пали след engine-running;
- factory dimming работи като при ръчни къси;
- ръчни позиции работят заводски.

### Phase 5: configurable functions

- Welcome;
- IGN/ACC;
- Goodbye;
- thresholds;
- BLE calibration.

## Какво остава за по-късно

Не е задължително за Rev A:

- GID overlay;
- From Start;
- steering wheel menu;
- истински BC tab;
- директно управление на GID brightness level.

## Минимален BOM за Rev A

Не е финална поръчка, а ориентир за PCB designer:

- ESP32-WROOM-32E или ESP32-WROOM-32UE;
- 3.3V CAN transceiver: TCAN332/TCAN330/SN65HVD230 клас;
- automotive buck converter или footprint за такъв;
- 3.3V regulator, ако buck-ът е 5V;
- automotive TVS diode за power input;
- reverse polarity MOSFET/diode solution;
- CAN ESD diode;
- input protection resistors;
- voltage dividers за 12V inputs;
- signal relays / opto-MOS / analog switch footprints;
- I2C ambient light sensor connector;
- programming header;
- automotive connectors;
- fuse holder или external fused feed.

## Неща, които трябва да се решат преди PCB layout

1. Къде физически ще стои модулът.
2. Какъв connector/harness ще се използва към light switch.
3. Колко switch линии реално трябва да се четат/емулират.
4. Дали engine-running идва от CAN или fallback input.
5. Дали искаме ESP32-WROOM-32E или ESP32-WROOM-32UE с external antenna.
6. Какъв output stage избира човекът след измерване на копчето.

## Червени флагове

Не приемай design, който:

- захранва директно D1S ballast;
- захранва директно lamp circuits паралелно на BCM/UEC/REC;
- няма fuse;
- няма reverse polarity защита;
- няма CAN TX disable за първи тест;
- няма fail-safe при изключен модул;
- изисква рязане на много заводски кабели;
- няма test pads и service header.

## Препоръка

Поръчай/прави Rev A като платка с пълен хардуерен капацитет, но я използвай поетапно.

Не активирай всички функции от първия ден. Първо read-only, после light switch emulation, после BLE конфигурации.
