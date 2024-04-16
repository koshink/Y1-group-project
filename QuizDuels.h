#pragma once

#include "mbed.h"
#include "TextLCD.h"
#include <vector>

struct Player {
    int score;
};

struct Question {
    char text[100];
    char answers[4][100];
    int correctAnswer;
};

class QuizDuels {
public:
    QuizDuels(TextLCD &lcd, DigitalIn &up, DigitalIn &down, DigitalIn &left, DigitalIn &right, DigitalIn &action, DigitalIn &menu);
    void startGame();
    
private:
    // Member variables
    TextLCD &lcd;
    DigitalIn &up;
    DigitalIn &down;
    DigitalIn &left;
    DigitalIn &right;
    DigitalIn &action;
    DigitalIn &menu;

    Player player1;
    Player player2;
    Player* currentPlayer;

    int currentQuestionIndex;
    int questionCount = sizeof(questions) / sizeof(questions[0]);
    bool inputReceived;

    int cursor[2];
    bool isGameOver;

        // Initialize questions here
    Question questions[2] = {
        {
            "What is 2+2?",
            {"1", "2", "3", "4"},
            3
        },
        {
            "What is the capital of France?",
            {"Berlin", "Paris", "London", "Rome"},
            1
        }
    };


 // Game logic methods
    int handleInput();
    void checkAnswer();
    bool checkGameOver();
    void displayWinner();

    // Rendering methods
    void renderQuestion();
    void renderAnswer();
    void renderScoreboard();
    void renderPlayerSwitch();

    // Utility methods
    void updateCursor(int &cursorPos, DigitalIn &button, int limit);
};
