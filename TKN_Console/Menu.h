#ifndef MENU_H
#define MENU_H

#include "Engine.h"
#include "GameRegistry.h"

extern Adafruit_ST7735 tft;
extern InputManager keys;
extern RTC_DS3231 rtc;
extern int8_t switchGame;
extern struct ConsoleSettings userConfig;
extern FileManager file;

class Menu : public Game {
private:
  enum Modes { ITEM_SEL,
               NUMBER_GET,
               SERIAL_READ };

  enum Screens { GAME_SEL_SCREEN,
                 SETTINGS_SCREEN };

  uint8_t serialDataBuffer[100] = {};
  uint8_t serialDataCounter = 0;

  void parseSerialData(uint8_t b);

  DateTime now;

  const uint8_t digitMatrix[10][9] = {
    { 0b00001111, 0b00001001, 0b00001001, 0b00001001, 0b00001001, 0b00001001, 0b00001001, 0b00001001, 0b00001111 },  //0
    { 0b00000001, 0b00000001, 0b00000001, 0b00000001, 0b00000001, 0b00000001, 0b00000001, 0b00000001, 0b00000001 },  //1
    { 0b00001111, 0b00000001, 0b00000001, 0b00000001, 0b00001111, 0b00001000, 0b00001000, 0b00001000, 0b00001111 },  //2
    { 0b00001111, 0b00000001, 0b00000001, 0b00000001, 0b00001111, 0b00000001, 0b00000001, 0b00000001, 0b00001111 },  //3
    { 0b00001001, 0b00001001, 0b00001001, 0b00001001, 0b00001111, 0b00000001, 0b00000001, 0b00000001, 0b00000001 },  //4
    { 0b00001111, 0b00001000, 0b00001000, 0b00001000, 0b00001111, 0b00000001, 0b00000001, 0b00000001, 0b00001111 },  //5
    { 0b00001111, 0b00001000, 0b00001000, 0b00001000, 0b00001111, 0b00001001, 0b00001001, 0b00001001, 0b00001111 },  //6
    { 0b00001111, 0b00000001, 0b00000001, 0b00000001, 0b00000001, 0b00000001, 0b00000001, 0b00000001, 0b00000001 },  //7
    { 0b00001111, 0b00001001, 0b00001001, 0b00001001, 0b00001111, 0b00001001, 0b00001001, 0b00001001, 0b00001111 },  //8
    { 0b00001111, 0b00001001, 0b00001001, 0b00001001, 0b00001111, 0b00000001, 0b00000001, 0b00000001, 0b00001111 },  //9
  };

  void setFastLEDdigit(uint8_t x, uint8_t y, uint8_t n);

  uint32_t clockUpdateMillis = 0;
  const uint16_t clockUpdateInterval = 1000;
  uint32_t brightnessChangeMillis = 0;
  const uint16_t brightnessChangeInterval = 200;
  bool lowBrightnessMode = false;

  const uint16_t COLOR_BG = ST7735_BLACK;
  const uint16_t COLOR_TITLE = ST7735_GREEN;
  const uint16_t COLOR_TEXT = ST7735_WHITE;
  const uint16_t COLOR_SEL_BG = 0x06FF;

  static constexpr const char* const settingsTitles[] = {
    "CLOCK",
    "GAME AUTO EXIT",
    "LCD SCREEN SAVER",
    "SERIAL DRAW MODE"
  };

  static constexpr uint8_t SETTINGS_ITEMS_COUNT = sizeof(settingsTitles) / sizeof(settingsTitles[0]);

  uint8_t scrollRectHeight = 0, currentPage = 0, previousPage = 0, currentScreen = GAME_SEL_SCREEN, mode = ITEM_SEL, digitIdx = 0;
  static inline uint8_t selectedIdx = 0;

  uint32_t screenSaverMillis = 0;

  int8_t values[2] = {};
  uint8_t maxValues[2] = {};

  void changeScreen(uint8_t n);
  void scrollMenu(int8_t idxDelta);
  void highlightItem(int8_t idx, bool highlight);
  void drawItem(uint8_t idx);
  void getTime(uint8_t startA, uint8_t startB, uint8_t maxA, uint8_t maxB);
  void drawDigit(uint8_t idx);
  void changeValue(int8_t delta);
  void changeDigit(int8_t delta);

public:
  void setup() override;
  void loop() override;
};


#endif