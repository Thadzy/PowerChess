#ifndef POWERCHESS_CORE_KNIGHT_H
#define POWERCHESS_CORE_KNIGHT_H

#include "PowerChess/Core/Piece.h"

namespace PowerChess {

    class Knight : public Piece {
      private:
        bool boostedForNextMove;
      public:
        Knight(Color c, Position pos);
        char getSymbol() const override;
        std::string getName() const override { return "Knight"; }
        std::unique_ptr<Piece> clone() const override;
        bool isValidMove(Position start, Position end, const Board& board) const override;

        void setBoostedForNextMove(bool boosted) { boostedForNextMove = boosted; }
        bool isBoostedForNextMove() const { return boostedForNextMove; }
    };

} // namespace PowerChess

#endif // POWERCHESS_CORE_KNIGHT_H