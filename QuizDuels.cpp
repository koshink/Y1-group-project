#include "QuizDuels.h"

QuizDuels::QuizDuels(TextLCD &lcd, DigitalIn &up, DigitalIn &down, DigitalIn &left, DigitalIn &right, DigitalIn &action, DigitalIn &menu) 
    : lcd(lcd), up(up), down(down), left(left), right(right), action(action), menu(menu), player1{4}, player2{0}, currentPlayer(&player1), currentQuestionIndex(0), isGameOver(false), cursor{0,0} {
        
}

void QuizDuels::startGame() {
    /* To ensure the questions are more random,
    set the seed for randomization using the 
    current time as a seed */
    srand(time(0));

    while(!checkGameOver()) {
        currentQuestionIndex = rand() % questionCount;  // Choose a random question
        renderQuestion();
        thread_sleep_for(1000);
        renderAnswer(); 
        if (handleInput()) {
            break;
        }
        
        renderScoreboard();
        thread_sleep_for(1000); 
        isGameOver = checkGameOver();
        if (!isGameOver) {
            renderPlayerSwitch();
            thread_sleep_for(2000); 
        } else {
            // Reset the game
            currentQuestionIndex = 0;
            isGameOver = false;
        }
        if(!menu) { break; }
    } 
    displayWinner();
}

void QuizDuels::updateCursor(int &cursorPos, DigitalIn &button, int limit) {
    if (!button && cursorPos != limit) {
        cursorPos += (limit > cursorPos) ? 1 : -1;
        renderAnswer();
    }
}

int QuizDuels::handleInput() {
    while(true) {
        updateCursor(cursor[0], up, 0);
        updateCursor(cursor[0], down, 1);
        updateCursor(cursor[1], left, 0);
        updateCursor(cursor[1], right, 1);
        
        if (!action) { break; }
        if (!menu) { return true; }
    }
    checkAnswer();
    return false;
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
}

void QuizDuels::checkAnswer() {
    int answer = cursor[0] * 2 + cursor[1];
    bool isCorrect = (answer == questions[currentQuestionIndex].correctAnswer);
    lcd.cls(); lcd.locate(0,0);
    isCorrect ? (lcd.printf("Correct!"), currentPlayer->score++) : lcd.printf("Incorrect!");
    thread_sleep_for(500);
    currentQuestionIndex = (currentQuestionIndex + 1) % questionCount;
    return;
}


void QuizDuels::renderScoreboard() {
    lcd.cls();
    thread_sleep_for(500);

    // Draw finish line
    lcd.locate(10,0); lcd.printf("|");
    lcd.locate(10,1); lcd.printf("|");

    // Calculates the position of each player and whether they have won
    int pos1 = (2 * player1.score) > 10 ? 10 : pos1;
    int pos2 = (2 * player2.score) > 10 ? 10 : pos2;

    // Draw the score for player 1
    lcd.locate(pos1, 0); lcd.printf("#");
    // Draw the score for player 2
    lcd.locate(pos2, 1); lcd.printf("#");

    thread_sleep_for(500);
    return;
}

void QuizDuels::renderPlayerSwitch() {
    currentPlayer = (currentPlayer == &player1) ? &player2 : &player1;

    lcd.cls();
    lcd.locate(0,0); 
    lcd.printf("Pass to");

    lcd.locate(0,1);
    lcd.printf("Player %d", (currentPlayer == &player1) ? 1 : 2);
    return;
}

void QuizDuels::displayWinner() {
        lcd.cls();

    // Determine the winner
    int winner = (player1.score > player2.score) ? 1 : 2;

    // Display the winner
    lcd.locate(0, 0);
    lcd.printf("Player %d", winner);
    lcd.locate(0, 1);
    lcd.printf("Wins!");

    return;
}

bool QuizDuels::checkGameOver() {
    return player1.score >= 5 || player2.score >= 5;
}