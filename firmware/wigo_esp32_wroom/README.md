# WIGO

RU:

WIGO — это игровая консоль на базе ESP32. Микроконтроллер создаёт точку доступа Wi‑Fi, раздаёт веб‑страницу для игроков и хранит состояние игры. Телефоны выступают в роли клиентов, а не отдельных игровых серверов.

## Аппаратное обеспечение

Стандартная конфигурация рассчитана на плату ESP32‑WROOM‑32, дисплей SSD1306 128×64 по шине I²C, три кнопки и необязательный делитель напряжения батареи.

| Компонент | Подключение к ESP32 по умолчанию |
| --- | --- |
| OLED SDA | GPIO 21 |
| OLED SCL | GPIO 22 |
| Кнопка UP (второй контакт на GND) | GPIO 25 |
| Кнопка OK (второй контакт на GND) | GPIO 26 |
| Кнопка DOWN (второй контакт на GND) | GPIO 27 |
| Выход делителя батареи | GPIO 34 (только вход ADC) |

### Изменение пинов и настроек устройства

Перед компиляцией отредактируйте файл [`Config.h`](Config.h). В нём задаются:

- назначения GPIO;
- адрес и поворот OLED‑экрана;
- сопротивления делителя батареи;
- пароль точки доступа;
- вместимость комнаты.

### Схема делителя батареи

Предполагается следующая схема:

```text
Плюс батареи ─ R1 ─┬─ ADC GPIO
                    R2
                     │
                    GND
```

Введите измеренные значения R1 и R2 в `Config.h`. Прошивка оценивает напряжение по делителю и показывает четыре диапазона заряда. Это не калиброванный процент заряда батареи. Перед использованием проверьте значения ADC и схему подключения.

По умолчанию дисплей развернут на 180° (`WIGO_OLED_ROTATION 2`). Если дисплей установлен в вертикальном положении, установите значение `0`. Адрес I²C по умолчанию: `0x3C`.

## Настройка в Arduino IDE

1. Откройте файл `Wigo_consol.ino` в Arduino IDE. Все файлы проекта должны находиться в папке со скетчем.
2. Выберите плату, соответствующую вашему модулю ESP32‑WROOM‑32 (обычно это **ESP32 Dev Module**).
3. Установите библиотеки через Library Manager:
   - **Adafruit GFX Library**
   - **Adafruit SSD1306**
   - **WebSockets by Markus Sattler**
4. Скомпилируйте и загрузите прошивку на устройство.

Проект не проверялся на всех версиях ядра ESP32 и библиотек. Если сборка не проходит, сохраните точные версии платы и библиотек вместе с выводом компилятора.

Веб‑страница встроена в `PlayerPage.h` и отдаётся напрямую из flash‑памяти; загрузка через LittleFS/SPIFFS не требуется.

## Подключение телефона

При запуске WIGO создаёт точку доступа с именем `WIGO-<код комнаты>`. Подключитесь к ней, используя пароль, заданный в `Config.h` через `WIGO_AP_PASSWORD`, затем откройте страницу:

```text
http://192.168.4.1
```

Пароль по умолчанию: `wigo-game`. Обязательно измените его перед тем, как использовать устройство в группе или передавать его другим людям. Это локальная игровая сеть, а не интернет‑сервис.

## Игры

- **Bunker** — от 3 и более готовых игроков. Игроки получают приватные карты персонажей, по очереди открывают карты, обсуждают и голосуют. Победа достигается, когда остаются два живых игрока. Таймеров нет.
- **Crocodile** — один игрок с телефона показывает слово для людей вокруг. На каждый раунд даётся 30 секунд; слова в рамках запуска не повторяются. Текущая колода содержит 390 разных карточек.
- **Mafia** — от 3 и более готовых игроков, но не более лимита комнаты. WIGO раздаёт одной роли мафии и остальным гражданским ролям. Первый подключённый игрок становится ведущим и видит все карты ролей. Ведущий управляет ходами и голосованием.
- **Alias** — один подключённый телефон передаётся между объясняющими. Объясняйте секретное слово вслух, не используя само слово и его части; группа отмечает правильные ответы. На каждое слово даётся 60 секунд.
- **Who Is the Spy?** — от 3 и более готовых игроков. Все, кроме одного случайно выбранного шпиона, получают одну и ту же секретную локацию. Игроки обсуждают вслух и голосуют; WIGO проверяет результат. Один игрок остаётся в роли шпиона.
- **Who Am I?** — от 2 и более готовых игроков. Перед стартом каждый участник предлагает персонажа для каждого другого игрока. Каждый игрок получает случайное предложение от другого участника, а затем пытается угадать, кто он.

## Лимит игроков

По умолчанию настроено 4 игрока. Параметр `WIGO_MAX_PLAYERS` в `Config.h` управляет слотами игроков и лимитом станций точки доступа. Библиотека WebSockets по умолчанию поддерживает до 5 подключённых клиентов, поэтому значения выше этого лимита требуют отдельной настройки.

Игра **Crocodile** намеренно требует ровно одного подключённого игрока. Показатель на стороне устройства учитывает только активных игровых клиентов, а не телефоны, просто подключённые к Wi‑Fi.

## Файлы проекта

- `Wigo_consol.ino` — запуск, меню, OLED‑экран, кнопки, измерение напряжения батареи и основной цикл.
- `Config.h` — пользовательские настройки GPIO и оборудования, пароль точки доступа и лимит игроков.
- `Network.ino` — работа SoftAP, HTTP‑маршруты, события WebSocket и обработка отключений/повторных подключений.
- `GameEngine.ino` — общее состояние игроков/лобби и логика игры «Бункер».
- `OtherGames.ino` — слова и таймеры для «Крокодила», роли, фазы и голосование для «Мафии».
- `PlayerPage.h` — встроенная адаптивная страница клиента на русском языке.

