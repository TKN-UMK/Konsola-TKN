#ifndef SPIKESGAME_H
#define SPIKESGAME_H

#include "Engine.h"

extern InputManager keys;
//extern FakeFastLED FastLED;

// ... reszta pliku bez zmian ...
class SpikesGame : public Game {
private:
    float birdX, birdY;
    float velY;
    uint32_t lastUpdate; // <--- DODAJ TO

    // Prędkości bazowe (teraz traktujemy je jako "na klatkę przy 60FPS")
    float gravity = 0.015f;
    float jumpForce = -0.25f;
    float speedX = 0.1f;
// ... reszta pliku ...

    int8_t dirX; 
    int score;
    
    bool isCountingDown;
    bool gameOver;
    uint32_t countdownStart;

    bool leftSpikes[20];
    bool rightSpikes[20];

    Color spikeColors[10] = {
        RED, BLUE, GREEN, ORANGE, PINK, 
        VIOLET, LIGHTBLUE, LIGHTGREEN, WHITE, YELLOW
    };

    void resetGame();
    void randomizeSpikes(bool side);
    void drawBigNumber(int num, Color c);
    void drawSmallDigit(int num, int xOffset, int yOffset, Color c);

public:
    void setup() override;
    void loop() override;
};

#endif