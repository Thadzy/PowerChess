#ifndef POWERCHESS_CORE_QUEEN_H
#define POWERCHESS_CORE_QUEEN_H

#include "PowerChess/Core/Piece.h"

namespace PowerChess {

    class Queen : public Piece {
      public:
        Queen(Color c, Position pos);
        char getSymbol() const override;
        std::string getName() const override { return "Queen"; }
        std::unique_ptr<Piece> clone() const override;
        bool isValidMove(Position start, Position end, const Board& board) const override;
    };

} // namespace PowerChess

#endif // POWERCHESS_CORE_QUEEN_H