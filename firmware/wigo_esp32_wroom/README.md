# WIGO

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
