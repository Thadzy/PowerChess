#include "Header/Core/Pawn.h"
#include "Header/Core/Board.h" // For board access
#include <cmath> // For std::abs

namespace HardChess
{
    Pawn::Pawn(Color c, Position pos)
        : Piece(c, PieceType::PAWN, pos) {}

    char Pawn::getSymbol() const
    {
        return (color == Color::WHITE) ? 'P' : 'p';
    }

    std::unique_ptr<Piece> Pawn::clone() const
    {
        auto newPiece = std::make_unique<Pawn>(color, position);
        newPiece->setHasMoved(this->hasMoved);
        return newPiece;
    }

    bool Pawn::isValidMove(Position start, Position end, const Board &board) const
    {
        if (!end.isValid() || start == end)
            return false;

        Piece *targetPieceOnEnd = board.getPiecePtr(end);
        if (targetPieceOnEnd && targetPieceOnEnd->getColor() == this->color)
        {
            return false; // Cannot capture own piece
        }

        int forwardDirection = (this->color == Color::WHITE) ? -1 : 1;
        int dr = end.row - start.row;
        int dc = end.col - start.col;

        // Standard one-step forward
        if (dc == 0 && dr == forwardDirection && !targetPieceOnEnd)
        {
            return true;
        }

        // Initial two-step forward
        if (!this->hasMoved && dc == 0 && dr == 2 * forwardDirection && !targetPieceOnEnd)
        {
            Position intermediate(start.row + forwardDirection, start.col);
            if (!board.getPiecePtr(intermediate)) // Check if path is clear
            {
                return true;
            }
        }

        // Capture
        if (std::abs(dc) == 1 && dr == forwardDirection)
        {
            if (targetPieceOnEnd && targetPieceOnEnd->getColor() != this->color)
            {
                return true; // Diagonal capture of opponent's piece
            }
            // En Passant
            if (!targetPieceOnEnd && end == board.getEnPassantTargetSquare())
            {
                 // The en passant target square is where the pawn lands.
                 // The actual captured pawn is adjacent to the start square.
                return true;
            }
        }
        return false;
    }
} // namespace HardChess