#include "PowerChess/Core/Game.h"
#include "PowerChess/Core/Player.h"
#include "PowerChess/UI/ConsoleUI.h"
#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include <string>

using namespace PowerChess;

// Function prototypes
void playMatch(ConsoleUI& ui, Player& player1, Player& player2, int roundsToWinMatch);
bool saveGame(ConsoleUI& ui, const Player& player1, const Player& player2, int currentRound);
bool loadGame(ConsoleUI& ui, Player& player1, Player& player2, int& currentRound);
void getPlayerNames(ConsoleUI& ui, std::string& name1, std::string& name2);

int main() {
    ConsoleUI ui;
    bool gameRunning = true;
    
    // Game state variables
    Player player1("Player 1", Color::WHITE);
    Player player2("Player 2", Color::BLACK);
    int roundsToWinMatch = 2;
    int currentRoundNumber = 1;
    bool gameInProgress = false;

    // Display welcome screen
    ui.clearScreen();
    ui.displayWelcomeScreen();
    
    while (gameRunning) {
        int choice = ui.displayMainMenu();
        
        switch (choice) {
            case 1: { // Start New Game
                ui.clearScreen();
                ui.displaySuccess("Starting new game! 🎮");
                
                // Get player names
                std::string name1, name2;
                getPlayerNames(ui, name1, name2);
                
                // Reset players
                player1 = Player(name1, Color::WHITE);
                player2 = Player(name2, Color::BLACK);
                currentRoundNumber = 1;
                gameInProgress = true;
                
                // Play the match
                playMatch(ui, player1, player2, roundsToWinMatch);
                gameInProgress = false;
                
                ui.pauseForUser();
                break;
            }
            
            case 2: { // Save Current Game
                if (gameInProgress) {
                    if (saveGame(ui, player1, player2, currentRoundNumber)) {
                        ui.displaySuccess("Game saved successfully! 💾");
                    } else {
                        ui.displayError("Failed to save game! 😞");
                    }
                } else {
                    ui.displayError("No game in progress to save!");
                }
                ui.pauseForUser();
                break;
            }
            
            case 3: { // Load Saved Game
                if (loadGame(ui, player1, player2, currentRoundNumber)) {
                    ui.displaySuccess("Game loaded successfully! 📂");
                    gameInProgress = true;
                    
                    // Continue the loaded game
                    playMatch(ui, player1, player2, roundsToWinMatch);
                    gameInProgress = false;
                } else {
                    ui.displayError("Failed to load game! 😞");
                }
                ui.pauseForUser();
                break;
            }
            
            case 4: { // Game Settings
                ui.clearScreen();
                ui.displayMessage("⚙️  Game Settings");
                ui.displayMessage("Current settings:");
                ui.displayMessage("• Rounds to win match: " + std::to_string(roundsToWinMatch));
                ui.displayMessage("• Starting hearts per player: 3");
                
                std::cout << "\nWould you like to change rounds to win? (y/n): ";
                char changeRounds;
                std::cin >> changeRounds;
                
                if (tolower(changeRounds) == 'y') {
                    std::cout << "Enter new rounds to win (1-5): ";
                    int newRounds;
                    while (!(std::cin >> newRounds) || newRounds < 1 || newRounds > 5) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "Invalid input! Enter 1-5: ";
                    }
                    roundsToWinMatch = newRounds;
                    ui.displaySuccess("Rounds to win updated to " + std::to_string(roundsToWinMatch) + "! ✅");
                }
                
                ui.pauseForUser();
                break;
            }
            
            case 5: { // View Statistics
                ui.clearScreen();
                ui.displayMessage("📊 Game Statistics");
                if (gameInProgress || player1.getScore() > 0 || player2.getScore() > 0) {
                    ui.displayPlayerStats(player1, player2);
                    ui.displayMessage("Current round: " + std::to_string(currentRoundNumber));
                    ui.displayMessage("Rounds needed to win: " + std::to_string(roundsToWinMatch));
                } else {
                    ui.displayMessage("No game statistics available. Start a new game first!");
                }
                ui.pauseForUser();
                break;
            }
            
            case 6: { // Help & Rules
                ui.clearScreen();
                ui.displayHelp();
                break;
            }
            
            case 7: { // Exit Game
                ui.displayGoodbye();
                gameRunning = false;
                break;
            }
            
            default:
                ui.displayError("Invalid choice! Please try again.");
                break;
        }
    }

    return 0;
}

