#include "Engine.h"

InputManager::ButtonInternal InputManager::buttons[BTN_COUNT];
struct repeating_timer InputManager::timer;

void InputManager::begin(uint8_t pL, uint8_t pR, uint8_t pU, uint8_t pD, uint8_t pEnt, uint8_t pEsc) {
  uint8_t pins[] = { pL, pR, pU, pD, pEnt, pEsc };

  for (uint8_t i = 0; i < BTN_COUNT; i++) {
    buttons[i].pin = pins[i];
    buttons[i].isDown.store(false);
    buttons[i].wasPressed.store(false);
    buttons[i].wasReleased.store(false);
    buttons[i].lockTime = 0;
    buttons[i].pressedFlag = false;
    buttons[i].counter = 0;
    pinMode(buttons[i].pin, INPUT_PULLUP);
  }
  add_repeating_timer_us(-100, timer_callback, NULL, &timer);
}

bool InputManager::timer_callback(struct repeating_timer* t) {
  uint32_t now = millis();

  for (uint8_t i = 0; i < BTN_COUNT; i++) {
    bool locked = (now - buttons[i].lockTime) <= debounceLockTime;

    if (digitalReadFast(buttons[i].pin) == LOW) {
      if (buttons[i].counter < debounceCounterMax) {
        buttons[i].counter++;
      } else if (buttons[i].pressedFlag == false && !locked) {
        buttons[i].pressedFlag = true;
        buttons[i].isDown.store(true);
        buttons[i].wasPressed.store(true);
        buttons[i].lockTime = now;
      }
    }

    else {
      if (buttons[i].counter > 0) {
        buttons[i].counter--;
      } else if (buttons[i].pressedFlag == true && !locked) {
        buttons[i].pressedFlag = false;
        buttons[i].isDown.store(false);
        buttons[i].wasReleased.store(true);
        buttons[i].lockTime = now;
      }
    }
  }
  return true;
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

void FileManager::saveData(const char* filename, void* data, size_t size) {
  File f = LittleFS.open(filename, "w");
  if (!f) return;
  f.write((uint8_t*)data, size);
  f.close();
}

bool FileManager::loadData(const char* filename, void* data, size_t size) {
  if (!LittleFS.exists(filename)) return false;
  File f = LittleFS.open(filename, "r");
  f.read((uint8_t*)data, size);
  f.close();
  return true;
}

void setPixel(uint8_t x, uint8_t y, Color color) {
  static const CRGB colorLUT[COLOR_COUNT][2] = {
    { 0x000000, 0x000000 },  //BLACK
    { 0xFF0000, 0xFF0000 },  //RED
    { 0xFF8000, 0xFF6000 },  //ORANGE
    { 0xFFFF00, 0xFFBA00 },  //YELLOW
    { 0x40FF10, 0x40B510 },  //LIGHTGREEN
    { 0x005000, 0x003A00 },  //GREEN
    { 0x00FFD0, 0x00C090 },  //LIGHTBLUE
    { 0x0000FF, 0x0000FF },  //BLUE
    { 0x5000FF, 0x5000FF },  //VIOLET
    { 0xFF00FF, 0xFF00FF },  //PINK
    { 0xFFFFFF, 0xF0D0B0 }   //WHITE
  };

  if (x >= 10 || y >= 20) return;
  leds[(9 - x) * 20 + (x % 2 ? 20 - y : 1 + y)] = colorLUT[color][(x == 1 || x == 3 || x == 6 || x == 8)];
}

void printCentered(const char* text, uint8_t yPos, uint8_t fontSize) {
  uint8_t len = strlen(text);
  tft.setTextSize(fontSize);
  tft.setCursor((160 - fontSize * (len * 5 + (len - 1))) / 2, yPos);
  tft.print(text);
}