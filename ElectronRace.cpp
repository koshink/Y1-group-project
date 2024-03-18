#include "ElectronRace.h"

ElectronRace::ElectronRace(TextLCD &lcd, DigitalIn &up, DigitalIn &down) 
    : lcd(lcd), up(up), down(down), playerPos(0), obstaclePos(lcd.columns() -1), isGameOver(false), score(0) {
    
    generateObstacle();
}


void ElectronRace::startGame() {
    while (true) {
        handleInput();
        updateGame();
        renderGame();
        isGameOver = checkGameOver();
        thread_sleep_for(100);
        if (isGameOver == true) {
            break;
        }
    }
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
    for (int i = 0; i < obstacleCount; ++i) {
        --obstacles[i].column;
        if (obstacles[i].column + obstacles[i].length <= 0) {
            for (int j = i; j < obstacleCount - 1; ++j) {
                obstacles[j] = obstacles[j + 1];
            }

            --obstacleCount;
            generateObstacle();
            ++score;
        }
    }
}

bool ElectronRace::checkGameOver() {
    for (int i = 0; i < obstacleCount; ++i) {
        if (obstacles[i].column == 2 && playerPos == obstacles[i].row) {
            lcd.locate(0,0);
            lcd.printf("GAME OVER");
            lcd.locate(0,1);
            lcd.printf("Score: %d", score);
            return true;
        } else {
            return false;
        }
    }
    return false;
}

void ElectronRace::generateObstacle() {
    int length = 1; 
    int position = rand() % 2;  

    obstacles[obstacleCount++] = {lcd.columns() - 1, position, length};
}

void ElectronRace::renderGame() {
    lcd.cls();

    lcd.locate(2, playerPos);
    lcd.putc('*');

    for (int i = 0; i < obstacleCount; ++i) {
        lcd.locate(obstacles[i].column, obstacles[i].row);
        for (int j = 0; j < obstacles[i].length; ++j) {
            lcd.putc('#');
        }
    }

    lcd.locate(0, 0);
    lcd.printf("%d", score);
}