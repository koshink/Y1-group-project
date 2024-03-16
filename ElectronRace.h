#pragma once

#include "mbed.h"
#include "TextLCD.h"

class ElectronRace {
private:
    TextLCd &lcd;
    // Other stuff like the electron and resistors

public:
    ElectronRace(TextLCD &lcd);
    void startGame();
};