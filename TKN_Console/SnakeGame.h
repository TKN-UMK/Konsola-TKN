#ifndef SNAKEGAME_H
#define SNAKEGAME_H

#include "Engine.h"

extern InputManager keys;


class SnakeGame : public Game {
private:
    int8_t snakeX[200];
    int8_t snakeY[200];
    uint8_t snakeLen;
    
    int8_t appleX;
    int8_t appleY;
    
    int8_t dirX;
    int8_t dirY;
    
    uint32_t lastMoveTime;
    uint32_t moveInterval;
    bool gameOver;

    int highScore; // Trzyma rekord tylko na czas włączonego symulatora

    void spawnApple();
    void resetGame();

public:
    void setup() override;
    void loop() override;
};

#endif