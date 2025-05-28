#ifndef HARDCHESS_CORE_PIECE_H
#define HARDCHESS_CORE_PIECE_H

#include <vector>
#include <string>
#include <memory>
#include "Header/Core/CommonTypes.h"

namespace HardChess
{
    class Board; // Forward declaration

    class Piece
    {
    protected:
        Color color;
        PieceType type;
        Position position;
        bool hasMoved;

    public:
        Piece(Color c, PieceType pt, Position pos)
            : color(c), type(pt), position(pos), hasMoved(false) {}
        virtual ~Piece() = default;

        Color getColor() const { return color; }
        PieceType getType() const { return type; }
        Position getPosition() const { return position; }
        void setPosition(Position newPos) { position = newPos; }
        bool getHasMoved() const { return hasMoved; }
        void setHasMoved(bool moved) { hasMoved = moved; }

        virtual char getSymbol() const = 0;
        virtual std::string getName() const = 0;
        virtual std::unique_ptr<Piece> clone() const = 0;
        virtual bool isValidMove(Position start, Position end, const Board &board) const = 0;

        virtual std::vector<Position> getPossibleMoves(Position start, const Board &board) const
        {
            std::vector<Position> possibleMoves;
            for (int r = 0; r < 8; ++r)
            {
                for (int c = 0; c < 8; ++c)
                {
                    Position endPos(r, c);
                    if (isValidMove(start, endPos, board))
                    {
                        possibleMoves.push_back(endPos);
                    }
                }
            }
            return possibleMoves;
        }

        std::string getColorString() const
        {
            return (color == Color::WHITE) ? "White" : "Black";
        }
    };
} // namespace HardChess

#endif // HARDCHESS_CORE_PIECE_H