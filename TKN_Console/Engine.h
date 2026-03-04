#ifndef ENGINE_H
#define ENGINE_H

#include <atomic>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#define FASTLED_INTERNAL
#include <FastLED.h>
#include <Wire.h>
#include "RTClib.h"
#include "hardware/timer.h"
#include <LittleFS.h>

#define DS3231_SDA_PIN 26
#define DS3231_SCL_PIN 27

#define WS2812B_PIN 8
#define WS2812B_NUM_LEDS 201
#define WS2812B_MIN_BRIGHTNESS 5
#define WS2812B_MAX_BRIGHTNESS 60  //DO NOT CHANGE!

#define ESC_BTN_PIN 0
#define ESC_LED_PIN 7

#define TFT_CS_PIN 5  //TFT SCK 2, TFT SDA 3
#define TFT_RST_PIN 1
#define TFT_A0_PIN 4
#define TFT_LED_PIN 6

#define BTN_L_PIN 9
#define BTN_R_PIN 11
#define BTN_U_PIN 10
#define BTN_D_PIN 12
#define BTN_ENT_PIN 13

#define LDR_PIN 28

const uint32_t btnExitHoldTime = 1500;

extern Adafruit_ST7735 tft;
extern CRGB leds[WS2812B_NUM_LEDS];

struct ConsoleSettings {
  uint32_t gameExitTimeout = 10000;
  uint32_t screenSaverTimeout = 10000;
};

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


enum Button { BTN_L,
              BTN_R,
              BTN_U,
              BTN_D,
              BTN_ENT,
              BTN_ESC,
              BTN_COUNT };

class InputManager {
private:
  struct ButtonInternal {
    uint8_t pin;
    uint8_t counter;
    bool pressedFlag;
    std::atomic<bool> isDown;
    std::atomic<bool> wasPressed;
    std::atomic<bool> wasReleased;
    uint32_t lockTime;
  };

  static ButtonInternal buttons[BTN_COUNT];
  static struct repeating_timer timer;
  static const uint32_t debounceLockTime = 10;
  static const uint8_t debounceCounterMax = 10;
  static bool timer_callback(struct repeating_timer* t);

public:
  void begin(uint8_t pL, uint8_t pR, uint8_t pU, uint8_t pD, uint8_t pEnt, uint8_t pEsc);
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

class FileManager {
public:
  void saveData(const char* filename, void* data, size_t size);
  bool loadData(const char* filename, void* data, size_t size);
};

void setPixel(uint8_t x, uint8_t y, Color color);

void printCentered(const char* text, uint8_t yPos, uint8_t fontSize);

#endif