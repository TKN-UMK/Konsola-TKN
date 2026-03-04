#ifndef GAME_REGISTRY_H
#define GAME_REGISTRY_H
#include "Engine.h"

#include "DemoGame.h"
#include "Tetris.h"
#include "Connect4.h"

#define ADDGAME(GAME_CLASS, GAME_NAME) { GAME_NAME, []() -> Game* { return new GAME_CLASS(); } },
typedef Game* (*GameClass)();

struct GameEntry {
  const char* const title;
  const GameClass gameObj;
};

const GameEntry GAMES_LIST[] = {
    ADDGAME(Tetris, "TETRIS")
    ADDGAME(Connect4, "CONNECT 4")
    ADDGAME(DemoGame, "DEMO")
};

const uint8_t TOTAL_GAMES_COUNT = sizeof(GAMES_LIST) / sizeof(GAMES_LIST[0]);
#endif