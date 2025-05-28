#ifndef POWERCHESS_UI_CONSOLEUI_H
#define POWERCHESS_UI_CONSOLEUI_H

#include <string>
#include <vector>
#include <memory>

#include "Header/Core/CommonTypes.h"

namespace HardChess {
    class Board;    
    class Player;   
    class PowerUp;  
}

namespace HardChess {

    class ConsoleUI {
    public:
        ConsoleUI() = default;

        // Display functions
        void displayWelcomeScreen() const;
        int displayMainMenu() const;
        void displayBoard(const Board& board) const;
        void displayPlayerStats(const Player& player1, const Player& player2) const;
        void displayPlayerTurn(const Player* currentPlayer) const;
        void displayMessage(const std::string& message) const;
        void displayError(const std::string& error) const;
        void displaySuccess(const std::string& success) const;
        void displayRoundResult(Player* winner) const; // nullptr for draw
        void displayMatchResult(Player* winner) const; // nullptr for draw
        void displayGameSaved(const std::string& filename) const;
        void displayGameLoaded(const std::string& filename) const;
        void displayHelp() const;
        void displayGoodbye() const;

        // Input functions
        std::string getPlayerMove(const Player& player) const;
        std::string promptForPieceSelection(const std::string& promptMessage) const;
        PieceType promptPawnPromotionChoice(const Player& player) const;
        std::string getSaveFileName() const;
        std::string getLoadFileName() const;
        
        std::string formatPosition(Position pos) const; 

        // Utility functions
        void clearScreen() const;
        void pauseForUser() const;
    };

} // namespace PowerChess

#endif // POWERCHESS_UI_CONSOLEUI_H