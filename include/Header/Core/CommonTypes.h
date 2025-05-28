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
        WHITE, // Represents the white color
        BLACK  // Represents the black color
    };

    enum class PieceType
    {
        PAWN,   // Represents a pawn piece
        ROOK,   // Represents a rook piece
        KNIGHT, // Represents a knight piece
        BISHOP, // Represents a bishop piece
        QUEEN,  // Represents a queen piece
        KING,    // Represents a king piece
        NONE
    };

    struct Position
    {
        int row;
        int col; // Represents a position on the chessboard using row and
                          // column indices'

        Position(int r = -1, int c = -1)
            : row(r), col(c)
        {
        } // Constructor to initialize position with default values

        bool isValid() const
        {
            return row >= 0 && row < 8 && col >= 0 && col < 8; // Check if the position is within the bounds of the
        }

        bool operator==(const Position &other) const {
            return row == other.row && col == other.col; // Overload the equality operator to compare two positions
        }
        bool operator!=(const Position &other) const {
            return !(*this == other); // Overload the inequality operator to compare two positions
        }

        //Forward declaration of the ConsoleUI class
        class Piece;
        class Board;
        class ConsoleUI; // Forward declaration of the ConsoleUI class
    };
} // namespace HardChess

#endif // HARDCHESS_CORE_COMMONTYPES_H