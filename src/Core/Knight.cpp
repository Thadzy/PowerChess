#include "PowerChess/Core/Knight.h"
#include "PowerChess/Core/Board.h" // For board.getPiece
#include <cmath> // For abs

namespace PowerChess {

    Knight::Knight(Color c, Position pos) : Piece(c, PieceType::KNIGHT, pos), boostedForNextMove(false) {}

    char Knight::getSymbol() const {
        return (color == Color::WHITE) ? 'N' : 'n';
    }

    std::unique_ptr<Piece> Knight::clone() const {
        auto newPiece = std::make_unique<Knight>(this->color, this->position);
        newPiece->setHasMoved(this->hasMoved);
        newPiece->setBoostedForNextMove(this->boostedForNextMove); // Also clone boost status
        return newPiece;
    }

    bool Knight::isValidMove(Position start, Position end, const Board& board) const {
        if (!end.isValid()) return false;
        if (start == end) return false;

        std::unique_ptr<Piece> targetPiece = board.getPiece(end);
        if (targetPiece && targetPiece->getColor() == this->color) {
            return false;
        }

        int dr = std::abs(start.row - end.row);
        int dc = std::abs(start.col - end.col);

        bool isNormalKnightMove = (dr == 2 && dc == 1) || (dr == 1 && dc == 2);

        if (boostedForNextMove) {
            bool isBoostedKingLikeMove = (dr <= 1 && dc <= 1 && (dr != 0 || dc != 0));
            return isNormalKnightMove || isBoostedKingLikeMove;
        }
        
        return isNormalKnightMove;
        // Knight jumps, so no path clearing check needed for normal move.
        // For boosted king-like move, it's only 1 square, so no path check needed either.
    }

} // namespace PowerChess