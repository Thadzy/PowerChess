#include "PowerChess/PowerUps/KnightBoostPowerUp.h"
#include "PowerChess/Core/Player.h"
#include "PowerChess/Core/Board.h"
#include "PowerChess/UI/ConsoleUI.h"
#include "PowerChess/Core/Knight.h" // To cast and call setBoostedForNextMove

namespace PowerChess {

    KnightBoostPowerUp::KnightBoostPowerUp()
        : PowerUp(PowerUpType::KNIGHT_BOOST, "Knight Boost", "Selected Knight gains enhanced movement for its next move.") {}

    std::unique_ptr<PowerUp> KnightBoostPowerUp::clone() const {
        return std::make_unique<KnightBoostPowerUp>(*this);
    }

    void KnightBoostPowerUp::apply(Player& player, Board& board, ConsoleUI& ui) {
        ui.displayMessage(player.getName() + " is using Knight Boost. Select one of your Knights.");
        
        Position knightPos(-1,-1);
        bool knightSelected = false;

        while(!knightSelected) {
            std::string posStr = ui.promptForPieceSelection("Enter position of Knight to boost (e.g., b1): ");
            if (posStr.length() == 2) {
                char file = posStr[0];
                char rank = posStr[1];
                 if (file >= 'a' && file <= 'h' && rank >= '1' && rank <= '8') {
                    knightPos = Position(7 - (rank - '1'), file - 'a');
                 } else {
                    ui.displayMessage("Invalid position format. Try again.");
                    continue;
                 }
            } else {
                 ui.displayMessage("Invalid input length. Position should be 2 characters (e.g., b1).");
                 continue;
            }


            if (knightPos.isValid()) {
                Piece* piece = board.getPiecePtr(knightPos);
                if (piece && piece->getColor() == player.getColor() && piece->getType() == PieceType::KNIGHT) {
                    Knight* knight = dynamic_cast<Knight*>(piece);
                    if (knight) {
                        knight->setBoostedForNextMove(true);
                        ui.displayMessage("Knight at " + ui.formatPosition(knightPos) + " has been boosted for its next move!");
                        knightSelected = true;
                    } else {
                        // Should not happen if type is KNIGHT
                        ui.displayMessage("Error: Piece is not a valid Knight object.");
                    }
                } else {
                    ui.displayMessage("No friendly Knight at " + ui.formatPosition(knightPos) + ". Try again.");
                }
            } else {
                ui.displayMessage("Invalid position. Try again.");
            }
        }
    }

} // namespace PowerChess