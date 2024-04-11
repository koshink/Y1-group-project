#include "TextLCD.h"

class Menu {
private:
    // Buttons that will be used in the class are passed by reference
    TextLCD &lcd;
    DigitalIn &up;
    DigitalIn &down;
    DigitalIn &left;
    DigitalIn &right;

    int selectedOption;
    const char *options[2];

public:
    Menu(TextLCD &lcd, DigitalIn &Up, DigitalIn &Down, DigitalIn &Left, DigitalIn &Right);
    
    void displayMenu();
    void navigateMenu(int direction);
    void selectOption();
};