#ifndef HARD_CORE_KING_H
#define HARD_CORE_KING_H

#include "Header/Core/Piece.h"

namespace HardChess {
    class King : public Piece {
      public:
        King(Color c, Position pos); // Constructor to initialize King piece with color and position
        char getSymbol() const override; // Get Symbol ex.'K' for King
        std::string getName() const override { return "King"; } // Get Name of the piece
        std::unique_ptr<Piece> clone() const override; // Create a clone of the King piece
        bool isValidMove(Position start, Position end, const Board& board) const override; // Check if the move is valid for a King piece
    }
}

#endif // POWERCHESS_CORE_KING_H