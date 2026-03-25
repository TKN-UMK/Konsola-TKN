#include "SnakeGame.h"


void SnakeGame::spawnApple() {
    bool valid = false;
    while (!valid) {
        appleX = rand() % 10; 
        appleY = rand() % 20; 
        valid = true;
        
        for (uint8_t i = 0; i < snakeLen; i++) {
            if (snakeX[i] == appleX && snakeY[i] == appleY) {
                valid = false;
                break;
            }
        }
    }
}

void SnakeGame::resetGame() {
    snakeLen = 3;
    snakeX[0] = 5; snakeY[0] = 10;
    snakeX[1] = 5; snakeY[1] = 11;
    snakeX[2] = 5; snakeY[2] = 12;
    
    dirX = 0;
    dirY = -1; 
    
    moveInterval = 300; 
    lastMoveTime = millis();
    gameOver = false;
    
    spawnApple();
    

}

void SnakeGame::setup() {
    highScore = 0; 
    resetGame();
}

void SnakeGame::loop() {
    if (gameOver) {
        if (keys.wasPressed(BTN_ENT)) {
            resetGame();
        }
        return;
    }

    if (keys.wasPressed(BTN_U) && dirY != 1)  { dirX = 0; dirY = -1; }
    if (keys.wasPressed(BTN_D) && dirY != -1) { dirX = 0; dirY = 1; }
    if (keys.wasPressed(BTN_L) && dirX != 1)  { dirX = -1; dirY = 0; }
    if (keys.wasPressed(BTN_R) && dirX != -1) { dirX = 1; dirY = 0; }

    if (millis() - lastMoveTime > moveInterval) {
        lastMoveTime = millis();

        int8_t nextX = snakeX[0] + dirX;
        int8_t nextY = snakeY[0] + dirY;

        bool collided = false;

        if (nextX < 0 || nextX > 9 || nextY < 0 || nextY > 19) {
            collided = true;
        }

        for (uint8_t i = 0; i < snakeLen; i++) {
            if (snakeX[i] == nextX && snakeY[i] == nextY) {
                collided = true;
                break;
            }
        }

        if (collided) {
            gameOver = true;
            int score = snakeLen - 3; 
            
         

            if (score > highScore) {
                highScore = score;
    
            }
            return; 
        }

        for (uint8_t i = snakeLen; i > 0; i--) {
            snakeX[i] = snakeX[i-1];
            snakeY[i] = snakeY[i-1];
        }
        
        snakeX[0] = nextX;
        snakeY[0] = nextY;

        if (nextX == appleX && nextY == appleY) {
            snakeLen++;
            if (moveInterval > 80) moveInterval -= 10; 
            spawnApple();
        }

        // --- ZMIANA KOLORU ZALEŻNIE OD ZJEDZONYCH JABŁEK ---
        int applesEaten = snakeLen - 3;
        Color headColor = LIGHTGREEN;
        Color bodyColor = GREEN;

        if (applesEaten >= 20) {
            headColor = YELLOW;     // Złota/żółta głowa za 20+ pkt
            bodyColor = ORANGE;     // Pomarańczowe ciało za 20+ pkt
        } else if (applesEaten >= 10) {
            headColor = PINK;
            bodyColor = VIOLET;
        } else if (applesEaten >= 5) {
            headColor = LIGHTBLUE;
            bodyColor = BLUE;
        }

        FastLED.clear(); 
        setPixel(appleX, appleY, RED); 
        
        for (uint8_t i = 0; i < snakeLen; i++) {
            setPixel(snakeX[i], snakeY[i], i == 0 ? headColor : bodyColor);
        }
        FastLED.show(); 
    }
}