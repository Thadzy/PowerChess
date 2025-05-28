#ifndef HARDCHESS_CORE_BOARD_H
#define HARDCHESS_CORE_BOARD_H

#include "Header/Core/CommonTypes.h"
#include "Header/Core/Piece.h"
#include <vector>
#include <memory>

namespace HardChess
{
    // Forward declarations
    class Pawn;
    class Rook;
    class Knight;
    class Bishop;
    class Queen;
    class King;
    class ConsoleUI; // For display method, if kept

    class Board
    {
    private:
        std::vector<std::vector<std::unique_ptr<Piece>>> grid;
        Position whiteKingPos;
        Position blackKingPos;
        Position enPassantTargetSquare; // Square a pawn can move to for en passant

        struct ValidationState
        {
            Position start, end;
            std::unique_ptr<Piece> capturedPiece; // For regular captures during validation
            bool pieceAtStartOriginalHasMoved;
            Position originalEnPassantTarget; // To restore en passant state
            // For castling validation, king's original hasMoved is covered
            // Rook's original hasMoved needs to be tracked if castling is part of tryMove
        };
        ValidationState currentValidationState;

    public:
        Board();
        Board(const Board &other);
        Board &operator=(const Board &other);

        void initializeBoard();
        
        Piece *getPiecePtr(Position pos) const;
        std::unique_ptr<Piece> getPieceClone(Position pos) const; // Renamed for clarity

        void setPiece(Position pos, std::unique_ptr<Piece> piece);
        std::unique_ptr<Piece> removePiece(Position pos);

        std::unique_ptr<Piece> movePiece(Position start, Position end, bool isEnPassantCapture = false, bool isCastlingMove = false);
        
        bool isPathClear(Position start, Position end) const;
        
        Position findKing(Color kingColor) const;
        bool isSquareAttacked(Position pos, Color attackerColor) const;
        bool isKingInCheck(Color kingColor) const;

        void recordPieceStatesForValidation(Position start, Position end);
        std::unique_ptr<Piece> tryMoveForValidation(Position start, Position end, bool isEnPassant = false, bool isCastling = false);
        void revertValidationMove(Position start, Position end, std::unique_ptr<Piece> originalPieceAtEnd);

        bool promotePawn(Position pawnPos, PieceType promotionType);

        Position getEnPassantTargetSquare() const { return enPassantTargetSquare; }
        void setEnPassantTargetSquare(Position pos) { enPassantTargetSquare = pos; }
        void clearEnPassantTargetSquare() { enPassantTargetSquare = Position(-1, -1); }
    };
} // namespace HardChess

#endif // HARDCHESS_CORE_BOARD_H