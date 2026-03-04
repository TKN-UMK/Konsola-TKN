#ifndef TETRIS_H
#define TETRIS_H

#include "Engine.h"

extern Adafruit_ST7735 tft;
extern InputManager keys;
extern FileManager file;

class Tetris;

class TetrisBlock {
private:
  Tetris *game;
  uint8_t block_matrix[4][4];

  const uint8_t blocks_shapes[7][4][4] = {
    { { 0, 0, 0, 0 },
      { 1, 1, 1, 1 },
      { 0, 0, 0, 0 },
      { 0, 0, 0, 0 } },

    { { 0, 0, 0, 0 },
      { 0, 0, 0, 0 },
      { 1, 1, 0, 0 },
      { 1, 1, 0, 0 } },

    { { 0, 0, 0, 0 },
      { 0, 1, 0, 0 },
      { 1, 1, 1, 0 },
      { 0, 0, 0, 0 } },

    { { 0, 0, 0, 0 },
      { 0, 1, 1, 0 },
      { 1, 1, 0, 0 },
      { 0, 0, 0, 0 } },

    { { 0, 0, 0, 0 },
      { 1, 1, 0, 0 },
      { 0, 1, 1, 0 },
      { 0, 0, 0, 0 } },

    { { 0, 0, 0, 0 },
      { 1, 0, 0, 0 },
      { 1, 1, 1, 0 },
      { 0, 0, 0, 0 } },

    { { 0, 0, 0, 0 },
      { 0, 0, 1, 0 },
      { 1, 1, 1, 0 },
      { 0, 0, 0, 0 } }
  };

  const uint8_t blocks_color_codes[7] = { RED, LIGHTBLUE, VIOLET, BLUE, GREEN, PINK, ORANGE };

  int8_t x, y;
  uint8_t block_size;
  uint8_t block_color = 0;
  TetrisBlock(Tetris *game_ptr)
    : game(game_ptr) {}
  void spawn(uint8_t xStart, uint8_t yStart, uint8_t n);
  void moveSideways(int8_t direction);
  void set_block(int8_t xStart, int8_t yStart, Color c);
  void matrix_rotation(int8_t dir = 1);
  bool rotate();
  bool moveDown();
  bool detect_collision(int8_t xDir, int8_t yDir);
  bool save_position();
  bool filled_line();

  friend class Tetris;
};

class Tetris : public Game {

private:
  TetrisBlock block;

  const uint16_t COLOR_BG = ST7735_BLACK;
  const uint16_t COLOR_TOP_BAR = ST7735_RED;
  const uint16_t COLOR_TEXT = ST7735_WHITE;
  const uint16_t COLOR_SCORE = ST7735_GREEN;
  const uint16_t COLOR_HIGH_SCORE = 0x06FF;

  struct {
    uint32_t hiScore = 0;
  } fileData;

  uint8_t matrix[10][21] = {};
  uint8_t lines_to_remove[21] = {};
  uint8_t blocks7Bag[7] = {};
  uint8_t blocks7BagCounter = 0;
  const uint16_t linesPoints[4] = { 100, 300, 500, 800 };
  uint8_t linesCount = 0;
  uint32_t score = 0;
  bool gameOver = false;

  uint32_t currentMillis = 0;

  bool moveDownSpeedLock = false;
  uint32_t moveDownMillis = 0;
  uint16_t moveDownSlowInterval = 1000;
  const uint16_t moveDownFastInterval = 50;

  uint32_t moveRMillis = 0, moveLMillis = 0;
  const uint16_t moveSidewaysTimeout = 350, moveSidewaysInterval = 50;

  enum actionNames { SET_LINES_BLACK,
                     REMOVE_LINES };

  struct {
    bool active = false;
    uint8_t action = 0;
    uint16_t interval = 0;
    uint32_t millis = 0;
  } actionDelay;

  void setActionDelay(uint8_t a, uint16_t d);
  void remove_lines();
  void generateBlock();
  void updateScore(uint16_t addPoints = 0);
  void printHiScore();

  friend class TetrisBlock;

public:
  Tetris();
  void setup() override;
  void loop() override;
};

#endif