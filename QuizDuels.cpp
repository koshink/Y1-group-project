#include "QuizDuels.h"

QuizDuels::QuizDuels(TextLCD &lcd, DigitalIn &up, DigitalIn &down, DigitalIn &left, DigitalIn &right, DigitalIn &action, DigitalIn &menu) 
    : lcd(lcd), up(up), down(down), left(left), right(right), action(action), menu(menu), player1{0}, player2{0}, currentPlayer(&player1), currentQuestionIndex(0), isGameOver(false), cursor{0,0} {
        
}

void QuizDuels::startGame() {
    /* To ensure the questions are more random,
    set the seed for randomization using the 
    current time as a seed */
    srand(time(0));

    // Main game loop
    while(!checkGameOver()) {
        do {
            currentQuestionIndex = rand() % questionCount;  // Choose a random question
        } while (currentQuestionIndex == lastQuestionIndex);  // Keep generating a new index until it's different from the last one

        lastQuestionIndex = currentQuestionIndex;  // Update the last question index

        // Show question screen
        renderQuestion();
        thread_sleep_for(2000);
        // Show answer screen and wait for an input
        renderAnswer(); 
        if (handleInput()) break;
        
        // When input pressed show the current scoreboard
        renderScoreboard();
        thread_sleep_for(1000); 

        // Check gameover
        isGameOver = checkGameOver();
        if (!isGameOver) {
            // Render the switch player screen
            renderPlayerSwitch();
            thread_sleep_for(2000); 
        } else {
            // Reset the game
            currentQuestionIndex = 0;
            isGameOver = false;
        }
        if (!menu) break;
    } 
    // Render the winner screen
    displayWinner();
}

void QuizDuels::updateCursor(int &cursorPos, DigitalIn &button, int limit) {
    // Move the cursor then update the answer screen
    if (!button && cursorPos != limit) {
        cursorPos += (limit > cursorPos) ? 1 : -1;
        renderAnswer();
    }
}

int QuizDuels::handleInput() {
    while(true) {
        // Move the cursor depending on the button pressed
        updateCursor(cursor[0], up, 0);
        updateCursor(cursor[0], down, 1);
        updateCursor(cursor[1], left, 0);
        updateCursor(cursor[1], right, 1);
        
        // If the action button is pressed move on
        if (!action) break; 
        // If the menu button is pressed show the menu
        if (!menu) return true; 
    }
    // Check if the selected answer is correct
    checkAnswer();
    return false;
}

void QuizDuels::renderQuestion() {
    // Reset the lcd then display the question
    lcd.cls();
    lcd.printf("%s", questions[currentQuestionIndex].text);
    thread_sleep_for(500);
    return;
}

void QuizDuels::renderAnswer() {
    lcd.cls();
    
    // Print each of the answers on screen at different coordinates
    for (int i = 0; i < 4; i++) {
        lcd.locate((i % 2) * 8 + 1, i / 2);
        lcd.printf("%s", questions[currentQuestionIndex].answers[i]);
    }

    // Rerender the selected question with the cursor
    lcd.locate((cursor[1] % 2) * 8, cursor[0]);
    lcd.printf(">%s", questions[currentQuestionIndex].answers[cursor[0] * 2 + cursor[1]]);
}

void QuizDuels::checkAnswer() {
    correctAnswer = false;
    // Check if the answer matches the 0 indexed value
    int answer = cursor[0] * 2 + cursor[1];
    bool isCorrect = (answer == questions[currentQuestionIndex].correctAnswer);
    // Reset and display result
    lcd.cls();
    isCorrect ? (lcd.printf("Correct!"), correctAnswer = true, currentPlayer->score++) : lcd.printf("Incorrect!");
    thread_sleep_for(500);
    return;
}

void QuizDuels::renderScoreboard() {
    lcd.cls();
    thread_sleep_for(500);

    // Draw finish line
    lcd.locate(10,0); lcd.putc(5);
    lcd.locate(10,1); lcd.putc(5);

    // Current Player indicator
    lcd.locate(15, (currentPlayer == &player1) ? 0 : 1); lcd.printf("<");        

    if (correctAnswer == true) {
        // Chooses which animation of score increasing to display based on whether the question was answered correctly
        if (currentPlayer == &player1) {
            renderCar(2*player1.score - 2, 2*player2.score);
            thread_sleep_for(200);
            renderCar(2*player1.score - 1, 2*player2.score);
            thread_sleep_for(200);
            renderCar(2*player1.score, 2*player2.score);
            thread_sleep_for(200);
        } else if (currentPlayer == &player2) {
            renderCar(2*player1.score, 2*player2.score - 2);
            thread_sleep_for(200);
            renderCar(2*player1.score, 2*player2.score - 1);
            thread_sleep_for(200);
            renderCar(2*player1.score, 2*player2.score);
            thread_sleep_for(200);
        }
    }
    renderCar(2*player1.score, 2*player2.score);

    thread_sleep_for(100);
    return;
}

void QuizDuels::renderCar(int pos1, int pos2) {

    // Ensure the position does not exceed the finish line
    pos1 = pos1 > 10 ? 10 : pos1;
    pos2 = pos2 > 10 ? 10 : pos2;

   if (currentPlayer == &player1) {
        // Clear the previous position of the car for player 1
        lcd.locate(pos1 - 1, 0); lcd.putc(254);
        lcd.locate(pos1, 0); lcd.putc(254);
   }

    // Draw the car for player 1
    lcd.locate(pos1, 0); lcd.putc(3);
    lcd.locate(pos1 + 1, 0); lcd.putc(4);
    
    if (currentPlayer == &player2) {
        // Clear the previous position of the car for player 2
        lcd.locate(pos2 - 1, 1); lcd.putc(254);
        lcd.locate(pos2, 1); lcd.putc(254);
    }

    // Draw the car for player 2
    lcd.locate(pos2, 1); lcd.putc(3);
    lcd.locate(pos2 + 1, 1); lcd.putc(4);
    
}

void QuizDuels::renderPlayerSwitch() {
    // Switch the current player
    currentPlayer = (currentPlayer == &player1) ? &player2 : &player1;

    lcd.cls(); lcd.printf("Pass to");
    lcd.locate(0,1); lcd.printf("Player %d", (currentPlayer == &player1) ? 1 : 2);

    return;
}

void QuizDuels::displayWinner() {
    lcd.cls();

    // Determine the winner
    int winner = (player1.score > player2.score) ? 1 : 2;

    // Display the winner
    lcd.locate(0, 0); lcd.printf("Player %d", winner);
    lcd.locate(0, 1); lcd.printf("Wins!");
    return;
}

bool QuizDuels::checkGameOver() {
    // Check if either player has gotten 5 questions right
    return player1.score >= 5 || player2.score >= 5;
}