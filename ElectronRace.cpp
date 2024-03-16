#include "ElectronRace.h"

ElectronRace::ElectronRace(TextLCD &lcd, DigitalIn &up, DigitalIn &down) 
    : lcd(lcd), up(up), down(down), playerPos(lcd.rows() / 2), obstaclePos(lcd.columns() -1), score(0) {
    
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
        if (obstacles[i].column > 0) {
            --obstacles[i].column;
        } else {
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
            score = 0;
        }
    }
}


void ElectronRace::renderGame() {
    lcd.cls();

    lcd.locate(0, playerPos);
    lcd.putc('*');

    for (int i = 0; i < obstacleCount; ++i) {
        for (int j = 0; j < obstacles[i].length; ++j) {
            lcd.locate(15, obstacles[i].row + j);
            lcd.putc('#');
        }
    }

    lcd.locate(lcd.columns() - 1, lcd.rows() - 1);
    lcd.printf("%d", score);
}


void ElectronRace::generateObstacle() {
    int length = rand() % lcd.rows() + 1;
    int position = rand() % (lcd.rows() - length + 1);

    obstacles[obstacleCount++] = {lcd.columns() - 1, position, length};
}