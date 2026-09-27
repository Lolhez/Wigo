#include "Config.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include "PlayerPage.h"

// APIs implemented in the other Arduino IDE tabs.
static uint8_t connectedPlayerCount();
static int8_t findPlayerBySocket(uint8_t socketId);
static int8_t currentHostPlayer();
static void sendText(uint8_t socketId, const String& message);
static void broadcastState();
static void handlePlayerMessage(uint8_t socketId, uint8_t* payload, size_t length);
static void checkVoteCompletion();
static void advanceRevealQueue();
static bool startOtherGame(uint8_t starterSocket);
static bool handleOtherGameCommand(uint8_t socketId, int8_t playerId, const char* message);
static void appendOtherGameState(String& message);
static void sendOtherGamePrivate(uint8_t socketId, uint8_t playerId);
static void tickOtherGames(uint32_t now);
static const char* otherGamePhase();
static void resetOtherGame();
static void onOtherGameDisconnect();
static void startAccessPoint();
static void serviceNetwork(uint32_t now);

static const int OLED_RESET = -1;
static const uint32_t BUTTON_DEBOUNCE_MS = 35;
static const uint32_t BATTERY_SAMPLE_MS = 2000;
static const uint32_t SPLASH_DURATION_MS = 900;
static const uint32_t RECONNECT_GRACE_MS = 120000;
static const uint32_t ACTION_INTERVAL_MS = 250;
static const uint8_t MAX_PLAYERS = WIGO_MAX_PLAYERS;

Adafruit_SSD1306 display(WIGO_OLED_WIDTH, WIGO_OLED_HEIGHT, &Wire, OLED_RESET);
bool displayAvailable = false;
WebServer httpServer(80);
WebSocketsServer webSocket(81);

struct Player {
  bool connected;
  bool ready;
  bool alive;
  uint8_t socketId;
  char name[25];
  char reconnectToken[9];
  uint8_t profession;
  uint8_t ageGender;
  uint8_t health;
  uint8_t hobby;
  uint8_t phobia;
  uint8_t baggage;
  uint8_t fact;
  uint8_t special;
  uint32_t lastSeenAt;
  uint32_t lastActionAt;
};

enum BunkerPhase : uint8_t { BUNKER_DISCUSSION, BUNKER_VOTING, BUNKER_REVEAL };
enum BunkerOutcome : uint8_t { BUNKER_UNFINISHED, BUNKER_VICTORY };
enum GameKind : uint8_t { GAME_BUNKER, GAME_CROCODILE, GAME_MAFIA, GAME_ALIAS, GAME_SPY, GAME_WHOAMI };

Player players[MAX_PLAYERS] = {};
char roomId[5] = "----";
bool accessPointStarted = false;
bool gameRunning = false;
GameKind selectedGame = GAME_BUNKER;
uint8_t bunkerRound = 0;
uint8_t bunkerCapacity = 0;
uint8_t bunkerScenarioIndex = 0;
uint8_t bunkerPhase = BUNKER_DISCUSSION;
BunkerOutcome gameOutcome = BUNKER_UNFINISHED;
uint8_t bunkerParticipantCount = 0;
uint8_t voteTarget[MAX_PLAYERS];
bool hasVoted[MAX_PLAYERS];
uint8_t revealedMask[MAX_PLAYERS] = {};
uint32_t lastUiRefreshAt = 0;
uint32_t lastApRetryAt = 0;



enum AppScreen : uint8_t {
  SCREEN_SPLASH,
  SCREEN_MAIN_MENU,
  SCREEN_GAMES,
  SCREEN_GAME_MENU,
  SCREEN_WIFI,
  SCREEN_SETTINGS,
  SCREEN_ABOUT
};

enum ButtonId : uint8_t { BUTTON_ID_UP, BUTTON_ID_OK, BUTTON_ID_DOWN, BUTTON_COUNT };

struct ButtonState {
  uint8_t pin;
  bool stablePressed;
  bool lastRawPressed;
  uint32_t rawChangedAt;
  bool justPressed;
};

ButtonState buttons[BUTTON_COUNT] = {
  {WIGO_GPIO_BUTTON_UP, false, false, 0, false},
  {WIGO_GPIO_BUTTON_OK, false, false, 0, false},
  {WIGO_GPIO_BUTTON_DOWN, false, false, 0, false}
};

