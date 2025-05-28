#include "Header/Core/Knight.h"
#include "Header/Core/Board.h" // For board.getPiecePtr
#include <cmath>           // For std::abs

namespace HardChess
{
    Knight::Knight(Color c, Position pos)
        : Piece(c, PieceType::KNIGHT, pos) {}

    char Knight::getSymbol() const
    {
        return (color == Color::WHITE) ? 'N' : 'n';
    }

    std::unique_ptr<Piece> Knight::clone() const
    {
        auto newPiece = std::make_unique<Knight>(color, position);
        newPiece->setHasMoved(this->hasMoved);
        return newPiece;
    }

    bool Knight::isValidMove(Position start, Position end, const Board &board) const
    {
        if (!end.isValid() || start == end)
            return false;

        Piece *targetPiece = board.getPiecePtr(end);
        if (targetPiece && targetPiece->getColor() == this->color)
        {
            return false; // Cannot capture own piece
        }

        int dr = std::abs(start.row - end.row);
        int dc = std::abs(start.col - end.col);

        // Standard knight moves (L-shape: 2 in one dir, 1 in perpendicular)
        return (dr == 2 && dc == 1) || (dr == 1 && dc == 2);
    }
} // namespace HardChess