#include "menu.h"
#include "ElectronRace.h"
#include "QuizDuels.h"

Menu::Menu(TextLCD &lcd, DigitalIn &up, DigitalIn &down, DigitalIn &left, DigitalIn &right, DigitalIn &action) : lcd(lcd), up(up), down(down), left(left), right(right), action(action), selectedOption(0) {

    // Add the different games as options
    options[0] = "Electron Race";
    options[1] = "Quiz Duels";
}

void Menu::displayMenu() {
    lcd.cls();
    // Calculate the number of objects in the array
    int optionsSize = sizeof(options) / sizeof(options[0]); 
    // Figure out which option is on top for scrolling
    int topOption = selectedOption < optionsSize - 1 ? selectedOption : optionsSize - 2; 
    // Display the options, putting an indicator if it is selected
    for (int i = topOption; i < topOption + 2 && i < optionsSize; i++) {
        lcd.locate(0, i - topOption);
        if (i == selectedOption) {
            lcd.printf(">%s", options[i]);
        } else {
            lcd.printf(" %s", options[i]);
        }
    }
}

void Menu::navigateMenu(int direction) {
    // Calculate the number of objects in the array
    int optionsSize = sizeof(options) / sizeof(options[0]);
    // Figure out the position of the selected option in the array 
    selectedOption += direction;
    // Wrap around to the end if it goes before the start
    if (selectedOption < 0) {
        selectedOption = optionsSize - 1;
    // Wrap around to the start if it goes past the end
    } else if (selectedOption >= optionsSize) { 
        selectedOption = 0; 
    }
    // Display the updated selection
    displayMenu();
}

void Menu::selectOption() {
    // Reset the cursor before printing the launch screen
    lcd.cls();
    lcd.locate(0, 0); lcd.printf("Launching");
    lcd.locate(0, 1); lcd.printf("%s", options[selectedOption]);
    thread_sleep_for(500);

    // Create a blank object of the games so they can relaunched without stack overflow errors
    ElectronRace *electron = nullptr;
    QuizDuels *quiz = nullptr;

    if (selectedOption == 0) {
        // Create a new ElectronRace object and start the game
        electron = new ElectronRace(lcd, up, down);
        electron->startGame();
    } else if (selectedOption == 1) {
        // Create a new QuizDuels object
        quiz = new QuizDuels(lcd, up, down, left, right, action);
        quiz->startGame();
    }


    // Delete the electronGame when on the main menu so it can be initialised again
    if (electron != nullptr) {
        delete electron;
        electron = nullptr;
    }
}