# Hardware and multiplayer test checklist

These are manual acceptance checks for the target ESP32 board. They are a plan, not a claim that the checks have already passed. Record the board/core/library versions, phone/browser, result, and Serial output for each run.

| ID | Check | Pass condition | If it fails, inspect |
| --- | --- | --- | --- |
| 01 | Cold boot | WIGO starts, shows the menu, and starts its AP | Supply voltage, Serial log, OLED wiring |
| 02 | OLED | Text is readable, correctly rotated, and not clipped | I²C address, SDA/SCL, `WIGO_OLED_ROTATION` |
| 03 | UP button | One short press moves selection up once | GPIO in `Config.h`, ground, debounce |
| 04 | OK button | Opens the selected screen/game | GPIO, switch contact, menu state |
| 05 | DOWN button | One short press moves selection down once | GPIO, ground, debounce |
| 06 | Battery reading | Estimated voltage tracks a multimeter consistently | Divider values, ADC node voltage, board calibration |
| 07 | Wi-Fi AP | Phone joins `WIGO-<room>` and opens `http://192.168.4.1` | AP password, DHCP, Serial log |
| 08 | One client | Name appears and ready state updates | Browser console, WebSocket connection |
| 09 | Capacity | Join clients up to `WIGO_MAX_PLAYERS`; next client is rejected by capacity | AP station count, WebSocket client limit, game slots |
| 10 | WebSocket idle | Lobby connection remains alive for 10 minutes | Heartbeat behavior, browser close code, Serial output |
| 11 | Disconnect | Disconnect during lobby, reveal, vote, and Mafia phases | Player state, reveal queue, vote completion |
| 12 | Reconnect | Restore Wi-Fi and reload the same tab; player slot is recovered | Room code, session storage, reconnect grace period |
| 13 | Lobby/game selection | Selected game title and ready requirements match; stale ready state is cleared | Device selection and `STATE` message |
| 14 | Bunker | Start with 3 and configured maximum; private cards, reveal queue, votes, and two-player victory work | `PROFILE`, `REVEALED`, vote results, survivor state |
| 15 | Crocodile | Solo start, timer, pass, correct guess, end, and restart work; deck reports 390 prompts | `crocTotal`, `crocPlayed`, score, prompt sequence |
| 16 | Alias | Start with one connected phone; pass it between explainers; correct, skip, timeout, and finish prompts | Secret prompt privacy, 60-second timer, score, prompt sequence |
| 17 | Mafia | Test minimum and maximum group; roles are private, host sees all, manual phases and voting work | Per-client role display, host reassignment, winner state |
| 18 | Who Is the Spy? | Test with 3 and configured maximum; only civilians receive the location; host starts voting; server resolves it | Role privacy, vote options, ties, winner state |
| 19 | Who Am I? | Test with 2 and configured maximum; each player submits a character for every other participant; nobody receives their own submission; screen reveals after five seconds | No self-assigned character, private card delivery, countdown recovery after reconnect |
| 20 | Simultaneous actions | Send two votes nearly together; each valid player action is counted once | Serial event order and resulting state |
| 21 | Game restart | Exit on device, select another game, and start again | Running/outcome flags, readiness, previous private cards |
| 22 | Memory stress | Repeatedly connect, disconnect, and play; no progressive heap loss or reset | Add/record free heap and largest block in a diagnostic build |
| 23 | Long run | Keep AP active for several hours and play multiple rounds | Serial log, supply voltage, WebSocket reconnects, heap trend |

Do not increase player capacity based only on successful compilation. Repeat checks 09, 19, 21, and 22 on the target board after changing `WIGO_MAX_PLAYERS`.
