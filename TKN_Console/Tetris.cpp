#include "Tetris.h"

Tetris::Tetris()
  : block(this) {}

void Tetris::setup() {
  block.spawn(3, 20, random(0, 7));
}

void Tetris::loop() {
  currentMillis = millis();

  if (!keys.isDown(BTN_L)) moveLMillis = currentMillis;
  if (!keys.isDown(BTN_R)) moveRMillis = currentMillis;
  if (keys.isDown(BTN_L) && keys.isDown(BTN_R)) moveLMillis = moveRMillis = currentMillis;
  if (!keys.isDown(BTN_D)) moveDownSpeedLock = false;

  if (!actionDelay.active) {
    if (keys.wasPressed(BTN_L)) {
      block.moveSideways(-1);
    }

    if (currentMillis - moveLMillis >= moveSidewaysTimeout) {
      moveLMillis = currentMillis - moveSidewaysTimeout + moveSidewaysInterval;
      block.moveSideways(-1);
    }

    if (keys.wasPressed(BTN_R)) {
      block.moveSideways(1);
    }

    if (currentMillis - moveRMillis >= moveSidewaysTimeout) {
      moveRMillis = currentMillis - moveSidewaysTimeout + moveSidewaysInterval;
      block.moveSideways(1);
    }

    if (keys.wasPressed(BTN_U) || keys.wasPressed(BTN_ENT)) {
      block.rotate();
    }

    if (currentMillis - moveDownMillis >= (keys.isDown(BTN_D) && !moveDownSpeedLock ? moveDownFastInterval : moveDownSlowInterval)) {
      moveDownMillis = currentMillis;

      if (!block.moveDown()) {
        if (!block.save_position()) { 
          switchGame = 0; //game over!!!
        }

        if (block.filled_line()) remove_lines();

        block.spawn(3, 20, random(0, 7));

        moveDownSpeedLock = true;
      }
    }
  }

  if (actionDelay.active && (currentMillis - actionDelay.millis >= actionDelay.interval)) {
    actionDelay.active = false;

    switch (actionDelay.action) {
      case SET_LINES_BLACK:
        for (uint8_t i = 0; i < 20; i++) {
          if (lines_to_remove[i]) {
            for (uint8_t j = 0; j < 10; j++)
              setPixel(j, 19 - i, BLACK);
          }
        }
        FastLED.show();
        setActionDelay(REMOVE_LINES, 250);
        break;

      case REMOVE_LINES:
        for (uint8_t l = 0; l < 4; l++) {
          for (uint8_t i = 0; i < 20; i++) {
            if (lines_to_remove[i]) {
              for (uint8_t j = i; j < 20; j++) {
                lines_to_remove[j] = lines_to_remove[j + 1];
                for (uint8_t k = 0; k < 10; k++)
                  matrix[k][j] = matrix[k][j + 1];
              }
              break;
            }
          }
        }

        for (uint8_t i = 0; i < 20; i++) {
          lines_to_remove[i] = 0;
          for (uint8_t j = 0; j < 10; j++)
            setPixel(j, 19 - i, (Color)matrix[j][i]);
        }

        FastLED.show();
        keys.clear();
        break;
    }
  }
}

void Tetris::setActionDelay(uint8_t a, uint16_t d) {
  actionDelay.interval = d;
  actionDelay.millis = currentMillis;
  actionDelay.action = a;
  actionDelay.active = true;
}

void Tetris::remove_lines() {

  for (uint8_t i = 0; i < 20; i++) {
    if (lines_to_remove[i]) {
      for (uint8_t j = 0; j < 10; j++)
        setPixel(j, 19 - i, WHITE);
    }
  }
  FastLED.show();

  setActionDelay(SET_LINES_BLACK, 400);
}