const char* const MAIN_ITEMS[] = {"Games", "WiFi", "Settings", "About"};
const uint8_t MAIN_ITEM_COUNT = sizeof(MAIN_ITEMS) / sizeof(MAIN_ITEMS[0]);
const char* const GAME_ITEMS[] = {"Bunker", "Crocodile", "Mafia", "Alias", "Spy", "Who am I?"};
const uint8_t GAME_ITEM_COUNT = sizeof(GAME_ITEMS) / sizeof(GAME_ITEMS[0]);

AppScreen screen = SCREEN_SPLASH;
uint8_t mainSelection = 0;
uint8_t gameSelection = 0;
uint8_t aboutPage = 0;
uint32_t screenEnteredAt = 0;
uint32_t lastBatterySampleAt = 0;
float batteryVoltage = 0.0f;
uint8_t batteryBars = 0;
bool batteryReadingAvailable = false;
bool batteryNeedsRedraw = false;

static void drawBatteryIcon() {
  // Four bars represent approximate voltage bands, not a calibrated fuel gauge.
  display.drawRect(105, 2, 20, 9, SSD1306_WHITE);
  display.fillRect(125, 5, 3, 3, SSD1306_WHITE);
  if (!batteryReadingAvailable) return;
  for (uint8_t i = 0; i < batteryBars; ++i) {
    display.fillRect(107 + i * 4, 4, 3, 5, SSD1306_WHITE);
  }
}

static void drawHeader(const char* title) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(4, 3);
  display.print(title);
  drawBatteryIcon();
  display.drawFastHLine(0, 14, 128, SSD1306_WHITE);
  display.drawPixel(2, 14, SSD1306_BLACK);
  display.drawPixel(125, 14, SSD1306_BLACK);
}

static void drawMenuItems(const char* const items[], uint8_t count, uint8_t selection) {
  const uint8_t maxVisible = 3;
  uint8_t first = 0;
  if (selection >= maxVisible) first = selection - maxVisible + 1;
  for (uint8_t row = 0; row < maxVisible && first + row < count; ++row) {
    const uint8_t index = first + row;
    const int16_t y = 18 + row * 13;
    const bool active = index == selection;
    if (active) {
      display.fillRoundRect(2, y, 124, 12, 3, SSD1306_WHITE);
      display.setTextColor(SSD1306_BLACK);
    } else {
      display.setTextColor(SSD1306_WHITE);
      display.drawRoundRect(2, y, 124, 12, 3, SSD1306_BLACK);
    }
    display.setCursor(7, y + 2);
    display.print(active ? ">" : "*");
    display.setCursor(19, y + 2);
    display.print(items[index]);
  }
  display.setTextColor(SSD1306_WHITE);
  display.drawFastHLine(0, 55, 128, SSD1306_WHITE);
  display.setCursor(4, 56);
  display.print("UP/DN MOVE    OK OPEN");
  display.display();
}

static void renderMainMenu() {
  if (!displayAvailable) return;
  drawHeader("WIGO");
  drawMenuItems(MAIN_ITEMS, MAIN_ITEM_COUNT, mainSelection);
}

static void renderGames() {
  if (!displayAvailable) return;
  drawHeader("GAMES");
  drawMenuItems(GAME_ITEMS, GAME_ITEM_COUNT, gameSelection);
}

static void renderBunkerMenu() {
  const char* title = selectedGame == GAME_CROCODILE ? "CROCODILE" :
                      selectedGame == GAME_MAFIA ? "MAFIA" :
                      selectedGame == GAME_ALIAS ? "ALIAS" :
                      selectedGame == GAME_SPY ? "SPY" :
                      selectedGame == GAME_WHOAMI ? "WHO AM I?" : "BUNKER";
  drawHeader(title);
  display.drawRoundRect(3, 18, 122, 16, 3, SSD1306_WHITE);
  display.setCursor(8, 22);
  display.print("ROOM  "); display.print(roomId);
  display.setCursor(5, 39);
  if (gameOutcome == BUNKER_VICTORY && selectedGame == GAME_BUNKER) {
    display.print("WINNER  ");
  } else if (gameOutcome != BUNKER_UNFINISHED) {
    display.print("FINISHED  ");
  } else if (gameRunning) {
    display.print("IN GAME  ");
  } else {
    display.print("PLAYERS  ");
  }
  if (gameOutcome == BUNKER_VICTORY && selectedGame == GAME_BUNKER) display.print("2");
  else if (gameOutcome != BUNKER_UNFINISHED) display.print("-");
  else if (gameRunning && selectedGame == GAME_BUNKER) display.print(bunkerRound);
  else if (gameRunning) display.print("-");
  else display.print(connectedPlayerCount());
  display.drawFastHLine(3, 51, 122, SSD1306_WHITE);
  display.setCursor(5, 55);
  display.print("192.168.4.1  OK:BACK");
  display.display();
}

