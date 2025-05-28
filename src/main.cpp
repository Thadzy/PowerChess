#include "PowerChess/Core/Game.h"
#include "PowerChess/Core/Player.h"
#include "PowerChess/UI/ConsoleUI.h"
#include "PowerChess/PowerUps/KnightBoostPowerUp.h" // For factory list
#include <iostream>
#include <vector>
#include <memory>

using namespace PowerChess;

int main() {
    ConsoleUI ui;
    Player player1("Player 1", Color::WHITE);
    Player player2("Player 2", Color::BLACK);

    int roundsToWinMatch = 2;
    int currentRoundNumber = 1;

    // Factory for available power-ups (could be more dynamic)
    // For simplicity, just know KnightBoost is available. The UI will offer it.
    // std::vector<std::unique_ptr<PowerUp>> powerUpFactory;
    // powerUpFactory.push_back(std::make_unique<KnightBoostPowerUp>());


    ui.displayMessage("Welcome to PowerChess!");
    ui.displayMessage("Win " + std::to_string(roundsToWinMatch) + " rounds to win the match. Each player has 3 hearts.");

    while (player1.getScore() < roundsToWinMatch && player2.getScore() < roundsToWinMatch &&
           player1.getHearts() > 0 && player2.getHearts() > 0) {
        
        ui.displayMessage("\n--- Starting Round " + std::to_string(currentRoundNumber) + " ---");
        ui.displayPlayerStats(player1, player2);

        Game currentRound(&player1, &player2, ui);
        currentRound.startRound();

        while (!currentRound.isRoundOver()) {
            currentRound.playTurn();
        }

        Player* roundWinner = currentRound.getRoundWinner();
        Player* roundLoser = nullptr;

        if (roundWinner) {
            roundWinner->incrementScore();
            ui.displayRoundResult(roundWinner); // Displays winner
            
            if (roundWinner == &player1) roundLoser = &player2;
            else roundLoser = &player1;

            roundLoser->loseHeart();
            ui.displayMessage(roundLoser->getName() + " loses a heart! Hearts remaining: " + std::to_string(roundLoser->getHearts()));

            if (roundLoser->getHearts() == 0) {
                ui.displayMessage(roundLoser->getName() + " has run out of hearts and loses the match!");
                // The other player wins automatically
                if (roundWinner->getScore() < roundsToWinMatch) { // Ensure they didn't already win this round
                     roundWinner->incrementScore(); // Give them enough score to win
                     while(roundWinner->getScore() < roundsToWinMatch) roundWinner->incrementScore();
                }
                break; // Exit match loop
            }

            // Offer power-up if match not over and loser has hearts
            if (player1.getScore() < roundsToWinMatch && player2.getScore() < roundsToWinMatch) {
                 std::vector<std::unique_ptr<PowerUp>> factory_list_for_ui_selection; 
                 // In a real scenario with multiple powerups, populate this.
                 // For now, UI::selectPowerUp will just offer KnightBoost.
                std::unique_ptr<PowerUp> chosenPowerUp = ui.selectPowerUp(*roundLoser, factory_list_for_ui_selection);
                if (chosenPowerUp) {
                    roundLoser->setActivePowerUp(std::move(chosenPowerUp));
                }
            }

        } else { // Stalemate (draw)
            ui.displayRoundResult(nullptr); // Display draw
            ui.displayMessage("No hearts lost this round.");
        }
        currentRoundNumber++;
    }

    ui.displayMessage("\n--- Match Finished ---");
    ui.displayPlayerStats(player1, player2);

    if (player1.getScore() >= roundsToWinMatch) {
        ui.displayMatchResult(&player1);
    } else if (player2.getScore() >= roundsToWinMatch) {
        ui.displayMatchResult(&player2);
    } else if (player1.getHearts() == 0 && player2.getHearts() > 0) {
        ui.displayMessage(player1.getName() + " ran out of hearts.");
        ui.displayMatchResult(&player2);
    } else if (player2.getHearts() == 0 && player1.getHearts() > 0) {
         ui.displayMessage(player2.getName() + " ran out of hearts.");
        ui.displayMatchResult(&player1);
    }
    else {
        // This case should ideally be covered by scores or heart conditions leading to a win.
        // If both reach 0 hearts simultaneously (unlikely with current flow) or scores are tied.
        ui.displayMessage("The match outcome is undetermined by score or hearts (edge case).");
    }

    return 0;
}