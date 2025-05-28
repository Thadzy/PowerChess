#ifndef HARDCHESS_CORE_KNIGHT_H
#define HARDCHESS_CORE_KNIGHT_H

#include "Header/Core/Piece.h"

namespace HardChess {
    class Knight : public Piece {
    public:
        Knight(Color c, Position pos) : Piece(c, pos) {} // Constructor to initialize Knight piece with color and position
        char getSymbol() const override; // Get Symbol ex.'N' for Knight
        std::string getName() const override { return "Knight"; } // Get Name of the piece
        std::unique_ptr<Piece> clone() const override; // Create a clone of the Knight piece
        bool isValidMove(Position start, Position end, const Board& board) const override; // Check if the move is valid for a Knight piece
    };
}

#endif // HARDCHESS_CORE_KNIGHT_H