## Известные ограничения

- Лимит игроков по умолчанию равен 4, и он не был нагрузочно протестирован при больших значениях.
- Данные для повторного подключения сохраняются в `sessionStorage` браузера и могут пережить перезагрузку страницы в том же окне. Закрытие вкладки или использование другой сессии браузера может привести к потере токена.
- Деление напряжения батареи является приблизительным; точное время работы или процент заряда не гарантируются.
- Поведение оборудования, стабильность при длительной работе и игровые сценарии на реальном железе требуют проверки на целевой плате.
- Ручной чек‑лист перед релизом находится в файле [`TESTING.md`](TESTING.md); там перечислены ожидаемые проверки, а не результаты уже выполненных тестов.

## Лицензия

Этот проект распространяется по лицензии [MIT License](LICENSE).

EN:

WIGO is an ESP32-based game console. The ESP32 creates a Wi‑Fi access point, serves the player page, and stores the game state. Phones act as clients rather than independent game servers.

## Hardware

The default configuration targets an ESP32‑WROOM‑32 board, a 128×64 SSD1306 I²C display, three buttons, and an optional battery voltage divider.

| Part | Default ESP32 connection |
| --- | --- |
| OLED SDA | GPIO 21 |
| OLED SCL | GPIO 22 |
| UP button, other contact to GND | GPIO 25 |
| OK button, other contact to GND | GPIO 26 |
| DOWN button, other contact to GND | GPIO 27 |
| Battery divider output | GPIO 34 (ADC input only) |

### Change pins and device settings

Edit [`Config.h`](Config.h) before compiling. In that file you configure:

- GPIO assignments;
- OLED address and rotation;
- battery divider resistor values;
- access point password;
- room capacity.

### Battery divider schematic

The divider is assumed to be:

```text
Battery positive ─ R1 ─┬─ ADC GPIO
                       R2
                        │
                       GND
```

Enter the measured R1 and R2 values in `Config.h`. The firmware estimates voltage from the divider and shows a four-step battery band. It does not report a calibrated battery percentage. Validate the ADC setup and wiring before using it in the field.

The display is rotated 180° by default (`WIGO_OLED_ROTATION 2`). Set it to `0` if your display is mounted upright. The default I²C address is `0x3C`.

## Arduino IDE setup

1. Open `Wigo_consol.ino` in Arduino IDE. Keep all project files in the sketch folder.
2. Select the board matching your ESP32‑WROOM‑32 module (commonly **ESP32 Dev Module**).
3. Install the required libraries with Library Manager:
   - **Adafruit GFX Library**
   - **Adafruit SSD1306**
   - **WebSockets by Markus Sattler**
4. Build and upload the firmware.

The project has not been verified against every ESP32 core/library version. If a build fails, record the selected board package and library versions together with the compiler output.

The web page is embedded in `PlayerPage.h` and served directly from flash memory; no LittleFS/SPIFFS upload is required.

## Connect a phone

At startup, WIGO creates an access point named `WIGO-<room code>`. Connect to it using the password configured by `WIGO_AP_PASSWORD` in `Config.h`, then open:

```text
http://192.168.4.1
```

The default password is `wigo-game`. Change it for your device before sharing it or using it with a group. This is a local party-game network, not an internet-facing service.

## Games

- **Bunker** — 3 or more ready players. Players receive private character cards, reveal cards in turn, discuss, and vote. Victory occurs when only two living players remain. There are no game timers.
- **Crocodile** — one phone player acts out a prompt for people nearby. Each prompt has a 30-second round; prompts do not repeat during a run. The current deck contains 390 distinct entries.
- **Mafia** — 3 or more ready players, up to the configured room capacity. WIGO deals one Mafia role and civilian roles. The first connected player is the host and sees all role cards. The host manages the game flow and voting.
- **Alias** — one connected phone is passed between explainers. Explain the secret word aloud without using the word or its parts; the group marks correct guesses. Each word has 60 seconds.
- **Who Is the Spy?** — 3 or more ready players. Everyone except one randomly selected spy privately receives the same secret location. Players discuss aloud and vote; WIGO checks the result.
- **Who Am I?** — 2 or more ready players. Before starting, each participant suggests a character for every other player. Each player privately receives a random suggestion from another participant and tries to guess who they are.

## Player capacity

The default is four players. `WIGO_MAX_PLAYERS` in `Config.h` controls the game slots and access-point station limit. The installed WebSockets library defaults to five server clients, so values above that require additional configuration.

The **Crocodile** game intentionally requires exactly one connected player. The device-side count reflects active game clients, not phones that are only associated with Wi‑Fi.

## Project files

- `Wigo_consol.ino` — startup, menu, OLED display, buttons, battery sampling, and main loop.
- `Config.h` — user-editable GPIO and hardware settings, AP password, and player capacity.
- `Network.ino` — SoftAP setup, HTTP routes, WebSocket events, and disconnect/reconnect handling.
- `GameEngine.ino` — shared player/lobby state and Bunker game logic.
- `OtherGames.ino` — Crocodile prompts/timer and Mafia roles/phases/voting logic.
- `PlayerPage.h` — embedded responsive client page (Russian-language UI).

## Known limits

- Player capacity defaults to four and has not been load-tested at higher values.
- Reconnect credentials are stored in browser `sessionStorage` and can survive a page reload in the same tab. Closing the tab/browser or using a different browser session may lose the token.
- Battery bands are approximate; no battery runtime or charge percentage is claimed.
- Hardware behavior, long-duration stability, and real-device game flows require testing on the target board.
- The manual pre-release checklist is in [`TESTING.md`](TESTING.md); it records expected checks rather than completed results.

## License

This project is released under the [MIT License](LICENSE).
