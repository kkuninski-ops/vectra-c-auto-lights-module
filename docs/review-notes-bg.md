# Бележки за преглед

Този проект е за преглед от човек с опит в автомобилна електроника, CAN и embedded.

## Текущ MVP фокус

Първата версия вече не гони GID меню като основна цел.

Фокусът е:

- safe light switch emulation;
- `AUTO` фарове без палене на xenon само на контакт;
- правилно factory dimming поведение за CD30/GID;
- Bluetooth/BLE приложение за настройки от телефона;
- GID overlay само като future етап.

## Какво искаме да потвърди reviewer-ът

- Дали архитектурата "емулация на копчето, без директно захранване на фарове" е правилна за Vectra C.
- Какъв е типът на light switch входовете: отделни линии, резистивна стълбица или друго.
- Кой е най-безопасният output stage към копчето: реле, analog switch, opto-MOS или резисторна емулация.
- Къде е най-чистата точка за MS-CAN: зад CD30/GID, OBD 3/11 или друг модул.
- Какъв engine-running сигнал е най-надежден: CAN RPM/status, D+ или напрежение + таймер.
- Дали factory dimming може да се активира стабилно чрез емулация на manual low beam.
- Дали BLE конфигурацията е безопасна: телефонът променя настройки, но не remote-control-ва фаровете директно.
- Като future етап: дали GID overlay подходът е приемлив и няма да пречи на check-control/BC/radio съобщения.

## Неща, които не трябва да се приемат за доказани

- Pinout от руския repair guide за конкретната 2008 кола.
- Че `AUTO` позицията е електрически еднаква с всички Vectra C варианти.
- Че factory GID tab може да се добави. Планираме overlay, не заводски tab.
- Че всички CAN IDs от други проекти ще съвпаднат 1:1 с тази кола.
- Че директно управление на GID/CD30 brightness level е възможно. Първият план е factory dimming чрез manual low beam emulation.

## Предложен ред за реална проверка

1. OP-COM screenshots/logs при `DRL disabled`.
2. Мултиметър/осцилоскоп на light switch буксата.
3. Read-only MS-CAN logger зад CD30/GID.
4. Потвърждение на engine-running/RPM/status сигнал.
5. Тест на light switch emulator на маса.
6. Първи тест в колата: само `AUTO + engine running -> manual low beam`.
7. BLE настройки и запис във flash.
8. Future: CD30/steering buttons и GID text overlay.

## Критични fail-safe изисквания

- При изключен модул ръчните светлини работят заводски.
- Модулът не backfeed-ва BCM/UEC/REC.
- Модулът не може да остави D1S ксенона включен при crash/restart.
- Manual low beam/sidelights имат приоритет над AUTO логиката.
- Захранването е с предпазител, reverse polarity защита и transient защита.
