// Prompt games, Mafia, Who Is the Spy?, and Who Am I?

static char whoamiCard[MAX_PLAYERS][80];
static char whoamiSuggestions[MAX_PLAYERS][MAX_PLAYERS][80];
static bool whoamiSubmitted[MAX_PLAYERS][MAX_PLAYERS];
static bool whoamiShown[MAX_PLAYERS];
static uint32_t whoamiRevealAt[MAX_PLAYERS];

static const char* const CROCODILE_WORDS[] = {
  "Самолёт", "Пингвин", "Зубная щётка", "Космонавт", "Подводная лодка", "Пылесос", "Дирижёр", "Кенгуру",
  "Пожарный", "Велосипед", "Попкорн", "Робот", "Лыжник", "Черепаха", "Фотограф", "Зонт",
  "Пират", "Стиральная машина", "Краб", "Фокусник", "Поезд", "Балерина", "Будильник", "Жираф",
  "Рыбак", "Скалолаз", "Телефон", "Снеговик", "Гитарист", "Медуза", "Повар", "Лифт",
  "Комар", "Футболист", "Зеркало", "Врач", "Верблюд", "Микроскоп", "Детектив", "Качели",
  "Вулкан", "Сёрфер", "Крокодил", "Молния", "Клоун", "Шахматист", "Ёж", "Трактор",
  "Палатка", "Осьминог", "Кран", "Боксёр", "Скрипка", "Лампочка", "Пчеловод", "Пинг-понг",
  "Лабиринт", "Орёл", "Карусель", "Почтальон", "Мороженое", "Ракета", "Теннисист", "Фламинго",
  "Аквалангист", "Бабочка", "Барабанщик", "Воздушный шар", "Горнолыжник", "Динозавр", "Дровосек", "Енот",
  "Жонглёр", "Замок", "Змея", "Ковбой", "Колесо обозрения", "Ласточка", "Мотоцикл", "Ныряльщик",
  "Обезьяна", "Пианино", "Пиратский корабль", "Роликовые коньки", "Саксофонист", "Светофор", "Слон", "Сёрфинг",
  "Танцор", "Термометр", "Улитка", "Фейерверк", "Хоккеист", "Цирк", "Чайник", "Шеф-повар",
  "Акула", "Аллигатор", "Арбуз", "Астронавт", "Бабушка", "Бегемот", "Бельевая верёвка", "Бинокль",
  "Блин", "Бобр", "Болельщик", "Борода", "Ботинок", "Боулинг", "Бульдозер", "Бутерброд",
  "Ведро", "Вертолёт", "Весы", "Ветер", "Волейбол", "Волшебная палочка", "Ворона", "Восьминогий паук",
  "Гантеля", "Гейзер", "Гиря", "Гладильная доска", "Гном", "Гол", "Гонщик", "Грабли",
  "Градусник", "Грузовик", "Дворник", "Дельфин", "Детская коляска", "Дождь", "Домино", "Дракон",
  "Дыня", "Ёлка", "Единорог", "Езда верхом", "Жвачка", "Завтрак", "Зажигалка",
  "Зубная боль", "Игра в прятки", "Изобретатель", "Иней", "Йог", "Кабан", "Калитка", "Калькулятор",
  "Камин", "Канатоходец", "Капитан", "Каратист", "Карета", "Каска", "Кенгуру с детёнышем", "Кирпич",
  "Кит", "Клавиатура", "Клей", "Ключ", "Ковёр", "Коза", "Козлёнок", "Колокол",
  "Компас", "Коньки", "Корова", "Котёнок", "Кофемашина", "Краска", "Крем для загара",
  "Крокет", "Крокодилья пасть", "Кролик", "Круассан", "Кувалда", "Кукуруза", "Купальник", "Лавина",
  "Ластик", "Ледокол", "Лейка", "Лесоруб", "Лимон", "Лифтёр", "Лодка", "Лошадь",
  "Лук и стрелы", "Лыжная палка", "Магнит", "Майка", "Макароны", "Маляр", "Мандарин", "Марафонец",
  "Медведь", "Мельница", "Микрофон", "Мим", "Мишень", "Молоток", "Морж", "Морская звезда",
  "Мусоровоз", "Мыльные пузыри", "Мяч", "Насос", "Настольная лампа", "Ножницы", "Носорог", "Облако",
  "Одеяло", "Оленёнок", "Оловянный солдатик", "Пальма", "Панда", "Парашют", "Парик", "Паровоз",
  "Парусник", "Пастух", "Пельмени", "Перчатки", "Песочные часы", "Петух", "Пилот", "Пинг-понгист",
  "Пирог", "Плавание", "Платок", "Платье", "Плоскогубцы", "Плотник", "Подушка", "Полицейский",
  "Пончик", "Портной", "Починка велосипеда", "Прачка", "Продавец мороженого", "Прыжок с парашютом", "Пчела", "Пылкая речь",
  "Рак", "Ракушка", "Расчёска", "Резиновая уточка", "Ремень", "Репортёр", "Рисование", "Робот-пылесос",
  "Рогатка", "Родео", "Рысь", "Садовник", "Самокат", "Сапёр", "Сарделька", "Скакалка",
  "Сковородка", "Скейтборд", "Скрип двери", "Слоны в цирке", "Смартфон", "Снегоуборщик", "Снегопад", "Собачья будка",
  "Сокол", "Сосулька", "Спасатель", "Спящий кот", "Статуя", "Степлер", "Страус", "Строитель",
  "Сумасшедший учёный", "Суши", "Табурет", "Тарелка", "Татуировщик", "Тележка", "Тигр", "Топор",
  "Торнадо", "Торт", "Трамвай", "Трубочист", "Турист с рюкзаком", "Тыква", "Уборка", "Удав",
  "Удар молнии", "Утюг", "Фокус с монетой", "Фонарь", "Фотобудка", "Футбольный вратарь", "Хлеб", "Хомяк",
  "Художник", "Цапля", "Цветочный горшок", "Цирковой силач", "Чемодан", "Череп", "Чихание", "Шарф",
  "Шахматная партия", "Швабра", "Швейная машинка", "Шерлок Холмс", "Шимпанзе", "Шинель", "Шоколадка", "Штопор",
  "Щенок", "Экскаватор", "Электрогитара", "Эскалатор", "Юла", "Яблочный пирог", "Яйцо всмятку", "Якорь",
  "Ящерица", "Акробат", "Аплодисменты", "Баскетболист", "Бег на месте", "Бензопила", "Библиотекарь", "Бокал",
  "Боксёрская груша", "Бумажный самолётик", "Водопад", "Ворота", "Гамбургер", "Гитара у костра", "Гончар", "Гусеница",
  "Дед Мороз", "Дирижёр оркестра", "Диско", "Домкрат", "Звездочёт", "Золотая рыбка", "Игра в шахматы", "Камера наблюдения",
  "Каникулы", "Картонная коробка", "Кассовый аппарат", "Качалка", "Ковбойская шляпа", "Кокос", "Кораблекрушение", "Кормление голубей",
  "Косичка", "Кошачий хвост", "Курица несёт яйцо", "Лак для ногтей", "Лампочка перегорела", "Летучая мышь", "Лягушка на кочке", "Морской конёк",
  "Мороженщик", "Муравейник", "Новый год", "Охотник", "Пожарная машина", "Поход в горы", "Почтовый голубь", "Прогулка под дождём",
  "Рыбалка", "Свадебный танец", "Салют", "Скалолазание", "Солнечный ожог", "Соревнование по бегу", "Тайный агент", "Урок музыки",
  "Фабрика конфет", "Фигурное катание", "Чистка зубов", "Шторм в море", "Щекотка", "Электричка", "Ярмарка", "Полёт на воздушном шаре"
};
static const uint16_t CROCODILE_WORD_COUNT = sizeof(CROCODILE_WORDS) / sizeof(CROCODILE_WORDS[0]);
static const uint16_t CROCODILE_NO_WORD = 0xFFFF;
static bool crocodileWordUsed[CROCODILE_WORD_COUNT];
static uint16_t crocodileWordsPlayed = 0;
static uint16_t crocodileScore = 0;
static uint16_t crocodileCurrentWord = CROCODILE_NO_WORD;
static uint32_t crocodileDeadline = 0;
static uint32_t lastCrocodileStateAt = 0;
static uint8_t firstConnectedPlayer();

