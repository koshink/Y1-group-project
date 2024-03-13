#include "mbed.h"
#include "TextLCD.h"

//LCD Display
TextLCD lcd(D0,D1,D2,D3,D4,D5, TextLCD::LCD20x4);


//Buttons
DigitalIn menu(D15, PullUp), action(D14, PullUp), up(D13, PullUp), left(D12, PullUp), down(D11, PullUp), right(D10, PullUp);

// main loop
int main() {
    lcd.cls();
    lcd.locate(0,0);
    while (true) {
        if (menu == false) {
            lcd.printf("menu\n");
            thread_sleep_for(500);
            lcd.cls();
        } else if (action == false) {
            lcd.printf("action\n");
            thread_sleep_for(500);
            lcd.cls();
        } else if (up == false) {
            lcd.printf("up\n");
            thread_sleep_for(500);
            lcd.cls();
        } else if (down == false) {
            lcd.printf("down\n");
            thread_sleep_for(500);
            lcd.cls();
        } else if (left == false) {
            lcd.printf("left\n");
            thread_sleep_for(500);
            lcd.cls();
        } else if (right == false) {
            lcd.printf("right\n");
            thread_sleep_for(500);
            lcd.cls();
        } else {
            lcd.cls();
        }
    }
}



