#ifndef POWERCHESS_CORE_BISHOP_H
#define POWERCHESS_CORE_BISHOP_H

#include "PowerChess/Core/Piece.h"

namespace PowerChess {

class Bishop : public Piece {
public:
    Bishop(Color c, Position pos);
    char getSymbol() const override;
    std::string getName() const override { return "Bishop"; }
    std::unique_ptr<Piece> clone() const override;
    bool isValidMove(Position start, Position end, const Board& board) const override;
};

} // namespace PowerChess

#endif // POWERCHESS_CORE_BISHOP_H