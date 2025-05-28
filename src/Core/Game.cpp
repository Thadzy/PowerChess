#include "Header/Core/Game.h"
#include <iostream>
#include <cctype>
#include <limits>

namespace HardChess {

Game::Game(Player* p1, Player* p2, ConsoleUI& consoleUi)
    : player1(p1), player2(p2), currentPlayer(p1), ui(consoleUi), roundState(RoundState::ONGOING) {}

void Game::startRound() {
    board.initializeBoard();
    currentPlayer = player1; // White starts
    roundState = RoundState::ONGOING;
    ui.displayBoard(board);
}

void Game::play() {
    startRound();
    while (!isRoundOver()) {
        playTurn();
    }
    
    // Display final game result
    Player* winner = getRoundWinner();
    if (winner) {
        ui.displayMessage("Game Over! " + winner->getName() + " wins!");
    } else {
        ui.displayMessage("Game Over! It's a draw!");
    }
}

void Game::playTurn() {
    if (isRoundOver()) return;

    ui.displayBoard(board);
    ui.displayMessage(currentPlayer->getName() + "'s turn (" + (currentPlayer == player1 ? "White" : "Black") + ")");

    // Get move input
    std::string startStr, endStr;
    Position start, end;
    PieceType promotionType = PieceType::NONE;

    while (true) {
        ui.displayMessage("Enter move (e.g., e2e4 or a7a8q for promotion): ");
        std::cin >> startStr >> endStr;
        start = parsePosition(startStr);
        end = parsePosition(endStr);

        // Check for promotion
        if (board.getPiecePtr(start) && board.getPiecePtr(start)->getType() == PieceType::PAWN &&
            ((currentPlayer == player1 && end.row == 0) || (currentPlayer == player2 && end.row == 7))) {
            ui.displayMessage("Promote pawn to (q,r,b,n): ");
            char promo;
            std::cin >> promo;
            switch (std::tolower(promo)) {
                case 'q': promotionType = PieceType::QUEEN; break;
                case 'r': promotionType = PieceType::ROOK; break;
                case 'b': promotionType = PieceType::BISHOP; break;
                case 'n': promotionType = PieceType::KNIGHT; break;
                default: promotionType = PieceType::QUEEN; break;
            }
        }

        if (makeMove(start, end, promotionType)) {
            break;
        } else {
            ui.displayError("Invalid move! Try again.");
        }
    }

    checkForEndOfRound();
    if (!isRoundOver()) {
        switchPlayer();
    }
}

bool Game::makeMove(const Position& start, const Position& end, PieceType promotionType) {
    Piece* piece = board.getPiecePtr(start);
    if (!piece || piece->getColor() != (currentPlayer == player1 ? Color::WHITE : Color::BLACK))
        return false;

    if (!piece->isValidMove(start, end, board))
        return false;

    // Use validation methods for proper move testing
    auto originalPieceAtEnd = board.tryMoveForValidation(start, end);
    bool leavesKingInCheck = board.isKingInCheck(piece->getColor());
    
    if (leavesKingInCheck) {
        // Undo the validation move
        board.revertValidationMove(start, end, std::move(originalPieceAtEnd));
        return false;
    }

    // If move is valid, revert validation and make actual move
    board.revertValidationMove(start, end, std::move(originalPieceAtEnd));
    
    // Make the actual move
    auto captured = board.movePiece(start, end);

    // Handle promotion
    if (piece->getType() == PieceType::PAWN &&
        ((piece->getColor() == Color::WHITE && end.row == 0) ||
         (piece->getColor() == Color::BLACK && end.row == 7))) {
        board.promotePawn(end, promotionType);
    }

    return true;
}

void Game::switchPlayer() {
    currentPlayer = (currentPlayer == player1) ? player2 : player1;
}

Position Game::parsePosition(const std::string& s) const {
    if (s.length() != 2) return Position(-1, -1);
    char file = std::tolower(s[0]);
    char rank = s[1];
    int col = file - 'a';
    int row = 8 - (rank - '0');
    if (row < 0 || row > 7 || col < 0 || col > 7) return Position(-1, -1);
    return Position(row, col);
}

bool Game::canPlayerMakeAnyLegalMove(Player* player) {
    Color color = (player == player1) ? Color::WHITE : Color::BLACK;
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* piece = board.getPiecePtr(Position(r, c));
            if (piece && piece->getColor() == color) {
                for (int r2 = 0; r2 < 8; ++r2) {
                    for (int c2 = 0; c2 < 8; ++c2) {
                        Position start(r, c);
                        Position end(r2, c2);
                        if (piece->isValidMove(start, end, board)) {
                            // Use validation methods to test the move
                            auto originalPieceAtEnd = board.tryMoveForValidation(start, end);
                            bool leavesKingInCheck = board.isKingInCheck(color);
                            board.revertValidationMove(start, end, std::move(originalPieceAtEnd));
                            
                            if (!leavesKingInCheck)
                                return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

void Game::checkForEndOfRound() {
    Color opponentColor = (currentPlayer == player1) ? Color::BLACK : Color::WHITE;
    Player* opponent = (currentPlayer == player1) ? player2 : player1;

    bool inCheck = board.isKingInCheck(opponentColor);
    bool hasLegalMove = canPlayerMakeAnyLegalMove(opponent);

    if (inCheck && !hasLegalMove) {
        roundState = (opponentColor == Color::WHITE) ? RoundState::CHECKMATE_BLACK_WINS : RoundState::CHECKMATE_WHITE_WINS;
        ui.displayBoard(board);
        ui.displayMessage("Checkmate! " + currentPlayer->getName() + " wins the round!");
    } else if (!inCheck && !hasLegalMove) {
        roundState = RoundState::STALEMATE;
        ui.displayBoard(board);
        ui.displayMessage("Stalemate! The round is a draw.");
    } else {
        roundState = RoundState::ONGOING;
    }
}

bool Game::isRoundOver() const {
    return roundState != RoundState::ONGOING;
}

Player* Game::getRoundWinner() const {
    if (roundState == RoundState::CHECKMATE_WHITE_WINS) return player1;
    if (roundState == RoundState::CHECKMATE_BLACK_WINS) return player2;
    return nullptr;
}

} // namespace HardChess