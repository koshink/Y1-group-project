#include "ElectronRace.h"
#include <ctime>

// Initialise the class by generating an obstacle
ElectronRace::ElectronRace(TextLCD &lcd, DigitalIn &up, DigitalIn &down) 
    : lcd(lcd), up(up), down(down), playerPos(0), obstacleCount(0), obstaclePos(lcd.columns() -1), isGameOver(false), score(0) {

    generateObstacle();
}


void ElectronRace::startGame() {
    int startSleepTime = 12; // Multiplier to set the starting sleepTime to 250ms
    double decreaseRate = 0.2; // Causes a steep drop at the beginning which evens out near the minimum
    int minSleepTime = 1; // The smallest interval between inputs (10ms);
    

    // Run through the game loop and check if the game is over each time
    do {
        handleInput();
        updateGame();
        renderGame();
        isGameOver = checkGameOver();
    
        /* Equation takes the form of Ae(-bx+1.5)
        It is a negative exponential so that the time between obstacles decreases
        over time before plateauing at a minimum sleep time */
        int sleepTime = startSleepTime * exp(-decreaseRate * score + 3);
        if (sleepTime < minSleepTime) {
            sleepTime = minSleepTime;
        }

        thread_sleep_for(sleepTime);

    } while (!isGameOver);
}


void ElectronRace::handleInput() {
    if (!up && playerPos > 0) {
        --playerPos;
    }

    if (!down && playerPos < 1) {
        ++playerPos;
    }
}


void ElectronRace::updateGame() {
    static bool obstacleGenerated = false;

    // Iterates over all obstacles in the array
    for (int i = 0; i < obstacleCount; ++i) {
        // Move all obstacles over to the left by one column
        --obstacles[i].column;

        // Checks if the obstacle has moved off screen
        if (obstacles[i].column + obstacles[i].length <= 0) {
            // Shifts all other obstacles to the left to overwrite it
            for (int j = i; j < obstacleCount - 1; ++j) {
                obstacles[j] = obstacles[j + 1];
            }

            --obstacleCount;
            ++score;
        }
    }

    if (obstacleCount == 0 || (obstacleCount > 0 && obstacles[obstacleCount - 1].column - obstacles[obstacleCount - 1].length == 9 && !obstacleGenerated)) {
        generateObstacle();
        obstacleGenerated = true;  // Set the flag to true after generating an obstacle
    } else if (obstacleCount > 0 && obstacles[obstacleCount - 1].column - obstacles[obstacleCount - 1].length <= 12) {
        obstacleGenerated = false;  // Reset the flag when the end of the last obstacle is not in column 12
    }
}




bool ElectronRace::checkGameOver() {
    for (int i = 0; i < obstacleCount; ++i) {
        // Check if the player's position matches the obstacle's position
        for (int j = 0; j < obstacles[i].length; ++j) {
            if (obstacles[i].column - j == 2 && playerPos == obstacles[i].row) {
                lcd.cls();
                lcd.locate(0,0); lcd.printf("GAME OVER");
                lcd.locate(0,1); lcd.printf("Score: %d", score);

                return true;
            }
        }
    }
    return false;
}


void ElectronRace::generateObstacle() {

    /* To ensure the obstacles are more random,
    set the seed for randomization using the 
    current time as a seed */
    srand(time(0)); 

    int length = rand() % 4 + 1; // Length of each obstacle generated
    int position = rand() % 2;  // Randomly choose the row of the obstacle

    // Creates a new obstacle in the array and adds it to the right of the screen
    obstacles[obstacleCount++] = {lcd.columns() - 1, position, length};
    return;
}


void ElectronRace::renderGame() {
    // Clears the LCD at the start of each frame
    lcd.cls();

    // Draws the player at the position specified
    lcd.locate(2, playerPos);
    lcd.putc('*');

    // Draws all of the obstacles at their positions on screen
    for (int i = 0; i < obstacleCount; ++i) {
        for (int j = 0; j < obstacles[i].length; ++j) {
            if (obstacles[i].column - j >= 0) {
                lcd.locate(obstacles[i].column - j, obstacles[i].row);
                lcd.putc('#');
            }
        }
    }
    
    /* Ensures the score is positioned as far right on the screen 
    moving the cursor left by 1 when printing more digits */
    int scoreLocation;
    
    if (score <= 9) {
        scoreLocation = 15;
    } else if (score <= 99) {
        scoreLocation = 14;
    } else {
        scoreLocation = 13;
    }
    
    lcd.locate(scoreLocation , 0);
    lcd.printf("%d", score);
}