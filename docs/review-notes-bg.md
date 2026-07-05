# Notes for review

Този проект е за преглед от човек с опит в автомобилна електроника, CAN и embedded.

## Какво искаме да потвърди reviewer-ът

- Дали архитектурата "емулация на копчето, без директно захранване на фарове" е правилна за Vectra C.
- Какъв е типът на light switch входовете: отделни линии, резистивна стълбица или друго.
- Кой е най-безопасният output stage към копчето: реле, analog switch, opto-MOS или резисторна емулация.
- Къде е най-чистата точка за MS-CAN: зад CD30/GID, OBD 3/11 или друг модул.
- Дали GID overlay подходът е приемлив и няма да пречи на check-control/BC/radio съобщения.
- Какъв engine-running сигнал е най-надежден: CAN RPM/status, D+ или напрежение + таймер.

## Неща, които не трябва да се приемат за доказани

- Pinout от руския repair guide за конкретната 2008 кола.
- Че `AUTO` позицията е електрически еднаква с всички Vectra C варианти.
- Че factory GID tab може да се добави. Планираме overlay, не заводски tab.
- Че всички CAN IDs от други проекти ще съвпаднат 1:1 с тази кола.

## Предложен ред за реална проверка

1. OP-COM screenshots/logs при `DRL disabled`.
2. Мултиметър/осцилоскоп на light switch буксата.
3. Read-only MS-CAN logger зад CD30/GID.
4. Потвърждение на CD30 button / steering wheel button CAN frames.
5. Тест на GID text overlay на маса или много кратко в колата.
6. Едва след това output към light switch emulator.

## Критични fail-safe изисквания

- При изключен модул ръчните светлини работят заводски.
- Модулът не backfeed-ва BCM/UEC/REC.
- Модулът не може да остави D1S ксенона включен при crash/restart.
- Manual low beam/sidelights имат приоритет над AUTO логиката.
- Захранването е с предпазител, reverse polarity защита и transient защита.
