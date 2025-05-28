#ifndef POWERCHESS_CORE_KING_H
#define POWERCHESS_CORE_KING_H

#include "PowerChess/Core/Piece.h"

namespace PowerChess {

    class King : public Piece {
      public:
        King(Color c, Position pos);
        char getSymbol() const override;
        std::string getName() const override { return "King"; }
        std::unique_ptr<Piece> clone() const override;
        bool isValidMove(Position start, Position end, const Board& board) const override;
        // Note: King's isValidMove should also check if 'end' is attacked by opponent
        // This check is usually done at game level to prevent illegal moves.
        // For piece-level check, it ensures the move is structurally king-like.
    };

} // namespace PowerChess

#endif // POWERCHESS_CORE_KING_H