/*
  Board: Wavehsare RP2040 Zero
  Flash Size: 2MB (Sketch: 1792KB, FS: 256KB)
  CPU Speed: 125 MHz
*/

#include "GameRegistry.h"
#include "Menu.h"

FileManager file;
InputManager keys;
RTC_DS3231 rtc;
CRGB leds[WS2812B_NUM_LEDS];
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS_PIN, TFT_A0_PIN, TFT_RST_PIN);
ConsoleSettings userConfig;
Game* activeGame = nullptr;
int8_t switchGame = -1;

void setup() {
  pinMode(TFT_LED_PIN, OUTPUT);
  pinMode(ESC_LED_PIN, OUTPUT);

  LittleFS.begin();

  Wire1.setSDA(DS3231_SDA_PIN);
  Wire1.setSCL(DS3231_SCL_PIN);
  Wire1.begin();
  rtc.begin(&Wire1);

  FastLED.addLeds<WS2812B, WS2812B_PIN, GRB>(leds, WS2812B_NUM_LEDS);
  FastLED.setBrightness(WS2812B_MAX_BRIGHTNESS);
  FastLED.clear();
  FastLED.show();

  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);

  file.loadData("userConfig", &userConfig, sizeof(userConfig));

  Serial.begin(115200);

  activeGame = new Menu();
  activeGame->setup();
}

void loop() {
  static uint8_t activeGameIdx = 0;
  uint32_t currentMillis = millis();
  static uint32_t gameExitMillis = currentMillis;
  static uint32_t btnExitMillis = currentMillis;

  if (activeGame != nullptr) activeGame->loop();

  if (activeGameIdx > 0) {
    if (!keys.isDown(BTN_ESC)) btnExitMillis = currentMillis;
    if (currentMillis - btnExitMillis >= btnExitHoldTime || currentMillis - gameExitMillis >= userConfig.gameExitTimeout) switchGame = 0;
    if (keys.isDown(BTN_D) || keys.isDown(BTN_U) || keys.isDown(BTN_L) || keys.isDown(BTN_R) || keys.isDown(BTN_ENT) || keys.isDown(BTN_ESC)) gameExitMillis = currentMillis;
    if (currentMillis - gameExitMillis >= userConfig.gameExitTimeout) switchGame = 0;
    digitalWrite(ESC_LED_PIN, (currentMillis - gameExitMillis >= userConfig.gameExitTimeout - 10000) ? currentMillis / 400 % 2 : HIGH);
  }

  if (switchGame >= 0 && activeGame != nullptr) {
    randomSeed(micros());
    FastLED.clear();
    FastLED.show();
    tft.fillScreen(ST7735_BLACK);
    keys.clear();
    gameExitMillis = currentMillis;
    btnExitMillis = currentMillis;
    activeGameIdx = switchGame;
    delete activeGame;

    if (switchGame > 0 && switchGame <= TOTAL_GAMES_COUNT) activeGame = GAMES_LIST[switchGame - 1].gameObj();
    else {
      activeGameIdx = 0;
      activeGame = new Menu();
    }

    switchGame = -1;
    if (activeGameIdx != 0) digitalWrite(ESC_LED_PIN, HIGH);
    if (activeGame != nullptr) activeGame->setup();
  }
}

void setup1() {
  keys.begin(BTN_L_PIN, BTN_R_PIN, BTN_U_PIN, BTN_D_PIN, BTN_ENT_PIN, ESC_BTN_PIN);
}

void loop1() {
  delay(1000);
}