#ifndef POWERCHESS_UI_CONSOLEUI_H
#define POWERCHESS_UI_CONSOLEUI_H

#include <string>
#include <vector>
#include "PowerChess/Core/CommonTypes.h" // For Board, Player, Position, PowerUp forward declares if needed
                                        // Actually needs full Board and Player for some methods.

// Forward declarations to minimize include dependencies in header
namespace PowerChess {
    class Board; 
    class Player;
    class PowerUp; // For displaying power-up info or selection
    enum class PowerUpChoice; // If using enum for selection
}


namespace PowerChess {

    class ConsoleUI {
      public:
        ConsoleUI() = default;

        void displayBoard(const Board& board) const;
        void displayPlayerStats(const Player& player1, const Player& player2) const;
        void displayPlayerTurn(const Player* currentPlayer) const;
        void displayMessage(const std::string& message) const;
        
        std::string getPlayerMove(const Player& player) const;
        std::string promptForPieceSelection(const std::string& promptMessage) const;


        void displayRoundResult(Player* winner) const; // nullptr for draw
        void displayMatchResult(Player* winner) const; // nullptr for draw

        // Power-up selection
        // Returns the chosen PowerUp (caller takes ownership) or nullptr if none chosen/available
        std::unique_ptr<PowerUp> selectPowerUp(Player& player, 
                                               const std::vector<std::unique_ptr<PowerUp>>& availablePowerUpsFactory);

        std::string formatPosition(Position pos) const; // Helper to convert Position to "a1" string

        PieceType promptPawnPromotionChoice(const Player& player) const;
    };

} // namespace PowerChess

#endif // POWERCHESS_UI_CONSOLEUI_H