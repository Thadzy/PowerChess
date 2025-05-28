#ifndef HARDCHESS_CORE_PIECE_H
#define HARDCHESS_CORE_PIECE_H

#include "PowerChess/Core/CommonTypes.h"
#include <vector>
#include <string>
#include <memory>

namespace HardChess {

    class Piece {
      protected:
        Color color;
        PieceType type;
        Position position;
        bool hasMoved;

      public:
      // Constructor to initialize a Piece with color, type, and position
        Piece(Color c, PieceType pt, Position pos)
            : color(c), type(pt), position(pos), hasMoved(false) {}
        virtual ~Piece() = default;

        // Getters and setters for piece properties
        Color getColor() const { return color; }
        PieceType getType() const { return type; }
        Position getPosition() const { return position; }
        void setPosition(Position newPos) { position = newPos; }
        bool getHasMoved() const { return hasMoved; }
        void setHasMoved(bool moved) { hasMoved = moved; }

        // Pure virtual functions to be implemented by derived classes
        virtual char getSymbol() const = 0;
        virtual std::string getName() const = 0;
        virtual std::unique_ptr<Piece> clone() const = 0;

        // Function to check if a move is valid for the piece
        virtual bool isValidMove(Position start, Position end, const Board& board) const = 0;
        virtual std::vector<Position> getPossibleMoves(Position start, const Board& board) const; // General implementation can use isValidMove for all squares

        // Function to check if the piece can attack a position
        std::string getColorString() const {
            return (color == Color::WHITE) ? "White" : "Black";
        }
    };

} // namespace PowerChess

#endif // POWERCHESS_CORE_PIECE_H