void TetrisBlock::spawn(uint8_t xStart, uint8_t yStart, uint8_t n) {
  block_color = blocks_color_codes[n];

  switch (n) {
    case 0:
      block_size = 4;
      break;
    case 1:
      block_size = 2;
      break;
    default:
      block_size = 3;
  }

  x = xStart;
  y = yStart;

  for (uint8_t i = 0; i < 4; i++) {
    for (uint8_t j = 0; j < 4; j++)
      block_matrix[i][j] = 0;
  }

  for (uint8_t i = 0; i < block_size; i++) {
    for (uint8_t j = 0; j < block_size; j++)
      block_matrix[i][j] = blocks_shapes[n][3 - j][i];
  }

  set_block(x, y, (Color)block_color);
  FastLED.show();
}

bool TetrisBlock::moveDown() {
  if (detect_collision(0, -1)) return 0;

  set_block(x, y, BLACK);
  y--;
  set_block(x, y, (Color)block_color);
  FastLED.show();

  return 1;
}

void TetrisBlock::moveSideways(int8_t direction) {

  if (!detect_collision(direction, 0)) {
    set_block(x, y, BLACK);
    x += direction;
    set_block(x, y, (Color)block_color);
    FastLED.show();
  }
}

void TetrisBlock::set_block(int8_t xStart, int8_t yStart, Color c) {
  for (uint8_t i = 0; i < block_size; i++) {
    for (uint8_t j = 0; j < block_size; j++) {
      if (block_matrix[i][j] && yStart + j < 20 && yStart + j >= 0) setPixel(xStart + i, 19 - (yStart + j), c);
    }
  }
  FastLED.show();
}

bool TetrisBlock::detect_collision(int8_t xDir, int8_t yDir) {
  for (uint8_t i = 0; i < block_size; i++) {
    for (uint8_t j = 0; j < block_size; j++) {
      if (block_matrix[i][j]) {
        if (y + j + yDir < 0 || x + i + xDir > 9 || x + i + xDir < 0) return 1;
        if (y + j + yDir < 20) {
          if (game->matrix[x + i + xDir][y + j + yDir]) return 1;
        }
      }
    }
  }
  return 0;
}

bool TetrisBlock::save_position() {
  for (uint8_t i = 0; i < block_size; i++) {
    for (uint8_t j = 0; j < block_size; j++)
      if (block_matrix[i][j]) {
        if (y + j >= 20) return 0;
        game->matrix[x + i][y + j] = block_color;
      }
  }
  return 1;
}

bool TetrisBlock::filled_line() {
  bool filled_any = 0, filled_current;

  for (uint8_t i = 0; i < block_size; i++) {
    if (y + i < 20) {
      filled_current = 1;
      for (uint8_t j = 0; j < 10; j++) {
        if (!game->matrix[j][y + i]) {
          filled_current = 0;
          break;
        }
      }
      if (filled_current) {
        game->lines_to_remove[y + i] = 1;
        filled_any = 1;
      }
    }
  }

  return filled_any;
}

void TetrisBlock::matrix_rotation(int8_t dir) {
  uint8_t tmp_block_matrix[block_size][block_size] = {};

  for (uint8_t i = 0; i < block_size; i++) {
    for (uint8_t j = 0; j < block_size; j++)
      tmp_block_matrix[i][j] = block_matrix[i][j];
  }

  for (uint8_t i = 0; i < block_size; i++) {
    for (uint8_t j = 0; j < block_size; j++)
      block_matrix[j * dir][(-i + block_size - 1) * dir] = tmp_block_matrix[i][j];
  }
}

bool TetrisBlock::rotate() {
  bool error = 0;
  set_block(x, y, BLACK);
  matrix_rotation();

  if (!detect_collision(0, 0)) {
  } else if (!detect_collision(-1, 0)) x--;
  else if (!detect_collision(1, 0)) x++;
  else {
    matrix_rotation();
    matrix_rotation();
    matrix_rotation();
    error = 1;
  }

  set_block(x, y, (Color)block_color);
  FastLED.show();
  return error;
}