enum MafiaRole : uint8_t { ROLE_CITIZEN, ROLE_MAFIA };
static MafiaRole mafiaRoles[MAX_PLAYERS];
static bool mafiaParticipant[MAX_PLAYERS];
static uint8_t mafiaPhase = 0; // 1 night, 2 day, 3 voting
static uint8_t mafiaRound = 0;
static char mafiaLastEliminated[25] = "";
static char mafiaLastRole[16] = "";
static char mafiaWinner[8] = "none";

static const char* const SPY_LOCATIONS[] = {
  "Аэропорт", "Больница", "Космическая станция", "Школа", "Пляж", "Ресторан", "Поезд", "Самолёт",
  "Пиратский корабль", "Цирк", "Музей", "Стадион", "Отель", "Полицейский участок", "Супермаркет", "Зоопарк",
  "Киностудия", "Банк", "Подводная лодка", "Горнолыжный курорт", "Пожарная станция", "Библиотека", "Ферма", "Космический корабль",
  "Театр", "Парк аттракционов", "Круизный лайнер", "Университет", "Автосервис", "Пекарня", "Замок", "Кемпинг"
};
static const uint8_t SPY_LOCATION_COUNT = sizeof(SPY_LOCATIONS) / sizeof(SPY_LOCATIONS[0]);
static bool spyParticipant[MAX_PLAYERS];
static uint8_t spyPlayerId = 255;
static uint8_t spyLocationId = 0;
static uint8_t spyPhase = 0; // 1 discussion, 2 voting
static char spyWinner[8] = "none";

