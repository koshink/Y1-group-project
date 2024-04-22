#include "mbed.h"
#include "TextLCD.h"
#include "menu.h"

// LCD Display
TextLCD lcd(D0,D1,D2,D3,D4,D5, TextLCD::LCD20x4);

// Buttons
DigitalIn menuButton(D13, PullUp), action(D12, PullUp), up(D11, PullUp), left(D10, PullUp), down(D9, PullUp), right(D8, PullUp);

// Main loop
int main() {
    // Clear screen and reset cursor
    lcd.cls();

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