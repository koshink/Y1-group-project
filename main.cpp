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
        {0x00, 0x0E, 0x11, 0x11, 0x11, 0x11, 0x0E, 0x00}, // Electron frame 2
        {0x00, 0x0E, 0x11, 0x11, 0x11, 0x0E, 0x00, 0x00}, // Electron frame 3
        {0x00, 0x00, 0x19, 0x0F, 0x1F, 0x1F, 0x0F, 0x06}, // Car Left
        {0x00, 0x00, 0x10, 0x18, 0x1E, 0x1F, 0x1F, 0x06}, // Car Right
        {0x15, 0x0A, 0x15, 0x0A, 0x15, 0x0A, 0x15, 0x0A}, // Finish line
        {0x0E, 0x11, 0x10, 0x1E, 0x10, 0x11, 0x0E, 0x00}, // Epsilon
        {0x00, 0x00, 0x0E, 0x11, 0x0C, 0x11, 0x0E, 0x00} // Smaller Epsilon
    };
    
    // Use a loop to define each custom character
    for(int i = 0; i < 8; i++) {
        lcd.defineCustomChar(i, customChars[i]);
    }

    thread_sleep_for(500);
    
    // Start Screen 
    lcd.cls();
    lcd.locate(4,0); // Center the text
    lcd.putc(7); lcd.printf("psi|"); lcd.putc(0); lcd.printf("n"); // Epsilon
    lcd.locate(5,1); // Center the text
    lcd.printf("Z"); lcd.putc(6); lcd.printf("RO"); // Zero

    thread_sleep_for(3000);
    
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