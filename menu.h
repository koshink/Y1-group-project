#include "TextLCD.h"
#include "ElectronRace.h"
#include "QuizDuels.h"

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

    ElectronRace *electron = nullptr;
    QuizDuels *quiz = nullptr;

    // Menu options
    const char *options[2];
    int selectedOption;
};