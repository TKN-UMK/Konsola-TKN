#include "SpikesGame.h"

void SpikesGame::drawSmallDigit(int num, int xOffset, int yOffset, Color c) {
    // 8-bitowe definicje cyfr 3x5
    bool digits[10][15] = {
        {1,1,1, 1,0,1, 1,0,1, 1,0,1, 1,1,1}, // 0
        {1, 1, 1, 1, 1}, // 1 - to jest 1x5, nie 3x5!
        {1,1,1, 0,0,1, 1,1,1, 1,0,0, 1,1,1}, // 2
        {1,1,1, 0,0,1, 1,1,1, 0,0,1, 1,1,1}, // 3
        {1,0,1, 1,0,1, 1,1,1, 0,0,1, 0,0,1}, // 4
        {1,1,1, 1,0,0, 1,1,1, 0,0,1, 1,1,1}, // 5
        {1,1,1, 1,0,0, 1,1,1, 1,0,1, 1,1,1}, // 6
        {1,1,1, 0,0,1, 0,1,0, 1,0,0, 1,0,0}, // 7
        {1,1,1, 1,0,1, 1,1,1, 1,0,1, 1,1,1}, // 8
        {1,1,1, 1,0,1, 1,1,1, 0,0,1, 1,1,1}  // 9
    };

    // Specjalna obsługa jedynki (1x5)
    if (num == 1) {
        for (int y = 0; y < 5; y++) {
            if (digits[num][y]) setPixel(xOffset, yOffset + y, c);
        }
        return; // Nie rysuj nic więcej dla jedynki
    }

    // Rysowanie pozostałych cyfr (3x5)
    for (int y = 0; y < 5; y++) {
        for (int x = 0; x < 3; x++) {
            if (digits[num][y * 3 + x]) setPixel(xOffset + x, yOffset + y, c);
        }
    }
}

// UZUPEŁNIONA FUNKCJA: Teraz obsługuje wszystkie cyfry 0-9 na dużym ekranie
void SpikesGame::drawBigNumber(int num, Color c) {
    if (num == 0) {
        for(int y=5; y<=15; y++) { setPixel(2, y, c); setPixel(7, y, c); }
        for(int x=2; x<=7; x++) { setPixel(x, 5, c); setPixel(x, 15, c); }
    } else if (num == 1) {
        for (int y = 5; y <= 15; y++) setPixel(5, y, c);
        setPixel(4, 6, c);
    } else if (num == 2) {
        for (int x = 2; x <= 7; x++) { setPixel(x, 5, c); setPixel(x, 10, c); setPixel(x, 15, c); }
        for (int y = 5; y <= 10; y++) setPixel(7, y, c);
        for (int y = 10; y <= 15; y++) setPixel(2, y, c);
    } else if (num == 3) {
        for (int x = 2; x <= 7; x++) { setPixel(x, 5, c); setPixel(x, 10, c); setPixel(x, 15, c); }
        for (int y = 5; y <= 15; y++) setPixel(7, y, c);
    } else if (num == 4) {
        for(int y=5; y<=10; y++) setPixel(2, y, c);
        for(int x=2; x<=7; x++) setPixel(x, 10, c);
        for(int y=5; y<=15; y++) setPixel(7, y, c);
    } else if (num == 5) {
        for (int x = 2; x <= 7; x++) { setPixel(x, 5, c); setPixel(x, 10, c); setPixel(x, 15, c); }
        for (int y = 5; y <= 10; y++) setPixel(2, y, c);
        for (int y = 10; y <= 15; y++) setPixel(7, y, c);
    } else if (num == 6) {
        for (int x = 2; x <= 7; x++) { setPixel(x, 5, c); setPixel(x, 10, c); setPixel(x, 15, c); }
        for (int y = 5; y <= 15; y++) setPixel(2, y, c);
        for (int y = 10; y <= 15; y++) setPixel(7, y, c);
    } else if (num == 7) {
        for (int x = 2; x <= 7; x++) setPixel(x, 5, c);
        for (int y = 5; y <= 15; y++) setPixel(7, y, c);
    } else if (num == 8) {
        for (int x = 2; x <= 7; x++) { setPixel(x, 5, c); setPixel(x, 10, c); setPixel(x, 15, c); }
        for (int y = 5; y <= 15; y++) { setPixel(2, y, c); setPixel(7, y, c); }
    } else if (num == 9) {
        for (int x = 2; x <= 7; x++) { setPixel(x, 5, c); setPixel(x, 10, c); setPixel(x, 15, c); }
        for (int y = 5; y <= 10; y++) setPixel(2, y, c);
        for (int y = 5; y <= 15; y++) setPixel(7, y, c);
    }
}

