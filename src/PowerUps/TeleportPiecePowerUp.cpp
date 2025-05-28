// #include "PowerChess/PowerUps/TeleportPiecePowerUp.h"
// #include "PowerChess/Core/Player.h"
// #include "PowerChess/Core/Board.h"
// #include "PowerChess/UI/ConsoleUI.h"

// namespace PowerChess {

//     TeleportPiecePowerUp::TeleportPiecePowerUp()
//         : PowerUp(PowerUpType::TELEPORT_PIECE, "Teleport", "Move any piece to an empty square without capturing.") {}

//     std::unique_ptr<PowerUp> TeleportPiecePowerUp::clone() const {
//         return std::make_unique<TeleportPiecePowerUp>(*this);
//     }

//     void TeleportPiecePowerUp::apply(Player& player, Board& board, ConsoleUI& ui) {
//         ui.displayMessage(player.getName() + " is using Teleport. Select a piece to teleport.");
        
//         Position fromPos(-1,-1);
//         Position toPos(-1,-1);
//         bool pieceSelected = false;
//         bool targetSelected = false;

//         // Select piece to teleport
//         while(!pieceSelected) {
//             std::string posStr = ui.promptForPieceSelection("Enter position of piece to teleport (e.g., b1): ");
//             if (posStr.length() == 2) {
//                 char file = posStr[0];
//                 char rank = posStr[1];
//                  if (file >= 'a' && file <= 'h' && rank >= '1' && rank <= '8') {
//                     fromPos = Position(7 - (rank - '1'), file - 'a');
//                  } else {
//                     ui.displayMessage("Invalid position format. Try again.");
//                     continue;
//                  }
//             } else {
//                  ui.displayMessage("Invalid input length. Position should be 2 characters (e.g., b1).");
//                  continue;
//             }

//             if (fromPos.isValid()) {
//                 Piece* piece = board.getPiecePtr(fromPos);
//                 if (piece && piece->getColor() == player.getColor()) {
//                     ui.displayMessage("Selected " + piece->getTypeName() + " at " + ui.formatPosition(fromPos));
//                     pieceSelected = true;
//                 } else {
//                     ui.displayMessage("No friendly piece at " + ui.formatPosition(fromPos) + ". Try again.");
//                 }
//             } else {
//                 ui.displayMessage("Invalid position. Try again.");
//             }
//         }

//         // Select target position
//         while(!targetSelected) {
//             std::string posStr = ui.promptForPieceSelection("Enter target position (empty square only, e.g., d4): ");
//             if (posStr.length() == 2) {
//                 char file = posStr[0];
//                 char rank = posStr[1];
//                  if (file >= 'a' && file <= 'h' && rank >= '1' && rank <= '8') {
//                     toPos = Position(7 - (rank - '1'), file - 'a');
//                  } else {
//                     ui.displayMessage("Invalid position format. Try again.");
//                     continue;
//                  }
//             } else {
//                  ui.displayMessage("Invalid input length. Position should be 2 characters (e.g., d4).");
//                  continue;
//             }

//             if (toPos.isValid()) {
//                 Piece* targetPiece = board.getPiecePtr(toPos);
//                 if (!targetPiece) {
//                     // Empty square - perform teleport
//                     Piece* piece = board.getPiecePtr(fromPos);
//                     board.movePiece(fromPos, toPos); // Use existing move method
//                     ui.displayMessage("Piece teleported from " + ui.formatPosition(fromPos) + " to " + ui.formatPosition(toPos) + "!");
//                     targetSelected = true;
//                 } else {
//                     ui.displayMessage("Target square is occupied. Teleport can only move to empty squares. Try again.");
//                 }
//             } else {
//                 ui.displayMessage("Invalid position. Try again.");
//             }
//         }
//     }

// } // namespace PowerChess