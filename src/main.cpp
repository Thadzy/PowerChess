#include "Header/Core/Game.h"
#include "Header/UI/ConsoleUI.h"
#include "Header/Core/Player.h"
#include <iostream>
#include <string>
#include <limits> // Required for std::numeric_limits

using namespace HardChess;

const int INITIAL_HEARTS = 3;
const int ROUNDS_TO_WIN_MATCH = 2; // Win match if score reaches this (or opponent hearts = 0)

int main()
{
    ConsoleUI ui;
    ui.clearScreen();
    ui.displayWelcomeScreen();
    ui.pauseForUser();

    bool running = true;
    while (running)
    {
        ui.clearScreen();
        int choice = ui.displayMainMenu();
        switch (choice)
        {
        case 1: // Start New Match
        {
            ui.clearScreen();
            ui.displayMessage("--- Starting New Match ---");

            std::string name1 = ui.getPlayerName("Enter name for White player: ");
            std::string name2 = ui.getPlayerName("Enter name for Black player: ");

            Player player1(name1, Color::WHITE, INITIAL_HEARTS);
            Player player2(name2, Color::BLACK, INITIAL_HEARTS);

            ui.displaySuccess("Players set: " + player1.getName() + " (White) vs " + player2.getName() + " (Black)");
            ui.pauseForUser();

            bool matchOver = false;
            while (!matchOver)
            {
                Game gameRound(&player1, &player2, ui); // Create a new game/round instance
                RoundState roundResult = gameRound.playRound(); // Play one round

                // Update scores and hearts based on round result
                if (roundResult == RoundState::CHECKMATE_WHITE_WINS)
                {
                    player1.incrementScore();
                    player2.loseHeart();
                }
                else if (roundResult == RoundState::CHECKMATE_BLACK_WINS)
                {
                    player2.incrementScore();
                    player1.loseHeart();
                }
                // Stalemate: no score change, no heart loss (or decide if it should cost a heart)

                // Check match end conditions
                if (player1.getScore() >= ROUNDS_TO_WIN_MATCH || player2.getHearts() == 0)
                {
                    ui.displayMatchResult(&player1, player1, player2);
                    matchOver = true;
                }
                else if (player2.getScore() >= ROUNDS_TO_WIN_MATCH || player1.getHearts() == 0)
                {
                    ui.displayMatchResult(&player2, player1, player2);
                    matchOver = true;
                }
                // Optional: Check for a draw match if both players run out of hearts simultaneously
                // or if a certain number of rounds are played without a clear winner based on score.
                
                if (matchOver) {
                    ui.pauseForUser();
                } else {
                    // Prepare for next round (board is re-initialized in gameRound.startRound())
                    ui.displayMessage("--- Preparing for Next Round ---");
                    ui.pauseForUser();
                }
            }
            break;
        }
        case 2: // Help & Rules
            ui.clearScreen();
            ui.displayHelp();
            break;
        case 3: // Exit Game
            ui.clearScreen();
            ui.displayGoodbye();
            running = false;
            break;
        default: // Should not happen due to input validation in displayMainMenu
            ui.displayError("Invalid choice! Please try again.");
            ui.pauseForUser();
            break;
        }
    }
    return 0;
}