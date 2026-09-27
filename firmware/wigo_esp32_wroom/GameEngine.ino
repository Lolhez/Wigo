// Bunker player state, action validation, lobby state, and vote resolution.

struct BunkerScenario {
  const char* title;
  const char* description;
  uint8_t capacityQuarter;
};

static const BunkerScenario SCENARIOS[] = {
  {"Пепельная зима", "Извержение закрыло небо пеплом. Урожай погибнет, холод усилится, помощь придёт не раньше чем через год.", 2},
  {"Большая вода", "После обрушения дамб города затоплены. Запасы пресной воды заражены, уровень воды растёт каждую неделю.", 2},
  {"Тихая эпидемия", "Новый вирус быстро распространяется. Бункер герметичен, но медицинские запасы ограничены.", 2},
  {"Красное небо", "Крупный астероид вызвал пожары и пылевую зиму. Выход наружу опасен, связь с внешним миром пропала.", 1},
  {"Солнечный шторм", "Сильная вспышка вывела из строя электросети и спутники. Запас энергии бункера рассчитан на несколько месяцев.", 3},
  {"Последний реактор", "Авария загрязнила регион. Фильтры бункера работают, но один из контуров очистки повреждён.", 2},
  {"Зелёный смог", "Токсичный смог накрыл мегаполис. Вентиляция бункера исправна, но фильтры скоро потребуют замены.", 2},
  {"Сдвиг полюсов", "Экстремальные штормы и морозы разрушили дороги и связь. Спасатели смогут добраться лишь через долгий срок.", 3}
};

