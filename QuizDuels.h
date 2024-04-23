#pragma once

#include "mbed.h"
#include "TextLCD.h"
#include <vector>

// Create a struct for each player
struct Player {
    int score;
};

// Create a struct to handle each question along with it's answers
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
    // Get the LCD and the buttons by reference
    TextLCD &lcd;
    DigitalIn &up;
    DigitalIn &down;
    DigitalIn &left;
    DigitalIn &right;
    DigitalIn &action;
    DigitalIn &menu;

    // The cursor has two coordinates for navigation (x,y)
    int cursor[2];
    bool isGameOver;
    bool correctAnswer;

    // Create each player
    Player player1;
    Player player2;
    Player *currentPlayer;

    // Create the questions 
    int currentQuestionIndex;
    int questionCount = sizeof(questions) / sizeof(questions[0]);

    // Question array
    Question questions[2] = {
        { "What is 2+2?", {"1", "2", "3", "4"}, 3},
        { "What is the capital of France?", {"Berlin", "Paris", "London", "Rome"}, 1}
    };


    // Game logic methods
    int handleInput();
    void checkAnswer();
    bool checkGameOver();
    void displayWinner();

    // Rendering methods
    void renderQuestion();
    void renderAnswer();
    void renderScoreboard(int player1Score, int player2Score);
    void renderPlayerSwitch();

    // Utility methods
    void updateCursor(int &cursorPos, DigitalIn &button, int limit);
};
