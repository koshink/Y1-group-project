#include "ElectronRace.h"

// Initialise the class by generating an obstacle
ElectronRace::ElectronRace(TextLCD &lcd, DigitalIn &up, DigitalIn &down, DigitalIn &menu) 
    : lcd(lcd), up(up), down(down), menu(menu), playerPos(0), obstacleCount(0), obstaclePos(lcd.columns() -1), isGameOver(false), score(0) {

    generateObstacle();
}


void ElectronRace::startGame() {

    inputThread.start(callback(this, &ElectronRace::handleInput));

    int startSleepTime = 12; // Multiplier to set the starting sleepTime to 250ms
    double decreaseRate = 0.2; // Causes a steep drop at the beginning which evens out near the minimum
    int minSleepTime = 1; // The smallest interval between inputs (10ms);
    
    // Run through the game loop and check if the game is over each time
    do {
        // handleInput();
        updateGame();
        renderGame();
        isGameOver = checkGameOver();
    
        /* Equation takes the form of Ae(-bx+1.5)
        It is a negative exponential so that the time between obstacles decreases
        over time before plateauing at a minimum sleep time */
        int sleepTime;
        if (startSleepTime * exp(-decreaseRate * score + 3) < minSleepTime) {
            sleepTime = minSleepTime;
        } else {
            sleepTime = startSleepTime * exp(-decreaseRate * score + 3);
        }

        thread_sleep_for(sleepTime);

        if(!menu) break;

    } while (!isGameOver);
}


void ElectronRace::handleInput() {
    while (true) {
        mutex.lock();
        playerPos = (!up && playerPos > 0) ? playerPos - 1 : playerPos;
        playerPos = (!down && playerPos < 1) ? playerPos + 1 : playerPos;
        mutex.unlock();

        if(!menu) {
            inputThread.join();
            break;
        }
        
        thread_sleep_for(20);
    }
}


void ElectronRace::updateGame() {
    // Stop an infinite number of obstacles from being generated
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
    int obstacleGen = (score < 20 && score > 10) ? 7 : 10;

    // Generate a new obstacle if the tail of the previous obstacle is in column 9
    if (obstacleCount == 0 || (obstacleCount > 0 && obstacles[obstacleCount - 1].column - obstacles[obstacleCount - 1].length == obstacleGen && !obstacleGenerated)) {
        generateObstacle();
        obstacleGenerated = true;  // Set the flag to true after generating an obstacle
    // Ensure the gap between obstacles is ate least 3 so columns 8-12
    } else if (obstacleCount > 0 && obstacles[obstacleCount - 1].column - obstacles[obstacleCount - 1].length <= 12) {
        obstacleGenerated = false;  // Reset the flag when the end of the last obstacle is not in column 12
    }
}

bool ElectronRace::checkGameOver() {
    for (int i = 0; i < obstacleCount; ++i) {
        // Check if the player's position matches the obstacle's position
        for (int j = 0; j < obstacles[i].length; ++j) {
            mutex.lock();
            if (obstacles[i].column - j == 2 && playerPos == obstacles[i].row) {
                lcd.cls();
                lcd.locate(0,0); lcd.printf("GAME OVER");
                lcd.locate(0,1); lcd.printf("Current: %dnA", score);
                thread_sleep_for(1000);
                return true;
            }
            mutex.unlock();
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
    mutex.lock();
    lcd.locate(2, playerPos);
    lcd.putc('*');
    mutex.unlock();

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
    int scoreLocation = (score <= 9) ? 15 : ((score <= 99) ? 14 : 13);
    
    lcd.locate(scoreLocation , 0);
    lcd.printf("%d", score);
}