static void renderWifi() {
  drawHeader("WI-FI");
  display.drawRoundRect(3, 18, 122, 17, 3, SSD1306_WHITE);
  display.setCursor(8, 23);
  display.print(accessPointStarted ? "SSID  WIGO-" : "AP NOT READY");
  if (accessPointStarted) display.print(roomId);
  display.setCursor(6, 40);
  display.print("PASS  "); display.println(WIGO_AP_PASSWORD);
  display.setCursor(6, 49);
  display.print("WEB   192.168.4.1");
  display.setCursor(80, 56);
  display.print("OK  BACK");
  display.display();
}

static void renderSettings() {
  drawHeader("SETTINGS");
  display.setCursor(5, 20);
  display.print("BATTERY");
  display.drawRoundRect(5, 32, 112, 10, 3, SSD1306_WHITE);
  display.fillRect(117, 35, 3, 4, SSD1306_WHITE);
  if (batteryReadingAvailable && batteryBars) display.fillRoundRect(8, 35, batteryBars * 26, 4, 2, SSD1306_WHITE);
  display.setCursor(5, 47);
  if (batteryReadingAvailable) { display.print(batteryVoltage, 2); display.print(" V  APPROX"); }
  else display.print("NO BATTERY READING");
  display.setCursor(80, 56);
  display.print("OK  BACK");
  display.display();
}

static void renderAbout() {
  if (!displayAvailable) return;
  drawHeader("ABOUT");
  switch (aboutPage) {
    case 0:
      display.setCursor(4, 21); display.println("WIGO");
      display.setCursor(4, 35); display.println("Version: 0.5.0");
      display.setCursor(4, 49); display.println("Author: Lolhez");
      break;
    case 1:
      display.setCursor(4, 20); display.print("OLED SDA: GPIO"); display.println(WIGO_GPIO_OLED_SDA);
      display.setCursor(4, 32); display.print("OLED SCL: GPIO"); display.println(WIGO_GPIO_OLED_SCL);
      display.setCursor(4, 44); display.print("Buttons: "); display.print(WIGO_GPIO_BUTTON_UP); display.print("/"); display.print(WIGO_GPIO_BUTTON_OK); display.print("/"); display.println(WIGO_GPIO_BUTTON_DOWN);
      display.setCursor(4, 56); display.print("Battery ADC: GPIO"); display.println(WIGO_GPIO_BATTERY_ADC);
      break;
    default:
      display.setCursor(4, 21); display.println("ESP32-WROOM-32");
      display.setCursor(4, 35); display.println("128x64 SSD1306 OLED");
      display.setCursor(4, 49); display.println("UP/DN: pages  OK: back");
      break;
  }
  display.display();
}

static void render() {
  if (!displayAvailable) return;
  switch (screen) {
    case SCREEN_SPLASH:
      display.clearDisplay();
      display.setTextColor(SSD1306_WHITE);
      display.drawRoundRect(9, 8, 110, 48, 8, SSD1306_WHITE);
      display.drawRoundRect(13, 12, 102, 40, 6, SSD1306_WHITE);
      display.setTextSize(2);
      display.setCursor(35, 19);
      display.print("WIGO");
      display.setTextSize(1);
      display.setCursor(37, 40);
      display.print("GAME CONSOLE");
      display.display();
      break;
    case SCREEN_MAIN_MENU: renderMainMenu(); break;
    case SCREEN_GAMES: renderGames(); break;
    case SCREEN_GAME_MENU: renderBunkerMenu(); break;
    case SCREEN_WIFI: renderWifi(); break;
    case SCREEN_SETTINGS: renderSettings(); break;
    case SCREEN_ABOUT: renderAbout(); break;
  }
}

