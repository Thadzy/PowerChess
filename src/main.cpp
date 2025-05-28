#include "Header/Core/Game.h"
#include "Header/UI/ConsoleUI.h"
#include "Header/Core/Player.h"
#include <iostream>
#include <string>

using namespace HardChess;

int main() {
    ConsoleUI ui;
    ui.clearScreen();
    ui.displayWelcomeScreen();

    bool running = true;
    while (running) {
        int choice = ui.displayMainMenu();
        switch (choice) {
            case 1: { // Start New Game
                ui.clearScreen();
                ui.displaySuccess("Starting new game! 🎮");

                std::string name1, name2;
                ui.displayMessage("👥 Player Setup");
                std::cout << "♔ Enter name for White player: ";
                std::cin.ignore();
                std::getline(std::cin, name1);
                if (name1.empty()) name1 = "White Player";
                std::cout << "♚ Enter name for Black player: ";
                std::getline(std::cin, name2);
                if (name2.empty()) name2 = "Black Player";
                ui.displaySuccess("Players set: " + name1 + " vs " + name2 + " ⚔️");

                Player player1(name1, Color::WHITE);
                Player player2(name2, Color::BLACK);

                int roundsToWinMatch = 2;
                Game match(&player1, &player2, ui);
                match.play();

                ui.pauseForUser();
                break;
            }
            case 2: // Help & Rules
                ui.clearScreen();
                ui.displayHelp();
                break;
            case 3: // Exit Game
                ui.displayGoodbye();
                running = false;
                break;
            default:
                ui.displayError("Invalid choice! Please try again.");
                break;
        }
    }
    return 0;
}