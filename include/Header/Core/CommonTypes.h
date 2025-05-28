#ifndef HARDCHESS_CORE_COMMONTYPES_H
#define HARDCHESS_CORE_COMMONTYPES_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace HardChess
{
    enum class Color
    {
        WHITE,
        BLACK
    };

    enum class PieceType
    {
        PAWN,
        ROOK,
        KNIGHT,
        BISHOP,
        QUEEN,
        KING,
        NONE
    };

    struct Position
    {
        int row;
        int col;

        Position(int r = -1, int c = -1)
            : row(r), col(c)
        {
        }

        bool isValid() const
        {
            return row >= 0 && row < 8 && col >= 0 && col < 8;
        }

        bool operator==(const Position &other) const
        {
            return row == other.row && col == other.col;
        }
        bool operator!=(const Position &other) const
        {
            return !(*this == other);
        }
    };
} // namespace HardChess

#endif // HARDCHESS_CORE_COMMONTYPES_H