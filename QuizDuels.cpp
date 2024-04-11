#include "QuizDuels.h"

// Initialise the class 
QuizDuels::QuizDuels(TextLCD &lcd, DigitalIn &up, DigitalIn &down, DigitalIn &left, DigitalIn &right, DigitalIn &action) 
    : lcd(lcd), up(up), down(down), left(left), right(right), action(action), player1{0}, player2{0}, currentPlayer(&player1), currentQuestionIndex(0), isGameOver(false), inputReceived(false) {
    
    renderQuestion();
}

void QuizDuels::startGame() {
    do {
        handleInput();
        renderQuestion();
        renderScoreboard();
        isGameOver = checkGameOver();
        if (!isGameOver) {
            renderPlayerSwitch();
        }
    } while (!isGameOver);
}

void QuizDuels::handleInput() {
    // Clear the current cursor position
    lcd.locate((cursor[1] % 2) * 8, cursor[0]);
    lcd.printf(" ");
    
    if (!up && cursor[0] > 0) {
        --cursor[0];
    }

    if (!down && cursor[0] < 1) {
        ++cursor[0];
    }

    if (!left && cursor[1] > 0) {
        --cursor[1];
    }

    if (!right && cursor[1] < 1) {
        ++cursor[1];
    }

    // Handle action button
    if (!action) {
        inputReceived = true;

        // Handle player input and update the game state
        int answer = cursor[1] * 2 + cursor[0];
        if (answer == questions[currentQuestionIndex].correctAnswer) {
            currentPlayer->score++;
        }
        currentQuestionIndex++;
    }
}

bool QuizDuels::checkGameOver() {
    // Check if the game is over
    return currentQuestionIndex >= 10; // Assuming there are 10 questions
}

void QuizDuels::renderQuestion() {
    lcd.cls();
    thread_sleep_for(200);

    // Display the question at currentQuestionIndex
    lcd.locate(0,0);
    lcd.printf("%s", questions[currentQuestionIndex].text);

    thread_sleep_for(1000);
    lcd.cls();
    
    // Display the answers
    for (int i = 0; i < 4; i++) {
        lcd.locate((i % 2) * 8 + 1, i / 2);
        lcd.printf("%s", questions[currentQuestionIndex].answers[i]);
    }

    // Make the selected answer blink
    while (!inputReceived) { // Assuming inputReceived is a boolean that becomes true when an input is received
        handleInput(); // Update the cursor position

        lcd.locate((cursor[1] % 2) * 8, cursor[0]);
        lcd.printf(">%s", questions[currentQuestionIndex].answers[cursor[1] * 2 + cursor[0]]);
        thread_sleep_for(500);
    }
}


void QuizDuels::renderScoreboard() {
    // Display the scores of player1 and player2
    lcd.cls();
    thread_sleep_for(200);
    lcd.locate(0,0);
    lcd.printf("Player 1 score: %d", player1.score);
    lcd.locate(0,1);
    lcd.printf("Player 2 score: %d", player2.score);
}

void QuizDuels::renderPlayerSwitch() {
    // Switch currentPlayer
    currentPlayer = (currentPlayer == &player1) ? &player2 : &player1;
}