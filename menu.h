#include "TextLCD.h"

class Menu {
private:
    TextLCD &lcd;
    DigitalIn &up;
    DigitalIn &down;
    int selectedOption;
    const char *options[2];

public:
    Menu(TextLCD &lcd, DigitalIn &up, DigitalIn &down);
    void displayMenu();
    void navigateMenu(int direction);
    void selectOption();
};