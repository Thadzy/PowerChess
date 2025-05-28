#ifndef HARDCHESS_CORE_PIECE_H
#define HARDCHESS_CORE_PIECE_H

#include <vector>
#include <string>
#include <memory>

#include "Header/Core/CommonTypes.h"

namespace HardChess {

    // Forward declaration to avoid circular include
    class Board;

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

        // Virtual move validation and possible move generation
        virtual bool isValidMove(Position start, Position end, const Board& board) const = 0;

        virtual std::vector<Position> getPossibleMoves(Position start, const Board& board) const {
            std::vector<Position> possibleMoves;
            for (int row = 0; row < 8; ++row) {
                for (int col = 0; col < 8; ++col) {
                    Position end{row, col};
                    if (isValidMove(start, end, board)) {
                        possibleMoves.push_back(end);
                    }
                }
            }
            return possibleMoves;
        }

        // Function to get piece color as string
        std::string getColorString() const {
            return (color == Color::WHITE) ? "White" : "Black";
        }
    };

} // namespace HardChess

#endif // HARDCHESS_CORE_PIECE_H