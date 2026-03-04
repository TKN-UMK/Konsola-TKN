#ifndef CONNECT4_H
#define CONNECT4_H

#include "Engine.h"

extern Adafruit_ST7735 tft;
extern InputManager keys;

class Connect4 : public Game {
private:
  const uint16_t COLOR_BG = ST7735_BLACK;
  const uint16_t COLOR_TOP_BAR = ST7735_YELLOW;
  const uint16_t COLOR_RED = ST7735_RED;
  const uint16_t COLOR_BLUE = 0x06FF;
  const uint16_t COLOR_GREEN = 0x05E0;

  Color boardOutlineColor = LIGHTGREEN;
  Color player1Color = RED;
  Color player2Color = LIGHTBLUE;
  Color line4Color = YELLOW;

  bool animateToken = false;
  bool dropToken = false;
  bool gameOver = false;
  bool draw = false;
  bool blinkState = 0;
  int8_t xPos = 3, yPos = -1;
  int8_t xLast = -1, yLast = -1;
  uint8_t startPlayer = 1;
  uint8_t currentPlayer = startPlayer;
  uint8_t boardMatrix[8][7];
  uint8_t tokenCounter[2];
  uint8_t line4coord[2][4] = {};
  uint8_t player1Score = 0;
  uint8_t player2Score = 0;

  uint32_t animateTokenMillis = 0;
  const uint16_t animateTokenInterval = 50;
  uint32_t tokenBlinkMillis = 0;
  const uint16_t tokenBlinkInterval = 400;

  void newRound();
  void removeToken();
  void addToken();
  void moveToken(int8_t xDir, int8_t yDir);
  bool scanLine(int8_t xStep, int8_t yStep);
  void printScore(uint8_t player, uint8_t score);
  void printMsg(const char* msg);

public:
  void setup() override;
  void loop() override;
};


#endif