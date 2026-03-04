#include "Engine.h"
#include "SimGame.h"

InputManager keys;
Game* activeGame = nullptr;

const uint32_t btnExitHoldTime = 1500;

RaylibColor ledMatrix[COLS][ROWS];
RaylibColor ledBuffer[COLS][ROWS];

int main() {
  InitWindow(10, 10, "Simulator");

  int monitor = GetCurrentMonitor();
  int monitorHeight = GetMonitorHeight(monitor);
  int monitorWidth = GetMonitorWidth(monitor);
  int CELL_SIZE = monitorHeight / (ROWS + 5.);
  int winHeight = CELL_SIZE * (ROWS + 2);
  int winWidth = CELL_SIZE * (COLS + 2);

  SetWindowSize(winWidth, winHeight);
  SetWindowPosition((monitorWidth - winWidth) / 2, (monitorHeight - winHeight) / 2);

  SetTargetFPS(60);

  for (int y = 0; y < ROWS; y++) {
    for (int x = 0; x < COLS; x++) {
      ledMatrix[x][y] = RL_BLACK;
      ledBuffer[x][y] = RL_BLACK;
    }
  }

  keys.begin(KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN, KEY_ENTER, KEY_BACKSPACE);
  activeGame = new SimGame();
  activeGame->setup();

  while (!WindowShouldClose()) {
	uint32_t currentMillis = millis();
	static uint32_t btnExitMillis = currentMillis;
    
	keys.updateInputs();
	
	if (!keys.isDown(BTN_ESC)) btnExitMillis = currentMillis;
	if (currentMillis - btnExitMillis >= btnExitHoldTime) break;
	
    activeGame->loop();

    BeginDrawing();
    ClearBackground((RaylibColor){ 30, 30, 30, 255 });

    for (int y = 0; y < ROWS; y++) {
      for (int x = 0; x < COLS; x++) {
        int posX = (x + 1) * CELL_SIZE;
        int posY = (y + 1) * CELL_SIZE;

        DrawRectangle(posX, posY, CELL_SIZE - 2, CELL_SIZE - 2, ledMatrix[x][y]);
        DrawRectangleLines(posX, posY, CELL_SIZE - 2, CELL_SIZE - 2, (RaylibColor){ 40, 40, 40, 255 });
      }
    }
    EndDrawing();
  }

  CloseWindow();
  return 0;
}