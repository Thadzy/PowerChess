#include "Header/Core/Rook.h"
#include "Header/Core/Board.h" // For board.isPathClear and board.getPiecePtr

namespace HardChess
{
    Rook::Rook(Color c, Position pos)
        : Piece(c, PieceType::ROOK, pos) {}

    char Rook::getSymbol() const
    {
        return (color == Color::WHITE) ? 'R' : 'r';
    }

    std::unique_ptr<Piece> Rook::clone() const
    {
        auto newPiece = std::make_unique<Rook>(color, position);
        newPiece->setHasMoved(this->hasMoved);
        return newPiece;
    }

    bool Rook::isValidMove(Position start, Position end, const Board &board) const
    {
        if (!end.isValid() || start == end)
            return false;

        Piece *targetPiece = board.getPiecePtr(end);
        if (targetPiece && targetPiece->getColor() == this->color)
        {
            return false; // Cannot capture own piece
        }

        // Must move horizontally or vertically
        if (start.row != end.row && start.col != end.col)
            return false;

        // Path must be clear
        if (!board.isPathClear(start, end))
            return false;

        return true;
    }
} // namespace HardChess