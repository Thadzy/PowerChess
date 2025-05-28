#include "Header/Core/Game.h"
#include <iostream>
#include <cctype>
#include <limits>

namespace Hardchess {

Game::Game(Player* p1, Player* p2, ConsoleUI& consoleUi)
    : player1(p1), player2(p2), currentPlayer(p1), ui(consoleUi), roundState(RoundState::ONGOING) {}

void Game::startRound() {
    board.initializeBoard();
    currentPlayer = player1; // White starts
    roundState = RoundState::ONGOING;
    ui.displayBoard(board);
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
        ui.displayMessage("Enter move (e.g., e2 e4): ");
        std::cin >> startStr >> endStr;
        start = parsePosition(startStr);
        end = parsePosition(endStr);

        // Promotion (optional)
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

bool Game::makeMove(Position start, Position end, PieceType promotionType) {
    Piece* piece = board.getPiecePtr(start);
    if (!piece || piece->getColor() != (currentPlayer == player1 ? Color::WHITE : Color::BLACK))
        return false;

    if (!piece->isValidMove(start, end, board))
        return false;

    // Simulate move for check
    auto captured = board.movePiece(start, end);
    bool leavesKingInCheck = board.isKingInCheck(piece->getColor());
    if (leavesKingInCheck) {
        // Undo move
        board.movePiece(end, start);
        board.setPiece(end, std::move(captured));
        return false;
    }

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
                            // Try move and see if king is left in check
                            auto captured = board.movePiece(start, end);
                            bool leavesKingInCheck = board.isKingInCheck(color);
                            board.movePiece(end, start);
                            board.setPiece(end, std::move(captured));
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

} // namespace Hardchess