// Include Guard Prevents the file from being included more than once
#ifndef HARDCHESS_CORE_BISHOP_H
#define HARDCHESS_CORE_BISHOP_H

#include "\Header\Core\Bishop.h"

namespace HardChess
{
    class Bishop : public Piece
    {
    public:
        Bishop(Color c, Position pos) : Piece(c, pos) {} // Declares a class Bishop that inherits from the base class Piece
        char getSymbol() const override; // Get Symbol ex.'B' for Bishop 
        std::string getName() const override { return "Bishop"; } // Get Name of the piece
        std::unique_ptr<Piece> clone() const override; // Create a clone of the Bishop piece
        bool isValidMove(Position start, Position end, const Board& board) cosnt override; // Check if the move is valid for a Bishop piece
    }
} 

#endif // HARDCHESS_CORE_BISHOP_H