static const char* const PROFESSIONS[] = {
  "Врач-терапевт · стаж 12 лет", "Хирург · стаж 8 лет", "Инженер-энергетик · стаж 15 лет", "Электрик · стаж 6 лет",
  "Агроном · стаж 11 лет", "Учитель химии · стаж 18 лет", "Пилот · стаж 9 лет", "Автомеханик · стаж 7 лет",
  "Микробиолог · стаж 5 лет", "Повар · стаж 14 лет", "Фельдшер · стаж 4 года", "Геолог · стаж 20 лет",
  "Ветеринар · стаж 10 лет", "Пожарный · стаж 13 лет", "Радиоинженер · стаж 3 года", "Строитель · стаж 16 лет",
  "Стоматолог · стаж 6 лет", "Медсестра · стаж 17 лет", "Химик-лаборант · стаж 9 лет", "Биотехнолог · стаж 4 года",
  "Слесарь-механик · стаж 22 года", "Сварщик · стаж 12 лет", "Архитектор · стаж 7 лет", "Лесничий · стаж 19 лет",
  "Радиоведущий · стаж 5 лет", "Водитель грузовика · стаж 24 года", "Эпидемиолог · стаж 11 лет", "Психолог · стаж 8 лет",
  "Спасатель · стаж 6 лет", "Метеоролог · стаж 14 лет", "Кондитер · стаж 3 года", "Программист систем · стаж 10 лет"
};
static const char* const AGE_GENDER[] = {
  "Мужчина, 19 лет", "Женщина, 22 года", "Мужчина, 27 лет", "Женщина, 31 год",
  "Мужчина, 35 лет", "Женщина, 38 лет", "Мужчина, 42 года", "Женщина, 45 лет",
  "Мужчина, 49 лет", "Женщина, 53 года", "Мужчина, 57 лет", "Женщина, 61 год",
  "Мужчина, 66 лет", "Женщина, 70 лет", "Мужчина, 74 года", "Женщина, 29 лет",
  "Мужчина, 18 лет", "Женщина, 25 лет", "Мужчина, 30 лет", "Женщина, 34 года",
  "Мужчина, 39 лет", "Женщина, 41 год", "Мужчина, 47 лет", "Женщина, 50 лет",
  "Мужчина, 55 лет", "Женщина, 59 лет", "Мужчина, 63 года", "Женщина, 67 лет",
  "Мужчина, 72 года", "Женщина, 76 лет", "Мужчина, 23 года", "Женщина, 18 лет"
};
static const char* const HEALTH[] = {
  "Полностью здоров", "Старая травма колена", "Астма", "Отличное здоровье", "Плохое зрение",
  "Аллергия на пыльцу", "Сильный иммунитет", "Бессонница", "Диабет 2-го типа", "Сломана рука",
  "Высокая выносливость", "Мигрени", "Нужны очки", "Быстрое восстановление", "Слабая спина", "Здоров, но истощён",
  "Хронический бронхит", "Непереносимость лактозы", "Повышенное давление", "Заживший перелом руки",
  "Проблемы со слухом", "Сильная близорукость", "Частые простуды", "Непереносимость холода",
  "Непереносимость жары", "Здоров, но есть шрам", "Склонность к аллергии", "Слабый иммунитет",
  "Быстро устаёт", "Хорошая координация", "Чувствительные зубы", "Здоров, нужна диета"
};
static const char* const HOBBIES[] = {
  "Первая помощь", "Рыбалка", "Ремонт техники", "Садоводство", "Кулинария", "Ориентирование",
  "Радиолюбительство", "Шахматы", "Стрельба из лука", "Шитьё", "Скалолазание", "Чтение карт",
  "Разведение животных", "Плавание", "Иностранные языки", "Наблюдение за погодой",
  "Столярное дело", "Вязание", "Первая помощь животным", "Сбор грибов",
  "Консервация продуктов", "Резьба по дереву", "Бег на длинные дистанции", "Фотография",
  "Уход за растениями", "Радиосвязь", "Кузнечное дело", "Поиск воды",
  "Охота и следопытство", "Домашнее сыроварение", "Ремонт одежды", "Тактические игры"
};
static const char* const PHOBIAS[] = {
  "Темнота", "Замкнутые пространства", "Высота", "Вода", "Насекомые", "Кровь", "Одиночество", "Огонь",
  "Громкие звуки", "Толпа", "Грызуны", "Глубина", "Болезни", "Потеря контроля", "Гроза", "Нет выраженной фобии",
  "Собаки", "Пауки", "Иглы", "Пожары", "Грязь и антисанитария", "Мёртвые тела", "Голод", "Заражение",
  "Остаться без связи", "Быстрое движение", "Лифты", "Глубокий снег", "Птицы", "Гром", "Быть отвергнутым", "Тишина и темнота"
};
static const char* const BAGGAGE[] = {
  "Аптечка", "Набор инструментов", "Фильтр для воды", "Подробная карта", "Рация с запасной батареей", "Пакет семян",
  "Прочная верёвка", "Ручной фонарь", "Запас консервов", "Солнечная панель", "Книга по медицине", "Охотничий нож",
  "Рюкзак с тёплой одеждой", "Набор для ремонта обуви", "Семейная фотография", "Пустые канистры",
  "Портативная горелка", "Запас батареек", "Тёплый спальник", "Складная пила",
  "Ручной фильтр воздуха", "Набор для шитья", "Крепкий термос", "Компас",
  "Сухой паёк на сутки", "Мультитул", "Защитные очки", "Респиратор",
  "Катушка прочной лески", "Книга по агрономии", "Набор посуды", "Запас мыла"
};
static const char* const FACTS[] = {
  "Знает азбуку Морзе", "Когда-то жил в деревне", "Умеет чинить насосы", "Пережил сильный шторм",
  "Знает основы ботаники", "Быстро учится", "Может надолго задерживать дыхание", "Помнит план старого метро",
  "Умеет вести переговоры", "Никогда не был за городом", "Отлично запоминает лица", "Боится открытого огня",
  "Знает основы электрики", "Много лет работал в команде", "Умеет экономить припасы", "Скрывает важную тайну",
  "Знает съедобные дикорастущие растения", "Умеет читать топографические карты", "Работал в экстремальном климате", "Знает основы первой помощи",
  "Умеет быстро разжечь огонь", "Помнит много рецептов", "Раньше жил без электричества", "Умеет обращаться с инструментами",
  "Умеет находить общий язык с детьми", "Знает несколько способов очистки воды", "Хорошо ориентируется по звёздам", "Умеет чинить одежду",
  "Однажды спас человека", "Никогда не держал домашних животных", "Склонен брать ответственность", "Имеет полезные связи за городом"
};
static const char* const SPECIALS[] = {
  "Сохраняет спокойствие в кризисе", "Умеет работать под давлением", "Обладает лидерскими качествами",
  "Внимателен к мелочам", "Легко находит общий язык", "Не теряет надежду", "Очень наблюдателен",
  "Быстро принимает решения", "Умеет поддержать команду", "Предпочитает всё планировать",
  "Силен духом", "Не имеет заметной особенности",
  "Недоверчив к незнакомцам", "Всегда ищет компромисс", "Предпочитает действовать в одиночку", "Отлично мотивирует других",
  "Склонен к риску", "Не любит перемены", "Способен признать ошибку", "Скрытен и немногословен",
  "Очень терпелив", "Импульсивен в споре", "Ставит общее выше личного", "Держит слово",
  "Часто сомневается в себе", "Умеет быстро успокоить конфликт", "Любит брать инициативу", "Необычно оптимистичен",
  "С уважением относится к чужому опыту", "Предпочитает сначала слушать", "Сохраняет чувство юмора", "Сильно переживает за близких"
};

