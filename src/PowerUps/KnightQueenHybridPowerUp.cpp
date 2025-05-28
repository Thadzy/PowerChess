// #include "PowerChess/PowerUps/KnightQueenHybridPowerUp.h"
// #include "PowerChess/Core/Player.h"
// #include "PowerChess/Core/Board.h"
// #include "PowerChess/UI/ConsoleUI.h"
// #include "PowerChess/Core/Knight.h"

// namespace PowerChess {

//     KnightQueenHybridPowerUp::KnightQueenHybridPowerUp()
//         : PowerUp(PowerUpType::KNIGHT_QUEEN_HYBRID, "Knight-Queen Hybrid", "Selected Knight gains Queen abilities for its next move.") {}

//     std::unique_ptr<PowerUp> KnightQueenHybridPowerUp::clone() const {
//         return std::make_unique<KnightQueenHybridPowerUp>(*this);
//     }

//     void KnightQueenHybridPowerUp::apply(Player& player, Board& board, ConsoleUI& ui) {
//         ui.displayMessage(player.getName() + " is using Knight-Queen Hybrid. Select one of your Knights.");
        
//         Position knightPos(-1,-1);
//         bool knightSelected = false;

//         while(!knightSelected) {
//             std::string posStr = ui.promptForPieceSelection("Enter position of Knight to enhance (e.g., b1): ");
//             if (posStr.length() == 2) {
//                 char file = posStr[0];
//                 char rank = posStr[1];
//                  if (file >= 'a' && file <= 'h' && rank >= '1' && rank <= '8') {
//                     knightPos = Position(7 - (rank - '1'), file - 'a');
//                  } else {
//                     ui.displayMessage("Invalid position format. Try again.");
//                     continue;
//                  }
//             } else {
//                  ui.displayMessage("Invalid input length. Position should be 2 characters (e.g., b1).");
//                  continue;
//             }

//             if (knightPos.isValid()) {
//                 Piece* piece = board.getPiecePtr(knightPos);
//                 if (piece && piece->getColor() == player.getColor() && piece->getType() == PieceType::KNIGHT) {
//                     Knight* knight = dynamic_cast<Knight*>(piece);
//                     if (knight) {
//                         knight->setQueenAbilityForNextMove(true); // You'll need to add this method to Knight class
//                         ui.displayMessage("Knight at " + ui.formatPosition(knightPos) + " now has Queen abilities for its next move!");
//                         knightSelected = true;
//                     } else {
//                         ui.displayMessage("Error: Piece is not a valid Knight object.");
//                     }
//                 } else {
//                     ui.displayMessage("No friendly Knight at " + ui.formatPosition(knightPos) + ". Try again.");
//                 }
//             } else {
//                 ui.displayMessage("Invalid position. Try again.");
//             }
//         }
//     }

// } // namespace PowerChess