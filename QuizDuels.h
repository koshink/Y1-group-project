#pragma once

#include "mbed.h"
#include "TextLCD.h"

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
    int lastQuestionIndex;
    int currentQuestionIndex;
    int questionCount = sizeof(questions) / sizeof(questions[0]);

    // Question array
    Question questions[25] = {
    {"What is the unit\nof current?", "Farad", "Volt", "Ohm", "Ampere", 3},
    {"What does a CPU\ndo?", "Store", "Process", "Display", "Cool", 1},
    {"What is a CPU?", "Wire", "Disc", "Chip", "Coil", 2},
    {"What type of \nsignal is HDMI?", "Audio", "Digital", "Radio", "Analog", 1},
    {"What does R \nmean in RAM?", "Random", "React", "Root", "Real", 0},
    {"Which is a \ndesktop OS?", "MacOS", "iOS", "Fire OS", "Android", 0},
    {"What is \nOhm's Law?", "C=QV", "V=IR", "R=I/V", "P=VI", 1},
    {"Time constant \nof capacitor?", "VC", "RC", "LC", "EC", 1},
    {"When closed, \na switch is...", "Both", "On", "Off", "None", 1},
    {"What does B \nmean in BIOS?", "Binary", "Basic", "Built", "Bit", 1},
    {"How many bits \nis a byte?", "8", "16", "2", "4", 0},
    {"Choose the odd\none out", "PyCharm", "C++", "Python", "Java", 0},
    {"Who was a \nfounder of Apple?", "L Page", "S Jobs", "B Gates", "S Brin", 1},
    {"What is the \nunit of power?", "Ohm", "Watt", "Farad", "Volt", 1},
    {"Which is a \nmobile OS?", "Android", "Windows", "Linux", "MacOS", 0},
    {"What is 8 \nin binary?", "1100", "0101", "0011", "1000", 3},
    {"What base is\nhexadecimal?", "8", "2", "10", "16", 3},
    {"What is \n1 AND 0?", "1", "None", "X", "0", 3},
    {"What is 1001\nin denary?", "9", "10", "11", "8", 0},
    {"Which do humans\ncount in?", "Denary", "Octal", "Binary", "Hex", 0},
    {"How many bytes\nis 16 bits?", "4", "1", "2", "16", 2},
    {"Which does a\nresistor use?", "Ohm", "Henry", "Farad", "None", 0},
    {"What does D\nmean in LED?", "Digit", "Diode", "Dome", "Device", 1},
    {"What is\n1 OR 0?", "X", "None", "1", "0", 2},
    {"When open,\na switch is...", "Both", "On", "None", "Off", 3}
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
    void renderCar(int pos1, int pos2);
    void renderPlayerSwitch();

    // Utility methods
    void updateCursor(int &cursorPos, DigitalIn &button, int limit);
};
