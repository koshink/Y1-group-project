#include "QuizDuels.h"

QuizDuels::QuizDuels(TextLCD &lcd, DigitalIn &up, DigitalIn &down, DigitalIn &left, DigitalIn &right, DigitalIn &action) 
    : lcd(lcd), up(up), down(down), left(left), right(right), action(action), player1{0}, player2{0}, currentPlayer(&player1), currentQuestionIndex(0), isGameOver(false), inputReceived(false) {
}

void QuizDuels::startGame() {
    do {
        renderQuestion();
        thread_sleep_for(500);
        renderAnswer(); 
        
        renderScoreboard();
        thread_sleep_for(500); 
        isGameOver = checkGameOver();
        if (!isGameOver) {
            renderPlayerSwitch();
            thread_sleep_for(500); 
        } else {
            // Reset the game
            currentQuestionIndex = 0;
            isGameOver = false;
        }
    } while (true); // Change this to true to keep the game looping
}

void QuizDuels::handleInput() {
    while(true) {
        if (!up && cursor[0] > 0) {
            --cursor[0];
            renderAnswer();
        }

        if (!down && cursor[0] < 1) {
            ++cursor[0];
            renderAnswer();
        }

        if (!left && cursor[1] > 0) {
            --cursor[1];
            renderAnswer();
        }

        if (!right && cursor[1] < 1) {
            ++cursor[1];
            renderAnswer();
        }
        
        if (!action) {
            break;
        }
    }
}

void QuizDuels::renderQuestion() {
    lcd.cls();
    thread_sleep_for(200);
    lcd.locate(0,0);
    lcd.printf("%s", questions[currentQuestionIndex].text);
    thread_sleep_for(500);

    return;
}

void QuizDuels::renderAnswer() {
    lcd.cls();
    
    for (int i = 0; i < 4; i++) {
        lcd.locate((i % 2) * 8 + 1, i / 2);
        lcd.printf("%s", questions[currentQuestionIndex].answers[i]);
    }

    lcd.locate((cursor[1] % 2) * 8, cursor[0]);
    lcd.printf(">%s", questions[currentQuestionIndex].answers[cursor[0] * 2 + cursor[1]]);
    handleInput();

    int answer = cursor[0] * 2 + cursor[1];
    bool isCorrect = (answer == questions[currentQuestionIndex].correctAnswer);
    lcd.cls();
    lcd.locate(0,0);
    if (isCorrect) {
        lcd.printf("Correct!");
        currentPlayer->score++;
    } else {
        lcd.printf("Incorrect!");
    }
    thread_sleep_for(500);
    currentQuestionIndex++;
    return;
}

void QuizDuels::renderScoreboard() {
    lcd.cls();
    thread_sleep_for(500);
    lcd.locate(0,0);
    lcd.printf("P1: %d", player1.score);
    lcd.locate(0,1);
    lcd.printf("P2: %d", player2.score);
    
    thread_sleep_for(500);
    return;
}

void QuizDuels::renderPlayerSwitch() {
    currentPlayer = (currentPlayer == &player1) ? &player2 : &player1;

    lcd.cls();
    lcd.locate(0,0);
    lcd.printf("Pass to Player %d", (currentPlayer == &player1) ? 1 : 2);
    return;
}

bool QuizDuels::checkGameOver() {
    return currentQuestionIndex >= sizeof(questions) / sizeof(questions[0]);
}