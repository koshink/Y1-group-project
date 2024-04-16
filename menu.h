#include "TextLCD.h"

class Menu {
public:
    Menu(TextLCD &lcd, DigitalIn &up, DigitalIn &down, DigitalIn &left, DigitalIn &right, DigitalIn &action, DigitalIn &menu);

    // Menu interaction methods
    void displayMenu();
    void navigateMenu(int direction);
    void selectOption();

private:
    // Display and buttons
    TextLCD &lcd;
    DigitalIn &up;
    DigitalIn &down;
    DigitalIn &left;
    DigitalIn &right;
    DigitalIn &action;
    DigitalIn &menu;

    // Menu options
    const char *options[2];
    int selectedOption;
};