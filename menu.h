#include "TextLCD.h"

class Menu {
private:
    // Buttons that will be used in the class are passed by reference
    TextLCD &lcd;
    DigitalIn &up;
    DigitalIn &down;
    DigitalIn &left;
    DigitalIn &right;
    DigitalIn &action;

    int selectedOption;
    const char *options[2];

public:
    Menu(TextLCD &lcd, DigitalIn &up, DigitalIn &down, DigitalIn &left, DigitalIn &right, DigitalIn &action);
    
    void displayMenu();
    void navigateMenu(int direction);
    void selectOption();
};