static const char* otherGamePhase() {
  if (selectedGame == GAME_CROCODILE) return gameRunning ? "turn" : "lobby";
  if (selectedGame == GAME_MAFIA) return mafiaPhase == 1 ? "night" : (mafiaPhase == 2 ? "day" : (mafiaPhase == 3 ? "voting" : "lobby"));
  if (selectedGame == GAME_ALIAS) return gameRunning ? "turn" : "lobby";
  if (selectedGame == GAME_SPY) return spyPhase == 2 ? "voting" : (spyPhase == 1 ? "discussion" : "lobby");
  if (selectedGame == GAME_WHOAMI) return gameRunning ? "guessing" : "lobby";
  return "discussion";
}

static void appendOtherGameState(String& message) {
  if (selectedGame == GAME_CROCODILE || selectedGame == GAME_ALIAS) {
    const uint32_t now = millis();
    const int32_t remaining = static_cast<int32_t>(crocodileDeadline - now);
    uint32_t left = gameRunning && remaining > 0 ? static_cast<uint32_t>(remaining) : 0;
    message += ",\"crocScore\":"; message += String(crocodileScore);
    message += ",\"crocPlayed\":"; message += String(crocodileWordsPlayed);
    message += ",\"crocTotal\":"; message += String(CROCODILE_WORD_COUNT);
    message += ",\"crocMs\":"; message += String(left);
    if (selectedGame == GAME_ALIAS) {
      message += ",\"aliasScore\":"; message += String(crocodileScore);
      message += ",\"aliasPlayed\":"; message += String(crocodileWordsPlayed);
      message += ",\"aliasTotal\":"; message += String(CROCODILE_WORD_COUNT);
      message += ",\"aliasMs\":"; message += String(left);
    }
  } else if (selectedGame == GAME_MAFIA) {
    message += ",\"mafiaRound\":"; message += String(mafiaRound);
    message += ",\"mafiaWinner\":\""; message += mafiaWinner; message += '"';
    message += ",\"mafiaLastEliminated\":\""; message += mafiaLastEliminated; message += '"';
    message += ",\"mafiaLastRole\":\""; message += mafiaLastRole; message += '"';
  } else if (selectedGame == GAME_SPY) {
    message += ",\"spyWinner\":\""; message += spyWinner; message += '"';
  }
}

static int16_t nextPromptWord() {
  uint16_t remaining = 0;
  for (uint16_t i = 0; i < CROCODILE_WORD_COUNT; ++i)
    if (!crocodileWordUsed[i]) ++remaining;
  if (!remaining) return -1;

  uint16_t choice = esp_random() % remaining;
  uint16_t selected = CROCODILE_NO_WORD;
  for (uint16_t i = 0; i < CROCODILE_WORD_COUNT; ++i) {
    if (crocodileWordUsed[i]) continue;
    if (choice == 0) { selected = i; break; }
    --choice;
  }
  if (selected == CROCODILE_NO_WORD) return -1;
  crocodileWordUsed[selected] = true;
  crocodileCurrentWord = selected;
  ++crocodileWordsPlayed;
  crocodileDeadline = millis() + (selectedGame == GAME_ALIAS ? 60000UL : 30000UL);
  uint8_t actor = firstConnectedPlayer();
  if (actor < MAX_PLAYERS) sendText(players[actor].socketId, String(selectedGame == GAME_ALIAS ? "AWORD|" : "WORD|") + CROCODILE_WORDS[selected]);
  return selected;
}

static bool allConnectedReady() {
  if (!connectedPlayerCount()) return false;
  for (uint8_t i = 0; i < MAX_PLAYERS; ++i) if (players[i].connected && !players[i].ready) return false;
  return true;
}

static uint8_t firstConnectedPlayer() {
  for (uint8_t i = 0; i < MAX_PLAYERS; ++i) if (players[i].connected) return i;
  return 255;
}

static void finishPromptGame() {
  gameRunning = false; gameOutcome = BUNKER_VICTORY; crocodileDeadline = 0;
  const char* name = selectedGame == GAME_ALIAS ? "Алиас" : "Крокодил";
  webSocket.broadcastTXT((String("RESULT|") + name + " завершён. Угадано: " + crocodileScore + " из " + crocodileWordsPlayed + ".").c_str());
  broadcastState();
}

