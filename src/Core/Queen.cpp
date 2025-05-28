#include "Header/Core/Queen.h"
#include "Header/Core/Board.h" // For board.isPathClear and board.getPiecePtr
#include <cmath>           // For std::abs

namespace HardChess
{
    Queen::Queen(Color c, Position pos)
        : Piece(c, PieceType::QUEEN, pos) {}

    char Queen::getSymbol() const
    {
        return (color == Color::WHITE) ? 'Q' : 'q';
    }

    std::unique_ptr<Piece> Queen::clone() const
    {
        auto newPiece = std::make_unique<Queen>(color, position);
        newPiece->setHasMoved(this->hasMoved);
        return newPiece;
    }

    bool Queen::isValidMove(Position start, Position end, const Board &board) const
    {
        if (!end.isValid() || start == end)
            return false;

        Piece *targetPiece = board.getPiecePtr(end);
        if (targetPiece && targetPiece->getColor() == this->color)
        {
            return false; // Cannot capture own piece
        }

        // Check if it's a rook-like move OR a bishop-like move
        bool isRookMove = (start.row == end.row || start.col == end.col);
        bool isBishopMove = (std::abs(start.row - end.row) == std::abs(start.col - end.col));

        if (!isRookMove && !isBishopMove)
            return false; // Not a valid queen move pattern

        // Path must be clear
        if (!board.isPathClear(start, end))
            return false;

        return true;
    }
} // namespace HardChess