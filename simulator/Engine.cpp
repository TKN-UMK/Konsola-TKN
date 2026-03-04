#include "Engine.h"

InputManager::ButtonInternal InputManager::buttons[BTN_COUNT];

extern RaylibColor ledMatrix[COLS][ROWS];
extern RaylibColor ledBuffer[COLS][ROWS];
FakeFastLED FastLED;

void InputManager::begin(uint16_t pL, uint16_t pR, uint16_t pU, uint16_t pD, uint16_t pEnt, uint16_t pEsc) {
  uint16_t keys[] = { pL, pR, pU, pD, pEnt, pEsc };

  for (uint8_t i = 0; i < BTN_COUNT; i++) {
    buttons[i].key = keys[i];
    buttons[i].isDown.store(false);
    buttons[i].wasPressed.store(false);
    buttons[i].wasReleased.store(false);
  }
}

void InputManager::updateInputs() {
  for (uint8_t i = 0; i < BTN_COUNT; i++) {
    buttons[i].isDown.store(IsKeyDown(buttons[i].key));

    if (IsKeyPressed(buttons[i].key)) buttons[i].wasPressed.store(true);
    if (IsKeyReleased(buttons[i].key)) buttons[i].wasReleased.store(true);
  }
}

bool InputManager::isDown(Button b) {
  return (b < BTN_COUNT) ? buttons[b].isDown.load() : false;
}

bool InputManager::wasPressed(Button b) {
  if (b >= BTN_COUNT) return false;
  return buttons[b].wasPressed.exchange(false);
}

bool InputManager::wasReleased(Button b) {
  if (b >= BTN_COUNT) return false;
  return buttons[b].wasReleased.exchange(false);
}

void InputManager::clear() {
  for (uint8_t i = 0; i < BTN_COUNT; i++) {
    buttons[i].wasPressed.store(false);
    buttons[i].wasReleased.store(false);
  }
}

void copyLedBuffer() {
  for (int y = 0; y < ROWS; y++) {
    for (int x = 0; x < COLS; x++) {
      ledMatrix[x][y] = ledBuffer[x][y];
    }
  }
}

void setPixel(uint8_t x, uint8_t y, Color color) {
  static const RaylibColor PALETTE[] = {
    { 0, 0, 0, 255 },        // BLACK
    { 255, 0, 0, 255 },      // RED
    { 255, 170, 0, 255 },    // ORANGE
    { 255, 238, 0, 255 },    // YELLOW
    { 114, 255, 89, 255 },   // LIGHTGREEN
    { 0, 144, 0, 255 },      // GREEN
    { 0, 255, 255, 255 },    // LIGHTBLUE
    { 0, 48, 255, 255 },     // BLUE
    { 188, 109, 252, 255 },  // VIOLET
    { 255, 71, 237, 255 },   // PINK
    { 255, 255, 255, 255 }   // WHITE
  };

  if (color < 0 || color > WHITE) return;

  ledBuffer[x][y] = PALETTE[color];
}

uint32_t millis() {
  return (uint32_t)(GetTime() * 1000);
}