static const uint8_t SCENARIO_COUNT = sizeof(SCENARIOS) / sizeof(SCENARIOS[0]);
static const uint8_t PROFESSION_COUNT = sizeof(PROFESSIONS) / sizeof(PROFESSIONS[0]);
static const uint8_t AGE_GENDER_COUNT = sizeof(AGE_GENDER) / sizeof(AGE_GENDER[0]);
static const uint8_t HEALTH_COUNT = sizeof(HEALTH) / sizeof(HEALTH[0]);
static const uint8_t HOBBY_COUNT = sizeof(HOBBIES) / sizeof(HOBBIES[0]);
static const uint8_t PHOBIA_COUNT = sizeof(PHOBIAS) / sizeof(PHOBIAS[0]);
static const uint8_t BAGGAGE_COUNT = sizeof(BAGGAGE) / sizeof(BAGGAGE[0]);
static const uint8_t FACT_COUNT = sizeof(FACTS) / sizeof(FACTS[0]);
static const uint8_t SPECIAL_COUNT = sizeof(SPECIALS) / sizeof(SPECIALS[0]);

static uint8_t revealOrder[MAX_PLAYERS];
static uint8_t revealOrderCount = 0;
static uint8_t revealCursor = 0;
static const char* const CARD_NAMES[] = {"Профессия", "Возраст и пол", "Здоровье", "Хобби", "Фобия", "Багаж", "Факт", "Особенность"};
static const char* cardValue(uint8_t id, uint8_t category) {
  Player& p = players[id];
  switch (category) {
    case 0: return PROFESSIONS[p.profession]; case 1: return AGE_GENDER[p.ageGender];
    case 2: return HEALTH[p.health]; case 3: return HOBBIES[p.hobby];
    case 4: return PHOBIAS[p.phobia]; case 5: return BAGGAGE[p.baggage];
    case 6: return FACTS[p.fact]; default: return SPECIALS[p.special];
  }
}
static void sendCardBoard(uint8_t socketId) {
  sendText(socketId, "BOARD|OK");
  for (uint8_t id = 0; id < MAX_PLAYERS; ++id) for (uint8_t c = 0; c < 8; ++c)
    if (players[id].name[0] && (revealedMask[id] & (1U << c)))
      sendText(socketId, String("REVEALED|") + id + "|" + c + "|" + cardValue(id, c));
}
static void advanceRevealQueue() {
  while (revealCursor < revealOrderCount) {
    uint8_t id = revealOrder[revealCursor];
    if (players[id].connected && players[id].alive && revealedMask[id] != 0xFF) break;
    ++revealCursor;
  }
  if (revealCursor >= revealOrderCount) bunkerPhase = BUNKER_DISCUSSION;
}
static void startRevealCycle() {
  revealOrderCount = revealCursor = 0;
  for (uint8_t i = 0; i < MAX_PLAYERS; ++i)
    if (players[i].connected && players[i].alive && revealedMask[i] != 0xFF) revealOrder[revealOrderCount++] = i;
  for (int16_t i = static_cast<int16_t>(revealOrderCount) - 1; i > 0; --i) {
    uint8_t j = esp_random() % (i + 1); uint8_t t = revealOrder[i]; revealOrder[i] = revealOrder[j]; revealOrder[j] = t;
  }
  bunkerPhase = BUNKER_REVEAL;
  advanceRevealQueue();
}

