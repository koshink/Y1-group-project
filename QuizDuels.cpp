#include "QuizDuels.h"

QuizDuels::QuizDuels(TextLCD &lcd, DigitalIn &up, DigitalIn &down, DigitalIn &left, DigitalIn &right, DigitalIn &action) 
    : lcd(lcd), up(up), down(down), left(left), right(right), action(action), player1{0}, player2{0}, currentPlayer(&player1), currentQuestionIndex(0), isGameOver(false), inputReceived(false) {
    
    renderQuestion();
}

void QuizDuels::startGame() {
    do {
        renderQuestion();
        thread_sleep_for(500); 

        while(action) {
        handleInput();
        renderAnswer();
        }
        
        renderScoreboard();
        thread_sleep_for(500); 
        isGameOver = checkGameOver();
        if (!isGameOver) {
            renderPlayerSwitch();
            thread_sleep_for(500); 
        }
    } while (!isGameOver);
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

    renderAnswer();
}

void QuizDuels::renderAnswer() {
    lcd.cls();
    
    for (int i = 0; i < 4; i++) {
        lcd.locate((i % 2) * 8 + 1, i / 2);
        lcd.printf("%s", questions[currentQuestionIndex].answers[i]);
    }

    while (action) { 
        lcd.locate((cursor[1] % 2) * 8, cursor[0]);
        lcd.printf(">%s", questions[currentQuestionIndex].answers[cursor[0] * 2 + cursor[1]]);
        handleInput();
    }

    int answer = cursor[1] * 2 + cursor[0];
    bool isCorrect = (answer == questions[currentQuestionIndex].correctAnswer);
    lcd.cls();
    lcd.locate(0,0);
    if (isCorrect) {
        lcd.printf("Correct!");
    } else {
        lcd.printf("Incorrect!");
    }
    thread_sleep_for(200);
}

void QuizDuels::checkAnswer(int answer) {
    if (answer == questions[currentQuestionIndex].correctAnswer) {
        currentPlayer->score++;
    }
    currentQuestionIndex++;
}

void QuizDuels::renderScoreboard() {
    lcd.cls();
    thread_sleep_for(500);
    lcd.locate(0,0);
    lcd.printf("Player 1 score: %d", player1.score);
    lcd.locate(0,1);
    lcd.printf("Player 2 score: %d", player2.score);
}

void QuizDuels::renderPlayerSwitch() {
    currentPlayer = (currentPlayer == &player1) ? &player2 : &player1;

    lcd.cls();
    lcd.locate(0,0);
    lcd.printf("Pass to player %d", (currentPlayer == &player1) ? 1 : 2);
}

bool QuizDuels::checkGameOver() {
    return currentQuestionIndex >= sizeof(questions) / sizeof(questions[0]);
}