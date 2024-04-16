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
public:
    ElectronRace(TextLCD &lcd, DigitalIn &Up, DigitalIn &Down, DigitalIn &menu);
    void startGame();

private:
    // Buttons that will be used in the class are passed by reference
    TextLCD &lcd;
    DigitalIn &up;
    DigitalIn &down;
    DigitalIn &menu;


    Thread inputThread;
    Mutex mutex;
    

    int playerPos; // The current row that the player is in
    int obstaclePos; // The column the obstacle is in
    int score;
    Obstacle obstacles[4]; // Array of the obstacles that exist
    int obstacleCount; // Keeps a count of the amount of obstacles in the game
    bool isGameOver;

    // Game logic methods
    void handleInput();
    void updateGame();
    void generateObstacle();
    bool checkGameOver();

    // Rendering method
    void renderGame();
};