static void updateButtons(uint32_t now) {
  for (uint8_t i = 0; i < BUTTON_COUNT; ++i) {
    ButtonState& button = buttons[i];
    const bool rawPressed = digitalRead(button.pin) == LOW;
    button.justPressed = false;
    if (rawPressed != button.lastRawPressed) {
      button.lastRawPressed = rawPressed;
      button.rawChangedAt = now;
    }
    if (rawPressed != button.stablePressed && now - button.rawChangedAt >= BUTTON_DEBOUNCE_MS) {
      button.stablePressed = rawPressed;
      button.justPressed = rawPressed;
    }
  }
}

static bool pressed(ButtonId id) { return buttons[id].justPressed; }

static void updateBattery(uint32_t now) {
  if (now - lastBatterySampleAt < BATTERY_SAMPLE_MS) return;
  lastBatterySampleAt = now;

  // Average a short burst to reduce conversion noise. This remains an estimate,
  // especially with a high-impedance divider or an uncalibrated board.
  uint32_t millivoltSum = 0;
  for (uint8_t sample = 0; sample < 8; ++sample)
    millivoltSum += analogReadMilliVolts(WIGO_GPIO_BATTERY_ADC);
  const uint32_t pinMillivolts = millivoltSum / 8;
  const uint8_t previousBars = batteryBars;
  batteryVoltage = (pinMillivolts / 1000.0f) *
                   ((WIGO_BATTERY_R1_OHMS + WIGO_BATTERY_R2_OHMS) / WIGO_BATTERY_R2_OHMS);
  batteryReadingAvailable = pinMillivolts > 0;

  // Li-ion voltage bands are only a rough indication of remaining charge.
  if (batteryVoltage < 3.35f) batteryBars = 1;
  else if (batteryVoltage < 3.60f) batteryBars = 2;
  else if (batteryVoltage < 3.90f) batteryBars = 3;
  else batteryBars = 4;
  if (previousBars != batteryBars) batteryNeedsRedraw = true;
}

static void enterScreen(AppScreen next) {
  screen = next;
  screenEnteredAt = millis();
  if (next == SCREEN_GAME_MENU) startAccessPoint();
  render();
}

static void handleMainMenu() {
  if (pressed(BUTTON_ID_UP)) {
    mainSelection = (mainSelection + MAIN_ITEM_COUNT - 1) % MAIN_ITEM_COUNT;
    renderMainMenu();
  } else if (pressed(BUTTON_ID_DOWN)) {
    mainSelection = (mainSelection + 1) % MAIN_ITEM_COUNT;
    renderMainMenu();
  } else if (pressed(BUTTON_ID_OK)) {
    switch (mainSelection) {
      case 0: gameSelection = 0; enterScreen(SCREEN_GAMES); break;
      case 1: enterScreen(SCREEN_WIFI); break;
      case 2: enterScreen(SCREEN_SETTINGS); break;
      case 3: aboutPage = 0; enterScreen(SCREEN_ABOUT); break;
    }
  }
}

