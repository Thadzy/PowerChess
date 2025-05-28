#include "Header/Core/Board.h"
#include "Header/Core/Pawn.h"
#include "Header/Core/Rook.h"
#include "Header/Core/Knight.h"
#include "Header/Core/Bishop.h"
#include "Header/Core/Queen.h"
#include "Header/Core/King.h"
#include <iostream> // For potential debugging, not strictly necessary for logic
#include <algorithm> // For std::find_if or other algorithms if used

namespace HardChess
{
    Board::Board() : grid(8), enPassantTargetSquare(-1, -1)
    {
        for (auto &row : grid)
        {
            row.resize(8, nullptr);
        }
        initializeBoard();
    }

    Board::Board(const Board &other) : grid(8), enPassantTargetSquare(other.enPassantTargetSquare)
    {
        for (auto &row : grid)
        {
            row.resize(8, nullptr);
        }
        for (int i = 0; i < 8; ++i)
        {
            for (int j = 0; j < 8; ++j)
            {
                if (other.grid[i][j])
                {
                    grid[i][j] = other.grid[i][j]->clone();
                }
                else
                {
                    grid[i][j] = nullptr;
                }
            }
        }
        whiteKingPos = other.whiteKingPos;
        blackKingPos = other.blackKingPos;
    }

    Board &Board::operator=(const Board &other)
    {
        if (this == &other)
        {
            return *this;
        }
        grid.assign(8, std::vector<std::unique_ptr<Piece>>(8, nullptr)); // Clear and resize

        for (int i = 0; i < 8; ++i)
        {
            for (int j = 0; j < 8; ++j)
            {
                if (other.grid[i][j])
                {
                    grid[i][j] = other.grid[i][j]->clone();
                }
                else
                {
                    grid[i][j] = nullptr;
                }
            }
        }
        whiteKingPos = other.whiteKingPos;
        blackKingPos = other.blackKingPos;
        enPassantTargetSquare = other.enPassantTargetSquare;
        return *this;
    }

    void Board::initializeBoard()
    {
        for (auto &row : grid)
        {
            for (auto &cell : row)
            {
                cell.reset();
            }
        }
        enPassantTargetSquare = Position(-1,-1); // Reset en passant

        // Pawns
        for (int c = 0; c < 8; ++c)
        {
            grid[1][c] = std::make_unique<Pawn>(Color::BLACK, Position(1, c));
            grid[6][c] = std::make_unique<Pawn>(Color::WHITE, Position(6, c));
        }
        // Rooks
        grid[0][0] = std::make_unique<Rook>(Color::BLACK, Position(0, 0));
        grid[0][7] = std::make_unique<Rook>(Color::BLACK, Position(0, 7));
        grid[7][0] = std::make_unique<Rook>(Color::WHITE, Position(7, 0));
        grid[7][7] = std::make_unique<Rook>(Color::WHITE, Position(7, 7));
        // Knights
        grid[0][1] = std::make_unique<Knight>(Color::BLACK, Position(0, 1));
        grid[0][6] = std::make_unique<Knight>(Color::BLACK, Position(0, 6));
        grid[7][1] = std::make_unique<Knight>(Color::WHITE, Position(7, 1));
        grid[7][6] = std::make_unique<Knight>(Color::WHITE, Position(7, 6));
        // Bishops
        grid[0][2] = std::make_unique<Bishop>(Color::BLACK, Position(0, 2));
        grid[0][5] = std::make_unique<Bishop>(Color::BLACK, Position(0, 5));
        grid[7][2] = std::make_unique<Bishop>(Color::WHITE, Position(7, 2));
        grid[7][5] = std::make_unique<Bishop>(Color::WHITE, Position(7, 5));
        // Queens
        grid[0][3] = std::make_unique<Queen>(Color::BLACK, Position(0, 3));
        grid[7][3] = std::make_unique<Queen>(Color::WHITE, Position(7, 3));
        // Kings
        grid[0][4] = std::make_unique<King>(Color::BLACK, Position(0, 4));
        blackKingPos = Position(0, 4);
        grid[7][4] = std::make_unique<King>(Color::WHITE, Position(7, 4));
        whiteKingPos = Position(7, 4);
    }

    Piece *Board::getPiecePtr(Position pos) const
    {
        if (!pos.isValid()) return nullptr;
        return grid[pos.row][pos.col].get();
    }

    std::unique_ptr<Piece> Board::getPieceClone(Position pos) const
    {
        if (!pos.isValid() || !grid[pos.row][pos.col]) return nullptr;
        return grid[pos.row][pos.col]->clone();
    }

