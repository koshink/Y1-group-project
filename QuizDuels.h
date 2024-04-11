#pragma once

#include "mbed.h"
#include "TextLCD.h"

struct Player {
    int score;
    int hasWon;
};

struct Question {
    int correctAnswer;
    int answers[3];
};

class QuizDuels {
private:
    // Buttons that will be used in the class are passed by reference
    TextLCD &lcd;
    DigitalIn &up;
    DigitalIn &down;
    DigitalIn &left;
    DigitalIn &right;

    // Creation of players
    Player player1;
    Player player2;

    int cursor[2]; // Array with the index being x, y
    bool isGameOver;



    void renderQuestion();
    void renderScoreboard();
    void renderPlayerSwitch();

    void handleInput();
    bool checkGameOver();

public:
    QuizDuels(TextLCD &lcd, DigitalIn &Up, DigitalIn &Down, DigitalIn &Left, DigitalIn &Right);
    void startGame();
};