void playMatch(ConsoleUI& ui, Player& player1, Player& player2, int roundsToWinMatch) {
    int currentRoundNumber = 1;
    
    ui.displayMessage("🎯 Welcome to PowerChess! 🎯");
    ui.displayMessage("Win " + std::to_string(roundsToWinMatch) + " rounds to win the match. Each player has 3 hearts. ❤️");

    while (player1.getScore() < roundsToWinMatch && player2.getScore() < roundsToWinMatch &&
           player1.getHearts() > 0 && player2.getHearts() > 0) {
        
        ui.displayMessage("\n🎮 ═══ Starting Round " + std::to_string(currentRoundNumber) + " ═══ 🎮");
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
            ui.displayMessage("💔 " + roundLoser->getName() + " loses a heart! Hearts remaining: " + std::to_string(roundLoser->getHearts()));

            if (roundLoser->getHearts() == 0) {
                ui.displayMessage("💀 " + roundLoser->getName() + " has run out of hearts and loses the match!");
                // The other player wins automatically
                if (roundWinner->getScore() < roundsToWinMatch) {
                     roundWinner->incrementScore();
                     while(roundWinner->getScore() < roundsToWinMatch) roundWinner->incrementScore();
                }
                break; // Exit match loop
            }

        } else { // Stalemate (draw)
            ui.displayRoundResult(nullptr); // Display draw
            ui.displayMessage("🤝 No hearts lost this round.");
        }
        currentRoundNumber++;
    }

    ui.displayMessage("\n🏁 ═══ Match Finished ═══ 🏁");
    ui.displayPlayerStats(player1, player2);

    if (player1.getScore() >= roundsToWinMatch) {
        ui.displayMatchResult(&player1);
    } else if (player2.getScore() >= roundsToWinMatch) {
        ui.displayMatchResult(&player2);
    } else if (player1.getHearts() == 0 && player2.getHearts() > 0) {
        ui.displayMessage("💀 " + player1.getName() + " ran out of hearts.");
        ui.displayMatchResult(&player2);
    } else if (player2.getHearts() == 0 && player1.getHearts() > 0) {
         ui.displayMessage("💀 " + player2.getName() + " ran out of hearts.");
        ui.displayMatchResult(&player1);
    }
    else {
        ui.displayMessage("🤷 The match outcome is undetermined (edge case).");
    }
}

bool saveGame(ConsoleUI& ui, const Player& player1, const Player& player2, int currentRound) {
    std::string filename = ui.getSaveFileName();
    
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    // Save game state in a simple format
    file << "PowerChess Save File v1.0\n";
    file << player1.getName() << "\n";
    file << player1.getScore() << "\n";
    file << player1.getHearts() << "\n";
    file << player2.getName() << "\n";
    file << player2.getScore() << "\n";
    file << player2.getHearts() << "\n";
    file << currentRound << "\n";
    
    file.close();
    ui.displayGameSaved(filename);
    return true;
}

bool loadGame(ConsoleUI& ui, Player& player1, Player& player2, int& currentRound) {
    std::string filename = ui.getLoadFileName();
    
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    std::string header;
    std::getline(file, header);
    
    if (header != "PowerChess Save File v1.0") {
        file.close();
        return false;
    }
    
    std::string name1, name2;
    int score1, score2, hearts1, hearts2;
    
    std::getline(file, name1);
    file >> score1 >> hearts1;
    file.ignore(); // consume newline
    std::getline(file, name2);
    file >> score2 >> hearts2 >> currentRound;
    
    file.close();
    
    // Restore player states
    player1 = Player(name1, Color::WHITE);
    player2 = Player(name2, Color::BLACK);
    
    // Set scores and hearts (you'll need setter methods in Player class)
    for (int i = 0; i < score1; i++) player1.incrementScore();
    for (int i = 0; i < score2; i++) player2.incrementScore();
    player1.setHearts(hearts1);
    player2.setHearts(hearts2);
    
    ui.displayGameLoaded(filename);
    return true;
}

void getPlayerNames(ConsoleUI& ui, std::string& name1, std::string& name2) {
    ui.displayMessage("👥 Player Setup");
    
    std::cout << "♔ Enter name for White player: ";
    std::cin.ignore(); // Clear any leftover newline
    std::getline(std::cin, name1);
    if (name1.empty()) name1 = "White Player";
    
    std::cout << "♚ Enter name for Black player: ";
    std::getline(std::cin, name2);
    if (name2.empty()) name2 = "Black Player";
    
    ui.displaySuccess("Players set: " + name1 + " vs " + name2 + " ⚔️");
}