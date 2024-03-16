#pragma once

#include "mbed.h"
#include "TextLCD.h"

struct Obstacle { 
    int column;
    int row;
    int length;
};


class ElectronRace {
private:
    TextLCD &lcd;
    DigitalIn &up;
    DigitalIn &down;
    int playerPos;
    int obstaclePos;
    int score;
    Obstacle obstacles[10];
    int obstacleCount;

    void updateGame();
    void renderGame();
    void handleInput();
    void generateObstacle();

public:
    ElectronRace(TextLCD &lcd, DigitalIn &Up, DigitalIn &Down);
    void startGame();
};