    void Board::setPiece(Position pos, std::unique_ptr<Piece> piece)
    {
        if (!pos.isValid()) return;
        grid[pos.row][pos.col] = std::move(piece);
        if (grid[pos.row][pos.col])
        {
            grid[pos.row][pos.col]->setPosition(pos);
            if (grid[pos.row][pos.col]->getType() == PieceType::KING)
            {
                if (grid[pos.row][pos.col]->getColor() == Color::WHITE) whiteKingPos = pos;
                else blackKingPos = pos;
            }
        }
    }
    
    std::unique_ptr<Piece> Board::removePiece(Position pos)
    {
        if (!pos.isValid() || !grid[pos.row][pos.col]) return nullptr;
        return std::move(grid[pos.row][pos.col]);
    }

    std::unique_ptr<Piece> Board::movePiece(Position start, Position end, bool isEnPassantCapture, bool isCastlingMove)
    {
        if (!start.isValid() || !end.isValid() || !grid[start.row][start.col]) return nullptr;

        std::unique_ptr<Piece> movedPiece = std::move(grid[start.row][start.col]);
        std::unique_ptr<Piece> capturedPiece = nullptr;

        // Current turn's en passant target is cleared *before* new one is set
        Position previousEnPassantTarget = enPassantTargetSquare;
        clearEnPassantTargetSquare();


        if (isEnPassantCapture)
        {
            // The pawn lands on enPassantTargetSquare, but captures a pawn on a different square
            Position capturedPawnPos = (movedPiece->getColor() == Color::WHITE) ?
                                       Position(end.row + 1, end.col) :
                                       Position(end.row - 1, end.col);
            if (capturedPawnPos.isValid() && grid[capturedPawnPos.row][capturedPawnPos.col] &&
                grid[capturedPawnPos.row][capturedPawnPos.col]->getType() == PieceType::PAWN) {
                capturedPiece = std::move(grid[capturedPawnPos.row][capturedPawnPos.col]);
            }
        }
        else
        {
            capturedPiece = std::move(grid[end.row][end.col]);
        }

        grid[end.row][end.col] = std::move(movedPiece);
        grid[end.row][end.col]->setPosition(end);
        grid[end.row][end.col]->setHasMoved(true);

        // Update king position if king moved
        if (grid[end.row][end.col]->getType() == PieceType::KING)
        {
            if (grid[end.row][end.col]->getColor() == Color::WHITE) whiteKingPos = end;
            else blackKingPos = end;
        }
        
        // Handle castling: move the rook
        if (isCastlingMove) {
            int rookOriginalCol = (end.col > start.col) ? 7 : 0; // Kingside or Queenside
            int rookNewCol = (end.col > start.col) ? start.col + 1 : start.col -1; // Rook lands next to king
            Position rookStartPos(start.row, rookOriginalCol);
            Position rookEndPos(start.row, rookNewCol);

            if (rookStartPos.isValid() && grid[rookStartPos.row][rookStartPos.col]) {
                 std::unique_ptr<Piece> rookToMove = std::move(grid[rookStartPos.row][rookStartPos.col]);
                 rookToMove->setPosition(rookEndPos);
                 rookToMove->setHasMoved(true);
                 grid[rookEndPos.row][rookEndPos.col] = std::move(rookToMove);
            }
        }

        // Set new en passant target if a pawn made a two-square move
        if (grid[end.row][end.col]->getType() == PieceType::PAWN && std::abs(start.row - end.row) == 2)
        {
            enPassantTargetSquare = Position((start.row + end.row) / 2, start.col);
        }

        return capturedPiece;
    }


    bool Board::isPathClear(Position start, Position end) const
    {
        int dr = (end.row > start.row) ? 1 : ((end.row < start.row) ? -1 : 0);
        int dc = (end.col > start.col) ? 1 : ((end.col < start.col) ? -1 : 0);

        int r = start.row + dr;
        int c = start.col + dc;

        while (r != end.row || c != end.col)
        {
            if (!Position(r, c).isValid()) return false; // Should not happen if start/end are valid
            if (grid[r][c]) return false; // Path is blocked
            r += dr;
            c += dc;
        }
        return true;
    }

    Position Board::findKing(Color kingColor) const
    {
        return (kingColor == Color::WHITE) ? whiteKingPos : blackKingPos;
    }

