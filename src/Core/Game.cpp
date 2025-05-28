#include "Header/Core/Game.h"
#include <iostream> // For UI interaction via ConsoleUI
#include <cctype>   // For std::tolower
#include <limits>   // For std::numeric_limits
#include <cmath>    // For std::abs in castling check

namespace HardChess
{
    Game::Game(Player *p1, Player *p2, ConsoleUI &consoleUi)
        : player1(p1), player2(p2), currentPlayer(p1), ui(consoleUi), roundState(RoundState::ONGOING) {}

    void Game::startRound()
    {
        board.initializeBoard();
        currentPlayer = (player1->getColor() == Color::WHITE) ? player1 : player2; // White always starts
        roundState = RoundState::ONGOING;
        // ui.displayBoard(board); // Display board at the start of playTurn instead
    }

    RoundState Game::playRound()
    {
        startRound();
        while (roundState == RoundState::ONGOING)
        {
            ui.clearScreen();
            ui.displayPlayerStats(*player1, *player2);
            ui.displayBoard(board);
            ui.displayCurrentPlayerTurn(*currentPlayer);

            if (board.isKingInCheck(currentPlayer->getColor())) {
                ui.displayMessage("Your King is in Check!");
            }

            std::string startStr, endStr;
            Position startPos, endPos;
            PieceType promotionType = PieceType::NONE;
            bool moveMade = false;

            while(!moveMade) {
                ui.getMoveInput(*currentPlayer, startStr, endStr);
                startPos = parsePosition(startStr);
                endPos = parsePosition(endStr);

                if (!startPos.isValid() || !endPos.isValid()) {
                    ui.displayError("Invalid position format. Use algebraic notation (e.g., e2 e4).");
                    continue;
                }

                Piece* pieceToMove = board.getPiecePtr(startPos);
                if (!pieceToMove || pieceToMove->getColor() != currentPlayer->getColor()) {
                    ui.displayError("Not your piece or empty square at " + startStr + ".");
                    continue;
                }

                // Pawn Promotion Check (before makeMove)
                if (pieceToMove->getType() == PieceType::PAWN) {
                    bool whitePromotion = (pieceToMove->getColor() == Color::WHITE && startPos.row == 1 && endPos.row == 0);
                    bool blackPromotion = (pieceToMove->getColor() == Color::BLACK && startPos.row == 6 && endPos.row == 7);
                     // Check actual end square for promotion, not just start
                    if (pieceToMove->getColor() == Color::WHITE && endPos.row == 0 ||
                        pieceToMove->getColor() == Color::BLACK && endPos.row == 7)
                    {
                        if (pieceToMove->isValidMove(startPos,endPos,board)) // Check if basic pawn move to promotion square is valid
                             promotionType = ui.promptPawnPromotionChoice(*currentPlayer);
                    }
                }
                
                if (makeMove(startPos, endPos, promotionType)) {
                    moveMade = true;
                } else {
                    ui.displayError("Invalid move! Try again.");
                    // Board state is unchanged by failed makeMove due to validation
                }
            }
            
            checkForEndOfRound();
            if (roundState == RoundState::ONGOING)
            {
                switchPlayer();
            }
        }
        // Display final board and result of the round
        ui.clearScreen();
        ui.displayPlayerStats(*player1, *player2);
        ui.displayBoard(board);
        std::string winnerName = "";
        if (roundState == RoundState::CHECKMATE_WHITE_WINS) winnerName = player1->getName();
        else if (roundState == RoundState::CHECKMATE_BLACK_WINS) winnerName = player2->getName();
        ui.displayRoundResult(roundState, winnerName);
        ui.pauseForUser();
        return roundState;
    }
    
    bool Game::isCastlingAttempt(const Piece* piece, Position start, Position end) const {
        return piece && piece->getType() == PieceType::KING && std::abs(end.col - start.col) == 2 && start.row == end.row;
    }

