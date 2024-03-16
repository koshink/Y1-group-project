#include "menu.h"
#include "ElectronRace.h"

Menu::Menu(TextLCD &lcd, DigitalIn &up, DigitalIn &down) : lcd(lcd), up(up), down(down), selectedOption(0) {
    options[0] = "Electron Race";
    options[1] = "Quiz Race";
}

void Menu::displayMenu() {
    lcd.cls();
    for (int i = 0; i < 2; i++) {
        lcd.locate(0, i);
        if (i == selectedOption) {
            lcd.printf(">%s", options[i]);
        } else {
            lcd.printf(" %s", options[i]);
        }
    }
}

void Menu::navigateMenu(int direction) {
    selectedOption += direction;
    if (selectedOption < 0) {
        selectedOption = 1;
    } else if (selectedOption > 1) {
        selectedOption = 0;
    }
    displayMenu();
}

void Menu::selectOption() {
    lcd.cls();
    lcd.locate(0, 0);
    lcd.printf("Launching");
    lcd.locate(0, 1);
    lcd.printf("%s", options[selectedOption]);

    if (selectedOption == 0) {
        ElectronRace game(lcd, up, down);
        game.startGame();
    }
}