    bool Board::isSquareAttacked(Position square, Color attackerColor) const
    {
        if (!square.isValid()) return false;
        for (int r = 0; r < 8; ++r)
        {
            for (int c = 0; c < 8; ++c)
            {
                Piece *p = getPiecePtr(Position(r, c));
                if (p && p->getColor() == attackerColor)
                {
                    // For pawn attacks, isValidMove logic is for movement, not just attack squares.
                    // Need a specific pawn attack check if isValidMove doesn't cover it for non-capture moves.
                    // However, if isValidMove(start, target, board) is used, it implies a full move check.
                    if (p->getType() == PieceType::PAWN) {
                        int forward = (p->getColor() == Color::WHITE) ? -1 : 1;
                        if (square.row == p->getPosition().row + forward) {
                            if (std::abs(square.col - p->getPosition().col) == 1) {
                                return true; // Pawn attacks diagonally
                            }
                        }
                    } else if (p->isValidMove(Position(r, c), square, *this)) {
                         // For non-pawn pieces, isValidMove can check if they attack the square.
                         // For pieces like rooks, bishops, queens, if isValidMove is true and isPathClear is true (which is part of their isValidMove),
                         // they attack. Knights jump, so path is not an issue. Kings attack adjacent squares.
                        return true;
                    }
                }
            }
        }
        return false;
    }

    bool Board::isKingInCheck(Color kingColor) const
    {
        Position kingPos = findKing(kingColor);
        if (!kingPos.isValid()) return true; // Should not happen, but safety
        return isSquareAttacked(kingPos, (kingColor == Color::WHITE) ? Color::BLACK : Color::WHITE);
    }
    
    // --- Validation Methods ---
    void Board::recordPieceStatesForValidation(Position start, Position end) {
        currentValidationState.start = start;
        currentValidationState.end = end;
        currentValidationState.capturedPiece = nullptr; // Will be set by tryMove
        currentValidationState.originalEnPassantTarget = enPassantTargetSquare;

        Piece* pieceAtStart = getPiecePtr(start);
        currentValidationState.pieceAtStartOriginalHasMoved = pieceAtStart ? pieceAtStart->getHasMoved() : false;
    }

    std::unique_ptr<Piece> Board::tryMoveForValidation(Position start, Position end, bool isEnPassant, bool isCastling) {
        recordPieceStatesForValidation(start, end);

        // Simulate the move but DO NOT change hasMoved for rooks in castling here yet, only the king.
        // The actual hasMoved update happens in the real movePiece.
        // King's hasMoved is critical for subsequent castling attempts within validation chains.
        std::unique_ptr<Piece> pieceToMove = removePiece(start); // Take piece from start
        std::unique_ptr<Piece> originalPieceAtEnd = nullptr;

        if (isEnPassant) {
            Position capturedPawnPos = (pieceToMove->getColor() == Color::WHITE) ?
                                       Position(end.row + 1, end.col) :
                                       Position(end.row - 1, end.col);
            originalPieceAtEnd = removePiece(capturedPawnPos); // "capture" the en-passant pawn
            currentValidationState.capturedPiece = originalPieceAtEnd ? originalPieceAtEnd->clone() : nullptr;
        } else {
            originalPieceAtEnd = removePiece(end); // Take piece from end (if any)
            currentValidationState.capturedPiece = originalPieceAtEnd ? originalPieceAtEnd->clone() : nullptr;
        }
        
        setPiece(end, std::move(pieceToMove)); // Place piece at end

        if (getPiecePtr(end)) { // if piece was successfully placed
            if (getPiecePtr(end)->getType() == PieceType::KING) {
                 // Update king position for check detection during validation
                if (getPiecePtr(end)->getColor() == Color::WHITE) whiteKingPos = end;
                else blackKingPos = end;
            }
            // Set hasMoved for the piece being validated temporarily
             getPiecePtr(end)->setHasMoved(true);
        }
        
        if (isCastling) {
            // Temporarily move rook for validation if castling
            int rookOriginalCol = (end.col > start.col) ? 7 : 0;
            int rookNewCol = (end.col > start.col) ? start.col + 1 : start.col -1;
            Position rookStartPos(start.row, rookOriginalCol);
            Position rookEndPos(start.row, rookNewCol);
            std::unique_ptr<Piece> rook = removePiece(rookStartPos);
            if(rook) {
                // Store this rook somehow if it needs to be restored with its own hasMoved state
                // For now, just move it.
                setPiece(rookEndPos, std::move(rook));
            }
        }


        return originalPieceAtEnd; // Return the piece that was originally at 'end' (or captured en-passant)
    }