static uint8_t connectedPlayerCount() {
  uint8_t count = 0;
  for (uint8_t i = 0; i < MAX_PLAYERS; ++i) if (players[i].connected) ++count;
  return count;
}

static int8_t findPlayerBySocket(uint8_t socketId) {
  for (uint8_t i = 0; i < MAX_PLAYERS; ++i)
    if (players[i].connected && players[i].socketId == socketId) return i;
  return -1;
}

static int8_t currentHostPlayer() {
  for (uint8_t i = 0; i < MAX_PLAYERS; ++i)
    if (players[i].connected && players[i].name[0]) return i;
  return -1;
}

static void sendText(uint8_t socketId, const String& message) {
  // WebSockets 2.x expects a mutable String reference.
  String payload = message;
  webSocket.sendTXT(socketId, payload);
}

static void broadcastState() {
  String message;
  message.reserve(560);
  message = "STATE|{\"room\":\"";
  message += roomId;
  message += "\",\"running\":";
  message += gameRunning ? "true" : "false";
  message += ",\"finished\":";
  message += gameOutcome != BUNKER_UNFINISHED ? "true" : "false";
  message += ",\"outcome\":\"";
  message += gameOutcome == BUNKER_VICTORY ? "victory" : "none";
  message += "\",\"phase\":\"";
  message += selectedGame == GAME_BUNKER ? (bunkerPhase == BUNKER_VOTING ? "voting" : (bunkerPhase == BUNKER_REVEAL ? "reveal" : "discussion")) : otherGamePhase();
  message += "\",\"host\":";
  message += String(static_cast<int>(currentHostPlayer()));
  message += ",\"capacity\":";
  message += String(bunkerCapacity);
  message += ",\"round\":";
  message += String(bunkerRound);
  message += ",\"game\":\"";
  message += selectedGame == GAME_BUNKER ? "bunker" :
             selectedGame == GAME_CROCODILE ? "crocodile" :
             selectedGame == GAME_MAFIA ? "mafia" :
             selectedGame == GAME_ALIAS ? "alias" :
             selectedGame == GAME_SPY ? "spy" : "whoami";
  message += '"';
  appendOtherGameState(message);
  message += ",\"revealer\":";
  message += String(revealCursor < revealOrderCount ? static_cast<int>(revealOrder[revealCursor]) : -1);
  message += ",\"queue\":[";
  for (uint8_t q = revealCursor; q < revealOrderCount; ++q) { if (q > revealCursor) message += ','; message += String(revealOrder[q]); }
  message += "],\"players\":[";
  bool first = true;
  for (uint8_t i = 0; i < MAX_PLAYERS; ++i) {
    if (!players[i].name[0]) continue;
    if (!first) message += ',';
    first = false;
    message += "{\"id\":" + String(i) + ",\"name\":\"";
    message += players[i].name;
    message += "\",\"ready\":";
    message += players[i].ready ? "true" : "false";
    message += ",\"connected\":";
    message += players[i].connected ? "true" : "false";
    message += ",\"alive\":";
    message += players[i].alive ? "true" : "false";
    message += ",\"mask\":";
    message += String(revealedMask[i]);
    message += '}';
  }
  message += "]}";
  webSocket.broadcastTXT(message);
}

