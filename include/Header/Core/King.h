#ifndef HARDCHESS_CORE_KING_H
#define HARDCHESS_CORE_KING_H

#include "Header/Core/Piece.h"

namespace HardChess
{
    class King : public Piece
    {
    public:
        King(Color c, Position pos);
        char getSymbol() const override;
        std::string getName() const override { return "King"; }
        std::unique_ptr<Piece> clone() const override;
        bool isValidMove(Position start, Position end, const Board &board) const override;
    };
} // namespace HardChess

#endif // HARDCHESS_CORE_KING_H