#include "mbed.h"
#include "TextLCD.h"
#include "menu.h"

// LCD Display
TextLCD lcd(D0,D1,D2,D3,D4,D5, TextLCD::LCD20x4);

// Buttons
DigitalIn menuButton(D13, PullUp), action(D12, PullUp), up(D11, PullUp), left(D10, PullUp), down(D9, PullUp), right(D8, PullUp);

// Main loop
int main() {

    char customChars[8][8] = {  
        {0x00, 0x00, 0x0E, 0x11, 0x11, 0x11, 0x0E, 0x00}, // Electron frame 1
        {0x00, 0x0A, 0x00, 0x04, 0x00, 0x11, 0x0E, 0x00}, // Electron frame 2
        {0x00, 0x00, 0x0E, 0x15, 0x15, 0x0E, 0x00, 0x00}, // Electron frame 3
        {0x00, 0x07, 0x08, 0x10, 0x10, 0x0F, 0x06, 0x00}, // Car Left
        {0x00, 0x18, 0x06, 0x01, 0x01, 0x1F, 0x0C, 0x00}, // Car Right
        {0x15, 0x0A, 0x15, 0x0A, 0x15, 0x0A, 0x15, 0x0A}, // Finish line
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
    };
    
    // Use a loop to define each custom character
    for(int i = 0; i < 8; i++) {
        lcd.defineCustomChar(i, customChars[i]);
    }

    // Use a loop to print each custom character
    for(int i = 0; i < 8; i++) {
        lcd.putc(i);
    }

    thread_sleep_for(9000);
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