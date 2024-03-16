#include "ElectronRace.h"

ElectronRace::ElectronRace(TextLCD &lcd, DigitalIn &up, DigitalIn &down) 
    : lcd(lcd), up(up), down(down), playerPos(0), obstaclePos(lcd.columns() -1), score(0) {
    
    generateObstacle();
}


void ElectronRace::startGame() {
    while (true) {
        handleInput();
        updateGame();
        renderGame();
        thread_sleep_for(100);
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
        if (obstacles[i].column + obstacles[i].length < 0) {
            for (int j = i; j < obstacleCount - 1; ++j) {
                obstacles[j] = obstacles[j + 1];
            }

            --obstacleCount;
            generateObstacle();
            ++score;
        }
    }

    for (int i = 0; i < obstacleCount; ++i) {
        if (playerPos >= obstacles[i].row && playerPos < obstacles[i].row + obstacles[i].length) {
            gameOver(score);
        }
    }
}



void ElectronRace::generateObstacle() {
    int length = rand() % 5 + 1; 
    int position = rand() % lcd.rows();  

    for (int i = 0; i < obstacleCount; ++i) {
        if (obstacles[i].column == lcd.columns() - 1) {
            obstacles[i] = {lcd.columns() - 1, position, length};
            return;
        }
    }

    obstacles[obstacleCount++] = {lcd.columns() - 1, position, length};
}

void ElectronRace::gameOver(int score) {
    lcd.locate(0,0);
    lcd.printf("GAME OVER");
    lcd.locate(0,1);
    lcd.printf("Score: %d", score);
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

    lcd.locate(lcd.columns() - 1, lcd.rows() - 1);
    lcd.printf("%d", score);
}