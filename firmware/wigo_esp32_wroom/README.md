# WIGO
RU:
WIGO — это игровая консоль на базе микроконтроллера ESP32. ESP32 создает точку доступа Wi-Fi, раздает веб-страницу для игроков и управляет состоянием игры. Телефоны выступают в роли клиентов; они не запускают игровой сервер.
Аппаратное обеспечение (Железо)
Стандартная конфигурация рассчитана на плату разработки ESP32-WROOM-32, I²C-дисплей SSD1306 с разрешением 128×64, три кнопки и дополнительный делитель напряжения для батареи.
Компонент	Подключение к ESP32 по умолчанию
OLED SDA	GPIO 21
OLED SCL	GPIO 22
Кнопка ВВЕРХ (UP) (второй контакт на GND)	GPIO 25
Кнопка ОК (второй контакт на GND)	GPIO 26
Кнопка ВНИЗ (DOWN) (второй контакт на GND)	GPIO 27
Выход делителя батареи	GPIO 34 (только вход ADC)
Изменение пинов и настроек устройства
Перед компиляцией отредактируйте файл Config.h. Там хранятся назначения GPIO, адрес/поворот OLED-экрана, сопротивление резисторов делителя батареи, пароль точки доступа и настроенный лимит игроков. Перед изменением GPIO сверьтесь со схемой распиновки вашей конкретной платы. Не используйте пины флэш-памяти ESP32 (GPIO 6–11). Избегайте использования страппинг-пинов (strapping pins), если не понимаете их требований при загрузке.
Схема делителя батареи подразумевает следующий вид:
text
Плюс батареи ─ R1 ─┬─ ADC GPIO
                   R2
                    │
                   GND
