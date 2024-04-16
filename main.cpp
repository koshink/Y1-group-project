#include "mbed.h"
#include "TextLCD.h"
#include "menu.h"



// Temporary import
#include "ElectronRace.h"
#include "QuizDuels.h"
// Remove



// LCD Display
TextLCD lcd(D0,D1,D2,D3,D4,D5, TextLCD::LCD20x4);

// Buttons
DigitalIn menuButton(D15, PullUp), action(D14, PullUp), up(D13, PullUp), left(D12, PullUp), down(D11, PullUp), right(D10, PullUp);

// Main loop
int main() {
    // Clear screen and reset cursor
    lcd.cls(); lcd.locate(0,0);

    
    // // Test the ElectronRace class skipping the menu
    // ElectronRace game(lcd, up, down);
    // game.startGame();

    // // Test the QuizRace class skipping the menu
    // QuizDuels game(lcd, up, down, left, right, action);
    // game.startGame();

    // Create the menu object then display it on the LCD
    Menu menu(lcd, up, down, left, right, action, menuButton);
    menu.displayMenu();

    // Check which button was pressed while on the main menu
    while (true) {
        // If multiple buttons are pressed, do nothing and refresh the menu
        int buttonPressCount = !menuButton + !action + !up + !down + !left + !right;
        (buttonPressCount > 1) ? menu.displayMenu() : void();
            
        (menuButton == false) ? menu.displayMenu() : 
        (action == false) ? menu.selectOption() : 
        (up == false) ? menu.navigateMenu(-1) : 
        (down == false) ? menu.navigateMenu(1) : void();
    }
}