    void Board::revertValidationMove(Position start, Position end, std::unique_ptr<Piece> originalPieceAtEndFromTryMove) {
        std::unique_ptr<Piece> movedPiece = removePiece(end); // Take the piece from 'end'

        // If it was castling, restore the rook too
        if (movedPiece && movedPiece->getType() == PieceType::KING && std::abs(end.col - start.col) == 2) {
            int rookNewCol = (end.col > start.col) ? start.col + 1 : start.col -1;
            int rookOriginalCol = (end.col > start.col) ? 7 : 0;
            Position rookCurrentPos(start.row, rookNewCol);
            Position rookOriginalPos(start.row, rookOriginalCol);
            
            std::unique_ptr<Piece> rook = removePiece(rookCurrentPos);
            if (rook) {
                // Rook's original hasMoved needs to be restored if we tracked it.
                // For simplicity, assume it wasn't moved if this King move is reverted.
                // A more robust validation state would track all involved pieces' original states.
                Piece* originalRook = getPiecePtr(rookOriginalPos); // Check if rook was overwritten
                if(!originalRook){ // only set if original spot is empty now.
                    setPiece(rookOriginalPos, std::move(rook));
                    if(getPiecePtr(rookOriginalPos)) getPiecePtr(rookOriginalPos)->setHasMoved(false); // Simplistic revert
                }

            }
        }
        
        setPiece(start, std::move(movedPiece)); // Place it back at 'start'

        if (getPiecePtr(start)) { // If piece was successfully placed back
            getPiecePtr(start)->setHasMoved(currentValidationState.pieceAtStartOriginalHasMoved);
            if (getPiecePtr(start)->getType() == PieceType::KING) {
                 // Restore king position
                if (getPiecePtr(start)->getColor() == Color::WHITE) whiteKingPos = start;
                else blackKingPos = start;
            }
        }

        // Restore the original piece at 'end' or the en-passant captured piece
        if (currentValidationState.end == end && currentValidationState.capturedPiece) { // regular capture or en-passant target was end
            if (enPassantTargetSquare.isValid() && currentValidationState.end == enPassantTargetSquare && originalPieceAtEndFromTryMove) {
                 Position actualCapturedPawnPos = (getPiecePtr(start)->getColor() == Color::WHITE) ?
                                       Position(end.row + 1, end.col) :
                                       Position(end.row - 1, end.col);
                 setPiece(actualCapturedPawnPos, std::move(originalPieceAtEndFromTryMove));
            } else if (originalPieceAtEndFromTryMove) {
                 setPiece(end, std::move(originalPieceAtEndFromTryMove));
            }
        } else if (originalPieceAtEndFromTryMove) { // Fallback if captured piece was on a different square (en-passant)
            // This case needs careful handling of where originalPieceAtEndFromTryMove came from.
            // For simplicity, we assume it came from 'end' or the en-passant captured square.
            // If en-passant occurred, originalPieceAtEndFromTryMove is the captured pawn.
            Position capturedPawnActualPos; // Determine this based on game logic.
            // For now, if it exists, assume it was on `end` if not the EP scenario.
            if (currentValidationState.capturedPiece && !isPathClear(currentValidationState.start, currentValidationState.end) ) { // rough guess it was from end
                 setPiece(end, std::move(originalPieceAtEndFromTryMove));
            }
        }
        
        enPassantTargetSquare = currentValidationState.originalEnPassantTarget;
    }
    // --- End Validation Methods ---


    bool Board::promotePawn(Position pawnPos, PieceType promotionType)
    {
        if (!pawnPos.isValid() || !grid[pawnPos.row][pawnPos.col] ||
            grid[pawnPos.row][pawnPos.col]->getType() != PieceType::PAWN)
            return false;
        
        Piece* pawn = grid[pawnPos.row][pawnPos.col].get();
        Color color = pawn->getColor();

        // Check if pawn is on promotion rank
        if (!((color == Color::WHITE && pawnPos.row == 0) || (color == Color::BLACK && pawnPos.row == 7)))
            return false;

        std::unique_ptr<Piece> newPiece;
        switch (promotionType)
        {
            case PieceType::QUEEN:  newPiece = std::make_unique<Queen>(color, pawnPos); break;
            case PieceType::ROOK:   newPiece = std::make_unique<Rook>(color, pawnPos);   break;
            case PieceType::BISHOP: newPiece = std::make_unique<Bishop>(color, pawnPos); break;
            case PieceType::KNIGHT: newPiece = std::make_unique<Knight>(color, pawnPos); break;
            default: return false; // Invalid promotion type
        }
        
        newPiece->setHasMoved(true); // Promoted piece is considered to have moved
        setPiece(pawnPos, std::move(newPiece));
        return true;
    }
} // namespace HardChess