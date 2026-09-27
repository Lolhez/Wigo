# 🎮 WIGO

> Portable game console based on ESP32 with Wi‑Fi multiplayer

WIGO is a standalone game device. The ESP32 creates a Wi‑Fi access point, serves the player page, and manages the game state. Phones connect as clients rather than hosting their own game server.

---

## 🔧 Hardware

Default configuration:
- ESP32‑WROOM‑32 development board
- 128×64 SSD1306 I²C display
- 3 control buttons
- Optional battery voltage divider

### Default connections

| Component | GPIO | Notes |
|-----------|------|-------|
| OLED SDA | 21 | I²C data |
| OLED SCL | 22 | I²C clock |
| UP button | 25 | Other contact to GND |
| OK button | 26 | Other contact to GND |
| DOWN button | 27 | Other contact to GND |
| Battery divider output | 34 | ADC input only |

### ⚙️ Change pins and device settings

Edit [`Config.h`](Config.h) before compiling:

- GPIO assignments
- OLED address and rotation
- Battery divider resistor values
- Access point password
- Player capacity

### 🔋 Battery divider

The divider is assumed to be:

```text
Battery positive ─ R1 ─┬─ ADC GPIO
                       R2
                        │
                       GND
```

To configure it:
1. Measure R1 and R2
2. Enter the values in `Config.h`
3. The firmware shows a 4-level battery indicator
4. This is approximate, not a calibrated percentage

Display settings:
- Rotated 180° by default (`WIGO_OLED_ROTATION 2`)
- Set to `0` if the display is mounted upright
- Default I²C address: `0x3C`

---

## 📱 Arduino IDE setup

### Requirements
- Arduino IDE
- Board: **ESP32 Dev Module** or equivalent ESP32‑WROOM‑32 board

### Steps

1. Open `Wigo_consol.ino` in Arduino IDE.
2. Keep all project files in the same sketch folder.
3. Select the board:
   - `Tools` → `Board` → `ESP32` → `ESP32 Dev Module`
4. Install required libraries via Library Manager:
   - **Adafruit GFX Library**
   - **Adafruit SSD1306**
   - **WebSockets by Markus Sattler**
5. Build and upload the firmware.

✅ The web page is embedded in `PlayerPage.h`, so no LittleFS/SPIFFS upload is needed.

⚠️ The project has not been verified against every ESP32 core/library version. If a build fails, record the exact board package and library versions along with the compiler output.

---

## 📡 Connect a phone

### Startup

When powered on, WIGO creates an access point named `WIGO-<room code>`.

### On the phone

1. Connect to the Wi‑Fi network `WIGO-XXXX`
2. Use the password from `WIGO_AP_PASSWORD` in `Config.h`
3. Open the browser and go to:

```text
http://192.168.4.1
```

The default password is `wigo-game`. Change it before sharing the device or using it with a group. This is a local party-game network, not an internet-facing service.

---

## 🎯 Games

| Game | Minimum players | Description |
|------|-----------------|-------------|
| **Bunker** | 3+ | Private character cards, discussion, voting, and elimination. Victory occurs when only two living players remain. No timers. |
| **Crocodile** | 1 | One player acts out a prompt for the group. Each prompt lasts 30 seconds. The deck includes 390 unique prompts. |
| **Mafia** | 3+ | One Mafia role and civilian roles. The first connected player becomes host and sees all role cards. |
| **Alias** | 2+ | One player explains a secret word without saying the word or its parts. The group marks correct guesses. Each word lasts 60 seconds. |
| **Who Is the Spy?** | 3+ | Everyone except one spy receives the same location. Players discuss and vote; WIGO checks the result. |
| **Who Am I?** | 2+ | Before the round, each participant suggests a character for the others. Each player receives a random suggestion and tries to guess who they are. |

---

## 👥 Player capacity

Default capacity: 4 players.

`WIGO_MAX_PLAYERS` in `Config.h` controls both the number of game slots and the Wi‑Fi station limit. The installed WebSockets library defaults to 5 server clients, so larger values may require additional configuration.

The **Crocodile** game intentionally requires exactly one connected player. The device-side count reflects active game clients, not phones that are merely associated with Wi‑Fi.

---

## 📂 Project files

- `Wigo_consol.ino` — startup, menu, OLED, buttons, battery sampling, and main loop
- `Config.h` — user-editable GPIO and hardware settings, AP password, and player capacity
- `Network.ino` — SoftAP, HTTP routes, WebSocket events, and reconnect handling
- `GameEngine.ino` — shared lobby/player state and Bunker logic
- `OtherGames.ino` — Crocodile prompts, timer, Mafia roles, phases, and voting logic
- `PlayerPage.h` — embedded responsive client page

---

## ⚡ Known limits

- Default player capacity is 4 and has not been load-tested at larger values
- Reconnect credentials are stored in browser `sessionStorage`; closing the tab/browser, or using a different browser session, may lose the token
- Battery bands are approximate; no runtime or calibrated battery percentage is claimed
- Hardware behavior, long-duration stability, and real-device game flows need testing on a target board
- The manual pre-release checklist is in [`TESTING.md`](TESTING.md)

---

## 📜 License

This project is released under the [MIT License](LICENSE).

---

## 🚀 Quick start

```text
1. Open the project in Arduino IDE
2. Edit Config.h for your board and hardware
3. Install required libraries
4. Compile and upload to the ESP32
5. Connect from a phone to the WIGO Wi‑Fi
6. Open http://192.168.4.1 and start playing
```
