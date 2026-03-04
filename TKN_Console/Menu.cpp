#include "Menu.h"

void Menu::setup() {
  FastLED.clear();
  FastLED.show();
  changeScreen(0);
  now = rtc.now();
  screenSaverMillis = millis();
}

void Menu::loop() {
  uint32_t currentMillis = millis();

  if (mode != SERIAL_READ) {
    if (currentMillis - clockUpdateMillis >= clockUpdateInterval) {
      clockUpdateMillis = currentMillis;
      now = rtc.now();
      setFastLEDdigit(0, 0, now.hour() / 10);
      setFastLEDdigit(6, 0, now.hour() % 10);
      setFastLEDdigit(0, 11, now.minute() / 10);
      setFastLEDdigit(6, 11, now.minute() % 10);
      FastLED.show();
    }

    if (currentMillis - brightnessChangeMillis >= brightnessChangeInterval) {
      brightnessChangeMillis = currentMillis;
      uint16_t adcVal = analogRead(LDR_PIN);

      if (adcVal < 150 && !lowBrightnessMode) {
        FastLED.setBrightness(WS2812B_MIN_BRIGHTNESS);
        FastLED.show();
        lowBrightnessMode = true;
      } else if (adcVal > 350 && lowBrightnessMode) {
        FastLED.setBrightness(WS2812B_MAX_BRIGHTNESS);
        FastLED.show();
        lowBrightnessMode = false;
      }
    }
  }

  if (currentMillis - screenSaverMillis >= userConfig.screenSaverTimeout) {
    digitalWrite(TFT_LED_PIN, HIGH);
    if (keys.wasPressed(BTN_U) || keys.wasPressed(BTN_D) || keys.wasPressed(BTN_L) || keys.wasPressed(BTN_R) || keys.wasPressed(BTN_ESC) || keys.wasPressed(BTN_ENT)) {
      digitalWrite(TFT_LED_PIN, LOW);
      screenSaverMillis = currentMillis;
    }
  } else {
    if (keys.isDown(BTN_U) || keys.isDown(BTN_D) || keys.isDown(BTN_L) || keys.isDown(BTN_R) || keys.isDown(BTN_ESC) || keys.isDown(BTN_ENT)) screenSaverMillis = currentMillis;

    if (keys.wasPressed(BTN_D)) {
      switch (mode) {
        case ITEM_SEL: scrollMenu(1); break;
        case NUMBER_GET: changeValue(-1); break;
      }
    }

    if (keys.wasPressed(BTN_U)) {
      switch (mode) {
        case ITEM_SEL: scrollMenu(-1); break;
        case NUMBER_GET: changeValue(1); break;
      }
    }

    if (keys.wasPressed(BTN_L)) {
      if (mode == NUMBER_GET) changeDigit(-1);
    }

    if (keys.wasPressed(BTN_R)) {
      if (mode == NUMBER_GET) changeDigit(1);
    }

    if (keys.wasPressed(BTN_ESC)) {
      if (currentScreen == GAME_SEL_SCREEN && keys.isDown(BTN_L) && keys.isDown(BTN_U) && keys.isDown(BTN_D) && !keys.isDown(BTN_R) && !keys.isDown(BTN_ENT)) changeScreen(1);
      else if (currentScreen == 1 && mode == ITEM_SEL) changeScreen(0);
      else if (currentScreen == 1 && mode == NUMBER_GET) changeScreen(1);
      else if (mode == SERIAL_READ) {
        FastLED.clear();
        changeScreen(1);
        mode = ITEM_SEL;
        lowBrightnessMode = false;
      }
    }

    if (keys.wasPressed(BTN_ENT)) {
      if (currentScreen == GAME_SEL_SCREEN) {
        FastLED.setBrightness(WS2812B_MAX_BRIGHTNESS);
        switchGame = selectedIdx + 1;
      } else if (mode == ITEM_SEL) {
        switch (selectedIdx) {
          case 0:
            tft.fillScreen(COLOR_BG);
            tft.setTextColor(COLOR_TEXT);
            printCentered("CLOCK", 23, 2);
            printCentered("FORMAT HH:MM", 50, 1);
            getTime(now.hour(), now.minute(), 23, 59);
            break;
          case 1:
            tft.fillScreen(COLOR_BG);
            tft.setTextColor(COLOR_TEXT);
            printCentered("GAME AUTO", 17, 2);
            printCentered("EXIT TIME", 37, 2);
            printCentered("FORMAT MM:SS", 57, 1);
            getTime(userConfig.gameExitTimeout / 60000, (userConfig.gameExitTimeout % 60000) / 1000, 99, 59);
            break;
          case 2:
            tft.fillScreen(COLOR_BG);
            tft.setTextColor(COLOR_TEXT);
            printCentered("LCD SCREEN", 17, 2);
            printCentered("SAVER TIME", 37, 2);
            printCentered("FORMAT MM:SS", 57, 1);
            getTime(userConfig.screenSaverTimeout / 60000, (userConfig.screenSaverTimeout % 60000) / 1000, 99, 59);
            break;
          case 3:
            tft.fillScreen(COLOR_BG);
            tft.drawRect(0, 0, 160, 128, COLOR_TITLE);
            tft.setTextColor(COLOR_TEXT);
            printCentered("SERIAL DRAW", 35, 2);
            printCentered("MODE ENABLED", 55, 2);
            printCentered("USE THE DRAWING TOOL", 80, 1);
            mode = SERIAL_READ;
            FastLED.clear();
            FastLED.setBrightness(WS2812B_MAX_BRIGHTNESS);
            FastLED.show();
            break;
        }

      } else if (mode == NUMBER_GET) {
        switch (selectedIdx) {
          case 0:
            clockUpdateMillis = 0;
            rtc.adjust(DateTime(2016, 1, 1, values[0], values[1], 0));
            break;
          case 1:
            userConfig.gameExitTimeout = 60000 * values[0] + 1000 * values[1];
            file.saveData("userConfig", &userConfig, sizeof(userConfig));
            break;
          case 2:
            userConfig.screenSaverTimeout = 60000 * values[0] + 1000 * values[1];
            file.saveData("userConfig", &userConfig, sizeof(userConfig));
            break;
        }
        changeScreen(1);
      }
    }
  }

  if (Serial.available()) parseSerialData(Serial.read());
}