    // Helper to check if castling is safe (not moving through/into check)
    bool Game::canCastle(const Piece* king, Position kingStart, Position kingEnd, Position& rookStart, Position& rookEnd) {
        if (board.isKingInCheck(king->getColor())) return false; // Cannot castle out of check

        int kingStep = (kingEnd.col > kingStart.col) ? 1 : -1;
        Position kingPathSquare(kingStart.row, kingStart.col + kingStep); // Square king passes through

        if (board.isSquareAttacked(kingPathSquare, (king->getColor() == Color::WHITE ? Color::BLACK : Color::WHITE))) {
            return false; // King cannot pass through an attacked square
        }
        // King's landing square safety will be checked by the general makeMove validation
        
        // Determine rook positions
        rookStart = Position(kingStart.row, (kingStep > 0) ? 7 : 0);
        rookEnd = Position(kingStart.row, kingStart.col + kingStep); // Rook moves next to king on path square

        Piece* rook = board.getPiecePtr(rookStart);
        if (!rook || rook->getType() != PieceType::ROOK || rook->getHasMoved()) return false;
        if (!board.isPathClear(kingStart, rookStart)) return false;

        return true; // Conditions met for castling attempt based on checks and path
    }


    bool Game::makeMove(const Position &start, const Position &end, PieceType promotionType)
    {
        Piece *piece = board.getPiecePtr(start);
        if (!piece || piece->getColor() != currentPlayer->getColor()) return false;

        bool isCastling = isCastlingAttempt(piece, start, end);
        bool isEnPassant = false;

        if (piece->getType() == PieceType::PAWN && end == board.getEnPassantTargetSquare() &&
            std::abs(start.col - end.col) == 1 && board.getPiecePtr(end) == nullptr ) { // must be diagonal to EP target sq and target sq empty
            isEnPassant = true;
        }


        if (!piece->isValidMove(start, end, board) && !isEnPassant) { // isValidMove for pawn might not know about en-passant state directly
             // For en-passant, Pawn::isValidMove should return true if end == board.getEnPassantTargetSquare()
             // Let's rely on Pawn::isValidMove to correctly identify en-passant possibilities based on board state
            if(! (piece->getType() == PieceType::PAWN && end == board.getEnPassantTargetSquare() && piece->isValidMove(start,end,board)))
                 return false;
            if(piece->getType() == PieceType::PAWN && end == board.getEnPassantTargetSquare() && piece->isValidMove(start,end,board)){
                isEnPassant = true; // Confirm it's a valid en-passant
            } else {
                return false;
            }
        }
        
        // Special castling safety checks (cannot move out of, through, or (implicitly by tryMove) into check)
        if (isCastling) {
            Position rookStart, rookEnd; // Will be filled by canCastle
            if (board.isKingInCheck(piece->getColor())) return false; // Cannot castle if in check

            int kingDir = (end.col > start.col) ? 1 : -1;
            Position kingPathSq(start.row, start.col + kingDir);
            if (board.isSquareAttacked(kingPathSq, (piece->getColor() == Color::WHITE ? Color::BLACK : Color::WHITE))) {
                return false; // Cannot castle through check
            }
            // The landing square (end) will be checked by tryMoveForValidation
        }


        // Try the move and check if it leaves the king in check
        Board tempBoard = board; // Create a temporary board for validation
        Piece* tempPiece = tempBoard.getPiecePtr(start);
        std::unique_ptr<Piece> tempCapturedPiece = nullptr;

        // Simulate move on tempBoard
        if (isCastling) {
            // For castling, simulate both king and rook move on tempBoard
            tempBoard.movePiece(start, end, false, true);
        } else {
            tempBoard.movePiece(start, end, isEnPassant, false);
        }


        if (tempBoard.isKingInCheck(piece->getColor()))
        {
            return false; // Move would leave king in check
        }

        // If move is valid and safe, make it on the actual board
        board.movePiece(start, end, isEnPassant, isCastling);

        // Handle promotion if applicable (after the move is made)
        Piece *movedPieceOnBoard = board.getPiecePtr(end); // Get the piece that just moved
        if (movedPieceOnBoard && movedPieceOnBoard->getType() == PieceType::PAWN && promotionType != PieceType::NONE)
        {
            if ((movedPieceOnBoard->getColor() == Color::WHITE && end.row == 0) ||
                (movedPieceOnBoard->getColor() == Color::BLACK && end.row == 7))
            {
                board.promotePawn(end, promotionType);
            }
        }
        return true;
    }

