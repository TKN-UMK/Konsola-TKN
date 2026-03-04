#include "Connect4.h"

void Connect4::newRound() {
  xPos = 3, yPos = -1;
  xLast = -1, yLast = -1;
  gameOver = false;
  draw = false;
  currentPlayer = startPlayer;
  startPlayer = !(startPlayer - 1) + 1;
  tokenCounter[0] = 0, tokenCounter[1] = 0;

  for (uint8_t i = 0; i < 8; i++) {
    for (uint8_t j = 0; j < 7; j++) boardMatrix[i][j] = 0;
  }

  FastLED.clear();
  for (uint8_t i = 2; i <= 9; i++) {
    setPixel(0, i, boardOutlineColor);
    setPixel(9, i, boardOutlineColor);
  }
  for (uint8_t i = 1; i <= 8; i++) setPixel(i, 9, boardOutlineColor);

  for (uint8_t i = 0; i <= 3; i++) {
    for (uint8_t j = 13; j <= 19; j++) {
      setPixel(i, j, player1Color);
      setPixel(i + 6, j, player2Color);
    }
  }

  moveToken(0, 0);
  removeToken();

  tft.fillScreen(COLOR_BG);
  tft.fillRect(0, 0, 160, 24, COLOR_TOP_BAR);
  tft.setTextColor(COLOR_BG);
  printCentered("CONNECT 4", 5, 2);
  tft.setTextColor(COLOR_GREEN);
  tft.drawLine(5, 75, 44, 75, COLOR_GREEN);
  tft.drawLine(114, 75, 154, 75, COLOR_GREEN);
  printCentered("SCOREBOARD", 72, 1);

  tft.setTextColor(currentPlayer == 1 ? COLOR_RED : COLOR_BLUE);
  printMsg(currentPlayer == 1 ? "RED'S TURN" : "BLUE'S TURN");
  printScore(1, player1Score);
  printScore(2, player2Score);

  keys.clear();
}

void Connect4::printScore(uint8_t player, uint8_t score) {
  char buf[10];
  const char* playerNames[] = { "RED", "BLUE" };
  sprintf(buf, "%s: %d", playerNames[player - 1], score);
  tft.fillRect(0, 85 + (player - 1) * 20, 160, 15, COLOR_BG);
  tft.setTextColor(player == 1 ? COLOR_RED : COLOR_BLUE);
  printCentered(buf, 85 + (player - 1) * 20, 2);
}

void Connect4::printMsg(const char* msg) {
  tft.fillRect(0, 40, 160, 15, COLOR_BG);
  printCentered(msg, 40, 2);
}

void Connect4::removeToken() {
  uint8_t p = currentPlayer - 1;
  setPixel(3 - tokenCounter[p] % 4 + 6 * p, tokenCounter[p] / 4 + 13, BLACK);
  tokenCounter[p]++;
  FastLED.show();
}

void Connect4::addToken() {
  uint8_t p = currentPlayer - 1;
  tokenCounter[p]--;
  setPixel(3 - tokenCounter[p] % 4 + 6 * p, tokenCounter[p] / 4 + 13, p ? LIGHTBLUE : RED);
  FastLED.show();
}

void Connect4::moveToken(int8_t xDir, int8_t yDir) {
  setPixel(xPos + 1, yPos + 2, BLACK);
  xPos += xDir;
  yPos += yDir;
  if (xPos < 0) xPos = 7;
  if (xPos > 7) xPos = 0;
  setPixel(xPos + 1, yPos + 2, currentPlayer == 1 ? player1Color : player2Color);
  FastLED.show();
}

bool Connect4::scanLine(int8_t xStep, int8_t yStep) {
  int8_t x = xPos - 3 * xStep, y = yPos - 3 * yStep;
  uint8_t counter = 0, lastToken = 0;

  for (uint8_t i = 0; i < 7; i++) {
    if (x >= 0 && x <= 7 && y >= 0 && y <= 6) {
      if (boardMatrix[x][y] == lastToken) {
        counter++;
        if (counter == 4 && lastToken) {
          for (uint8_t j = 0; j < 4; j++) {
            line4coord[0][j] = x - j * xStep;
            line4coord[1][j] = y - j * yStep;
          }
          return true;
        }
      } else {
        lastToken = boardMatrix[x][y];
        counter = 1;
      }
    }

    x += xStep;
    y += yStep;
  }

  return false;
}


void Connect4::setup() {
  newRound();
}

void Connect4::loop() {
  uint32_t currentMillis = millis();

  if (!animateToken && !gameOver) {
    if (keys.wasPressed(BTN_R)) {
      moveToken(1, 0);
    }

    if (keys.wasPressed(BTN_L)) {
      moveToken(-1, 0);
    }

    if (keys.wasPressed(BTN_D)) {
      animateToken = true;
      dropToken = true;
    }

    if (keys.wasPressed(BTN_U) && xLast != -1) {
      animateToken = true;
      dropToken = false;
      setPixel(xPos + 1, yPos + 2, BLACK);
      addToken();
      xPos = xLast;
      yPos = yLast;
      currentPlayer = !(currentPlayer - 1) + 1;
    }
  }

  if (keys.wasPressed(BTN_ENT) && gameOver) {
    newRound();
  }

  if (gameOver && !draw && currentMillis - tokenBlinkMillis >= tokenBlinkInterval) {
    tokenBlinkMillis = currentMillis;

    for (uint8_t j = 0; j < 4; j++)
      setPixel(line4coord[0][j] + 1, line4coord[1][j] + 2, blinkState ? (currentPlayer == 1 ? player1Color : player2Color) : line4Color);
    FastLED.show();
    blinkState = !blinkState;
  }

  if (animateToken && currentMillis - animateTokenMillis >= animateTokenInterval) {
    animateTokenMillis = currentMillis;

    if (dropToken) {
      if (boardMatrix[xPos][(yPos + 1) % 7] || yPos == 6) {
        animateToken = false;
        keys.clear();

        if (yPos >= 0) {
          boardMatrix[xPos][yPos] = currentPlayer;
          xLast = xPos;
          yLast = yPos;

          if (scanLine(1, 1) || scanLine(1, -1) || scanLine(1, 0) || scanLine(0, 1)) {
            gameOver = true;
            blinkState = 0;
            tft.setTextColor(currentPlayer == 1 ? COLOR_RED : COLOR_BLUE);
            printMsg(currentPlayer == 1 ? "RED WINS!" : "BLUE WINS!");
            printScore(currentPlayer, currentPlayer == 1 ? ++player1Score : ++player2Score);
            return;
          } else if (tokenCounter[0] + tokenCounter[1] == 56) {
            gameOver = true;
            draw = true;
            tft.setTextColor(ST7735_ORANGE);
            printMsg("DRAW!");
            return;
          }

          currentPlayer = !(currentPlayer - 1) + 1;
          yPos = -1;
          removeToken();
          moveToken(0, 0);
          tft.setTextColor(currentPlayer == 1 ? COLOR_RED : COLOR_BLUE);
          printMsg(currentPlayer == 1 ? "RED'S TURN" : "BLUE'S TURN");
        }
      } else moveToken(0, 1);
    }

    else {
      moveToken(0, -1);

      if (yPos == -1) {
        boardMatrix[xLast][yLast] = 0;
        xLast = -1;
        tft.setTextColor(currentPlayer == 1 ? COLOR_RED : COLOR_BLUE);
        printMsg(currentPlayer == 1 ? "RED'S TURN" : "BLUE'S TURN");
        animateToken = false;
        keys.clear();
      }
    }
  }
}