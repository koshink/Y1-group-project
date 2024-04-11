#include "QuizDuels.h"

// Initialise the class 
QuizDuels::QuizDuels(TextLCD &lcd, DigitalIn &up, DigitalIn &down, DigitalIn &left, DigitalIn &right) 
    : lcd(lcd), up(up), down(down), left(left), right(right) {
    renderQuestion();
}

void QuizDuels::startGame() {

    handleInput();
    renderQuestion();
    renderScoreboard();
    checkGameOver();

    do {

        renderPlayerSwitch();
        handleInput();
        renderQuestion();
        renderScoreboard();
        checkGameOver();
        


    } while (!isGameOver);
}

void QuizDuels::handleInput() {
    if (!up && cursor[1] > 0) {
    --cursor[1];
    }

    if (!down && cursor[1] < 1) {
        ++cursor[1];
    }

    if (!left && cursor[0] < 1) {
        ++cursor[0];
    }

    if (!right && cursor[0] < 1) {
        ++cursor[0];
    }
}

bool QuizDuels::checkGameOver() {
    return false;
}

void QuizDuels::renderQuestion() {
    lcd.cls();
    thread_sleep_for(200);

    lcd.locate(0,0);
    lcd.printf("topL");

    lcd.locate(8,0);
    lcd.printf("topR");

    lcd.locate(0,1);
    lcd.printf("botL");

    lcd.locate(8,1);
    lcd.printf("botR");
}

void QuizDuels::renderScoreboard() {
}

void QuizDuels::renderPlayerSwitch() {
}