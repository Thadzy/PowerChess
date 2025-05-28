#ifndef HARDCHESS_CORE_QUEEN_H
#define HARDCHESS_CORE_QUEEN_H

#include "Header/Core/Piece.h"

namespace HardChess
{
    class Queen : public Piece
    {
    public:
        Queen(Color c, Position pos);
        char getSymbol() const override;
        std::string getName() const override { return "Queen"; }
        std::unique_ptr<Piece> clone() const override;
        bool isValidMove(Position start, Position end, const Board &board) const override;
    };
} // namespace HardChess

#endif // HARDCHESS_CORE_QUEEN_H