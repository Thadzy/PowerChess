#include "Header/Core/Bishop.h"
#include "Header/Core/Board.h" // For board.isPathClear and board.getPiecePtr
#include <cmath>           // For std::abs

namespace HardChess
{
    Bishop::Bishop(Color c, Position pos)
        : Piece(c, PieceType::BISHOP, pos) {}

    char Bishop::getSymbol() const
    {
        return (color == Color::WHITE) ? 'B' : 'b';
    }

    std::unique_ptr<Piece> Bishop::clone() const
    {
        auto newPiece = std::make_unique<Bishop>(color, position);
        newPiece->setHasMoved(this->hasMoved); // Ensure hasMoved is copied
        return newPiece;
    }

    bool Bishop::isValidMove(Position start, Position end, const Board &board) const
    {
        if (!end.isValid() || start == end)
            return false;

        Piece *targetPiece = board.getPiecePtr(end);
        if (targetPiece && targetPiece->getColor() == this->color)
        {
            return false; // Cannot capture own piece
        }

        int dr = end.row - start.row;
        int dc = end.col - start.col;

        // Must move diagonally
        if (std::abs(dr) != std::abs(dc))
            return false;

        // Path must be clear
        if (!board.isPathClear(start, end))
            return false;

        return true;
    }
} // namespace HardChess