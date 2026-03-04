#ifndef ENGINE_H
#define ENGINE_H

#include <iostream>
#include <atomic>
#include <stdint.h>
#include "raylib.h"

const int COLS = 10;
const int ROWS = 20;

extern RaylibColor ledBuffer[COLS][ROWS];

enum Button { BTN_L,
              BTN_R,
              BTN_U,
              BTN_D,
              BTN_ENT,
              BTN_ESC,
              BTN_COUNT };

enum Color {
  BLACK,
  RED,
  ORANGE,
  YELLOW,
  LIGHTGREEN,
  GREEN,
  LIGHTBLUE,
  BLUE,
  VIOLET,
  PINK,
  WHITE,
  COLOR_COUNT
};

class InputManager {
private:
  struct ButtonInternal {
    uint16_t key;
    std::atomic<bool> isDown;
    std::atomic<bool> wasPressed;
    std::atomic<bool> wasReleased;
  };

  static ButtonInternal buttons[BTN_COUNT];

public:
  void begin(uint16_t pL, uint16_t pR, uint16_t pU, uint16_t pD, uint16_t pEnt, uint16_t pEsc);
  void updateInputs();
  void clear();
  bool isDown(Button b);
  bool wasPressed(Button b);
  bool wasReleased(Button b);
};

class Game {
public:
  virtual void setup() = 0;
  virtual void loop() = 0;
  virtual ~Game() {}
};

void copyLedBuffer();
void setPixel(uint8_t x, uint8_t y, Color color);
uint32_t millis();

struct FakeFastLED {
  void show() {
    copyLedBuffer();
  }

  void clear() {
    for (int y = 0; y < ROWS; y++) {
      for (int x = 0; x < COLS; x++)
        ledBuffer[x][y] = RL_BLACK;
    }
  }
};


#endif