static void issueMafiaOptions(uint8_t playerId) {
  String message = "ACTION|VOTE|";
  bool first = true;
  for (uint8_t i = 0; i < MAX_PLAYERS; ++i) {
    if (!players[i].connected || !players[i].alive || i == playerId) continue;
    if (!first) message += ',';
    first = false;
    message += String(i) + ':' + players[i].name;
  }
  sendText(players[playerId].socketId, message);
}

static void sendMafiaRole(uint8_t socketId, uint8_t playerId) {
  const char* role = mafiaRoles[playerId] == ROLE_MAFIA ? "Мафия" : "Мирный житель";
  sendText(socketId, String("ROLE|") + role);
}

static void sendMafiaHostCards(uint8_t socketId) {
  String message = "HOST_ROLES|";
  bool first = true;
  for (uint8_t i = 0; i < MAX_PLAYERS; ++i) if (mafiaParticipant[i] && players[i].name[0]) {
    if (!first) message += ',';
    first = false;
    message += String(i) + ':' + players[i].name + ':' + (mafiaRoles[i] == ROLE_MAFIA ? "Мафия" : "Мирный житель");
  }
  sendText(socketId, message);
}

static void sendSpyRole(uint8_t socketId, uint8_t playerId) {
  if (!spyParticipant[playerId]) return;
  if (playerId == spyPlayerId) sendText(socketId, "SPY_ROLE|spy");
  else sendText(socketId, String("SPY_ROLE|citizen|") + SPY_LOCATIONS[spyLocationId]);
}

static void sendSpyOptions(uint8_t playerId) {
  String message = "SPY_OPTIONS|";
  bool first = true;
  for (uint8_t i = 0; i < MAX_PLAYERS; ++i) {
    if (!players[i].connected || !players[i].alive || i == playerId) continue;
    if (!first) message += ',';
    first = false;
    message += String(i) + ':' + players[i].name;
  }
  sendText(players[playerId].socketId, message);
}

static void finishSpyVote() {
  if (!gameRunning || selectedGame != GAME_SPY || spyPhase != 2) return;
  for (uint8_t i = 0; i < MAX_PLAYERS; ++i)
    if (players[i].alive && players[i].connected && spyParticipant[i] && !hasVoted[i]) return;

  uint8_t totals[MAX_PLAYERS] = {};
  for (uint8_t i = 0; i < MAX_PLAYERS; ++i)
    if (hasVoted[i] && voteTarget[i] < MAX_PLAYERS) ++totals[voteTarget[i]];
  uint8_t target = 255, highest = 0;
  bool tie = false;
  for (uint8_t i = 0; i < MAX_PLAYERS; ++i) {
    if (!players[i].alive || !totals[i]) continue;
    if (totals[i] > highest) { target = i; highest = totals[i]; tie = false; }
    else if (totals[i] == highest) tie = true;
  }
  if (tie || target == 255) {
    spyPhase = 1;
    memset(hasVoted, 0, sizeof(hasVoted));
    webSocket.broadcastTXT("RESULT|Голоса разделились. Обсудите ещё и запустите голосование снова.");
    broadcastState();
    return;
  }

  players[target].alive = false;
  if (target == spyPlayerId) {
    strcpy(spyWinner, "town");
    webSocket.broadcastTXT((String("RESULT|") + players[target].name + " оказался шпионом. Победили остальные игроки!").c_str());
  } else {
    strcpy(spyWinner, "spy");
    webSocket.broadcastTXT((String("RESULT|") + players[target].name + " не был шпионом. Шпион победил!").c_str());
  }
  gameRunning = false;
  gameOutcome = BUNKER_VICTORY;
  broadcastState();
}

static void mafiaCheckWinner() {
  uint8_t mafia = 0, town = 0;
  for (uint8_t i = 0; i < MAX_PLAYERS; ++i) if (players[i].alive && players[i].name[0]) {
    if (mafiaRoles[i] == ROLE_MAFIA) ++mafia; else ++town;
  }
  if (!mafia) { strcpy(mafiaWinner, "town"); gameRunning = false; gameOutcome = BUNKER_VICTORY; webSocket.broadcastTXT("RESULT|Мафия разоблачена. Победили мирные жители!"); }
  else if (mafia >= town) { strcpy(mafiaWinner, "mafia"); gameRunning = false; gameOutcome = BUNKER_VICTORY; webSocket.broadcastTXT("RESULT|Мафия сравнялась числом с мирными жителями. Победила мафия!"); }
}

