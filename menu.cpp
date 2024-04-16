#include "menu.h"
#include "ElectronRace.h"
#include "QuizDuels.h"

Menu::Menu(TextLCD &lcd, DigitalIn &up, DigitalIn &down, DigitalIn &left, DigitalIn &right, DigitalIn &action, DigitalIn &menu) : lcd(lcd), up(up), down(down), left(left), right(right), action(action), menu(menu), selectedOption(0) {

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
        lcd.locate(0, i - topOption); lcd.printf((i == selectedOption) ? ">%s" : " %s", options[i]);
    }
}

void Menu::navigateMenu(int direction) {
    // Calculate the number of objects in the array
    int optionsSize = sizeof(options) / sizeof(options[0]);
    // Figure out the position of the selected option in the array 
    selectedOption += direction;
    // Wrap around to the end if it goes before the start
    selectedOption = (selectedOption < 0) ? optionsSize - 1 : (selectedOption >= optionsSize) ? 0 : selectedOption;
    // Display the updated selection
    displayMenu();
    thread_sleep_for(200);
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

    // Depending on the selected option, create a new game object and start the game
    if (selectedOption == 0) {
        electron = new ElectronRace(lcd, up, down, menu);
        electron->startGame();
    } else {
        quiz = new QuizDuels(lcd, up, down, left, right, action, menu);
        quiz->startGame();
    }

    // Delete the electronGame when on the main menu so it can be initialised again
    if (electron) {
        delete electron;
        electron = nullptr;
    }

    // Delete the quiz game when on the main menu so it can be initialised again
    if (quiz) {
        delete quiz;
        quiz = nullptr;
    }
}