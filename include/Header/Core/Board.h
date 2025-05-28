#ifndef HARDCHESS_CORE_BOARD_H
#define HARDCHESS_CORE_BOARD_H

#include "Header/Core/CommonTypes.h"
#include "Header/Core/Piece.h"
#include "Header/UI/ConsoleUI.h"
#include <vector>
#include <memory>

namespace HardChess
{
    // Forward declarations of piece classes to avoid circular dependencies
    class Pawn;
    class Rook;
    class Knight;
    class Bishop;
    class Queen;
    class King;

    // Board class represents the chessboard, which is an 8x8 grid of pieces
    class Board
    {
    private:
        std::vector<std::vector<std::unique_ptr<Piece>>> grid;
        Position whiteKingPos; // Position of the white king
        Position blackKingPos; // Position of the black king

        // Struct to hold the state for move validation
        struct ValidationState
        {
            Position start, end; // Start and end positions of the move
            std::unique_ptr<Piece> capturedPiece; // Piece that is captured during the move
            bool pieceAtStartOriginalHasMoved; // Flag to indicate if the piece at the start position has moved
        };

        ValidationState currentValidationState; // Current validation state

    public:
        Board();                              // Default constructor
        Board(const Board &other);            // Deep copy constructor
        Board &operator=(const Board &other); // Deep copy assignment

        void initializeBoard();            // Initialize the board with pieces in their starting positions
        void display(ConsoleUI &ui) const; // Display the board using the ConsoleUI class

        // Get a pointer to the piece at a specific position
        Piece *getPiecePtr(Position pos) const;
        
        // Get a copy of the piece at a specific position
        std::unique_ptr<Piece> getPiece(Position pos) const;

        void setPiece(Position pos, std::unique_ptr<Piece> piece); // Set a piece at a specific position
        std::unique_ptr<Piece> removePiece(Position pos);         // Remove a piece from a specific position

        // Move a piece from start to end position, returns captured piece if any
        std::unique_ptr<Piece> movePiece(Position start, Position end);
        
        // Check if the path between two positions is clear
        bool isPathClear(Position start, Position end) const;
        
        Position findKing(Color kingColor) const;                             // Find the position of the king of a specific color
        bool isSquareAttacked(Position pos, Color attackerColor) const;       // Check if a square is attacked by a piece of a specific color
        bool isKingInCheck(Color kingColor) const;                           // Check if the king of a specific color is in check

        // Methods for move validation
        void recordPieceStatesForValidation(Position start, Position end);   // Record the states of pieces for move validation
        std::unique_ptr<Piece> tryMoveForValidation(Position start, Position end); // Try to move a piece for validation purposes
        void revertValidationMove(Position start, Position end, std::unique_ptr<Piece> originalPieceAtEnd);
        // Revert the move made for validation purposes

        bool promotePawn(Position pawnPos, PieceType promotionType); // Promote a pawn to a different piece type
    };
}

#endif // HARDCHESS_CORE_BOARD_H