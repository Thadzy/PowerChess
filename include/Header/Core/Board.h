#ifndef HARDCHESS_CORE_BOARD_H
#define HARDCHESS_CORE_BOARD_H

#include "Header/Core/CommomTypes.h"
#include "Header/Core/Piece.h"
#include <vector>
#include <memory>

namespace HardChess
{
  // Forward declarations of piece classes to avoid circular dependencies
  class Pawn, class Rook, class Knight, class Bishop, class Queen, class King;

  // Board
  class Board
  {
  // Represents the chessboard, which is an 8x8 grid of pieces
  private:
    std::vector<std::vector<std::unique_ptr<Piece>>> grid;
    position whiteKingPos; // Positions of the white and black kings
    postion blackKingPos; // Positions of the white and black kings
    // Helper function to check if a position is within the bounds of the board

    struct ValidMoveState
    { // Struct to hold the state of a valid move
      Position start, end; // Start and end positions of the move
      std::unique_ptr<Piece> CapturePiece; // Piece that is captured during the move
      bool pieceAtStartOriginalHasMoved; // Flag to indicate if the piece at the start position has moved
      bool PieceAtEndOriginalHasMoved; // Flag to indicate if the piece at the end position has moved
  }

  public : Board();
    Board(const Board &other);            // Deep copy constructor
    Board &operator=(const Board &other); // Deep copy assignment

    void initializeBoard();            // Initialize the board with pieces in their starting positions
    void display(ConsoleUI &ui) cosnt; // Display the board using the ConsoleUI class

    std::unique_ptr<Piece> &getPieceAt(Position pos) cosnt; // Get a reference to the piece at a specific position

    // Get a pointer to the piece at a specific position
    Piece* getPiecePtr(Position pos) const; // Get a pointer to the piece at a specific position

    void setPiece(Position pos, std::unique_ptr<Piece> piece); // Set a piece at a specific position
    std::unique_ptr<Piece> removePiece(Position pos); // Remove a piece from a specific position

    // Return Capture Piece at a specific position
    std::unique_ptr<Piece> movePiece(Position start, Position end) const; // Move a piece from the start position to the end position
    Position findKing(Color kingColor) const; // Find the position of the king of a specific color
    bool isSquareAttacked(Position pos, Color attackerColor) const; // Check if a square is attacked by a piece of a specific color
    bool isKingInCheck(Color kingColor) const; // Check if the king of a specific color is in check

    // Method for mmove validation
    void recordPieceStatesForValidation(Postion start, Position end); // Record the states of pieces for move validation
    std::unique_ptr<Piece> tryMoveForValidation(Postion start, Position end); // Try to move a piece for validation purposes
    void revertValidationMove(Postion start, Postion end, std:unique_ptr<Piece> originalPieceAtEnd);
    // Revert the move made for validation purposes
    
    bool promotePawn(Position pawnPos, PieceType promotionType); // Promote a pawn to a different piece type
  };
}

#endif // HARDCHESS_CORE_BOARD_H