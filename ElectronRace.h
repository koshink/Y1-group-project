#pragma once

#include "mbed.h"
#include "TextLCD.h"

// Create a struct to represent an obstacle 
struct Obstacle { 
    int column;
    int row;
    int length;
};


class ElectronRace {
private:
    // Buttons that will be used in the class are passed by reference
    TextLCD &lcd;
    DigitalIn &up;
    DigitalIn &down;


    int playerPos; // The current row that the player is in
    int obstaclePos; // The column the obstacle is in
    int score;
    Obstacle obstacles[4]; // Array of the obstacles that exist
    int obstacleCount; // Keeps a count of the amount of obstacles in the game
    bool isGameOver;

    void updateGame();
    void renderGame();
    void handleInput();
    void generateObstacle();
    bool checkGameOver();
    void resetGame();
  

public:
    ElectronRace(TextLCD &lcd, DigitalIn &Up, DigitalIn &Down);
    void startGame();
};