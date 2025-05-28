#ifndef POWERCHESS_CORE_PAWN_H
#define POWERCHESS_CORE_PAWN_H

#include "PowerChess/Core/Piece.h"

namespace PowerChess {

    class Pawn : public Piece {
      public:
        Pawn(Color c, Position pos);
        char getSymbol() const override;
        std::string getName() const override { return "Pawn"; }
        std::unique_ptr<Piece> clone() const override;
        bool isValidMove(Position start, Position end, const Board& board) const override;
    };

} // namespace PowerChess

#endif // POWERCHESS_CORE_PAWN_H