static void finishMafiaVote() {
  if (!gameRunning || mafiaPhase != 3) return;
  uint8_t waiting = 0;
  for (uint8_t i = 0; i < MAX_PLAYERS; ++i) if (players[i].alive && players[i].connected && !hasVoted[i]) ++waiting;
  if (waiting) return;
  uint8_t totals[MAX_PLAYERS] = {};
  for (uint8_t i = 0; i < MAX_PLAYERS; ++i) if (hasVoted[i] && voteTarget[i] < MAX_PLAYERS) ++totals[voteTarget[i]];
  uint8_t target = 255, maxVotes = 0; bool tie = false;
  for (uint8_t i = 0; i < MAX_PLAYERS; ++i) if (players[i].alive && totals[i]) {
    if (totals[i] > maxVotes) { maxVotes = totals[i]; target = i; tie = false; }
    else if (totals[i] == maxVotes) tie = true;
  }
  if (!tie && target < MAX_PLAYERS) {
    players[target].alive = false;
    strncpy(mafiaLastEliminated, players[target].name, sizeof(mafiaLastEliminated) - 1);
    strcpy(mafiaLastRole, mafiaRoles[target] == ROLE_MAFIA ? "Мафия" : "Мирный житель");
    webSocket.broadcastTXT((String("RESULT|") + players[target].name + " выбыл. Роль раскрыта: " + mafiaLastRole + ".").c_str());
    int8_t host = currentHostPlayer();
    if (host >= 0) sendText(players[host].socketId, String("HOST|Напоминание ведущему: ") + mafiaLastEliminated + " выбыл. Его роль — " + mafiaLastRole + ". Отметь выбывшего и сообщи об этом игрокам.");
  } else webSocket.broadcastTXT("RESULT|Голоса разделились. Никто не выбыл.");
  mafiaCheckWinner();
  if (gameRunning) { mafiaPhase = 2; memset(hasVoted, 0, sizeof(hasVoted)); }
  broadcastState();
}

