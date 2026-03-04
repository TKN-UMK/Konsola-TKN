#ifndef DEMOGAME_H
#define DEMOGAME_H

#include "Engine.h"

extern InputManager keys;
extern Adafruit_ST7735 tft;

class DemoGame : public Game {
private:
  Color testPixelColor = LIGHTGREEN;
  Color pressedColor = PINK;
  Color releasedColor = BLUE;

  bool blink = false, blinkState = 0;
  uint32_t blinkMillis = 0;
  const uint16_t blinkInterval = 500;

  int8_t xPos = 0, yPos = 0;

  void move(int8_t xDir, int8_t yDir);

public:
  void setup() override;
  void loop() override;
};

#endif