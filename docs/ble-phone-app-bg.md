# Bluetooth phone app MVP

## Нова посока

Фокусът се връща към по-практичен MVP:

- управление на `AUTO` фаровете;
- правилно поведение на CD30/GID яркостта;
- конфигурация през Bluetooth приложение от телефона;
- без GID menu/overlay като задължителна първа версия.

GID overlay и `From Start` остават като future етап.

## Какво трябва да управлява приложението

Приложението не трябва да командва директно фаровете в реално време като remote control.

По-безопасният модел е:

1. Телефонът променя настройки.
2. ESP32 записва настройките във flash.
3. Модулът сам изпълнява логиката в колата.
4. При липса на телефон всичко продължава да работи по последно записаните настройки.

## Настройки за фарове

Минимални настройки:

- `module_enabled`: on/off;
- `auto_lights_enabled`: on/off;
- `acc_lights`: off / sidelights / fog / low beam / safe combination;
- `drl_lights`: low beam + sidelights;
- `drl_start_delay_ms`: delay след engine-running;
- `welcome_enabled`: on/off;
- `welcome_only_dark`: yes/no;
- `welcome_lights`: off / sidelights / fog / low beam / safe combination;
- `welcome_duration_s`: време;
- `goodbye_enabled`: on/off;
- `goodbye_only_dark`: yes/no;
- `goodbye_lights`: off / sidelights / fog / low beam / safe combination;
- `goodbye_duration_s`: време;
- `dark_threshold`: праг за тъмно;
- `light_threshold`: праг за светло;
- `sensor_hysteresis`: защита от постоянно превключване при здрач.

## Настройки за яркост / dimming

Важно разграничение:

- сигурната цел е `AUTO` режимът да активира заводското night/dimming поведение като ръчни `къси`;
- директно управление на яркостта на CD30/GID от телефона не е гарантирано, докато не се провери дали яркостта може да се командва безопасно по CAN или illumination линия.

Първа версия:

- `brightness_policy = factory_manual_low_beam`;
- денем и нощем в `AUTO + engine running` модулът емулира manual low beam;
- CD30/GID и таблото трябва да се димират както при ръчно включени къси;
- dimmer врътката в колата остава основният контрол за яркост.

Възможни future настройки, само ако се докажат безопасно:

- `force_day_brightness`: никога не димира в AUTO;
- `force_night_dimming`: винаги активира night/dimming в AUTO;
- `auto_brightness_by_sensor`: денем bright, нощем factory dimming;
- `gid_brightness_level`: директно ниво, само ако GID/CD30 го поддържа безопасно.

## BLE вместо класически Bluetooth

Препоръчителен вариант: BLE, не Bluetooth Classic.

Причини:

- ESP32 го поддържа добре;
- ниска консумация;
- лесно се прави mobile app;
- достатъчно е за настройки и статус.

## BLE GATT структура

Предложени services:

### Device service

- firmware version;
- hardware version;
- module status;
- fault status;
- reset command.

### Lighting config service

- read/write на всички настройки за Welcome/ACC/DRL/Goodbye;
- save config;
- load defaults.

### Sensor service

- ambient light raw;
- day/night state;
- battery voltage;
- engine-running state;
- switch position;
- last decision/reason.

### Brightness policy service

- brightness policy;
- current dimming decision;
- factory dimming active/not active, ако може да се прочете.

## Phone app UI

Първа версия може да е проста:

```text
Auto Lights
[x] Enabled

DRL
Lights: Low + Side
Start delay: 0.5s

ACC
Lights: Off

Welcome
[x] Enabled
Lights: Side + Fog
Time: 20s
Dark only: Yes

Goodbye
[x] Enabled
Lights: Side
Time: 20s
Dark only: Yes

Brightness
Policy: Factory dimming

Sensor
Dark threshold: 1400
Light threshold: 1900
```

## Safety за Bluetooth

Bluetooth приложението не трябва да може да направи опасни неща по време на движение.

Ограничения:

- disable/enable е позволено;
- промяна на времена и прагове е позволена;
- тест на ксенон/мъгли да е позволен само при engine off;
- factory/manual mode винаги има приоритет;
- ако BLE връзката падне, модулът не променя текущото си решение внезапно;
- всички настройки се валидират с min/max стойности.

## Какво остава извън MVP

Не е част от първия фокус:

- GID overlay menu;
- `From Start`;
- navigation с бутони на волана;
- истински BC tab;
- директно GID brightness level control.

Тези неща може да се върнат по-късно, след като фаровете и dimming поведението са стабилни.