Используйте код с осторожностью.
Введите измеренные значения R1 и R2 в Config.h. Прошивка рассчитывает примерное напряжение по делителю и показывает четырехсегментную шкалу заряда. Она не отображает откалиброванный процент батареи. Перед подключением аккумулятора проверьте напряжение на пине ADC и всю схему зарядки/защиты; примерные значения в коде не являются гарантией безопасности оборудования.
По умолчанию дисплей развернут на 180 градусов (WIGO_OLED_ROTATION 2). Установите значение 0, если ваш дисплей установлен вертикально (прямо). I²C-адрес по умолчанию — 0x3C.
Настройка в Arduino IDE
1. Откройте файл Wigo_consol.ino в Arduino IDE. Храните все файлы проекта в этой папке с эскизом (скетчем).
2. Выберите плату, соответствующую вашему модулю ESP32-WROOM-32 (обычно это ESP32 Dev Module).
3. Установите через Менеджер библиотек (Library Manager) следующие библиотеки: Adafruit GFX Library, Adafruit SSD1306 и WebSockets от Markus Sattler.
4. Скомпилируйте и загрузите прошивку. Проект не тестировался со всеми версиями ядер ESP32 и библиотек; если сборка завершится ошибкой, запишите выбранный пакет платы, версии библиотек и вывод компилятора.
Веб-страница встроена в файл PlayerPage.h и раздается из флэш-памяти; загрузка через LittleFS/SPIFFS не требуется.
Подключение телефона
При запуске WIGO создает точку доступа с именем WIGO-<код комнаты>. Подключитесь к ней, используя пароль, заданный параметром WIGO_AP_PASSWORD в Config.h, а затем откройте в браузере:
http://192.168.4.1
Пароль по умолчанию: wigo-game. Обязательно измените его для своего устройства перед тем, как делиться им или использовать в компании. Это локальная сеть для домашних игр (party games), а не сервис, смотрящий в интернет.
Игры
• Бункер (Bunker) — от 3 и более готовых игроков. Игроки получают втайне карты персонажей, по очереди открывают их, обсуждают и голосуют. Победа наступает, когда остаются два живых игрока. Игровых таймеров нет.
• Крокодил (Crocodile) — один игрок с телефона показывает загаданное слово людям вокруг. На каждое слово дается 30-секундный раунд; слова не повторяются в течение одной игры. Текущая колода содержит 390 уникальных записей.
• Мафия (Mafia) — от 3 и более готовых игроков (в пределах настроенной вместимости комнаты). WIGO распределяет одну роль Мафии и роли мирных жителей. Первый подключившийся игрок становится ведущим и видит карты всех ролей. Ведущий вручную переключает день/ночь и запускает дневное голосование. Автоматических ночных действий или таймеров фаз нет.
• Алиас (Alias) — один подключенный телефон передается между объясняющими. Объясняйте секретное слово вслух, не используя само слово или его однокоренные части; группа отмечает правильные ответы. На каждое слово дается 60 секунд.
• Кто шпион? (Who Is the Spy?) — от 3 и более готовых игроков. Все, кроме одного случайно выбранного шпиона, тайно получают одну и ту же локацию. Игроки обсуждают ее вслух и голосуют; WIGO проверяет результат. Ведущий может запустить голосование, но сам никогда не видит секретные карты других игроков.
• Кто я? (Who Am I?) — от 2 и более готовых игроков. Перед стартом каждый участник придумывает персонажа для любого другого игрока. Каждый игрок тайно получает случайный вариант, предложенный другим участником (свою собственную идею получить нельзя). Нажмите кнопку на экране, переверните телефон экраном наружу и прижмите ко лбу; через пять секунд персонаж отобразится, и все остальные его увидят. Таймера раунда нет.
Лимит игроков
По умолчанию установлено 4 игрока. Параметр WIGO_MAX_PLAYERS в Config.h управляет игровыми слотами и лимитом станций точки доступа. Установленная библиотека WebSockets по умолчанию поддерживает до 5 клиентов сервера, поэтому значения до 5 работают «из коробки».
Для работы с более чем 5 клиентами необходимо задать флаг WEBSOCKETS_SERVER_CLIENT_MAX как глобальный флаг сборки как для скетча, так и для компиляции библиотеки WebSockets; определение его только в заголовке проекта может привести к несовместимости структуры классов. ESP32 SoftAP поддерживает максимум 10 станций, но емкость выше 5 не тестировалась на производительность и может превысить доступную память. Оставьте значение по умолчанию, если вы не можете провести стресс-тест памяти на вашей плате с несколькими устройствами.
Игра «Крокодил» намеренно требует ровно одного подключенного игрока. Счетчик на стороне устройства показывает подключенных игровых клиентов, а не просто телефоны, подключенные к Wi-Fi.
Файлы проекта
• Wigo_consol.ino — запуск, меню, OLED-экран, кнопки, замер батареи и основной цикл (main loop).
• Config.h — редактируемые пользователем настройки GPIO и оборудования, пароль точки доступа и лимит игроков.
• Network.ino — работа SoftAP, HTTP-маршруты, события WebSocket и обработка отключений/переподключений.
• GameEngine.ino — общее состояние игроков/лобби и логика игры «Бункер».
• OtherGames.ino — слова/таймер «Крокодила» и роли/фазы/голосование «Мафии».
• PlayerPage.h — встроенная адаптивная страница клиента на русском языке.
Известные ограничения
• Лимит игроков по умолчанию равен 4 и не тестировался под нагрузкой при более высоких значениях.
• Данные для переподключения хранятся в sessionStorage браузера и могут сохраняться при перезагрузке страницы в той же вкладке. Закрытие вкладки/браузера или использование другой сессии браузера может привести к потере токена.
• Деления батареи являются приблизительными; точное время работы или процент заряда не гарантируются.
• Поведение оборудования, стабильность при длительной работе и игровые процессы на реальных устройствах требуют тестирования на целевой плате.
• Ручной чек-лист перед релизом находится в файле TESTING.md; он содержит список ожидаемых проверок, а не готовые результаты.
Лицензия
Этот проект распространяется под лицензией MIT.
EN:
WIGO is an ESP32-hosted game console. The ESP32 creates the Wi-Fi access point, serves the player page, and owns the game state. Phones are clients; they do not host a game server.

## Hardware

The default configuration targets an ESP32-WROOM-32 development board, a 128×64 SSD1306 I²C display, three buttons, and an optional battery voltage divider.

| Part | Default ESP32 connection |
| --- | --- |
| OLED SDA | GPIO 21 |
| OLED SCL | GPIO 22 |
| UP button, other contact to GND | GPIO 25 |
| OK button, other contact to GND | GPIO 26 |
| DOWN button, other contact to GND | GPIO 27 |
| Battery divider output | GPIO 34 (ADC input only) |

### Change pins and device settings

Edit [`Config.h`](Config.h) before compiling. GPIO assignments, OLED address/rotation, battery-divider resistor values, access-point password, and the configured player capacity are kept there. Check your exact board pinout before changing GPIOs. Do not use ESP32 flash pins GPIO 6–11. Avoid strapping pins unless you understand their boot-time requirements.

The battery divider is assumed to be:

