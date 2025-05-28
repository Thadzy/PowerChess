#include "PowerChess/Core/Game.h"
#include "PowerChess/Core/Knight.h" // For resetting Knight boost
#include <iostream>
#include <algorithm> // for std::all_of, std::none_of if needed

namespace PowerChess {

    Game::Game(Player* p1, Player* p2, ConsoleUI& consoleUi)
        : player1(p1), player2(p2), currentPlayer(nullptr), ui(consoleUi), roundState(RoundState::ONGOING) {
        // Board is default constructed and initializes itself
    }

    void Game::startRound() {
        board.initializeBoard(); // Reset board to starting positions
        currentPlayer = player1; // White always starts
        roundState = RoundState::ONGOING;
        ui.displayMessage("Round started. " + currentPlayer->getName() + " (" + currentPlayer->getColorString() + ") to move.");

        // Apply power-ups at the start of the round
        auto apply_powerup_if_any = [&](Player* p) {
            if (p->hasActivePowerUp()) {
                std::unique_ptr<PowerUp> pu = p->consumeActivePowerUp();
                ui.displayMessage(p->getName() + " is activating " + pu->getName() + "!");
                pu->apply(*p, board, ui); // Pass ui for interaction (e.g. selecting Knight)
            }
        };
        apply_powerup_if_any(player1);
        apply_powerup_if_any(player2);

        ui.displayBoard(board); // Display initial board
    }

    Position Game::parsePosition(const std::string& s) const {
        if (s.length() < 2) return Position(-1, -1);
        char file = s[0];
        char rank = s[1];

        if (file < 'a' || file > 'h' || rank < '1' || rank > '8') {
            return Position(-1, -1);
        }
        // Board: (0,0) is a8, (7,7) is h1
        // Input: a1 ... h8
        // 'a' maps to col 0, 'h' to col 7
        // '1' maps to row 7, '8' to row 0
        return Position(7 - (rank - '1'), file - 'a');
    }
    
    bool Game::makeMove(Position start, Position end, PieceType promotionType) {
        if (!start.isValid() || !end.isValid()) {
            ui.displayMessage("Invalid position format or out of bounds.");
            return false;
        }

        Piece* pieceToMove = board.getPiecePtr(start);
        if (!pieceToMove) {
            ui.displayMessage("No piece at starting position " + ui.formatPosition(start) + ".");
            return false;
        }

        if (pieceToMove->getColor() != currentPlayer->getColor()) {
            ui.displayMessage("Cannot move opponent's piece.");
            return false;
        }
        
        // Check if the piece-specific move is valid (ignoring self-check for now)
        if (!pieceToMove->isValidMove(start, end, board)) {
             ui.displayMessage("Piece at " + ui.formatPosition(start) + " cannot move to " + ui.formatPosition(end) + " according to its rules.");
            return false;
        }

        // Simulate move to check for self-check
        Board tempBoard = board; // Use copy constructor
        std::unique_ptr<Piece> tempCaptured = tempBoard.movePiece(start, end); // Simulate on temp board
        
        if (tempBoard.isKingInCheck(currentPlayer->getColor())) {
            ui.displayMessage("Invalid move: Your King would be in check.");
            return false; // Move is illegal as it leaves King in check
        }

        // If all checks pass, make the move on the actual board
        std::unique_ptr<Piece> capturedPiece = board.movePiece(start, end);
        if (capturedPiece) {
            ui.displayMessage(capturedPiece->getColorString() + " " + capturedPiece->getName() + " captured at " + ui.formatPosition(end));
        }

        // Handle Knight Boost reset
        Piece* movedPiecePtr = board.getPiecePtr(end); // Get the piece that just moved
        if (movedPiecePtr && movedPiecePtr->getType() == PieceType::KNIGHT) {
            Knight* knight = dynamic_cast<Knight*>(movedPiecePtr);
            if (knight && knight->isBoostedForNextMove()) {
                knight->setBoostedForNextMove(false);
                ui.displayMessage(knight->getColorString() + " Knight's boost has been used.");
            }
        }

        // Handle Pawn Promotion
        if (pieceToMove->getType() == PieceType::PAWN) {
            Color color = pieceToMove->getColor();
            if ((color == Color::WHITE && end.row == 0) || (color == Color::BLACK && end.row == 7)) {
                if (promotionType != PieceType::NONE) {
                    board.promotePawn(end, promotionType);
                    ui.displayMessage("Pawn promoted at " + ui.formatPosition(end) + "!");
                } else {
                    // This case should be handled by getPlayerMove asking for promotion type
                    ui.displayMessage("Error: Pawn reached promotion rank but no promotion type specified.");
                    // Potentially revert move or force a default promotion (e.g. Queen)
                    // For simplicity, assume getPlayerMove ensures promotionType is set.
                }
            }
        }
        return true;
    }