static bool startOtherGame(uint8_t starterSocket) {
  const uint8_t count = connectedPlayerCount();
  const bool soloPromptGame = selectedGame == GAME_CROCODILE || selectedGame == GAME_ALIAS;
  const uint8_t minimum = soloPromptGame ? 1 : (selectedGame == GAME_WHOAMI ? 2 : 3);
  if (count < minimum) {
    const char* error = soloPromptGame ? "ERROR|Подключи один телефон для игры" : (selectedGame == GAME_WHOAMI ? "ERROR|Для игры «Кто я?» нужно минимум 2 игрока" : "ERROR|Для этой игры нужно минимум 3 игрока");
    sendText(starterSocket, error);
    return true;
  }
  if (soloPromptGame && count != 1) {
    sendText(starterSocket, selectedGame == GAME_ALIAS ? "ERROR|Алиас играется по очереди на одном телефоне" : "ERROR|Крокодил рассчитан на одного игрока у телефона");
    return true;
  }
  if (!allConnectedReady()) {
    sendText(starterSocket, "ERROR|Сначала нажмите «Готов» на каждом подключённом телефоне");
    return true;
  }
  if (selectedGame == GAME_WHOAMI) {
    for (uint8_t author = 0; author < MAX_PLAYERS; ++author) if (players[author].connected)
      for (uint8_t target = 0; target < MAX_PLAYERS; ++target) if (players[target].connected && target != author && !whoamiSubmitted[author][target]) {
        sendText(starterSocket, "ERROR|Каждый игрок должен придумать персонажа для всех участников");
        return true;
      }
  }
  gameRunning = true; gameOutcome = BUNKER_UNFINISHED; bunkerRound = 1;
  for (uint8_t i = 0; i < MAX_PLAYERS; ++i) players[i].alive = players[i].connected;
  if (soloPromptGame) {
    memset(crocodileWordUsed, 0, sizeof(crocodileWordUsed)); crocodileWordsPlayed = crocodileScore = 0;
    nextPromptWord(); lastCrocodileStateAt = millis();
    if (selectedGame == GAME_ALIAS) webSocket.broadcastTXT("RESULT|Передавайте телефон объясняющему. Объясняй слово словами за 60 секунд!");
    else webSocket.broadcastTXT("RESULT|Показывай слово людям рядом. У них 30 секунд, чтобы угадать!");
  } else if (selectedGame == GAME_MAFIA) {
    memset(hasVoted, 0, sizeof(hasVoted)); memset(mafiaRoles, ROLE_CITIZEN, sizeof(mafiaRoles)); memset(mafiaParticipant, 0, sizeof(mafiaParticipant)); strcpy(mafiaWinner, "none");
    mafiaLastEliminated[0] = '\0'; mafiaLastRole[0] = '\0'; mafiaRound = 1; mafiaPhase = 1;
    uint8_t ids[MAX_PLAYERS], countPlayers = 0;
    for (uint8_t i = 0; i < MAX_PLAYERS; ++i) if (players[i].alive) { ids[countPlayers++] = i; mafiaParticipant[i] = true; }
    for (int16_t i = countPlayers - 1; i > 0; --i) { uint8_t j = esp_random() % (i + 1); uint8_t t = ids[i]; ids[i] = ids[j]; ids[j] = t; }
    mafiaRoles[ids[0]] = ROLE_MAFIA;
    for (uint8_t i = 0; i < countPlayers; ++i) sendMafiaRole(players[ids[i]].socketId, ids[i]);
    int8_t host = currentHostPlayer();
    if (host >= 0) sendMafiaHostCards(players[host].socketId);
    webSocket.broadcastTXT("RESULT|Роли розданы. Обсудите подозрения; ведущий может открыть голосование.");
    broadcastState();
  } else if (selectedGame == GAME_SPY) {
    memset(hasVoted, 0, sizeof(hasVoted));
    memset(spyParticipant, 0, sizeof(spyParticipant));
    spyPlayerId = 255;
    spyLocationId = esp_random() % SPY_LOCATION_COUNT;
    strcpy(spyWinner, "none");
    spyPhase = 1;
    uint8_t active[MAX_PLAYERS], activeCount = 0;
    for (uint8_t i = 0; i < MAX_PLAYERS; ++i) {
      if (!players[i].connected) continue;
      players[i].alive = true;
      spyParticipant[i] = true;
      active[activeCount++] = i;
    }
    spyPlayerId = active[esp_random() % activeCount];
    for (uint8_t i = 0; i < activeCount; ++i) sendSpyRole(players[active[i]].socketId, active[i]);
    webSocket.broadcastTXT("RESULT|У всех одно тайное место, кроме шпиона. Задавайте вопросы и обсуждайте подозрения.");
    broadcastState();
  } else if (selectedGame == GAME_WHOAMI) {
    memset(whoamiShown, 0, sizeof(whoamiShown));
    memset(whoamiRevealAt, 0, sizeof(whoamiRevealAt));
    for (uint8_t i = 0; i < MAX_PLAYERS; ++i) if (players[i].connected) {
      uint8_t authors[MAX_PLAYERS], authorCount = 0;
      for (uint8_t author = 0; author < MAX_PLAYERS; ++author)
        if (players[author].connected && author != i && whoamiSubmitted[author][i]) authors[authorCount++] = author;
      for (int16_t j = authorCount - 1; j > 0; --j) {
        uint8_t k = esp_random() % (j + 1);
        uint8_t tmp = authors[j]; authors[j] = authors[k]; authors[k] = tmp;
      }
      if (authorCount) strncpy(whoamiCard[i], whoamiSuggestions[authors[0]][i], sizeof(whoamiCard[i]) - 1);
      whoamiCard[i][sizeof(whoamiCard[i]) - 1] = '\0';
    }
    webSocket.broadcastTXT("RESULT|Каждому достался персонаж, предложенный другим игроком. Нажми кнопку, переверни телефон экраном наружу и поднеси ко лбу.");
    broadcastState();
  }
  broadcastState();
  return true;
}

static void sendOtherGamePrivate(uint8_t socketId, uint8_t playerId) {
  if (selectedGame == GAME_CROCODILE || selectedGame == GAME_ALIAS) {
    if (gameRunning && playerId == firstConnectedPlayer() && crocodileCurrentWord < CROCODILE_WORD_COUNT)
      sendText(socketId, String(selectedGame == GAME_ALIAS ? "AWORD|" : "WORD|") + CROCODILE_WORDS[crocodileCurrentWord]);
  } else if (selectedGame == GAME_MAFIA) {
    sendMafiaRole(socketId, playerId);
    if (playerId == currentHostPlayer()) {
      sendMafiaHostCards(socketId);
      if (mafiaLastEliminated[0]) sendText(socketId, String("HOST|Напоминание ведущему: ") + mafiaLastEliminated + " выбыл. Его роль — " + mafiaLastRole + ".");
    }
    if (gameRunning && mafiaPhase == 3 && players[playerId].alive && !hasVoted[playerId]) issueMafiaOptions(playerId);
  } else if (selectedGame == GAME_SPY && gameRunning) {
    sendSpyRole(socketId, playerId);
    if (spyPhase == 2 && players[playerId].alive && !hasVoted[playerId]) sendSpyOptions(playerId);
  } else if (selectedGame == GAME_WHOAMI && gameRunning && whoamiCard[playerId][0]) {
    if (whoamiShown[playerId]) sendText(socketId, String("WHO_WORD|") + whoamiCard[playerId]);
    else if (whoamiRevealAt[playerId]) {
      int32_t remaining = static_cast<int32_t>(whoamiRevealAt[playerId] - millis());
      sendText(socketId, String("WHO_COUNTDOWN|") + String(remaining > 0 ? remaining : 0));
    }
  }
}

