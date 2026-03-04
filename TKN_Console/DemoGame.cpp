#include "DemoGame.h"

void DemoGame::move(int8_t xDir, int8_t yDir) {
  setPixel(xPos, yPos, BLACK);
  if (xPos + xDir >= 0 && xPos + xDir <= 9) xPos += xDir;
  if (yPos + yDir >= 0 && yPos + yDir <= 15) yPos += yDir;
  setPixel(xPos, yPos, testPixelColor);
  FastLED.show();
}

void DemoGame::setup() {
  setPixel(xPos, yPos, testPixelColor);
  setPixel(1, 18, releasedColor);
  setPixel(3, 18, releasedColor);
  setPixel(2, 17, releasedColor);
  setPixel(2, 19, releasedColor);
  setPixel(0, 19, releasedColor);
  FastLED.show();
}

void DemoGame::loop() {
  uint32_t currentMillis = millis();

  if (currentMillis - blinkMillis >= blinkInterval) {
    blinkMillis = currentMillis;
    setPixel(9, 19, blinkState ? WHITE : BLACK);
    FastLED.show();
    blinkState = !blinkState;
  }

  if (keys.wasPressed(BTN_L)) {
    setPixel(1, 18, pressedColor);
    move(-1, 0);
  }
  if (keys.wasReleased(BTN_L)) {
    setPixel(1, 18, releasedColor);
    FastLED.show();
  }


  if (keys.wasPressed(BTN_R)) {
    setPixel(3, 18, pressedColor);
    move(1, 0);
  }
  if (keys.wasReleased(BTN_R)) {
    setPixel(3, 18, releasedColor);
    FastLED.show();
  }


  if (keys.wasPressed(BTN_U)) {
    setPixel(2, 17, pressedColor);
    move(0, -1);
  }
  if (keys.wasReleased(BTN_U)) {
    setPixel(2, 17, releasedColor);
    FastLED.show();
  }


  if (keys.wasPressed(BTN_D)) {
    setPixel(2, 19, pressedColor);
    move(0, 1);
  }
  if (keys.wasReleased(BTN_D)) {
    setPixel(2, 19, releasedColor);
    FastLED.show();
  }


  if (keys.wasPressed(BTN_ENT)) {
    setPixel(0, 19, pressedColor);
    FastLED.show();
  }
  if (keys.wasReleased(BTN_ENT)) {
    setPixel(0, 19, releasedColor);
    FastLED.show();
  }


  if (keys.wasPressed(BTN_ESC)) {
    if (keys.isDown(BTN_ENT)) {
      FastLED.clear();
      FastLED.show();
    } else {
      setPixel(xPos, yPos, BLACK);
      xPos = 0;
      yPos = 0;
      setPixel(xPos, yPos, testPixelColor);
      FastLED.show();
    }
  }
}