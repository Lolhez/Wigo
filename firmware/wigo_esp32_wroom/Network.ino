// Wi-Fi access point, HTTP routes, WebSocket transport, and network servicing.
#include "Config.h"

static void onWebSocketEvent(uint8_t socketId, WStype_t type, uint8_t* payload, size_t length) {
  if (type == WStype_CONNECTED) {
    Serial.printf("[WS] client %u connected\n", socketId);
    sendText(socketId, String("ROOM|") + roomId);
    const char* game = selectedGame == GAME_CROCODILE ? "crocodile" :
                       selectedGame == GAME_MAFIA ? "mafia" :
                       selectedGame == GAME_ALIAS ? "alias" :
                       selectedGame == GAME_SPY ? "spy" :
                       selectedGame == GAME_WHOAMI ? "whoami" : "bunker";
    sendText(socketId, String("GAME|") + game);
  } else if (type == WStype_TEXT) {
    handlePlayerMessage(socketId, payload, length);
  } else if (type == WStype_DISCONNECTED) {
    Serial.printf("[WS] client %u disconnected\n", socketId);
    int8_t slot = findPlayerBySocket(socketId);
    if (slot >= 0) {
      players[slot].connected = false;
      players[slot].lastSeenAt = millis();
      if (gameRunning && bunkerPhase == BUNKER_REVEAL) advanceRevealQueue();
      broadcastState();
      if (gameRunning && bunkerPhase == BUNKER_VOTING) checkVoteCompletion();
      if (gameRunning && selectedGame != GAME_BUNKER) onOtherGameDisconnect();
    }
  }
}

static void startAccessPoint() {
  if (accessPointStarted) return;
  lastApRetryAt = millis();
  const char alphabet[] = "ABCDEFGHJKLMNPQRSTUVWXYZ23456789";
  for (uint8_t i = 0; i < 4; ++i) roomId[i] = alphabet[esp_random() % (sizeof(alphabet) - 1)];
  roomId[4] = '\0';
  WiFi.mode(WIFI_AP);
  WiFi.setSleep(false);
  WiFi.softAPsetHostname("wigo");
  accessPointStarted = WiFi.softAP((String("WIGO-") + roomId).c_str(), WIGO_AP_PASSWORD, 1, 0, MAX_PLAYERS);
  if (accessPointStarted) {
    Serial.printf("[WiFi] AP WIGO-%s, IP %s\n", roomId, WiFi.softAPIP().toString().c_str());
    httpServer.begin();
    webSocket.begin();
    webSocket.enableHeartbeat(30000, 10000, 3);
    webSocket.onEvent(onWebSocketEvent);
  }
}

static void serviceNetwork(uint32_t now) {
  if (!accessPointStarted) {
    if (now - lastApRetryAt >= 10000) startAccessPoint();
    return;
  }
  httpServer.handleClient();
  webSocket.loop();
  for (uint8_t i = 0; i < MAX_PLAYERS; ++i) {
    if (players[i].name[0] && !players[i].connected && now - players[i].lastSeenAt > RECONNECT_GRACE_MS) {
      memset(&players[i], 0, sizeof(players[i]));
      broadcastState();
    }
  }
}