static bool handleOtherGameCommand(uint8_t socketId, int8_t playerId, const char* message) {
  if (playerId < 0) return false;
  if (selectedGame == GAME_WHOAMI && !gameRunning && strncmp(message, "WHO_CARD:", 9) == 0) {
    const char* targetText = message + 9;
    char* end = nullptr;
    const long targetValue = strtol(targetText, &end, 10);
    if (end == targetText || *end != ':' || targetValue < 0 || targetValue >= MAX_PLAYERS || targetValue == playerId || !players[targetValue].connected) return true;
    char card[80] = {};
    const char* src = end + 1;
    size_t out = 0;
    while (*src && out < sizeof(card) - 1) {
      unsigned char c = static_cast<unsigned char>(*src++);
      if (c < 32 || c == '|' || c == ':' || c == '\\') continue;
      card[out++] = static_cast<char>(c);
    }
    while (out && card[out - 1] == ' ') card[--out] = '\0';
    if (out) {
      strncpy(whoamiSuggestions[playerId][targetValue], card, sizeof(whoamiSuggestions[playerId][targetValue]) - 1);
      whoamiSuggestions[playerId][targetValue][sizeof(whoamiSuggestions[playerId][targetValue]) - 1] = '\0';
      whoamiSubmitted[playerId][targetValue] = true;
      broadcastState();
    }
    return true;
  }
  if (selectedGame == GAME_WHOAMI && gameRunning && whoamiCard[playerId][0]) {
    if (strcmp(message, "WHO_START") == 0 && !whoamiShown[playerId] && !whoamiRevealAt[playerId]) {
      whoamiRevealAt[playerId] = millis() + 5000UL;
      sendText(socketId, "WHO_COUNTDOWN|5000");
      return true;
    }
  }
  if ((selectedGame == GAME_CROCODILE || selectedGame == GAME_ALIAS) && gameRunning && playerId == firstConnectedPlayer()) {
    const char* got = selectedGame == GAME_ALIAS ? "ALIAS:GOT" : "CROCO:GOT";
    const char* pass = selectedGame == GAME_ALIAS ? "ALIAS:PASS" : "CROCO:PASS";
    const char* end = selectedGame == GAME_ALIAS ? "ALIAS:END" : "CROCO:END";
    if (strcmp(message, end) == 0) { finishPromptGame(); return true; }
    if (strcmp(message, got) == 0) { ++crocodileScore; webSocket.broadcastTXT("RESULT|Слово угадано!"); if (nextPromptWord() < 0) finishPromptGame(); else broadcastState(); lastCrocodileStateAt = millis(); return true; }
    if (strcmp(message, pass) == 0) { webSocket.broadcastTXT("RESULT|Слово пропущено."); if (nextPromptWord() < 0) finishPromptGame(); else broadcastState(); lastCrocodileStateAt = millis(); return true; }
  }
  if (selectedGame == GAME_MAFIA && gameRunning && playerId == currentHostPlayer()) {
    if (strcmp(message, "M_DAY") == 0 && mafiaPhase == 1) {
      mafiaPhase = 2; webSocket.broadcastTXT("RESULT|Ведущий объявил день."); broadcastState(); return true;
    }
    if (strcmp(message, "M_NIGHT") == 0 && mafiaPhase == 2) {
      mafiaPhase = 1; ++mafiaRound; webSocket.broadcastTXT("RESULT|Ведущий объявил ночь."); broadcastState(); return true;
    }
    if (strcmp(message, "M_VOTE_START") == 0 && mafiaPhase == 2) {
      mafiaPhase = 3; memset(hasVoted, 0, sizeof(hasVoted));
      for (uint8_t i = 0; i < MAX_PLAYERS; ++i) if (players[i].alive && players[i].connected) issueMafiaOptions(i);
      broadcastState(); return true;
    }
  }
  if (selectedGame == GAME_MAFIA && gameRunning && players[playerId].alive && strncmp(message, "M_VOTE:", 7) == 0 && mafiaPhase == 3 && !hasVoted[playerId]) {
    const int target = atoi(message + 7);
    if (target < 0 || target >= MAX_PLAYERS || target == playerId || !players[target].alive || !players[target].connected) { sendText(socketId, "ERROR|Выбери другого живого игрока"); return true; }
    voteTarget[playerId] = target; hasVoted[playerId] = true; sendText(socketId, "RESULT|Голос учтён."); finishMafiaVote(); return true;
  }
  if (selectedGame == GAME_SPY && gameRunning && playerId == currentHostPlayer() && strcmp(message, "SPY_VOTE_START") == 0 && spyPhase == 1) {
    spyPhase = 2;
    memset(hasVoted, 0, sizeof(hasVoted));
    for (uint8_t i = 0; i < MAX_PLAYERS; ++i)
      if (players[i].connected && players[i].alive && spyParticipant[i]) sendSpyOptions(i);
    broadcastState();
    return true;
  }
  if (selectedGame == GAME_SPY && gameRunning && spyPhase == 2 && spyParticipant[playerId] && players[playerId].alive && !hasVoted[playerId] && strncmp(message, "SPY_VOTE:", 9) == 0) {
    const int target = atoi(message + 9);
    if (target < 0 || target >= MAX_PLAYERS || target == playerId || !spyParticipant[target] || !players[target].alive || !players[target].connected) {
      sendText(socketId, "ERROR|Выбери другого участника");
      return true;
    }
    voteTarget[playerId] = target;
    hasVoted[playerId] = true;
    sendText(socketId, "RESULT|Голос учтён.");
    finishSpyVote();
    return true;
  }
  return false;
}