static void sendScenario(uint8_t socketId) {
  const BunkerScenario& scenario = SCENARIOS[bunkerScenarioIndex];
  String message = "SCENARIO|";
  message += scenario.title;
  message += '|';
  message += scenario.description;
  message += '|';
  message += String(bunkerCapacity);
  message += '|';
  message += String(bunkerParticipantCount);
  sendText(socketId, message);
}

static void sendProfile(uint8_t socketId, uint8_t playerId) {
  String message = "PROFILE|Профессия|";
  message += PROFESSIONS[players[playerId].profession];
  message += "|Возраст и пол|";
  message += AGE_GENDER[players[playerId].ageGender];
  message += "|Здоровье|";
  message += HEALTH[players[playerId].health];
  message += "|Хобби|";
  message += HOBBIES[players[playerId].hobby];
  message += "|Фобия|";
  message += PHOBIAS[players[playerId].phobia];
  message += "|Багаж|";
  message += BAGGAGE[players[playerId].baggage];
  message += "|Факт|";
  message += FACTS[players[playerId].fact];
  message += "|Особенность|";
  message += SPECIALS[players[playerId].special];
  sendText(socketId, message);
}

static void sendVoteOptions(uint8_t socketId, uint8_t voterId) {
  String message = "VOTE|";
  bool first = true;
  for (uint8_t i = 0; i < MAX_PLAYERS; ++i) {
    if (i == voterId || !players[i].connected || !players[i].alive) continue;
    if (!first) message += ',';
    first = false;
    message += String(i) + ':' + players[i].name;
  }
  sendText(socketId, message);
}

static void checkVoteCompletion() {
  uint8_t remainingVoters = 0;
  for (uint8_t i = 0; i < MAX_PLAYERS; ++i) {
    if (!players[i].connected || !players[i].alive) continue;
    if (hasVoted[i]) continue;
    for (uint8_t target = 0; target < MAX_PLAYERS; ++target) {
      if (target != i && players[target].connected && players[target].alive) {
        ++remainingVoters;
        break;
      }
    }
  }
  if (remainingVoters) return;

  uint8_t totals[MAX_PLAYERS] = {};
  for (uint8_t i = 0; i < MAX_PLAYERS; ++i)
    if (hasVoted[i] && voteTarget[i] < MAX_PLAYERS) ++totals[voteTarget[i]];
  uint8_t eliminated = 255;
  uint8_t highest = 0;
  bool tie = false;
  for (uint8_t i = 0; i < MAX_PLAYERS; ++i) {
    if (!players[i].alive || totals[i] == 0) continue;
    if (totals[i] > highest) { highest = totals[i]; eliminated = i; tie = false; }
    else if (totals[i] == highest) tie = true;
  }
  if (!tie && eliminated < MAX_PLAYERS) players[eliminated].alive = false;

  uint8_t survivors = 0;
  for (uint8_t i = 0; i < MAX_PLAYERS; ++i)
    if (players[i].alive && players[i].name[0]) ++survivors;
  String result = "RESULT|";
  if (tie || eliminated == 255) result += "Ничья. Никто не покидает бункер.";
  else result += String(players[eliminated].name) + " покидает бункер.";

  if (survivors == 2) {
    gameOutcome = BUNKER_VICTORY;
    gameRunning = false;
    result += " Остались двое. Это победители!";
  } else {
    ++bunkerRound;
    for (uint8_t i = 0; i < MAX_PLAYERS; ++i) hasVoted[i] = false;
    startRevealCycle();
    result += " Начинается очередь раскрытия карточек.";
  }
  webSocket.broadcastTXT(result);
  broadcastState();
}

static void dealCategory(uint8_t* output, const uint8_t* playerIds, uint8_t playerCount, uint8_t deckSize) {
  uint8_t deck[40];
  for (uint8_t i = 0; i < deckSize; ++i) deck[i] = i;
  for (int16_t i = deckSize - 1; i > 0; --i) {
    const uint8_t other = esp_random() % (i + 1);
    const uint8_t temp = deck[i];
    deck[i] = deck[other];
    deck[other] = temp;
  }
  for (uint8_t i = 0; i < playerCount; ++i) output[playerIds[i]] = deck[i];
}

