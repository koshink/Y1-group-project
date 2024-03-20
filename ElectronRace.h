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
    Obstacle obstacles[3];
    int obstacleCount;
    bool isGameOver;

    void updateGame();
    void renderGame();
    void handleInput();
    void generateObstacle();
    bool checkGameOver();
  

public:
    ElectronRace(TextLCD &lcd, DigitalIn &Up, DigitalIn &Down);
    ~ElectronRace();
    void startGame();
};