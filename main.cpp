#include "mbed.h"
#include "TextLCD.h"
#include "menu.h"

//LCD Display
TextLCD lcd(D0,D1,D2,D3,D4,D5, TextLCD::LCD20x4);

//Buttons
DigitalIn menuButton(D15, PullUp), action(D14, PullUp), up(D13, PullUp), left(D12, PullUp), down(D11, PullUp), right(D10, PullUp);

// main loop
int main() {
    lcd.cls();
    lcd.locate(0,0);
    Menu menu(lcd, up, down);
    menu.displayMenu();
    while (true) {
        if (menuButton == false) {
            menu.displayMenu();
            thread_sleep_for(500);
        } else if (action == false) {
            menu.selectOption();
            thread_sleep_for(500);
        } else if (up == false) {
            menu.navigateMenu(-1);
            thread_sleep_for(500);
        } else if (down == false) {
            menu.navigateMenu(1);
            thread_sleep_for(500);
        }
    }
}