static void dealCharacterCards(const uint8_t* playerIds, uint8_t playerCount) {
  uint8_t dealt[MAX_PLAYERS] = {};
  dealCategory(dealt, playerIds, playerCount, PROFESSION_COUNT);
  for (uint8_t i = 0; i < playerCount; ++i) players[playerIds[i]].profession = dealt[playerIds[i]];
  dealCategory(dealt, playerIds, playerCount, AGE_GENDER_COUNT);
  for (uint8_t i = 0; i < playerCount; ++i) players[playerIds[i]].ageGender = dealt[playerIds[i]];
  dealCategory(dealt, playerIds, playerCount, HEALTH_COUNT);
  for (uint8_t i = 0; i < playerCount; ++i) players[playerIds[i]].health = dealt[playerIds[i]];
  dealCategory(dealt, playerIds, playerCount, HOBBY_COUNT);
  for (uint8_t i = 0; i < playerCount; ++i) players[playerIds[i]].hobby = dealt[playerIds[i]];
  dealCategory(dealt, playerIds, playerCount, PHOBIA_COUNT);
  for (uint8_t i = 0; i < playerCount; ++i) players[playerIds[i]].phobia = dealt[playerIds[i]];
  dealCategory(dealt, playerIds, playerCount, BAGGAGE_COUNT);
  for (uint8_t i = 0; i < playerCount; ++i) players[playerIds[i]].baggage = dealt[playerIds[i]];
  dealCategory(dealt, playerIds, playerCount, FACT_COUNT);
  for (uint8_t i = 0; i < playerCount; ++i) players[playerIds[i]].fact = dealt[playerIds[i]];
  dealCategory(dealt, playerIds, playerCount, SPECIAL_COUNT);
  for (uint8_t i = 0; i < playerCount; ++i) players[playerIds[i]].special = dealt[playerIds[i]];
}