void Menu::setFastLEDdigit(uint8_t x, uint8_t y, uint8_t n) {
  for (uint8_t i = 0; i < 9; i++) {
    for (uint8_t j = 0; j < 4; j++)
      setPixel(j + x, i + y, (digitMatrix[n][i] >> (3 - j)) & 1 ? RED : BLACK);
  }
}

void Menu::changeScreen(uint8_t n) {
  mode = ITEM_SEL;
  if (n != currentScreen) {
    selectedIdx = 0;
    currentScreen = n;
  }
  uint8_t screenElements = (currentScreen == GAME_SEL_SCREEN ? TOTAL_GAMES_COUNT : SETTINGS_ITEMS_COUNT);
  scrollRectHeight = (94 + screenElements - 1) / screenElements;

  tft.fillScreen(COLOR_BG);
  tft.fillRect(0, 0, 160, 24, COLOR_TITLE);
  tft.drawRect(0, 0, 160, 128, COLOR_TITLE);
  tft.setTextColor(COLOR_BG);
  printCentered(currentScreen == GAME_SEL_SCREEN ? "SELECT GAME" : "SETTINGS", 5, 2);
  for (uint8_t i = 0; i < (screenElements > 5 ? 5 : screenElements); i++) drawItem(i);
  scrollMenu(0);

  digitalWrite(ESC_LED_PIN, currentScreen);
}

