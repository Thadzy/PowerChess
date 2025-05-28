#ifndef HARDCHESS_UI_CONSOLEUI_H
#define HARDCHESS_UI_CONSOLEUI_H

#include <string>
#include <vector>
#include <memory>
#include "Header/Core/CommonTypes.h" // For Position, PieceType

namespace HardChess
{
    // Forward declarations
    class Board;
    class Player;

    class ConsoleUI
    {
    public:
        ConsoleUI() = default;

        // Display functions
        void displayWelcomeScreen() const;
        int displayMainMenu() const; // Returns user choice
        void displayBoard(const Board &board) const;
        void displayPlayerStats(const Player &player1, const Player &player2) const; // Pass by const ref
        void displayCurrentPlayerTurn(const Player &player) const; // Pass by const ref
        void displayMessage(const std::string &message) const;
        void displayError(const std::string &error) const;
        void displaySuccess(const std::string &success) const;
        void displayRoundResult(RoundState result, const std::string& winnerName) const;
        void displayMatchResult(const Player* winner, const Player& p1, const Player& p2) const; // nullptr for draw
        // void displayGameSaved(const std::string& filename) const; // For future
        // void displayGameLoaded(const std::string& filename) const; // For future
        void displayHelp() const;
        void displayGoodbye() const;

        // Input functions
        void getMoveInput(const Player& player, std::string& startStr, std::string& endStr) const;
        PieceType promptPawnPromotionChoice(const Player &player) const;
        // std::string getSaveFileName() const; // For future
        // std::string getLoadFileName() const; // For future
        std::string getPlayerName(const std::string& prompt) const;


        // Utility functions
        void clearScreen() const;
        void pauseForUser() const;
        std::string formatPosition(Position pos) const; // If needed for messages
    };

} // namespace HardChess

#endif // HARDCHESS_UI_CONSOLEUI_H