    void Game::playTurn() {
        if (isRoundOver()) return;

        ui.displayPlayerTurn(currentPlayer);
        
        bool moveMade = false;
        while(!moveMade) {
            std::string moveStr = ui.getPlayerMove(*currentPlayer);
            if (moveStr.empty()) { // e.g. user wants to quit or an error in input
                ui.displayMessage("Invalid input format for move. Try again (e.g., e2e4 or a7a8q).");
                continue;
            }

            Position startPos = parsePosition(moveStr.substr(0, 2));
            Position endPos = parsePosition(moveStr.substr(2, 2));
            PieceType promotionTarget = PieceType::NONE;

            if (moveStr.length() == 5) { // Potential promotion: e.g., e7e8q
                char promotionChar = moveStr[4];
                switch (promotionChar) {
                    case 'q': promotionTarget = PieceType::QUEEN; break;
                    case 'r': promotionTarget = PieceType::ROOK; break;
                    case 'b': promotionTarget = PieceType::BISHOP; break;
                    case 'n': promotionTarget = PieceType::KNIGHT; break;
                    default: 
                        ui.displayMessage("Invalid promotion piece: " + std::string(1, promotionChar) + ". Use q, r, b, or n.");
                        continue; // Ask for move again
                }
            }
            
            // Validate pawn promotion input necessity
            Piece* p = board.getPiecePtr(startPos);
            if (p && p->getType() == PieceType::PAWN) {
                Color pieceColor = p->getColor();
                bool isPromotionSquare = (pieceColor == Color::WHITE && endPos.row == 0) || (pieceColor == Color::BLACK && endPos.row == 7);
                if (p->isValidMove(startPos, endPos, board) && isPromotionSquare && promotionTarget == PieceType::NONE) {
                     ui.displayMessage("Pawn promotion required. Append q, r, b, or n to your move (e.g. " + moveStr.substr(0,4) + "q).");
                     continue;
                }
            }


            moveMade = makeMove(startPos, endPos, promotionTarget);
            if (!moveMade) {
                ui.displayMessage("Please try your move again.");
            }
        }

        ui.displayBoard(board);
        checkForEndOfRound();

        if (!isRoundOver()) {
            switchPlayer();
            if (board.isKingInCheck(currentPlayer->getColor())) {
                ui.displayMessage(currentPlayer->getName() + "'s King is in check!");
            }
        }
    }

    bool Game::canPlayerMakeAnyLegalMove(Player* player) {
        Color playerColor = player->getColor();
        for (int r = 0; r < 8; ++r) {
            for (int c = 0; c < 8; ++c) {
                Position start(r, c);
                Piece* piece = board.getPiecePtr(start);
                if (piece && piece->getColor() == playerColor) {
                    std::vector<Position> possibleMoves = piece->getPossibleMoves(start, board);
                    for (const auto& end : possibleMoves) {
                        // Simulate move to check if it resolves check or is legal
                        Board tempBoard = board;
                        tempBoard.movePiece(start, end); // Simulate on temp board
                        if (!tempBoard.isKingInCheck(playerColor)) {
                            return true; // Found a legal move
                        }
                    }
                }
            }
        }
        return false;
    }

    void Game::checkForEndOfRound() {
        Player* opponent = (currentPlayer == player1) ? player2 : player1;
        bool opponentInCheck = board.isKingInCheck(opponent->getColor());
        bool opponentHasLegalMoves = canPlayerMakeAnyLegalMove(opponent);

        if (opponentInCheck && !opponentHasLegalMoves) {
            roundState = (currentPlayer->getColor() == Color::WHITE) ? RoundState::CHECKMATE_WHITE_WINS : RoundState::CHECKMATE_BLACK_WINS;
            ui.displayMessage("Checkmate! " + currentPlayer->getName() + " wins the round.");
        } else if (!opponentInCheck && !opponentHasLegalMoves) {
            roundState = RoundState::STALEMATE;
            ui.displayMessage("Stalemate! The round is a draw.");
        }
        // Other draw conditions (50-move, threefold repetition) not implemented
    }


    void Game::switchPlayer() {
        currentPlayer = (currentPlayer == player1) ? player2 : player1;
    }

    bool Game::isRoundOver() const {
        return roundState != RoundState::ONGOING;
    }

    Player* Game::getRoundWinner() const {
        if (roundState == RoundState::CHECKMATE_WHITE_WINS) return player1;
        if (roundState == RoundState::CHECKMATE_BLACK_WINS) return player2;
        return nullptr; // Draw or ongoing
    }


} // namespace PowerChess