void Menu::scrollMenu(int8_t idxDelta) {
  uint8_t screenElements = (currentScreen == GAME_SEL_SCREEN ? TOTAL_GAMES_COUNT : SETTINGS_ITEMS_COUNT);
  if (selectedIdx + idxDelta >= 0 && selectedIdx + idxDelta < screenElements) {
    highlightItem(selectedIdx, false);
    selectedIdx += idxDelta;

    currentPage = selectedIdx / 5;
    if (currentPage != previousPage) {
      tft.fillRect(5, 28, 144, 94, COLOR_BG);
      for (uint8_t i = 5 * currentPage; i <= (5 * currentPage + 4 > screenElements - 1 ? screenElements - 1 : 5 * currentPage + 4); i++) drawItem(i);
    }
    previousPage = currentPage;

    tft.fillRect(152, 28, 4, 94, COLOR_BG);
    for (uint8_t i = 0; i < 19; i++) tft.fillRect(153, i * 5 + 30, 2, 2, COLOR_TEXT);
    tft.fillRect(152, 28 + (selectedIdx * 93 + screenElements - 1) / screenElements, 4, scrollRectHeight, COLOR_TEXT);

    highlightItem(selectedIdx, true);
  }
}

void Menu::highlightItem(int8_t idx, bool highlight) {
  tft.drawRect(5, idx % 5 * 19 + 28, 144, 18, highlight ? COLOR_SEL_BG : COLOR_BG);
}

void Menu::drawItem(uint8_t idx) {
  tft.setTextColor(COLOR_TEXT);
  tft.setCursor(15, idx % 5 * 19 + 34);
  tft.setTextSize(1);
  tft.print(currentScreen == GAME_SEL_SCREEN ? GAMES_LIST[idx].title : settingsTitles[idx]);
}

void Menu::getTime(uint8_t startA, uint8_t startB, uint8_t maxA, uint8_t maxB) {
  mode = NUMBER_GET;
  values[0] = startA;
  values[1] = startB;
  maxValues[0] = maxA;
  maxValues[1] = maxB;

  tft.drawRect(0, 0, 160, 128, COLOR_TITLE);
  tft.setCursor(70, 80);
  tft.setTextColor(COLOR_TEXT);
  tft.setTextSize(4);
  tft.print(":");

  for (uint8_t i = 0; i < 4; i++) drawDigit(i);
}

void Menu::drawDigit(uint8_t idx) {
  tft.setTextColor(digitIdx == idx ? COLOR_BG : COLOR_TEXT);
  tft.setTextSize(4);
  tft.setCursor(22 + (idx + (idx > 1)) * 24, 80);
  tft.fillRect(20 + (idx + (idx > 1)) * 24, 78, 24, 32, digitIdx == idx ? COLOR_TEXT : COLOR_BG);
  tft.print(idx % 2 ? (values[idx / 2] % 10) : (values[idx / 2] / 10));
}

void Menu::changeValue(int8_t delta) {
  uint8_t i = digitIdx / 2;
  values[i] += (digitIdx % 2 ? 1 : 10) * delta;
  if (values[i] < 0) values[i] = 0;
  else if (values[i] > maxValues[i]) values[i] = maxValues[i];
  drawDigit(i * 2);
  drawDigit(i * 2 + 1);
}

void Menu::changeDigit(int8_t delta) {
  if (digitIdx + delta >= 0 && digitIdx + delta < 4) {
    digitIdx += delta;
    drawDigit(digitIdx);
    drawDigit(digitIdx - delta);
  }
}

void Menu::parseSerialData(uint8_t b) {
  if (mode != SERIAL_READ) return;

  if (b == 0xF1) serialDataCounter = 0;
  else if (b == 0xF0) {
    if (serialDataCounter == 100) {
      for (uint8_t i = 0; i < 100; i++) {
        setPixel((i * 2) % 10, (i * 2) / 10, (Color)((serialDataBuffer[i] >> 4) & 0x0F));
        setPixel((i * 2 + 1) % 10, (i * 2 + 1) / 10, (Color)(serialDataBuffer[i] & 0x0F));
      }
      FastLED.show();
      serialDataCounter = 0;
    }
  } else {
    if (serialDataCounter >= 100) serialDataCounter = 0;
    serialDataBuffer[serialDataCounter++] = b;
  }
}