static void handlePlayerMessage(uint8_t socketId, uint8_t* payload, size_t length) {
  if (length == 0 || length > 96) { sendText(socketId, "ERROR|Слишком длинное сообщение"); return; }
  char message[97];
  memcpy(message, payload, length);
  message[length] = '\0';
  const uint32_t now = millis();
  int8_t playerId = findPlayerBySocket(socketId);

  if (strncmp(message, "JOIN:", 5) == 0) {
    char* name = message + 5;
    char* token = strchr(name, ':');
    if (token) *token++ = '\0';
    // Keep names short, valid UTF-8, and safe for the small JSON state message.
    uint8_t out = 0;
    const uint8_t maxNameBytes = sizeof(players[0].name) - 1;
    for (uint8_t i = 0; name[i] && out < maxNameBytes;) {
      const uint8_t c = static_cast<uint8_t>(name[i]);
      if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
          (c >= '0' && c <= '9') || c == ' ' || c == '-' || c == '_') {
        name[out++] = static_cast<char>(c);
        ++i;
        continue;
      }

      uint8_t sequenceLength = 0;
      if (c >= 0xC2 && c <= 0xDF) sequenceLength = 2;
      else if (c >= 0xE0 && c <= 0xEF) sequenceLength = 3;
      else if (c >= 0xF0 && c <= 0xF4) sequenceLength = 4;

      bool validSequence = sequenceLength > 0 && out + sequenceLength <= maxNameBytes;
      for (uint8_t j = 1; validSequence && j < sequenceLength; ++j) {
        const uint8_t continuation = static_cast<uint8_t>(name[i + j]);
        if ((continuation & 0xC0) != 0x80) validSequence = false;
      }
      if (validSequence && sequenceLength == 3) {
        const uint8_t second = static_cast<uint8_t>(name[i + 1]);
        if ((c == 0xE0 && second < 0xA0) || (c == 0xED && second >= 0xA0)) validSequence = false;
      }
      if (validSequence && sequenceLength == 4) {
        const uint8_t second = static_cast<uint8_t>(name[i + 1]);
        if ((c == 0xF0 && second < 0x90) || (c == 0xF4 && second > 0x8F)) validSequence = false;
      }
      if (validSequence) {
        memmove(name + out, name + i, sequenceLength);
        out += sequenceLength;
        i += sequenceLength;
      } else {
        ++i;
      }
    }
    name[out] = '\0';
    if (!out) strcpy(name, "Игрок");

    int8_t slot = -1;
    if (token && token[0]) {
      for (uint8_t i = 0; i < MAX_PLAYERS; ++i)
        if (players[i].name[0] && strcmp(players[i].reconnectToken, token) == 0 &&
            now - players[i].lastSeenAt <= RECONNECT_GRACE_MS) slot = i;
    }
    if (slot < 0) {
      if (gameRunning || gameOutcome != BUNKER_UNFINISHED) {
        sendText(socketId, "ERROR|Раунд уже идёт или завершён");
        return;
      }
      for (uint8_t i = 0; i < MAX_PLAYERS; ++i)
        if (!players[i].name[0] || (!players[i].connected && now - players[i].lastSeenAt > RECONNECT_GRACE_MS)) { slot = i; break; }
    }
    if (slot < 0) { sendText(socketId, "ERROR|Комната заполнена"); return; }
    Player& player = players[slot];
    if (!player.name[0] || (token && strcmp(player.reconnectToken, token) != 0)) {
      memset(&player, 0, sizeof(player));
      snprintf(player.reconnectToken, sizeof(player.reconnectToken), "%08lx", (unsigned long)esp_random());
      player.alive = true;
    }
    strncpy(player.name, name, sizeof(player.name) - 1);
    Serial.printf("[Game] player slot %d joined as %s\n", slot, player.name);
    player.socketId = socketId;
    player.connected = true;
    player.lastSeenAt = now;
    sendText(socketId, String("WELCOME|") + roomId + "|" + player.reconnectToken + "|" + String(slot));
    if (gameRunning || gameOutcome != BUNKER_UNFINISHED) {
      if (selectedGame == GAME_BUNKER) { sendScenario(socketId); sendProfile(socketId, slot); }
      else sendOtherGamePrivate(socketId, slot);
      if (gameRunning && bunkerPhase == BUNKER_VOTING && player.alive && !hasVoted[slot]) sendVoteOptions(socketId, slot);
      if (gameRunning && selectedGame == GAME_BUNKER) sendCardBoard(socketId);
    }
    broadcastState();
    return;
  }

  if (playerId < 0) { sendText(socketId, "ERROR|Сначала войдите в комнату"); return; }
  Player& player = players[playerId];
  player.lastSeenAt = now;
  Serial.printf("[Game] command client=%u player=%d command=%s\n", socketId, playerId, message);
  const bool whoamiAction = selectedGame == GAME_WHOAMI &&
    (strncmp(message, "WHO_CARD:", 9) == 0 || strcmp(message, "WHO_START") == 0);
  if (!whoamiAction && now - player.lastActionAt < ACTION_INTERVAL_MS) return;
  player.lastActionAt = now;
  if (selectedGame != GAME_BUNKER && handleOtherGameCommand(socketId, playerId, message)) return;
  if (strcmp(message, "READY") == 0 && !gameRunning && gameOutcome == BUNKER_UNFINISHED) {
    player.ready = !player.ready;
    broadcastState();
  } else if (strcmp(message, "START") == 0 && !gameRunning && gameOutcome == BUNKER_UNFINISHED) {
    if (selectedGame != GAME_BUNKER) { startOtherGame(socketId); return; }
    bool allReady = connectedPlayerCount() >= 3;
    for (uint8_t i = 0; i < MAX_PLAYERS; ++i)
      if (players[i].connected && !players[i].ready) allReady = false;
    if (!allReady) { sendText(socketId, "ERROR|Нужно минимум 3 готовых игрока"); return; }
    gameRunning = true;
    bunkerRound = 1;
    bunkerPhase = BUNKER_DISCUSSION;
    gameOutcome = BUNKER_UNFINISHED;
    memset(hasVoted, 0, sizeof(hasVoted));
    bunkerScenarioIndex = esp_random() % SCENARIO_COUNT;
    const BunkerScenario& scenario = SCENARIOS[bunkerScenarioIndex];
    memset(revealedMask, 0, sizeof(revealedMask));
    uint8_t playerIds[MAX_PLAYERS];
    bunkerParticipantCount = 0;
    for (uint8_t i = 0; i < MAX_PLAYERS; ++i) {
      players[i].alive = players[i].connected && players[i].name[0];
      if (players[i].alive) playerIds[bunkerParticipantCount++] = i;
    }
    bunkerCapacity = 2;
    dealCharacterCards(playerIds, bunkerParticipantCount);
    startRevealCycle();
    for (uint8_t i = 0; i < bunkerParticipantCount; ++i) {
      const uint8_t id = playerIds[i];
      sendProfile(players[id].socketId, id);
    }
    for (uint8_t i = 0; i < MAX_PLAYERS; ++i)
      if (players[i].connected) sendScenario(players[i].socketId);
    webSocket.broadcastTXT("RESULT|Игра началась. Изучите свои карточки и обсудите, кого взять в бункер.");
    broadcastState();
  } else if (strcmp(message, "NEXT") == 0 && gameRunning && gameOutcome == BUNKER_UNFINISHED &&
             bunkerPhase == BUNKER_DISCUSSION && playerId == currentHostPlayer()) {
    uint8_t survivors = 0;
    for (uint8_t i = 0; i < MAX_PLAYERS; ++i) if (players[i].alive && players[i].name[0]) ++survivors;
    bunkerPhase = BUNKER_VOTING;
    memset(hasVoted, 0, sizeof(hasVoted));
    for (uint8_t i = 0; i < MAX_PLAYERS; ++i)
      if (players[i].connected && players[i].alive) sendVoteOptions(players[i].socketId, i);
    broadcastState();
  } else if (strncmp(message, "VOTE:", 5) == 0 && gameRunning && bunkerPhase == BUNKER_VOTING && !hasVoted[playerId]) {
    const int target = atoi(message + 5);
    if (target < 0 || target >= MAX_PLAYERS || target == playerId ||
        !players[target].connected || !players[target].alive) {
      sendText(socketId, "ERROR|Нельзя голосовать за этого игрока"); return;
    }
    voteTarget[playerId] = target;
    hasVoted[playerId] = true;
    sendText(socketId, "RESULT|Голос учтён");
    checkVoteCompletion();
  } else if (strcmp(message, "LOOK") == 0 && gameRunning) {
    sendCardBoard(socketId);
  } else if (strncmp(message, "REVEAL:", 7) == 0 && gameRunning && bunkerPhase == BUNKER_REVEAL &&
             revealCursor < revealOrderCount && revealOrder[revealCursor] == playerId) {
    int category = atoi(message + 7);
    if (category < 0 || category >= 8 || (revealedMask[playerId] & (1U << category))) {
      sendText(socketId, "ERROR|Эта карточка уже открыта или не существует"); return;
    }
    revealedMask[playerId] |= (1U << category);
    webSocket.broadcastTXT((String("REVEALED|") + playerId + "|" + category + "|" + cardValue(playerId, category)).c_str());
    ++revealCursor;
    if (revealCursor >= revealOrderCount) startRevealCycle();
    else advanceRevealQueue();
    broadcastState();
  } else if (strcmp(message, "RESET") == 0 && gameOutcome != BUNKER_UNFINISHED) {
    if (selectedGame != GAME_BUNKER) resetOtherGame();
    gameRunning = false;
    gameOutcome = BUNKER_UNFINISHED;
    bunkerRound = 0;
    bunkerCapacity = 0;
    bunkerPhase = BUNKER_DISCUSSION;
    memset(hasVoted, 0, sizeof(hasVoted));
    memset(revealedMask, 0, sizeof(revealedMask)); revealOrderCount = revealCursor = 0;
    for (uint8_t i = 0; i < MAX_PLAYERS; ++i) {
      players[i].ready = false;
      players[i].alive = false;
    }
    broadcastState();
  }
}


