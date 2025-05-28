#ifndef POWERCHESS_CORE_ROOK_H
#define POWERCHESS_CORE_ROOK_H

#include "PowerChess/Core/Piece.h"

namespace PowerChess {

    class Rook : public Piece {
      public:
        Rook(Color c, Position pos);
        char getSymbol() const override;
        std::string getName() const override { return "Rook"; }
        std::unique_ptr<Piece> clone() const override;
        bool isValidMove(Position start, Position end, const Board& board) const override;
    };

} // namespace PowerChess

#endif // POWERCHESS_CORE_ROOK_H