```text
Battery positive ─ R1 ─┬─ ADC GPIO
                       R2
                        │
                       GND
```

Enter the measured R1 and R2 values in `Config.h`. The firmware estimates voltage from the divider and shows a four-step voltage band. It does not report a calibrated battery percentage. Verify the ADC pin voltage and the complete charging/protection circuit before connecting a battery; the example values are not a hardware safety certification.

The display is rotated 180 degrees by default (`WIGO_OLED_ROTATION 2`). Set it to `0` if your display is mounted upright. The default I²C address is `0x3C`.

## Arduino IDE setup

1. Open `Wigo_consol.ino` in Arduino IDE. Keep all project files in this sketch folder.
2. Select the board matching your ESP32-WROOM-32 module (commonly **ESP32 Dev Module**).
3. Install **Adafruit GFX Library**, **Adafruit SSD1306**, and **WebSockets by Markus Sattler** using Library Manager.
4. Build and upload. The project has not been verified against every ESP32 core/library version; if a build fails, record the selected board package and library versions with the compiler output.

The web page is embedded in `PlayerPage.h` and served from flash; no LittleFS/SPIFFS upload is needed.

## Connect a phone

At startup, WIGO creates an access point named `WIGO-<room code>`. Connect to it using the password configured by `WIGO_AP_PASSWORD` in `Config.h`, then open:

```text
http://192.168.4.1
```

The default password is `wigo-game`. Change it for your device before sharing or using it with a group. This is a local party-game network, not an internet-facing service.

## Games

- **Bunker** — 3 or more ready players. Players receive private character cards, reveal cards in turn, discuss, and vote. Victory occurs when two living players remain. There are no game timers.
- **Crocodile** — one phone player acts out a prompt for people nearby. Each prompt has a 30-second round; prompts do not repeat during a run. The current deck contains 390 distinct entries.
- **Mafia** — 3 or more ready players, up to the configured room capacity. WIGO deals one Mafia role and civilian roles. The first connected player is the host and sees all role cards. The host manually switches night/day and starts daytime voting. There are no automatic night actions or phase timers.
- **Alias** — one connected phone is passed between explainers. Explain the secret word aloud without using the word or its parts; the group marks correct guesses. Each word has 60 seconds.
- **Who Is the Spy?** — 3 or more ready players. Everyone except one randomly selected spy privately receives the same secret location. Players discuss aloud and vote; WIGO checks the result. The host can start voting but never receives other players' secret cards.
- **Who Am I?** — 2 or more ready players. Before starting, each participant suggests a character for every other player. Each player privately receives a random suggestion from another participant, so they cannot receive their own idea. Press the on-screen button, turn the phone screen outward, and hold it to your forehead; after five seconds the character appears for everyone else to see. There is no round timer.

## Player capacity

The default is four players. `WIGO_MAX_PLAYERS` in `Config.h` controls the game slots and access-point station limit. The installed WebSockets library defaults to five server clients, so values up to five work with its default capacity. More than five requires setting `WEBSOCKETS_SERVER_CLIENT_MAX` as a global build flag for **both** the sketch and the WebSockets library compilation; defining it only in a project header can create incompatible class layouts. ESP32 SoftAP supports at most 10 stations, but capacity above five is **not performance-tested** and may exceed available memory. Keep the default unless you can run a multi-device and memory stress test on your board.

The Crocodile game intentionally requires exactly one connected player. The device-side count shows connected game clients, not phones merely associated with Wi-Fi.

## Project files

- `Wigo_consol.ino` — startup, menu, OLED, buttons, battery sampling, and main loop.
- `Config.h` — user-editable GPIO and hardware settings, AP password, and player capacity.
- `Network.ino` — SoftAP, HTTP route, WebSocket events, and disconnect/reconnect handling.
- `GameEngine.ino` — shared player/lobby state and Bunker game logic.
- `OtherGames.ino` — Crocodile prompts/timer and Mafia roles/phases/voting.
- `PlayerPage.h` — embedded responsive Russian-language client page.

## Known limits

- Player capacity defaults to four and has not been load-tested at higher values.
- Reconnect credentials are stored in browser `sessionStorage` and can survive a page reload in the same tab. Closing the tab/browser or using a different browser session may lose the token.
- Battery bands are approximate; no battery runtime or charge percentage is claimed.
- Hardware behavior, long-duration stability, and real-device game flows require testing on the target board.
- The manual pre-release checklist is in [`TESTING.md`](TESTING.md); it records expected checks, not completed results.

## License

This project is released under the [MIT License](LICENSE).