void SpikesGame::randomizeSpikes(bool side) {
    for(int i=0; i<20; i++) {
        if(!side) leftSpikes[i] = false;
        else rightSpikes[i] = false;
    }
    int count = 3 + (score / 5);
    if (count > 15) count = 15;
    int placed = 0;
    while (placed < count) {
        int pos = 1 + (rand() % 18);
        if (!side) { if (!leftSpikes[pos]) { leftSpikes[pos] = true; placed++; } } 
        else { if (!rightSpikes[pos]) { rightSpikes[pos] = true; placed++; } }
    }
}

void SpikesGame::resetGame() {
    birdX = 5.0f; birdY = 10.0f; velY = 0.0f; dirX = 1;
    score = 0; gameOver = false; isCountingDown = true;
    countdownStart = millis();
    lastUpdate = millis(); // <--- Zainicjalizuj czas
    for(int i=0; i<20; i++) { leftSpikes[i] = false; rightSpikes[i] = false; }
}

void SpikesGame::setup() { resetGame(); }

void SpikesGame::loop() {
    uint32_t now = millis();
    // Obliczamy ile milisekund minęło od ostatniej klatki
    uint32_t elapsed = now - lastUpdate;
    lastUpdate = now;

    // Współczynnik czasu (1.0 = idealne 60 FPS, czyli ~16.6ms)
    // Dzięki temu nie musisz zmieniać swoich stałych (gravity, speedX)
    float dt = elapsed / 16.66f;

    // Zabezpieczenie przed "teleportacją" (np. po zatrzymaniu okna)
    if (dt > 3.0f) dt = 3.0f; 

    if (gameOver) {
        FastLED.clear();
        if (score < 10) drawBigNumber(score, WHITE);
        else {
            int tens = score / 10; int ones = score % 10;
            int totalWidth = (tens == 1 ? 1 : 3) + (ones == 1 ? 1 : 3) + 1;
            int startX = (10 - totalWidth) / 2;
            drawSmallDigit(tens, startX, 7, WHITE);
            drawSmallDigit(ones, startX + (tens == 1 ? 1 : 3) + 1, 7, WHITE);
        }
        FastLED.show();
        if (keys.wasPressed(BTN_ENT)) resetGame();
        return;
    }

    if (isCountingDown) {
        uint32_t countdownElapsed = now - countdownStart;
        FastLED.clear();
        if (countdownElapsed < 1000) drawBigNumber(3, RED);
        else if (countdownElapsed < 2000) drawBigNumber(2, ORANGE);
        else if (countdownElapsed < 3000) drawBigNumber(1, GREEN);
        else { isCountingDown = false; randomizeSpikes(true); }
        FastLED.show();
        return;
    }

    // --- FIZYKA ZALEŻNA OD CZASU (dt) ---
    if (keys.wasPressed(BTN_U) || keys.wasPressed(BTN_ENT)) {
        velY = jumpForce; // Skok to impuls, nie mnożymy go przez dt na starcie
    }

    velY += gravity * dt;    // Grawitacja działa w czasie
    birdY += velY * dt;       // Ruch pionowy w czasie
    birdX += (speedX * dirX) * dt; // Ruch poziomy w czasie

    int bX = (int)round(birdX);
    int bY = (int)round(birdY);

    // --- KOLIZJE ---
    if (bX <= 1 && leftSpikes[bY]) gameOver = true;
    if (bX >= 8 && rightSpikes[bY]) gameOver = true;

    if (birdX >= 9.0f) { 
        dirX = -1; birdX = 8.9f; score++; randomizeSpikes(false); 
    } else if (birdX <= 0.0f) { 
        dirX = 1; birdX = 0.1f; score++; randomizeSpikes(true); 
    }

    if (bY <= 0 || bY >= 19) gameOver = true;

    if (!gameOver) {
        FastLED.clear();
        int colorIdx = (score / 5) % 10;
        Color currentSpikeColor = spikeColors[colorIdx];
        
        // Ramka i kolce
        for(int i=0; i<20; i++) { setPixel(0, i, WHITE); setPixel(9, i, WHITE); }
        for(int i=1; i<9; i++) { setPixel(i, 0, currentSpikeColor); setPixel(i, 19, currentSpikeColor); }
        for(int i=1; i<19; i++) {
            if(leftSpikes[i]) { setPixel(0, i, currentSpikeColor); setPixel(1, i, currentSpikeColor); }
            if(rightSpikes[i]) { setPixel(9, i, currentSpikeColor); setPixel(8, i, currentSpikeColor); }
        }
        setPixel(bX, bY, YELLOW); 
        FastLED.show();
    }
}