#include "PowerChess/UI/ConsoleUI.h"
#include "PowerChess/Core/Board.h"   // For accessing board grid and pieces
#include "PowerChess/Core/Player.h"  // For player info
#include "PowerChess/PowerUps/PowerUp.h" // For PowerUp info
#include "PowerChess/PowerUps/KnightBoostPowerUp.h" // For creating specific powerups
#include <iostream>
#include <limits> // For std::numeric_limits

namespace PowerChess {

    void ConsoleUI::displayBoard(const Board& board) const {
        std::cout << std::endl;
        std::cout << "    a b c d e f g h" << std::endl;
        std::cout << "  +-----------------+" << std::endl;
        for (int r = 0; r < 8; ++r) {
            std::cout << 8 - r << " | ";
            for (int c = 0; c < 8; ++c) {
                Piece* piece = board.getPiecePtr(Position(r, c));
                if (piece) {
                    std::cout << piece->getSymbol() << " ";
                } else {
                    // Alternating . and space for empty squares for better readability
                    std::cout << ((r + c) % 2 == 0 ? "." : " ") << " ";
                }
            }
            std::cout << "| " << 8 - r << std::endl;
        }
        std::cout << "  +-----------------+" << std::endl;
        std::cout << "    a b c d e f g h" << std::endl;
        std::cout << std::endl;
    }

    void ConsoleUI::displayPlayerStats(const Player& player1, const Player& player2) const {
        std::cout << "--- Player Stats ---" << std::endl;
        std::cout << player1.getName() << " (White): Hearts=" << player1.getHearts() << ", Score=" << player1.getScore() << std::endl;
        std::cout << player2.getName() << " (Black): Hearts=" << player2.getHearts() << ", Score=" << player2.getScore() << std::endl;
        std::cout << "--------------------" << std::endl;
    }

    void ConsoleUI::displayPlayerTurn(const Player* currentPlayer) const {
        if (currentPlayer) {
             std::cout << "--- " << currentPlayer->getName() << "'s Turn (" << currentPlayer->getColorString() << ") ---" << std::endl;
        }
    }

    void ConsoleUI::displayMessage(const std::string& message) const {
        std::cout << "[INFO] " << message << std::endl;
    }

    std::string ConsoleUI::getPlayerMove(const Player& player) const {
        std::string moveStr;
        std::cout << player.getName() << ", enter your move (e.g., e2e4 or a7a8q for promotion): ";
        std::cin >> moveStr;
        // Basic validation can be done here or in Game logic
        if (std::cin.fail()) {
            std::cin.clear(); // clear error flags
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard invalid input
            return "";
        }
        return moveStr;
    }
    
    std::string ConsoleUI::promptForPieceSelection(const std::string& promptMessage) const {
        std::string posStr;
        std::cout << promptMessage;
        std::cin >> posStr;
         if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return "";
        }
        return posStr;
    }


    void ConsoleUI::displayRoundResult(Player* winner) const {
        if (winner) {
            std::cout << ">>> Round Over! Winner: " << winner->getName() << " (" << winner->getColorString() << ") <<<" << std::endl;
        } else {
            std::cout << ">>> Round Over! It's a draw (Stalemate). <<<" << std::endl;
        }
    }

    void ConsoleUI::displayMatchResult(Player* winner) const {
        std::cout << "\n=========================" << std::endl;
        if (winner) {
            std::cout << ">>> MATCH OVER! Congratulations " << winner->getName() << ", you won the match! <<<" << std::endl;
        } else {
            std::cout << ">>> MATCH OVER! The match is a draw! <<<" << std::endl;
        }
        std::cout << "=========================\n" << std::endl;
    }

    std::unique_ptr<PowerUp> ConsoleUI::selectPowerUp(Player& player, 
                                                      const std::vector<std::unique_ptr<PowerUp>>& availablePowerUpsFactory) {
        // For now, only KnightBoost is available. More can be added.
        // This factory vector would ideally contain functions to create powerups, or just a list of types.
        // Simplified: assume only KnightBoost is always the option.

        std::cout << player.getName() << ", you lost the round. You can select a power-up for the next round." << std::endl;
        std::cout << "Available Power-ups:" << std::endl;
        // In a more advanced system, you'd list them from availablePowerUpsFactory
        std::cout << "1. Knight Boost (Selected Knight gains enhanced movement for its next move)" << std::endl;
        std::cout << "Enter the number of the power-up to select (or 0 to skip): ";

        int choice;
        std::cin >> choice;

        while (std::cin.fail() || choice < 0 || choice > 1) { // Assuming only 1 power-up for now
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid choice. Please enter 0 or 1: ";
            std::cin >> choice;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // consume newline

        if (choice == 1) {
            std::cout << "Knight Boost selected!" << std::endl;
            return std::make_unique<KnightBoostPowerUp>();
        }
        
        std::cout << "No power-up selected." << std::endl;
        return nullptr;
    }

    std::string ConsoleUI::formatPosition(Position pos) const {
        if (!pos.isValid()) return "??";
        char file = 'a' + pos.col;
        char rank = '1' + (7 - pos.row);
        return std::string(1, file) + std::string(1, rank);
    }
    
    PieceType ConsoleUI::promptPawnPromotionChoice(const Player& player) const {
        std::cout << player.getName() << ", your pawn can be promoted!" << std::endl;
        std::cout << "Choose piece to promote to (q=Queen, r=Rook, b=Bishop, n=Knight): ";
        char choiceChar;
        while (true) {
            std::cin >> choiceChar;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // consume rest of line
            switch (tolower(choiceChar)) {
                case 'q': return PieceType::QUEEN;
                case 'r': return PieceType::ROOK;
                case 'b': return PieceType::BISHOP;
                case 'n': return PieceType::KNIGHT;
                default: std::cout << "Invalid choice. Enter q, r, b, or n: ";
            }
        }
    }


} // namespace PowerChess