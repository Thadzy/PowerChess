#include "Header/Core/King.h"
#include "Header/Core/Board.h"
#include "Header/Core/Rook.h" // For castling checks
#include <cmath>         // For std::abs

namespace HardChess
{
    King::King(Color c, Position pos)
        : Piece(c, PieceType::KING, pos) {}

    char King::getSymbol() const
    {
        return (color == Color::WHITE) ? 'K' : 'k';
    }

    std::unique_ptr<Piece> King::clone() const
    {
        auto newPiece = std::make_unique<King>(color, position);
        newPiece->setHasMoved(this->hasMoved);
        return newPiece;
    }

    bool King::isValidMove(Position start, Position end, const Board &board) const
    {
        if (!end.isValid() || start == end)
            return false;

        Piece *targetPieceOnEnd = board.getPiecePtr(end);
        if (targetPieceOnEnd && targetPieceOnEnd->getColor() == this->color)
        {
            return false; // Cannot capture own piece
        }

        int dr = std::abs(start.row - end.row);
        int dc = std::abs(start.col - end.col);

        // Standard king move (one square in any direction)
        if (dr <= 1 && dc <= 1)
        {
            return true;
        }

        // Castling: King moves two squares horizontally
        if (dr == 0 && dc == 2 && !this->hasMoved)
        {
            // Determine if kingside or queenside castling
            int rookCol = (end.col > start.col) ? 7 : 0; // Kingside (7) or Queenside (0)
            Position rookPos(start.row, rookCol);
            Piece *rookPiece = board.getPiecePtr(rookPos);

            if (rookPiece && rookPiece->getType() == PieceType::ROOK && rookPiece->getColor() == this->color && !rookPiece->getHasMoved())
            {
                // Path between king and rook must be clear
                // Note: The board/game logic will handle checks for moving through/into check
                if (board.isPathClear(start, rookPos)) {
                    return true; // King part of castling is valid if path to rook is clear and rook is eligible
                }
            }
        }
        return false;
    }
} // namespace HardChess