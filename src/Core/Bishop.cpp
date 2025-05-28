// filepath: [Bishop.cpp](http://_vscodecontentref_/2)
#include "PowerChess/Core/Bishop.h"
#include "PowerChess/Core/Board.h"

namespace PowerChess {

Bishop::Bishop(Color c, Position pos)
    : Piece(c, PieceType::BISHOP, pos) {}

char Bishop::getSymbol() const {
    return (color == Color::WHITE) ? 'B' : 'b';
}

std::unique_ptr<Piece> Bishop::clone() const {
    return std::make_unique<Bishop>(color, position);
}

bool Bishop::isValidMove(Position start, Position end, const Board& board) const {
    if (!start.isValid() || !end.isValid() || start == end)
        return false;

    int dr = end.row - start.row;
    int dc = end.col - start.col;

    // Must move diagonally
    if (std::abs(dr) != std::abs(dc))
        return false;

    int stepR = (dr > 0) ? 1 : -1;
    int stepC = (dc > 0) ? 1 : -1;
    int r = start.row + stepR;
    int c = start.col + stepC;

    // Check all squares between start and end
    while (r != end.row && c != end.col) {
        if (board.getPiecePtr(Position(r, c)) != nullptr)
            return false;
        r += stepR;
        c += stepC;
    }

    // Can't capture own piece
    const Piece* dest = board.getPiecePtr(end);
    if (dest && dest->getColor() == color)
        return false;

    return true;
}

} // namespace PowerChess