static void tickOtherGames(uint32_t now) {
  if (selectedGame == GAME_WHOAMI && gameRunning) {
    bool changed = false;
    for (uint8_t i = 0; i < MAX_PLAYERS; ++i) {
      if (whoamiRevealAt[i] && static_cast<int32_t>(now - whoamiRevealAt[i]) >= 0) {
        whoamiRevealAt[i] = 0; whoamiShown[i] = true;
        if (players[i].connected) sendText(players[i].socketId, String("WHO_WORD|") + whoamiCard[i]);
        changed = true;
      }
    }
    if (changed) broadcastState();
  }
  if ((selectedGame == GAME_CROCODILE || selectedGame == GAME_ALIAS) && gameRunning && connectedPlayerCount() && static_cast<int32_t>(now - crocodileDeadline) >= 0) {
    webSocket.broadcastTXT(selectedGame == GAME_ALIAS ? "RESULT|Время объяснять вышло." : "RESULT|Время вышло.");
    if (nextPromptWord() < 0) finishPromptGame();
    else { broadcastState(); lastCrocodileStateAt = now; }
  }
  if ((selectedGame == GAME_CROCODILE || selectedGame == GAME_ALIAS) && gameRunning && connectedPlayerCount() && now - lastCrocodileStateAt >= 1000) { lastCrocodileStateAt = now; broadcastState(); }
  if (selectedGame == GAME_MAFIA && gameRunning && connectedPlayerCount() && mafiaPhase == 3) finishMafiaVote();
  if (selectedGame == GAME_SPY && gameRunning && spyPhase == 2) finishSpyVote();
}

static void onOtherGameDisconnect() {
  if (!connectedPlayerCount()) return;
  if (selectedGame == GAME_MAFIA) {
    if (mafiaPhase == 3) finishMafiaVote();
    int8_t host = currentHostPlayer();
    if (host >= 0 && gameRunning) {
      sendMafiaHostCards(players[host].socketId);
      if (mafiaLastEliminated[0]) sendText(players[host].socketId, String("HOST|Напоминание ведущему: ") + mafiaLastEliminated + " выбыл. Его роль — " + mafiaLastRole + ".");
    }
  } else if (selectedGame == GAME_SPY && spyPhase == 2) {
    finishSpyVote();
    if (gameRunning && spyPhase == 2) {
      for (uint8_t i = 0; i < MAX_PLAYERS; ++i)
        if (players[i].connected && players[i].alive && spyParticipant[i] && !hasVoted[i]) sendSpyOptions(i);
    }
  }
}

static void resetOtherGame() {
  crocodileWordsPlayed = crocodileScore = 0; crocodileCurrentWord = CROCODILE_NO_WORD; crocodileDeadline = 0;
  memset(crocodileWordUsed, 0, sizeof(crocodileWordUsed)); memset(mafiaRoles, ROLE_CITIZEN, sizeof(mafiaRoles)); memset(mafiaParticipant, 0, sizeof(mafiaParticipant));
  mafiaPhase = mafiaRound = 0; mafiaLastEliminated[0] = '\0'; mafiaLastRole[0] = '\0'; strcpy(mafiaWinner, "none");
  memset(spyParticipant, 0, sizeof(spyParticipant)); spyPlayerId = 255; spyLocationId = 0; spyPhase = 0; strcpy(spyWinner, "none");
  memset(whoamiCard, 0, sizeof(whoamiCard)); memset(whoamiSuggestions, 0, sizeof(whoamiSuggestions)); memset(whoamiSubmitted, 0, sizeof(whoamiSubmitted)); memset(whoamiShown, 0, sizeof(whoamiShown)); memset(whoamiRevealAt, 0, sizeof(whoamiRevealAt));
}