static void handleScreenInput() {
  switch (screen) {
    case SCREEN_MAIN_MENU:
      handleMainMenu();
      break;
    case SCREEN_GAMES:
      if (pressed(BUTTON_ID_UP)) {
        gameSelection = (gameSelection + GAME_ITEM_COUNT - 1) % GAME_ITEM_COUNT;
        renderGames();
      } else if (pressed(BUTTON_ID_DOWN)) {
        gameSelection = (gameSelection + 1) % GAME_ITEM_COUNT;
        renderGames();
      } else if (pressed(BUTTON_ID_OK)) {
        if (!gameRunning) {
          GameKind nextGame = static_cast<GameKind>(gameSelection);
          selectedGame = nextGame;
          gameOutcome = BUNKER_UNFINISHED;
          bunkerRound = bunkerCapacity = bunkerParticipantCount = 0;
          bunkerPhase = BUNKER_DISCUSSION;
          memset(hasVoted, 0, sizeof(hasVoted));
          memset(revealedMask, 0, sizeof(revealedMask));
          resetOtherGame();
          for (uint8_t i = 0; i < MAX_PLAYERS; ++i) { players[i].ready = false; players[i].alive = false; }
          broadcastState();
        }
        enterScreen(SCREEN_GAME_MENU);
      }
      break;
    case SCREEN_GAME_MENU:
      if (pressed(BUTTON_ID_OK)) {
        // Leaving a game on the device must also release its running state so
        // another game can be selected and started without a reboot.
        gameRunning = false;
        gameOutcome = BUNKER_UNFINISHED;
        bunkerRound = bunkerCapacity = bunkerParticipantCount = 0;
        bunkerPhase = BUNKER_DISCUSSION;
        memset(hasVoted, 0, sizeof(hasVoted));
        memset(revealedMask, 0, sizeof(revealedMask));
        resetOtherGame();
        for (uint8_t i = 0; i < MAX_PLAYERS; ++i) {
          players[i].ready = false;
          players[i].alive = false;
        }
        broadcastState();
        enterScreen(SCREEN_MAIN_MENU);
      }
      break;
    case SCREEN_WIFI:
    case SCREEN_SETTINGS:
      if (pressed(BUTTON_ID_OK)) enterScreen(SCREEN_MAIN_MENU);
      break;
    case SCREEN_ABOUT:
      if (pressed(BUTTON_ID_UP)) {
        aboutPage = (aboutPage + 2) % 3;
        renderAbout();
      } else if (pressed(BUTTON_ID_DOWN)) {
        aboutPage = (aboutPage + 1) % 3;
        renderAbout();
      } else if (pressed(BUTTON_ID_OK)) {
        enterScreen(SCREEN_MAIN_MENU);
      }
      break;
    case SCREEN_SPLASH:
      break;
  }
}

static void configureWebServer() {
  httpServer.on("/", HTTP_GET, []() {
    httpServer.sendHeader("Cache-Control", "no-store, no-cache, must-revalidate");
    httpServer.send_P(200, "text/html; charset=utf-8", PLAYER_PAGE);
  });
  httpServer.onNotFound([]() {
    httpServer.send(404, "text/plain", "WIGO page not found");
  });
}

void setup() {
  Serial.begin(115200);
  for (uint8_t i = 0; i < BUTTON_COUNT; ++i) {
    pinMode(buttons[i].pin, INPUT_PULLUP);
  }

  analogReadResolution(12);
  analogSetPinAttenuation(WIGO_GPIO_BATTERY_ADC, ADC_11db);
  Wire.begin(WIGO_GPIO_OLED_SDA, WIGO_GPIO_OLED_SCL);

  displayAvailable = display.begin(SSD1306_SWITCHCAPVCC, WIGO_OLED_I2C_ADDRESS);
  if (displayAvailable) display.setRotation(WIGO_OLED_ROTATION);
  else Serial.println("[OLED] Init failed; continuing without display");
  configureWebServer();
  startAccessPoint();
  const uint32_t now = millis();
  for (uint8_t i = 0; i < BUTTON_COUNT; ++i) {
    const bool initialPressed = digitalRead(buttons[i].pin) == LOW;
    buttons[i].stablePressed = initialPressed;
    buttons[i].lastRawPressed = initialPressed;
    buttons[i].rawChangedAt = now;
  }
  lastBatterySampleAt = now - BATTERY_SAMPLE_MS;
  updateBattery(now);
  screenEnteredAt = now;
  render();
}

void loop() {
  const uint32_t now = millis();
  updateButtons(now);
  updateBattery(now);
  serviceNetwork(now);
  tickOtherGames(now);

  if (screen == SCREEN_SPLASH && now - screenEnteredAt >= SPLASH_DURATION_MS) {
    enterScreen(SCREEN_MAIN_MENU);
  } else {
    handleScreenInput();
  }
  if ((screen == SCREEN_GAME_MENU || screen == SCREEN_WIFI) && now - lastUiRefreshAt >= 1000) {
    lastUiRefreshAt = now;
    render();
  }
  if (batteryNeedsRedraw && screen != SCREEN_SPLASH) {
    batteryNeedsRedraw = false;
    render();
  }
}


