#include "TextLCD.h"

class Menu {
private:
    TextLCD &lcd;
    int selectedOption;
    const char *options[2];

public:
    Menu(TextLCD &lcd);
    void displayMenu();
    void navigateMenu(int direction);
    void selectOption();
};