    void Game::switchPlayer()
    {
        currentPlayer = (currentPlayer == player1) ? player2 : player1;
    }

    Position Game::parsePosition(const std::string &s) const
    {
        if (s.length() != 2) return Position(-1, -1); // Invalid format
        char file = std::tolower(s[0]);
        char rank = s[1];

        if (file < 'a' || file > 'h' || rank < '1' || rank > '8')
            return Position(-1, -1); // Out of bounds

        int col = file - 'a';
        int row = 7 - (rank - '1'); // '1' maps to row 7, '8' to row 0
        return Position(row, col);
    }

    bool Game::canPlayerMakeAnyLegalMove(Player *player)
    {
        Color playerColor = player->getColor();
        for (int r = 0; r < 8; ++r)
        {
            for (int c = 0; c < 8; ++c)
            {
                Position start(r, c);
                Piece *p = board.getPiecePtr(start);
                if (p && p->getColor() == playerColor)
                {
                    for (int r2 = 0; r2 < 8; ++r2)
                    {
                        for (int c2 = 0; c2 < 8; ++c2)
                        {
                            Position end(r2, c2);
                            if (start == end) continue;

                            bool isCastlingPossible = isCastlingAttempt(p, start, end);
                            bool isEnPassantPossible = (p->getType() == PieceType::PAWN && 
                                                       end == board.getEnPassantTargetSquare() &&
                                                       std::abs(start.col - end.col) == 1 &&
                                                       board.getPiecePtr(end) == nullptr);
                            
                            if (p->isValidMove(start, end, board) || isEnPassantPossible)
                            {
                                Board tempBoard = board; // Use a copy for validation
                                Piece* tempPiece = tempBoard.getPiecePtr(start);
                                
                                // Simulate move on tempBoard
                                bool tempIsCastling = isCastlingPossible && p->isValidMove(start,end,board);
                                if (tempIsCastling) {
                                     // Check special castling safety rules before simulating
                                    if (tempBoard.isKingInCheck(playerColor)) continue;
                                    int kingDir = (end.col > start.col) ? 1 : -1;
                                    Position kingPathSq(start.row, start.col + kingDir);
                                    if (tempBoard.isSquareAttacked(kingPathSq, (playerColor == Color::WHITE ? Color::BLACK : Color::WHITE))) {
                                        continue; 
                                    }
                                    tempBoard.movePiece(start, end, false, true);
                                } else {
                                     tempBoard.movePiece(start, end, isEnPassantPossible, false);
                                }


                                if (!tempBoard.isKingInCheck(playerColor))
                                {
                                    return true; // Found a legal move
                                }
                            }
                        }
                    }
                }
            }
        }
        return false; // No legal moves found
    }

    void Game::checkForEndOfRound()
    {
        // Check for current player's opponent
        Player* opponent = (currentPlayer == player1) ? player2 : player1;
        Color opponentColor = opponent->getColor();

        bool opponentInCheck = board.isKingInCheck(opponentColor);
        bool opponentHasLegalMove = canPlayerMakeAnyLegalMove(opponent);

        if (opponentInCheck && !opponentHasLegalMove)
        {
            roundState = (currentPlayer->getColor() == Color::WHITE) ? RoundState::CHECKMATE_WHITE_WINS : RoundState::CHECKMATE_BLACK_WINS;
        }
        else if (!opponentInCheck && !opponentHasLegalMove)
        {
            roundState = RoundState::STALEMATE;
        }
        else
        {
            roundState = RoundState::ONGOING;
        }
    }
} // namespace HardChess