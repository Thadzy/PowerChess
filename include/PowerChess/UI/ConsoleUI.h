#ifndef POWERCHESS_UI_CONSOLEUI_H
#define POWERCHESS_UI_CONSOLEUI_H

#include <string>
#include <vector>
#include <memory> // Required for std::unique_ptr

// Include CommonTypes.h to define Position and PieceType fully.
// Why: When a type like 'Position' is passed by value (e.g., 'Position pos'),
// the compiler needs to know its exact size and structure to properly
// allocate memory and copy data. Forward declarations are only sufficient
// for pointers or references, where the compiler only needs to know the type exists.
#include "PowerChess/Core/CommonTypes.h" 

// Forward declarations for classes that are only used via pointers/references
// and whose full definitions are not needed in this header for compilation.
namespace PowerChess {
    class Board;    
    class Player;   
    class PowerUp;  
    // PieceType is now included from CommonTypes.h, so no need for forward declaration here.
    // However, if PieceType was a simple 'enum' (not 'enum class') and not used by value,
    // a forward declaration 'enum PieceType;' might still be valid in some cases,
    // but including its definition is generally safer and clearer.
}

namespace PowerChess {

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
        
        // Power-up selection
        // Returns the chosen PowerUp (caller takes ownership via unique_ptr) or nullptr if none chosen.
        // std::unique_ptr ensures proper memory management.
        // std::unique_ptr<PowerUp> selectPowerUp(Player& player, 
        //                                        const std::vector<std::unique_ptr<PowerUp>>& availablePowerUpsFactory);

        // Helper function: 'Position' is now recognized due to the include of CommonTypes.h
        std::string formatPosition(Position pos) const; 

        // Utility functions
        void clearScreen() const;
        void pauseForUser() const;
    };

} // namespace PowerChess

#endif // POWERCHESS_UI_CONSOLEUI_H