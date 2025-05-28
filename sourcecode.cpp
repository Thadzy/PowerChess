// // File: include/PowerChess/Core/CommonTypes.h
// #ifndef POWERCHESS_CORE_COMMONTYPES_H
// #define POWERCHESS_CORE_COMMONTYPES_H

// #include <vector>
// #include <string>
// #include <optional> // For std::optional

// namespace PowerChess {

// // Enum for player colors
// enum class PlayerColor {
//     WHITE,
//     BLACK,
//     NONE // For empty squares or neutral elements
// };

// // Enum for piece types
// enum class PieceType {
//     PAWN,
//     ROOK,
//     KNIGHT,
//     BISHOP,
//     QUEEN,
//     KING,
//     NONE // For empty squares
// };

// // Struct to represent a position on the board (0-indexed)
// struct Position {
//     int row;
//     int col;

//     Position(int r = -1, int c = -1) : row(r), col(c) {}

//     bool isValid() const {
//         return row >= 0 && row < 8 && col >= 0 && col < 8;
//     }

//     bool operator==(const Position& other) const {
//         return row == other.row && col == other.col;
//     }
    
//     // For using Position as a key in std::map or std::set if needed
//     bool operator<(const Position& other) const {
//         if (row != other.row) {
//             return row < other.row;
//         }
//         return col < other.col;
//     }
// };

// // Struct to represent a move
// struct Move {
//     Position from;
//     Position to;
//     PieceType promotionPiece = PieceType::NONE; // For pawn promotion

//     Move(Position f, Position t, PieceType promo = PieceType::NONE) : from(f), to(t), promotionPiece(promo) {}
// };

// // Enum for Game State (for a round)
// enum class RoundState {
//     PLAYING,
//     CHECK,
//     CHECKMATE,
//     STALEMATE,
//     ROUND_OVER, // Intermediate state before next round or power-up
//     POWERUP_SELECTION
// };

// // Enum for PowerUp Types (expandable)
// enum class PowerUpType {
//     KNIGHT_BOOST_QUEEN_MOVE,
//     // Add other power-up types here
//     NONE
// };

// } // namespace PowerChess

// #endif // POWERCHESS_CORE_COMMONTYPES_H

// // File: include/PowerChess/Core/Piece.h
// #ifndef POWERCHESS_CORE_PIECE_H
// #define POWERCHESS_CORE_PIECE_H

// #include "CommonTypes.h"
// #include <vector>
// #include <string>
// #include <memory> // For std::unique_ptr

// namespace PowerChess {

// class Board; // Forward declaration
// class Player; // Forward declaration

// class Piece {
// protected:
//     PlayerColor color;
//     PieceType type;
//     Position position;
//     bool hasMoved;

// public:
//     Piece(PlayerColor c, PieceType t, Position p) : color(c), type(t), position(p), hasMoved(false) {}
//     virtual ~Piece() = default;

//     PlayerColor getColor() const { return color; }
//     PieceType getType() const { return type; }
//     Position getPosition() const { return position; }
//     void setPosition(Position newPos) { position = newPos; }
//     bool getHasMoved() const { return hasMoved; }
//     void setHasMoved(bool moved) { hasMoved = moved; }

//     // Pure virtual function for getting valid moves
//     // Takes the board state to check for obstructions, friendly pieces, etc.
//     // Takes current player to check for special conditions like Knight Boost
//     virtual std::vector<Position> getValidMoves(const Board& board, const Player* currentPlayer = nullptr) const = 0;

//     // Virtual function to get a character representation of the piece
//     virtual char getSymbol() const = 0;
    
//     // Helper to check if a move puts own king in check
//     bool wouldBeInCheckAfterMove(const Board& board, Position toPos) const;
// };

// } // namespace PowerChess

// #endif // POWERCHESS_CORE_PIECE_H

// // File: src/Core/Piece.cpp
// #include "PowerChess/Core/Piece.h"
// #include "PowerChess/Core/Board.h" // Required for full definition for wouldBeInCheckAfterMove
// #include "PowerChess/Core/Player.h" // Required for Player definition

// namespace PowerChess {

// // Basic implementation - can be expanded
// // This is a simplified check. A full check would involve simulating the move.
// bool Piece::wouldBeInCheckAfterMove(const Board& board, Position toPos) const {
//     // Simulate the move on a temporary board
//     Board tempBoard = board; // Requires Board to have a copy constructor
//     tempBoard.movePieceInternal(this->position, toPos); // A simplified internal move

//     // Find the current player's king on the temporary board
//     Position kingPos = tempBoard.findKing(this->color);
//     if (!kingPos.isValid()) return true; // Should not happen

//     // Check if the king is under attack on the temporary board
//     return tempBoard.isSquareAttacked(kingPos, (this->color == PlayerColor::WHITE ? PlayerColor::BLACK : PlayerColor::WHITE));
// }

// } // namespace PowerChess


// // File: include/PowerChess/Core/Pawn.h
// #ifndef POWERCHESS_CORE_PAWN_H
// #define POWERCHESS_CORE_PAWN_H

// #include "Piece.h"

// namespace PowerChess {

// class Pawn : public Piece {
// public:
//     Pawn(PlayerColor c, Position p) : Piece(c, PieceType::PAWN, p) {}
//     std::vector<Position> getValidMoves(const Board& board, const Player* currentPlayer = nullptr) const override;
//     char getSymbol() const override;
// };

// } // namespace PowerChess

// #endif // POWERCHESS_CORE_PAWN_H

// // File: src/Core/Pawn.cpp
// #include "PowerChess/Core/Pawn.h"
// #include "PowerChess/Core/Board.h" // For Board class
// #include "PowerChess/Core/Player.h" // For Player class (if needed for power-ups)

// namespace PowerChess {

// std::vector<Position> Pawn::getValidMoves(const Board& board, const Player* currentPlayer) const {
//     std::vector<Position> moves;
//     int direction = (color == PlayerColor::WHITE) ? -1 : 1; // White moves up (row decreases), Black moves down

//     // Forward move
//     Position oneStep{position.row + direction, position.col};
//     if (board.isPositionValid(oneStep) && board.getPieceAt(oneStep) == nullptr) {
//         if (!wouldBeInCheckAfterMove(board, oneStep)) moves.push_back(oneStep);
        
//         // Double step from starting position
//         if (!hasMoved) {
//             Position twoSteps{position.row + 2 * direction, position.col};
//             if (board.isPositionValid(twoSteps) && board.getPieceAt(twoSteps) == nullptr) {
//                  if (!wouldBeInCheckAfterMove(board, twoSteps)) moves.push_back(twoSteps);
//             }
//         }
//     }

//     // Captures
//     Position captureLeft{position.row + direction, position.col - 1};
//     if (board.isPositionValid(captureLeft)) {
//         const Piece* target = board.getPieceAt(captureLeft);
//         if (target != nullptr && target->getColor() != color) {
//             if (!wouldBeInCheckAfterMove(board, captureLeft)) moves.push_back(captureLeft);
//         }
//     }

//     Position captureRight{position.row + direction, position.col + 1};
//     if (board.isPositionValid(captureRight)) {
//         const Piece* target = board.getPieceAt(captureRight);
//         if (target != nullptr && target->getColor() != color) {
//             if (!wouldBeInCheckAfterMove(board, captureRight)) moves.push_back(captureRight);
//         }
//     }

//     // En passant (simplified - requires tracking last move)
//     // For a full implementation, the Board would need to store the last move made.
//     // If board.lastMove was a pawn moving two steps to position.col +/- 1, position.row
//     // then en passant is possible.

//     return moves;
// }

// char Pawn::getSymbol() const {
//     return (color == PlayerColor::WHITE) ? 'P' : 'p';
// }

// } // namespace PowerChess

// // File: include/PowerChess/Core/Rook.h
// #ifndef POWERCHESS_CORE_ROOK_H
// #define POWERCHESS_CORE_ROOK_H

// #include "Piece.h"

// namespace PowerChess {

// class Rook : public Piece {
// public:
//     Rook(PlayerColor c, Position p) : Piece(c, PieceType::ROOK, p) {}
//     std::vector<Position> getValidMoves(const Board& board, const Player* currentPlayer = nullptr) const override;
//     char getSymbol() const override;
// };

// } // namespace PowerChess

// #endif // POWERCHESS_CORE_ROOK_H

// // File: src/Core/Rook.cpp
// #include "PowerChess/Core/Rook.h"
// #include "PowerChess/Core/Board.h"
// #include "PowerChess/Core/Player.h"

// namespace PowerChess {

// std::vector<Position> Rook::getValidMoves(const Board& board, const Player* currentPlayer) const {
//     std::vector<Position> moves;
//     int dr[] = {-1, 1, 0, 0}; // Directions: up, down, left, right
//     int dc[] = {0, 0, -1, 1};

//     for (int i = 0; i < 4; ++i) {
//         for (int j = 1; j < 8; ++j) {
//             Position nextPos{position.row + dr[i] * j, position.col + dc[i] * j};
//             if (!board.isPositionValid(nextPos)) break;

//             const Piece* target = board.getPieceAt(nextPos);
//             if (target == nullptr) {
//                 if (!wouldBeInCheckAfterMove(board, nextPos)) moves.push_back(nextPos);
//             } else {
//                 if (target->getColor() != color) {
//                     if (!wouldBeInCheckAfterMove(board, nextPos)) moves.push_back(nextPos);
//                 }
//                 break; // Blocked by a piece
//             }
//         }
//     }
//     return moves;
// }

// char Rook::getSymbol() const {
//     return (color == PlayerColor::WHITE) ? 'R' : 'r';
// }

// } // namespace PowerChess

// // File: include/PowerChess/Core/Knight.h
// #ifndef POWERCHESS_CORE_KNIGHT_H
// #define POWERCHESS_CORE_KNIGHT_H

// #include "Piece.h"

// namespace PowerChess {

// class Knight : public Piece {
// public:
//     Knight(PlayerColor c, Position p) : Piece(c, PieceType::KNIGHT, p) {}
//     std::vector<Position> getValidMoves(const Board& board, const Player* currentPlayer = nullptr) const override;
//     char getSymbol() const override;
// };

// } // namespace PowerChess

// #endif // POWERCHESS_CORE_KNIGHT_H

// // File: src/Core/Knight.cpp
// #include "PowerChess/Core/Knight.h"
// #include "PowerChess/Core/Board.h"
// #include "PowerChess/Core/Player.h" // For checking Knight Boost

// namespace PowerChess {

// std::vector<Position> Knight::getValidMoves(const Board& board, const Player* currentPlayer) const {
//     std::vector<Position> moves;

//     // Standard Knight moves
//     int drNight[] = {-2, -2, -1, -1, 1, 1, 2, 2};
//     int dcNight[] = {-1, 1, -2, 2, -2, 2, -1, 1};

//     for (int i = 0; i < 8; ++i) {
//         Position nextPos{position.row + drNight[i], position.col + dcNight[i]};
//         if (board.isPositionValid(nextPos)) {
//             const Piece* target = board.getPieceAt(nextPos);
//             if (target == nullptr || target->getColor() != color) {
//                 if (!wouldBeInCheckAfterMove(board, nextPos)) moves.push_back(nextPos);
//             }
//         }
//     }
    
//     // Knight Boost: Queen-like movement for one turn
//     if (currentPlayer && currentPlayer->getColor() == this->color && currentPlayer->isKnightBoostActiveForPiece(this)) {
//         // Rook-like moves (part of Queen)
//         int drQueenRook[] = {-1, 1, 0, 0};
//         int dcQueenRook[] = {0, 0, -1, 1};
//         for (int i = 0; i < 4; ++i) {
//             for (int j = 1; j < 8; ++j) {
//                 Position nextPos{position.row + drQueenRook[i] * j, position.col + dcQueenRook[i] * j};
//                 if (!board.isPositionValid(nextPos)) break;
//                 const Piece* target = board.getPieceAt(nextPos);
//                 if (target == nullptr) {
//                     if (!wouldBeInCheckAfterMove(board, nextPos)) moves.push_back(nextPos);
//                 } else {
//                     if (target->getColor() != color) {
//                          if (!wouldBeInCheckAfterMove(board, nextPos)) moves.push_back(nextPos);
//                     }
//                     break; 
//                 }
//             }
//         }
//         // Bishop-like moves (part of Queen)
//         int drQueenBishop[] = {-1, -1, 1, 1};
//         int dcQueenBishop[] = {-1, 1, -1, 1};
//         for (int i = 0; i < 4; ++i) {
//             for (int j = 1; j < 8; ++j) {
//                 Position nextPos{position.row + drQueenBishop[i] * j, position.col + dcQueenBishop[i] * j};
//                 if (!board.isPositionValid(nextPos)) break;
//                 const Piece* target = board.getPieceAt(nextPos);
//                 if (target == nullptr) {
//                     if (!wouldBeInCheckAfterMove(board, nextPos)) moves.push_back(nextPos);
//                 } else {
//                     if (target->getColor() != color) {
//                         if (!wouldBeInCheckAfterMove(board, nextPos)) moves.push_back(nextPos);
//                     }
//                     break;
//                 }
//             }
//         }
//     }


//     return moves;
// }

// char Knight::getSymbol() const {
//     return (color == PlayerColor::WHITE) ? 'N' : 'n';
// }

// } // namespace PowerChess

// // File: include/PowerChess/Core/Bishop.h
// #ifndef POWERCHESS_CORE_BISHOP_H
// #define POWERCHESS_CORE_BISHOP_H

// #include "Piece.h"

// namespace PowerChess {

// class Bishop : public Piece {
// public:
//     Bishop(PlayerColor c, Position p) : Piece(c, PieceType::BISHOP, p) {}
//     std::vector<Position> getValidMoves(const Board& board, const Player* currentPlayer = nullptr) const override;
//     char getSymbol() const override;
// };

// } // namespace PowerChess

// #endif // POWERCHESS_CORE_BISHOP_H

// // File: src/Core/Bishop.cpp
// #include "PowerChess/Core/Bishop.h"
// #include "PowerChess/Core/Board.h"
// #include "PowerChess/Core/Player.h"

// namespace PowerChess {

// std::vector<Position> Bishop::getValidMoves(const Board& board, const Player* currentPlayer) const {
//     std::vector<Position> moves;
//     int dr[] = {-1, -1, 1, 1}; // Directions: diagonals
//     int dc[] = {-1, 1, -1, 1};

//     for (int i = 0; i < 4; ++i) {
//         for (int j = 1; j < 8; ++j) {
//             Position nextPos{position.row + dr[i] * j, position.col + dc[i] * j};
//             if (!board.isPositionValid(nextPos)) break;

//             const Piece* target = board.getPieceAt(nextPos);
//             if (target == nullptr) {
//                 if (!wouldBeInCheckAfterMove(board, nextPos)) moves.push_back(nextPos);
//             } else {
//                 if (target->getColor() != color) {
//                     if (!wouldBeInCheckAfterMove(board, nextPos)) moves.push_back(nextPos);
//                 }
//                 break; // Blocked
//             }
//         }
//     }
//     return moves;
// }

// char Bishop::getSymbol() const {
//     return (color == PlayerColor::WHITE) ? 'B' : 'b';
// }

// } // namespace PowerChess

// // File: include/PowerChess/Core/Queen.h
// #ifndef POWERCHESS_CORE_QUEEN_H
// #define POWERCHESS_CORE_QUEEN_H

// #include "Piece.h"

// namespace PowerChess {

// class Queen : public Piece {
// public:
//     Queen(PlayerColor c, Position p) : Piece(c, PieceType::QUEEN, p) {}
//     std::vector<Position> getValidMoves(const Board& board, const Player* currentPlayer = nullptr) const override;
//     char getSymbol() const override;
// };

// } // namespace PowerChess

// #endif // POWERCHESS_CORE_QUEEN_H

// // File: src/Core/Queen.cpp
// #include "PowerChess/Core/Queen.h"
// #include "PowerChess/Core/Board.h"
// #include "PowerChess/Core/Player.h"

// namespace PowerChess {

// std::vector<Position> Queen::getValidMoves(const Board& board, const Player* currentPlayer) const {
//     std::vector<Position> moves;
//     // Combines Rook and Bishop movement
//     int dr[] = {-1, 1, 0, 0, -1, -1, 1, 1}; // All 8 directions
//     int dc[] = {0, 0, -1, 1, -1, 1, -1, 1};

//     for (int i = 0; i < 8; ++i) {
//         for (int j = 1; j < 8; ++j) {
//             Position nextPos{position.row + dr[i] * j, position.col + dc[i] * j};
//             if (!board.isPositionValid(nextPos)) break;

//             const Piece* target = board.getPieceAt(nextPos);
//             if (target == nullptr) {
//                 if (!wouldBeInCheckAfterMove(board, nextPos)) moves.push_back(nextPos);
//             } else {
//                 if (target->getColor() != color) {
//                     if (!wouldBeInCheckAfterMove(board, nextPos)) moves.push_back(nextPos);
//                 }
//                 break; // Blocked
//             }
//         }
//     }
//     return moves;
// }

// char Queen::getSymbol() const {
//     return (color == PlayerColor::WHITE) ? 'Q' : 'q';
// }

// } // namespace PowerChess

// // File: include/PowerChess/Core/King.h
// #ifndef POWERCHESS_CORE_KING_H
// #define POWERCHESS_CORE_KING_H

// #include "Piece.h"

// namespace PowerChess {

// class King : public Piece {
// public:
//     King(PlayerColor c, Position p) : Piece(c, PieceType::KING, p) {}
//     std::vector<Position> getValidMoves(const Board& board, const Player* currentPlayer = nullptr) const override;
//     char getSymbol() const override;
//     // Add castling availability checks if implementing castling
// };

// } // namespace PowerChess

// #endif // POWERCHESS_CORE_KING_H

// // File: src/Core/King.cpp
// #include "PowerChess/Core/King.h"
// #include "PowerChess/Core/Board.h"
// #include "PowerChess/Core/Player.h"

// namespace PowerChess {

// std::vector<Position> King::getValidMoves(const Board& board, const Player* currentPlayer) const {
//     std::vector<Position> moves;
//     int dr[] = {-1, -1, -1, 0, 0, 1, 1, 1}; // All 8 adjacent squares
//     int dc[] = {-1, 0, 1, -1, 1, -1, 0, 1};

//     for (int i = 0; i < 8; ++i) {
//         Position nextPos{position.row + dr[i], position.col + dc[i]};
//         if (board.isPositionValid(nextPos)) {
//             const Piece* target = board.getPieceAt(nextPos);
//             // Cannot move into a square attacked by the opponent
//             if (!board.isSquareAttacked(nextPos, (color == PlayerColor::WHITE ? PlayerColor::BLACK : PlayerColor::WHITE))) {
//                 if (target == nullptr || target->getColor() != color) {
//                      // Check if this move puts the king in check (should be implicitly handled by isSquareAttacked for target)
//                      // but an explicit check is safer for king moves.
//                     Board tempBoard = board;
//                     tempBoard.movePieceInternal(this->position, nextPos); // Simulate
//                     if (!tempBoard.isKingInCheck(this->color)) {
//                          moves.push_back(nextPos);
//                     }
//                 }
//             }
//         }
//     }
//     // TODO: Castling logic
//     // 1. King hasn't moved.
//     // 2. Corresponding Rook hasn't moved.
//     // 3. Squares between King and Rook are empty.
//     // 4. King does not pass through or land on an attacked square.
//     return moves;
// }

// char King::getSymbol() const {
//     return (color == PlayerColor::WHITE) ? 'K' : 'k';
// }

// } // namespace PowerChess

// // File: include/PowerChess/Core/Board.h
// #ifndef POWERCHESS_CORE_BOARD_H
// #define POWERCHESS_CORE_BOARD_H

// #include "CommonTypes.h"
// #include "Piece.h"
// #include <vector>
// #include <memory> // For std::unique_ptr, std::shared_ptr

// namespace PowerChess {

// class Board {
// private:
//     std::vector<std::vector<std::unique_ptr<Piece>>> grid;
//     // To store captured pieces if needed
//     // std::vector<std::unique_ptr<Piece>> capturedWhitePieces;
//     // std::vector<std::unique_ptr<Piece>> capturedBlackPieces;
//     Move lastMove = {Position(-1,-1), Position(-1,-1)};


// public:
//     Board(); // Constructor to set up the initial board
//     Board(const Board& other); // Copy constructor
//     Board& operator=(const Board& other); // Copy assignment

//     // Rule of five: destructor, move constructor, move assignment (if managing resources directly)
//     // Since unique_ptr handles memory, default destructor is okay.
//     // Move constructor/assignment might be useful for efficiency if Board objects are moved often.
//     // For now, copy constructor is most important for simulations like `wouldBeInCheckAfterMove`.


//     const Piece* getPieceAt(Position pos) const;
//     Piece* getPieceAtModifiable(Position pos); // To modify piece (e.g. setHasMoved)
//     void setPieceAt(Position pos, std::unique_ptr<Piece> piece);
//     std::unique_ptr<Piece> removePieceAt(Position pos);


//     void initializeBoard();
//     void displayConsole() const; // For debugging

//     bool isPositionValid(Position pos) const;
    
//     // Moves a piece, handles captures, returns captured piece if any
//     std::unique_ptr<Piece> movePiece(Position from, Position to, PieceType promotionType = PieceType::NONE);
//     void movePieceInternal(Position from, Position to); // Simplified for simulation


//     bool isPathClear(Position start, Position end) const; // For Rooks, Bishops, Queens

//     Position findKing(PlayerColor kingColor) const;
//     bool isSquareAttacked(Position square, PlayerColor attackerColor) const;
//     bool isKingInCheck(PlayerColor kingColor) const;
//     bool hasLegalMoves(PlayerColor playerColor, const Player* playerPtr); // playerPtr for KnightBoost check

//     Move getLastMove() const { return lastMove; }
//     void setLastMove(Move move) { lastMove = move; }
// };

// } // namespace PowerChess

// #endif // POWERCHESS_CORE_BOARD_H

// // File: src/Core/Board.cpp
// #include "PowerChess/Core/Board.h"
// #include "PowerChess/Core/Pawn.h"
// #include "PowerChess/Core/Rook.h"
// #include "PowerChess/Core/Knight.h"
// #include "PowerChess/Core/Bishop.h"
// #include "PowerChess/Core/Queen.h"
// #include "PowerChess/Core/King.h"
// #include "PowerChess/Core/Player.h" // For hasLegalMoves to pass player context
// #include <iostream> // For displayConsole
// #include <stdexcept> // For runtime_error

// namespace PowerChess {

// Board::Board() {
//     grid.resize(8, std::vector<std::unique_ptr<Piece>>(8, nullptr));
//     initializeBoard();
// }

// // Deep copy constructor
// Board::Board(const Board& other) {
//     grid.resize(8, std::vector<std::unique_ptr<Piece>>(8, nullptr));
//     for (int r = 0; r < 8; ++r) {
//         for (int c = 0; c < 8; ++c) {
//             if (other.grid[r][c] != nullptr) {
//                 const Piece* p = other.grid[r][c].get();
//                 // This is a bit tricky with unique_ptr and polymorphism for copying.
//                 // A common solution is a virtual clone() method in Piece.
//                 // For simplicity here, we'll re-create based on type.
//                 switch (p->getType()) {
//                     case PieceType::PAWN:   grid[r][c] = std::make_unique<Pawn>(p->getColor(), p->getPosition()); break;
//                     case PieceType::ROOK:   grid[r][c] = std::make_unique<Rook>(p->getColor(), p->getPosition()); break;
//                     case PieceType::KNIGHT: grid[r][c] = std::make_unique<Knight>(p->getColor(), p->getPosition()); break;
//                     case PieceType::BISHOP: grid[r][c] = std::make_unique<Bishop>(p->getColor(), p->getPosition()); break;
//                     case PieceType::QUEEN:  grid[r][c] = std::make_unique<Queen>(p->getColor(), p->getPosition()); break;
//                     case PieceType::KING:   grid[r][c] = std::make_unique<King>(p->getColor(), p->getPosition()); break;
//                     default: break;
//                 }
//                 if (grid[r][c]) {
//                     grid[r][c]->setHasMoved(p->getHasMoved());
//                 }
//             }
//         }
//     }
//     this->lastMove = other.lastMove;
// }


// Board& Board::operator=(const Board& other) {
//     if (this == &other) return *this;
//     grid.assign(8, std::vector<std::unique_ptr<Piece>>(8, nullptr)); // Clear current grid
//      for (int r = 0; r < 8; ++r) {
//         for (int c = 0; c < 8; ++c) {
//             if (other.grid[r][c] != nullptr) {
//                 const Piece* p = other.grid[r][c].get();
//                 switch (p->getType()) {
//                     case PieceType::PAWN:   grid[r][c] = std::make_unique<Pawn>(p->getColor(), p->getPosition()); break;
//                     case PieceType::ROOK:   grid[r][c] = std::make_unique<Rook>(p->getColor(), p->getPosition()); break;
//                     case PieceType::KNIGHT: grid[r][c] = std::make_unique<Knight>(p->getColor(), p->getPosition()); break;
//                     case PieceType::BISHOP: grid[r][c] = std::make_unique<Bishop>(p->getColor(), p->getPosition()); break;
//                     case PieceType::QUEEN:  grid[r][c] = std::make_unique<Queen>(p->getColor(), p->getPosition()); break;
//                     case PieceType::KING:   grid[r][c] = std::make_unique<King>(p->getColor(), p->getPosition()); break;
//                     default: break;
//                 }
//                  if (grid[r][c]) {
//                     grid[r][c]->setHasMoved(p->getHasMoved());
//                 }
//             }
//         }
//     }
//     this->lastMove = other.lastMove;
//     return *this;
// }


// void Board::initializeBoard() {
//     // Clear board first
//     for (int r = 0; r < 8; ++r) {
//         for (int c = 0; c < 8; ++c) {
//             grid[r][c].reset();
//         }
//     }

//     // Place Pawns
//     for (int c = 0; c < 8; ++c) {
//         grid[1][c] = std::make_unique<Pawn>(PlayerColor::BLACK, Position(1, c));
//         grid[6][c] = std::make_unique<Pawn>(PlayerColor::WHITE, Position(6, c));
//     }

//     // Place Rooks
//     grid[0][0] = std::make_unique<Rook>(PlayerColor::BLACK, Position(0, 0));
//     grid[0][7] = std::make_unique<Rook>(PlayerColor::BLACK, Position(0, 7));
//     grid[7][0] = std::make_unique<Rook>(PlayerColor::WHITE, Position(7, 0));
//     grid[7][7] = std::make_unique<Rook>(PlayerColor::WHITE, Position(7, 7));

//     // Place Knights
//     grid[0][1] = std::make_unique<Knight>(PlayerColor::BLACK, Position(0, 1));
//     grid[0][6] = std::make_unique<Knight>(PlayerColor::BLACK, Position(0, 6));
//     grid[7][1] = std::make_unique<Knight>(PlayerColor::WHITE, Position(7, 1));
//     grid[7][6] = std::make_unique<Knight>(PlayerColor::WHITE, Position(7, 6));

//     // Place Bishops
//     grid[0][2] = std::make_unique<Bishop>(PlayerColor::BLACK, Position(0, 2));
//     grid[0][5] = std::make_unique<Bishop>(PlayerColor::BLACK, Position(0, 5));
//     grid[7][2] = std::make_unique<Bishop>(PlayerColor::WHITE, Position(7, 2));
//     grid[7][5] = std::make_unique<Bishop>(PlayerColor::WHITE, Position(7, 5));

//     // Place Queens
//     grid[0][3] = std::make_unique<Queen>(PlayerColor::BLACK, Position(0, 3));
//     grid[7][3] = std::make_unique<Queen>(PlayerColor::WHITE, Position(7, 3));

//     // Place Kings
//     grid[0][4] = std::make_unique<King>(PlayerColor::BLACK, Position(0, 4));
//     grid[7][4] = std::make_unique<King>(PlayerColor::WHITE, Position(7, 4));
// }

// const Piece* Board::getPieceAt(Position pos) const {
//     if (!isPositionValid(pos)) return nullptr;
//     return grid[pos.row][pos.col].get();
// }

// Piece* Board::getPieceAtModifiable(Position pos) {
//     if (!isPositionValid(pos)) return nullptr;
//     return grid[pos.row][pos.col].get();
// }


// void Board::setPieceAt(Position pos, std::unique_ptr<Piece> piece) {
//     if (!isPositionValid(pos)) return;
//     grid[pos.row][pos.col] = std::move(piece);
//     if (grid[pos.row][pos.col]) {
//         grid[pos.row][pos.col]->setPosition(pos);
//     }
// }

// std::unique_ptr<Piece> Board::removePieceAt(Position pos) {
//     if (!isPositionValid(pos) || !grid[pos.row][pos.col]) return nullptr;
//     return std::move(grid[pos.row][pos.col]);
// }


// std::unique_ptr<Piece> Board::movePiece(Position from, Position to, PieceType promotionType) {
//     if (!isPositionValid(from) || !isPositionValid(to) || !grid[from.row][from.col]) {
//         // Or throw an exception
//         return nullptr;
//     }

//     std::unique_ptr<Piece> movedPiece = std::move(grid[from.row][from.col]);
//     std::unique_ptr<Piece> capturedPiece = nullptr;

//     if (grid[to.row][to.col]) { // Capture
//         capturedPiece = std::move(grid[to.row][to.col]);
//     }

//     grid[to.row][to.col] = std::move(movedPiece);
//     grid[to.row][to.col]->setPosition(to);
//     grid[to.row][to.col]->setHasMoved(true);

//     // Pawn Promotion
//     if (grid[to.row][to.col]->getType() == PieceType::PAWN) {
//         if ((grid[to.row][to.col]->getColor() == PlayerColor::WHITE && to.row == 0) ||
//             (grid[to.row][to.col]->getColor() == PlayerColor::BLACK && to.row == 7)) {
//             if (promotionType != PieceType::NONE) {
//                 PlayerColor color = grid[to.row][to.col]->getColor();
//                 switch (promotionType) {
//                     case PieceType::QUEEN:  grid[to.row][to.col] = std::make_unique<Queen>(color, to); break;
//                     case PieceType::ROOK:   grid[to.row][to.col] = std::make_unique<Rook>(color, to); break;
//                     case PieceType::BISHOP: grid[to.row][to.col] = std::make_unique<Bishop>(color, to); break;
//                     case PieceType::KNIGHT: grid[to.row][to.col] = std::make_unique<Knight>(color, to); break;
//                     default: /* Should not happen if promotionType is validated */ break; 
//                 }
//                  if(grid[to.row][to.col]) grid[to.row][to.col]->setHasMoved(true);
//             } else {
//                 // Default to Queen if no type specified (or handle error)
//                 grid[to.row][to.col] = std::make_unique<Queen>(grid[to.row][to.col]->getColor(), to);
//                 if(grid[to.row][to.col]) grid[to.row][to.col]->setHasMoved(true);
//             }
//         }
//     }
//     lastMove = Move(from, to, promotionType);
//     return capturedPiece;
// }

// // Simplified move for simulations, no capture handling, no promotion
// void Board::movePieceInternal(Position from, Position to) {
//     if (!isPositionValid(from) || !isPositionValid(to) || !grid[from.row][from.col]) {
//         return;
//     }
//     grid[to.row][to.col] = std::move(grid[from.row][from.col]);
//     if (grid[to.row][to.col]) {
//         grid[to.row][to.col]->setPosition(to);
//         // Don't set hasMoved here, as it's a simulation
//     }
// }


// void Board::displayConsole() const {
//     std::cout << "  a b c d e f g h" << std::endl;
//     std::cout << " -----------------" << std::endl;
//     for (int r = 0; r < 8; ++r) {
//         std::cout << 8 - r << "|";
//         for (int c = 0; c < 8; ++c) {
//             if (grid[r][c]) {
//                 std::cout << grid[r][c]->getSymbol() << "|";
//             } else {
//                 std::cout << " |";
//             }
//         }
//         std::cout << " " << 8 - r << std::endl;
//         std::cout << " -----------------" << std::endl;
//     }
//     std::cout << "  a b c d e f g h" << std::endl;
// }

// bool Board::isPositionValid(Position pos) const {
//     return pos.row >= 0 && pos.row < 8 && pos.col >= 0 && pos.col < 8;
// }

// bool Board::isPathClear(Position start, Position end) const {
//     // Simplified: assumes straight or diagonal line. Does not validate if move is rook/bishop like.
//     // That's done by the piece's getValidMoves.
//     int dr = (end.row > start.row) ? 1 : ((end.row < start.row) ? -1 : 0);
//     int dc = (end.col > start.col) ? 1 : ((end.col < start.col) ? -1 : 0);

//     Position current = {start.row + dr, start.col + dc};
//     while (current.row != end.row || current.col != end.col) {
//         if (!isPositionValid(current)) return false; // Should not happen if move is valid
//         if (getPieceAt(current) != nullptr) return false; // Path is blocked
//         current.row += dr;
//         current.col += dc;
//     }
//     return true;
// }


// Position Board::findKing(PlayerColor kingColor) const {
//     for (int r = 0; r < 8; ++r) {
//         for (int c = 0; c < 8; ++c) {
//             const Piece* p = getPieceAt({r, c});
//             if (p != nullptr && p->getType() == PieceType::KING && p->getColor() == kingColor) {
//                 return {r, c};
//             }
//         }
//     }
//     return {-1, -1}; // Should not happen in a valid game
// }

// bool Board::isSquareAttacked(Position square, PlayerColor attackerColor) const {
//     for (int r = 0; r < 8; ++r) {
//         for (int c = 0; c < 8; ++c) {
//             const Piece* p = getPieceAt({r, c});
//             if (p != nullptr && p->getColor() == attackerColor) {
//                 // For pawn attacks, getValidMoves is complex as it includes non-capturing moves.
//                 // We need a specific "canAttackSquare" method or check carefully.
//                 // For now, a simplified check:
//                 if (p->getType() == PieceType::PAWN) {
//                     int direction = (p->getColor() == PlayerColor::WHITE) ? -1 : 1;
//                     Position captureLeft{p->getPosition().row + direction, p->getPosition().col - 1};
//                     Position captureRight{p->getPosition().row + direction, p->getPosition().col + 1};
//                     if (captureLeft == square || captureRight == square) return true;
//                 } else {
//                     std::vector<Position> moves = p->getValidMoves(*this); // Pass current player as null for general check
//                     for (const auto& move : moves) {
//                         if (move == square) {
//                              // For sliding pieces, ensure path is clear to the target square (already part of getValidMoves)
//                             return true;
//                         }
//                     }
//                 }
//             }
//         }
//     }
//     return false;
// }

// bool Board::isKingInCheck(PlayerColor kingColor) const {
//     Position kingPos = findKing(kingColor);
//     if (!kingPos.isValid()) return false; // Or throw error
//     return isSquareAttacked(kingPos, (kingColor == PlayerColor::WHITE ? PlayerColor::BLACK : PlayerColor::WHITE));
// }

// bool Board::hasLegalMoves(PlayerColor playerColor, const Player* playerPtr) {
//     for (int r = 0; r < 8; ++r) {
//         for (int c = 0; c < 8; ++c) {
//             Piece* p = getPieceAtModifiable({r,c}); // Modifiable to pass to getValidMoves if it changes state (it shouldn't)
//             if (p && p->getColor() == playerColor) {
//                 std::vector<Position> moves = p->getValidMoves(*this, playerPtr);
//                 if (!moves.empty()) {
//                     return true;
//                 }
//             }
//         }
//     }
//     return false;
// }


// } // namespace PowerChess

// // File: include/PowerChess/Core/Player.h
// #ifndef POWERCHESS_CORE_PLAYER_H
// #define POWERCHESS_CORE_PLAYER_H

// #include "CommonTypes.h"
// #include "PowerChess/PowerUps/PowerUp.h" // Forward declare or include full
// #include <vector>
// #include <string>
// #include <memory>

// namespace PowerChess {

// class Piece; // Forward declaration

// class Player {
// private:
//     PlayerColor color;
//     int hearts;
//     int roundsWon;
//     std::vector<std::unique_ptr<PowerUp>> availablePowerUps;
//     PowerUpType activePowerUpType; // Type of the currently active power-up
//     const Piece* knightBoostedPiece; // Specific knight that has the boost active

// public:
//     Player(PlayerColor c, int initialHearts = 3);

//     PlayerColor getColor() const { return color; }
//     int getHearts() const { return hearts; }
//     void decrementHeart();
//     bool isOutOfHearts() const { return hearts <= 0; }

//     int getRoundsWon() const { return roundsWon; }
//     void incrementRoundsWon() { roundsWon++; }

//     void addPowerUp(std::unique_ptr<PowerUp> powerUp);
//     const std::vector<std::unique_ptr<PowerUp>>& getAvailablePowerUps() const { return availablePowerUps; }
//     bool hasPowerUp(PowerUpType type) const;
    
//     // Activate a power-up by type, potentially targeting a piece
//     bool activatePowerUp(PowerUpType type, Piece* targetPiece = nullptr); 
//     void deactivateCurrentPowerUp();
//     PowerUpType getActivePowerUpType() const { return activePowerUpType; }

//     bool isKnightBoostActiveForPiece(const Piece* piece) const;


//     std::string getName() const { return (color == PlayerColor::WHITE) ? "White" : "Black"; }
// };

// } // namespace PowerChess

// #endif // POWERCHESS_CORE_PLAYER_H

// // File: src/Core/Player.cpp
// #include "PowerChess/Core/Player.h"
// #include "PowerChess/PowerUps/KnightBoostPowerUp.h" // For specific power-up logic if needed
// #include "PowerChess/Core/Piece.h" // For Piece type

// namespace PowerChess {

// Player::Player(PlayerColor c, int initialHearts)
//     : color(c), hearts(initialHearts), roundsWon(0), activePowerUpType(PowerUpType::NONE), knightBoostedPiece(nullptr) {}

// void Player::decrementHeart() {
//     if (hearts > 0) {
//         hearts--;
//     }
// }

// void Player::addPowerUp(std::unique_ptr<PowerUp> powerUp) {
//     availablePowerUps.push_back(std::move(powerUp));
// }

// bool Player::hasPowerUp(PowerUpType type) const {
//     for (const auto& pu : availablePowerUps) {
//         if (pu->getType() == type) {
//             return true;
//         }
//     }
//     return false;
// }

// bool Player::activatePowerUp(PowerUpType typeToActivate, Piece* targetPiece) {
//     // Find and remove the power-up from available list
//     for (auto it = availablePowerUps.begin(); it != availablePowerUps.end(); ++it) {
//         if ((*it)->getType() == typeToActivate) {
//             // Power-up found, activate it
//             activePowerUpType = typeToActivate;
//             if (typeToActivate == PowerUpType::KNIGHT_BOOST_QUEEN_MOVE) {
//                 if (targetPiece && targetPiece->getType() == PieceType::KNIGHT && targetPiece->getColor() == this->color) {
//                     knightBoostedPiece = targetPiece;
//                     // (*it)->apply(this, targetPiece); // PowerUp::apply might not be needed if state is on Player
//                     availablePowerUps.erase(it); // Consume the power-up
//                     return true;
//                 } else {
//                     // Invalid target for knight boost
//                     activePowerUpType = PowerUpType::NONE; // Revert
//                     return false; 
//                 }
//             }
//             // For other power-ups, targetPiece might not be needed at activation time
//             // (*it)->apply(this, targetPiece); 
//             availablePowerUps.erase(it); // Consume the power-up
//             return true;
//         }
//     }
//     return false; // Power-up not found or not usable
// }

// void Player::deactivateCurrentPowerUp() {
//     // This is called after the power-up's effect has been used (e.g., after boosted Knight moves)
//     if (activePowerUpType == PowerUpType::KNIGHT_BOOST_QUEEN_MOVE) {
//         knightBoostedPiece = nullptr;
//     }
//     activePowerUpType = PowerUpType::NONE;
//     // Some power-ups might have a 'remove' effect if they were long-lasting
// }

// bool Player::isKnightBoostActiveForPiece(const Piece* piece) const {
//     return activePowerUpType == PowerUpType::KNIGHT_BOOST_QUEEN_MOVE && knightBoostedPiece == piece;
// }


// } // namespace PowerChess

// // File: include/PowerChess/PowerUps/PowerUp.h
// #ifndef POWERCHESS_POWERUPS_POWERUP_H
// #define POWERCHESS_POWERUPS_POWERUP_H

// #include "PowerChess/Core/CommonTypes.h"
// #include <string>

// namespace PowerChess {

// class Player; // Forward declaration
// class Piece;  // Forward declaration
// class Board;  // Forward declaration

// class PowerUp {
// protected:
//     PowerUpType type;
//     std::string name;
//     std::string description;

// public:
//     PowerUp(PowerUpType t, std::string n, std::string d) : type(t), name(n), description(d) {}
//     virtual ~PowerUp() = default;

//     PowerUpType getType() const { return type; }
//     std::string getName() const { return name; }
//     std::string getDescription() const { return description; }

//     // Apply the power-up. May modify player, piece, or board.
//     // For some power-ups, activation is handled by Player, and Piece checks Player's state.
//     // This apply might be for immediate, one-off effects not tied to a piece's next move.
//     virtual void apply(Player* player, Piece* piece, Board& board) {}
//     // Remove or revert the power-up's effects, if necessary.
//     virtual void remove(Player* player, Piece* piece, Board& board) {}
// };

// } // namespace PowerChess

// #endif // POWERCHESS_POWERUPS_POWERUP_H

// // File: src/PowerUps/PowerUp.cpp
// #include "PowerChess/PowerUps/PowerUp.h"
// // No specific implementation needed in base class cpp usually

// namespace PowerChess {
// // Base class methods are virtual or simple getters, defined in header or inline.
// }


// // File: include/PowerChess/PowerUps/KnightBoostPowerUp.h
// #ifndef POWERCHESS_POWERUPS_KNIGHTBOOSTPOWERUP_H
// #define POWERCHESS_POWERUPS_KNIGHTBOOSTPOWERUP_H

// #include "PowerUp.h"

// namespace PowerChess {

// class KnightBoostPowerUp : public PowerUp {
// public:
//     KnightBoostPowerUp() : PowerUp(PowerUpType::KNIGHT_BOOST_QUEEN_MOVE, 
//                                    "Knight's Royal Dance", 
//                                    "Allows one Knight to move like a Queen for its next turn.") {}

//     // The effect is primarily handled by Player state and Knight's getValidMoves.
//     // Apply/Remove might be used for more complex setup/teardown if needed.
//     void apply(Player* player, Piece* piece, Board& board) override;
//     void remove(Player* player, Piece* piece, Board& board) override;
// };

// } // namespace PowerChess

// #endif // POWERCHESS_POWERUPS_KNIGHTBOOSTPOWERUP_H

// // File: src/PowerUps/KnightBoostPowerUp.cpp
// #include "PowerChess/PowerUps/KnightBoostPowerUp.h"
// #include "PowerChess/Core/Player.h"
// #include "PowerChess/Core/Piece.h"
// #include "PowerChess/Core/Board.h"

// namespace PowerChess {

// void KnightBoostPowerUp::apply(Player* player, Piece* piece, Board& board) {
//     // The actual "boost" is managed by Player::activePowerUpType and Player::knightBoostedPiece
//     // and checked within Knight::getValidMoves.
//     // This apply method could be used if the power-up had an immediate effect on the board
//     // or needed to register itself with some game system. For this design, it's minimal.
//     if (player && piece && piece->getType() == PieceType::KNIGHT && piece->getColor() == player->getColor()) {
//         // player->activatePowerUp(this->type, piece); // This logic is now in Player::activatePowerUp
//         // For this specific power-up, the Player class handles the state change.
//     }
// }

// void KnightBoostPowerUp::remove(Player* player, Piece* piece, Board& board) {
//     // Similarly, Player::deactivateCurrentPowerUp handles resetting the state.
//     // This could be used to clean up any board modifications if the power-up did that.
// }

// } // namespace PowerChess


// // File: include/PowerChess/Core/Game.h
// #ifndef POWERCHESS_CORE_GAME_H
// #define POWERCHESS_CORE_GAME_H

// #include "CommonTypes.h"
// #include "Board.h"
// #include "Player.h"
// #include <memory> // For std::unique_ptr
// #include <vector>

// namespace PowerChess {

// class Game {
// private:
//     Board board;
//     Player player1; // White
//     Player player2; // Black
//     Player* currentPlayer;
//     RoundState currentRoundState;
//     bool overallGameOver;
//     PlayerColor roundWinner; // Who won the current/last round

//     void switchTurn();
//     void updateRoundState(); // Checks for check, checkmate, stalemate
//     void endRound(PlayerColor winner); // winner can be NONE for stalemate
//     void offerPowerUpToLoser(Player* loser);

// public:
//     Game();

//     void startNewGame();
//     void startNewRound();
    
//     bool processMove(Position from, Position to, PieceType promotionType = PieceType::NONE);

//     const Board& getBoard() const { return board; }
//     Board& getBoardModifiable() { return board; } // For Graphics to interact
//     Player* getCurrentPlayer() const { return currentPlayer; }
//     PlayerColor getCurrentPlayerColor() const { return currentPlayer->getColor(); }
//     const Player& getPlayer1() const { return player1; }
//     const Player& getPlayer2() const { return player2; }
//     Player* getPlayer(PlayerColor color);


//     RoundState getCurrentRoundState() const { return currentRoundState; }
//     bool isOverallGameOver() const { return overallGameOver; }
//     PlayerColor getOverallWinner() const; // Determines who won 2 out of 3

//     std::string getStatusMessage() const; // For display

//     // For power-up interaction
//     bool attemptActivatePowerUp(Player* player, PowerUpType type, Piece* targetPiece);
//     void postMoveCleanUp(); // Deactivate one-turn power-ups etc.

//     // For Graphics to query valid moves
//     std::vector<Position> getValidMovesForPieceAt(Position pos) const;
//     bool needsPawnPromotion(Position at) const;
// };

// } // namespace PowerChess

// #endif // POWERCHESS_CORE_GAME_H

// // File: src/Core/Game.cpp
// #include "PowerChess/Core/Game.h"
// #include "PowerChess/PowerUps/KnightBoostPowerUp.h" // To create power-ups
// #include "PowerChess/Core/Piece.h" // For piece types
// #include <iostream> // For debug messages
// #include <algorithm> // For std::find_if

// namespace PowerChess {

// Game::Game()
//     : player1(PlayerColor::WHITE), player2(PlayerColor::BLACK),
//       currentPlayer(nullptr), currentRoundState(RoundState::PLAYING),
//       overallGameOver(false), roundWinner(PlayerColor::NONE) {
//     startNewGame();
// }

// void Game::startNewGame() {
//     player1 = Player(PlayerColor::WHITE);
//     player2 = Player(PlayerColor::BLACK);
//     overallGameOver = false;
//     startNewRound();
// }

// void Game::startNewRound() {
//     board.initializeBoard();
//     currentPlayer = &player1; // White always starts
//     currentRoundState = RoundState::PLAYING;
//     roundWinner = PlayerColor::NONE;
//     player1.deactivateCurrentPowerUp(); // Ensure no power-ups carry over incorrectly
//     player2.deactivateCurrentPowerUp();
//     std::cout << "New round started. " << currentPlayer->getName() << "'s turn." << std::endl;
//     updateRoundState(); // Initial check state
// }

// void Game::switchTurn() {
//     currentPlayer = (currentPlayer->getColor() == PlayerColor::WHITE) ? &player2 : &player1;
//     std::cout << currentPlayer->getName() << "'s turn." << std::endl;
//     updateRoundState();
// }

// void Game::updateRoundState() {
//     if (currentRoundState == RoundState::ROUND_OVER || currentRoundState == RoundState::POWERUP_SELECTION) {
//         // Don't update if round is already marked as over or in powerup phase
//         return;
//     }

//     bool inCheck = board.isKingInCheck(currentPlayer->getColor());
//     bool hasMoves = board.hasLegalMoves(currentPlayer->getColor(), currentPlayer);

//     if (inCheck) {
//         if (!hasMoves) {
//             currentRoundState = RoundState::CHECKMATE;
//             PlayerColor winner = (currentPlayer->getColor() == PlayerColor::WHITE) ? PlayerColor::BLACK : PlayerColor::WHITE;
//             endRound(winner);
//         } else {
//             currentRoundState = RoundState::CHECK;
//         }
//     } else {
//         if (!hasMoves) {
//             currentRoundState = RoundState::STALEMATE;
//             endRound(PlayerColor::NONE); // Stalemate is a draw
//         } else {
//             currentRoundState = RoundState::PLAYING;
//         }
//     }
// }

// void Game::endRound(PlayerColor winnerColor) {
//     currentRoundState = RoundState::ROUND_OVER;
//     roundWinner = winnerColor;

//     Player* roundLoser = nullptr;

//     if (winnerColor == PlayerColor::WHITE) {
//         player1.incrementRoundsWon();
//         player2.decrementHeart();
//         roundLoser = &player2;
//         std::cout << "Round over. White wins the round." << std::endl;
//     } else if (winnerColor == PlayerColor::BLACK) {
//         player2.incrementRoundsWon();
//         player1.decrementHeart();
//         roundLoser = &player1;
//         std::cout << "Round over. Black wins the round." << std::endl;
//     } else { // Stalemate
//         std::cout << "Round over. Stalemate." << std::endl;
//         // No hearts lost on stalemate, no rounds won.
//     }

//     // Check for overall game over
//     if (player1.getRoundsWon() >= 2 || player2.isOutOfHearts()) {
//         overallGameOver = true;
//         std::cout << "Overall game over! Winner: White" << std::endl;
//         return;
//     }
//     if (player2.getRoundsWon() >= 2 || player1.isOutOfHearts()) {
//         overallGameOver = true;
//         std::cout << "Overall game over! Winner: Black" << std::endl;
//         return;
//     }
    
//     // If game not over, and there was a loser who still has hearts, offer power-up
//     if (roundLoser && !roundLoser->isOutOfHearts()) {
//         offerPowerUpToLoser(roundLoser);
//         currentRoundState = RoundState::POWERUP_SELECTION; // Change state
//     } else if (!overallGameOver) {
//         // If no power-up phase (e.g. stalemate or loser out of hearts but game not over by rounds)
//         // proceed to next round directly (or wait for user input in UI)
//         // For console, we might auto-start or have a prompt.
//         // For SFML, UI will handle this transition.
//         std::cout << "Prepare for the next round..." << std::endl;
//     }
// }


// void Game::offerPowerUpToLoser(Player* loser) {
//     if (loser) {
//         std::cout << loser->getName() << " lost the round and gets a power-up!" << std::endl;
//         // For now, automatically give KnightBoostPowerUp
//         loser->addPowerUp(std::make_unique<KnightBoostPowerUp>());
//         std::cout << loser->getName() << " received: " << KnightBoostPowerUp().getName() << std::endl;
//         // In a UI, this would be a selection screen.
//     }
// }

// bool Game::processMove(Position from, Position to, PieceType promotionType) {
//     if (currentRoundState != RoundState::PLAYING && currentRoundState != RoundState::CHECK) {
//         std::cerr << "Cannot move when round state is not PLAYING or CHECK." << std::endl;
//         return false;
//     }

//     Piece* pieceToMove = board.getPieceAtModifiable(from);
//     if (!pieceToMove || pieceToMove->getColor() != currentPlayer->getColor()) {
//         std::cerr << "Invalid piece selection or not your turn." << std::endl;
//         return false;
//     }

//     std::vector<Position> validMoves = pieceToMove->getValidMoves(board, currentPlayer);
//     bool isValidMove = false;
//     for (const auto& movePos : validMoves) {
//         if (movePos == to) {
//             isValidMove = true;
//             break;
//         }
//     }

//     if (!isValidMove) {
//         std::cerr << "Invalid move for the selected piece." << std::endl;
//         return false;
//     }

//     // Simulate move to check if it puts own king in check
//     Board tempBoard = board;
//     Piece* tempPiece = tempBoard.getPieceAtModifiable(from);
//     if (tempPiece) { // Should always be true here
//         // Simulate the move on the temp board
//         tempBoard.movePieceInternal(from, to); // Use internal move for simulation
//         if (tempBoard.isKingInCheck(currentPlayer->getColor())) {
//             std::cerr << "Invalid move: would put your own king in check." << std::endl;
//             return false;
//         }
//     }


//     // Actual move
//     board.movePiece(from, to, promotionType);
//     std::cout << currentPlayer->getName() << " moved " << pieceToMove->getSymbol() 
//               << " from (" << from.row << "," << from.col << ") to (" 
//               << to.row << "," << to.col << ")." << std::endl;

//     postMoveCleanUp(); // Deactivate any one-turn power-ups
//     switchTurn(); // This will also call updateRoundState
//     return true;
// }


// Player* Game::getPlayer(PlayerColor color) {
//     if (player1.getColor() == color) return &player1;
//     if (player2.getColor() == color) return &player2;
//     return nullptr;
// }


// PlayerColor Game::getOverallWinner() const {
//     if (!overallGameOver) return PlayerColor::NONE;
//     if (player1.getRoundsWon() >= 2) return PlayerColor::WHITE;
//     if (player2.getRoundsWon() >= 2) return PlayerColor::BLACK;
//     // If out of hearts
//     if (player1.isOutOfHearts() && player2.getRoundsWon() > player1.getRoundsWon()) return PlayerColor::BLACK;
//     if (player2.isOutOfHearts() && player1.getRoundsWon() > player2.getRoundsWon()) return PlayerColor::WHITE;
//     // Could be a draw if both out of hearts simultaneously with equal round wins, though unlikely with current rules.
//     return PlayerColor::NONE; // Should ideally be covered by above
// }

// std::string Game::getStatusMessage() const {
//     std::string msg;
//     if (overallGameOver) {
//         PlayerColor winner = getOverallWinner();
//         if (winner == PlayerColor::WHITE) msg = "Game Over! White wins the match!";
//         else if (winner == PlayerColor::BLACK) msg = "Game Over! Black wins the match!";
//         else msg = "Game Over! It's a draw (unexpected).";
//         return msg;
//     }

//     msg = "P1 (W): " + std::to_string(player1.getHearts()) + " <3, " + std::to_string(player1.getRoundsWon()) + " wins | ";
//     msg += "P2 (B): " + std::to_string(player2.getHearts()) + " <3, " + std::to_string(player2.getRoundsWon()) + " wins. ";
//     msg += "Current Turn: " + currentPlayer->getName() + ". ";

//     switch (currentRoundState) {
//         case RoundState::PLAYING: msg += "Playing."; break;
//         case RoundState::CHECK: msg += "CHECK!"; break;
//         case RoundState::CHECKMATE: msg += "CHECKMATE! " + ((roundWinner == PlayerColor::WHITE) ? "White" : "Black") + " wins round."; break;
//         case RoundState::STALEMATE: msg += "STALEMATE! Round is a draw."; break;
//         case RoundState::ROUND_OVER: msg += "Round Over. "; break; // Winner already announced
//         case RoundState::POWERUP_SELECTION:
//             Player* loser = (roundWinner == player1.getColor()) ? &player2 : &player1;
//             if (roundWinner == PlayerColor::NONE) loser = nullptr; // Stalemate, no loser for powerup
//             if (loser && !loser->getAvailablePowerUps().empty()){
//                  msg += loser->getName() + " choosing power-up.";
//             } else {
//                  msg += "Preparing next round..."; // Fallback if no powerup or already chosen
//             }
//             break;
//     }
//     return msg;
// }

// bool Game::attemptActivatePowerUp(Player* player, PowerUpType type, Piece* targetPiece) {
//     if (player != currentPlayer) return false; // Can only activate on your turn
    
//     // Typically, power-ups are activated *before* a move.
//     // The KnightBoost needs a target Knight.
//     if (type == PowerUpType::KNIGHT_BOOST_QUEEN_MOVE) {
//         if (!targetPiece || targetPiece->getType() != PieceType::KNIGHT || targetPiece->getColor() != player->getColor()) {
//             std::cerr << "Knight Boost must target one of your Knights." << std::endl;
//             return false;
//         }
//     }

//     if (player->activatePowerUp(type, targetPiece)) {
//         std::cout << player->getName() << " activated power-up: " << typeid(*player->getAvailablePowerUps()[0]).name() << std::endl; // Simplified name
//         // If it's an immediate effect power-up, it might modify board here.
//         // KnightBoost is a state change for the next Knight move.
//         return true;
//     }
//     std::cerr << player->getName() << " could not activate power-up." << std::endl;
//     return false;
// }

// void Game::postMoveCleanUp() {
//     // Deactivate any power-ups that are for a single turn, like KnightBoost
//     if (currentPlayer->getActivePowerUpType() == PowerUpType::KNIGHT_BOOST_QUEEN_MOVE) {
//         currentPlayer->deactivateCurrentPowerUp();
//         std::cout << currentPlayer->getName() << "'s Knight Boost has been used." << std::endl;
//     }
// }

// std::vector<Position> Game::getValidMovesForPieceAt(Position pos) const {
//     const Piece* piece = board.getPieceAt(pos);
//     if (piece && piece->getColor() == currentPlayer->getColor()) {
//         // Filter out moves that would put own king in check
//         std::vector<Position> rawMoves = piece->getValidMoves(board, currentPlayer);
//         std::vector<Position> legalMoves;
//         for (const auto& move : rawMoves) {
//             Board tempBoard = board; // Create a copy of the board
//             // Simulate the move on the temporary board
//             // We need a way to get a non-const piece from const board for simulation, or make Piece::wouldBeInCheckAfterMove smarter
//             // For now, we assume Piece::getValidMoves already does a basic check or we do it here.
//             // The more robust check is done in processMove. This is for UI highlighting.
            
//             // Simplified: assume getValidMoves from Piece already filters self-checks.
//             // A truly robust getValidMoves would need to simulate each move and check for self-check.
//             // The Piece::wouldBeInCheckAfterMove is a step in that direction.
//             // For UI, we can show raw moves and let processMove be the final validator.
//             // Or, iterate and check:
//             Board tempBoardForCheck = board;
//             tempBoardForCheck.movePieceInternal(pos, move); // Simplified internal move for check
//             if (!tempBoardForCheck.isKingInCheck(currentPlayer->getColor())) {
//                 legalMoves.push_back(move);
//             }
//         }
//         return legalMoves;
//         //return piece->getValidMoves(board, currentPlayer); // This might include moves into check
//     }
//     return {};
// }

// bool Game::needsPawnPromotion(Position at) const {
//     const Piece* piece = board.getPieceAt(at);
//     if (piece && piece->getType() == PieceType::PAWN) {
//         if ((piece->getColor() == PlayerColor::WHITE && at.row == 0) ||
//             (piece->getColor() == PlayerColor::BLACK && at.row == 7)) {
//             return true;
//         }
//     }
//     return false;
// }


// } // namespace PowerChess

// // File: include/PowerChess/Graphics/Graphics.h
// #ifndef POWERCHESS_GRAPHICS_GRAPHICS_H
// #define POWERCHESS_GRAPHICS_GRAPHICS_H

// #include <SFML/Graphics.hpp>
// #include "PowerChess/Core/CommonTypes.h"
// #include <string>
// #include <vector>
// #include <optional>

// namespace PowerChess {

// class Game; // Forward declaration
// class Board;
// class Player;
// class Piece;

// class Graphics {
// private:
//     sf::RenderWindow window;
//     sf::Font font;
//     // Textures for pieces could be added here
//     // sf::Texture pieceTextures[12]; 

//     float squareSize;
//     sf::Vector2f boardOffset;

//     Position selectedPiecePos;
//     std::vector<Position> highlightedMoves;

//     // For pawn promotion UI
//     bool showPromotionDialog;
//     PlayerColor promotionPlayerColor;
//     Position promotionSquare;

//     // For power-up UI
//     bool showPowerUpDialog;
//     Player* playerForPowerUp; // Player who can activate a powerup
//     Position knightSelectionPos; // If KnightBoost needs a specific knight to be clicked
//     bool awaitingKnightForBoost;


//     void loadResources();
//     void drawBoard(const Board& board);
//     void drawPieces(const Board& board);
//     void drawHighlights();
//     void drawUI(const Game& game);
//     void drawPromotionDialog();
//     void drawPowerUpDialog(const Player& player);


//     Position screenToBoardPos(sf::Vector2i mousePos) const;
//     sf::Vector2f boardToScreenPos(Position boardPos) const;

// public:
//     Graphics();
//     ~Graphics() = default;

//     bool isOpen() const { return window.isOpen(); }
//     void close() { window.close(); }

//     void handleEvents(Game& game); // Game might be needed to process input directly
//     void render(const Game& game);

//     // Gets a move from the player (from, to) using mouse clicks
//     // Returns std::nullopt if no complete move is made in this event cycle
//     std::optional<Move> getPlayerMoveInput(const Game& game); 
//     std::optional<PieceType> getPromotionChoiceInput();
//     std::optional<PowerUpType> getPowerUpActivationChoice(Player* player, Piece*& targetPiece); // Modifies targetPiece for KnightBoost

//     void setSelectedPiece(Position pos, const std::vector<Position>& validMoves);
//     void clearSelection();

//     void startPromotion(PlayerColor forPlayer, Position atSquare);
//     void endPromotion() { showPromotionDialog = false; }

//     void startPowerUpSelection(Player* player);
//     void endPowerUpSelection() { showPowerUpDialog = false; awaitingKnightForBoost = false; knightSelectionPos = {-1,-1}; }
// };

// } // namespace PowerChess

// #endif // POWERCHESS_GRAPHICS_GRAPHICS_H

// // File: src/Graphics/Graphics.cpp
// #include "PowerChess/Graphics/Graphics.h"
// #include "PowerChess/Core/Game.h" // For Game state access
// #include "PowerChess/Core/Board.h"
// #include "PowerChess/Core/Piece.h"
// #include "PowerChess/Core/Player.h"
// #include "PowerChess/PowerUps/PowerUp.h" // For power-up names
// #include <iostream> // For error messages

// // Define paths for resources (adjust as needed)
// const std::string FONT_PATH = "assets/fonts/arial.ttf"; // User needs to provide this

// namespace PowerChess {

// Graphics::Graphics() : 
//     squareSize(80.0f), 
//     boardOffset(50.0f, 50.0f),
//     selectedPiecePos(-1, -1),
//     showPromotionDialog(false),
//     promotionPlayerColor(PlayerColor::NONE),
//     promotionSquare(-1,-1),
//     showPowerUpDialog(false),
//     playerForPowerUp(nullptr),
//     knightSelectionPos(-1,-1),
//     awaitingKnightForBoost(false)
// {
//     sf::ContextSettings settings;
//     settings.antialiasingLevel = 8;
//     window.create(sf::VideoMode(800, 800), "PowerChess", sf::Style::Default, settings);
//     window.setFramerateLimit(60);
//     loadResources();
// }

// void Graphics::loadResources() {
//     if (!font.loadFromFile(FONT_PATH)) {
//         std::cerr << "Error loading font: " << FONT_PATH << std::endl;
//         // Handle error, maybe use a fallback or exit
//         // For now, game will run without text if font fails.
//     }
//     // TODO: Load piece textures if using sprites
// }

// Position Graphics::screenToBoardPos(sf::Vector2i mousePos) const {
//     int col = static_cast<int>((mousePos.x - boardOffset.x) / squareSize);
//     int row = static_cast<int>((mousePos.y - boardOffset.y) / squareSize);
//     if (row >= 0 && row < 8 && col >= 0 && col < 8) {
//         return {row, col};
//     }
//     return {-1, -1}; // Invalid position
// }

// sf::Vector2f Graphics::boardToScreenPos(Position boardPos) const {
//     return {
//         boardOffset.x + boardPos.col * squareSize,
//         boardOffset.y + boardPos.row * squareSize
//     };
// }

// void Graphics::drawBoard(const Board& board_data) { // Renamed param to avoid conflict
//     for (int r = 0; r < 8; ++r) {
//         for (int c = 0; c < 8; ++c) {
//             sf::RectangleShape square(sf::Vector2f(squareSize, squareSize));
//             square.setPosition(boardOffset.x + c * squareSize, boardOffset.y + r * squareSize);
//             if ((r + c) % 2 == 0) {
//                 square.setFillColor(sf::Color(238, 238, 210)); // Light square
//             } else {
//                 square.setFillColor(sf::Color(118, 150, 86));  // Dark square
//             }
//             window.draw(square);
//         }
//     }
// }

// void Graphics::drawPieces(const Board& board_data) {
//     sf::Text pieceText;
//     pieceText.setFont(font);
//     pieceText.setCharacterSize(static_cast<unsigned int>(squareSize * 0.7f)); // Adjust size as needed

//     for (int r = 0; r < 8; ++r) {
//         for (int c = 0; c < 8; ++c) {
//             const Piece* piece = board_data.getPieceAt({r, c});
//             if (piece) {
//                 pieceText.setString(std::string(1, piece->getSymbol()));
//                 // Center text in square
//                 sf::FloatRect textRect = pieceText.getLocalBounds();
//                 pieceText.setOrigin(textRect.left + textRect.width / 2.0f,
//                                    textRect.top + textRect.height / 2.0f);
//                 pieceText.setPosition(boardOffset.x + c * squareSize + squareSize / 2.0f,
//                                      boardOffset.y + r * squareSize + squareSize / 2.0f);
                
//                 pieceText.setFillColor(piece->getColor() == PlayerColor::WHITE ? sf::Color::White : sf::Color(50,50,50));
//                 window.draw(pieceText);
//             }
//         }
//     }
// }

// void Graphics::drawHighlights() {
//     // Highlight selected piece
//     if (selectedPiecePos.isValid()) {
//         sf::RectangleShape highlight(sf::Vector2f(squareSize, squareSize));
//         highlight.setPosition(boardToScreenPos(selectedPiecePos));
//         highlight.setFillColor(sf::Color(100, 100, 255, 100)); // Semi-transparent blue
//         window.draw(highlight);
//     }

//     // Highlight valid moves
//     for (const auto& pos : highlightedMoves) {
//         sf::CircleShape moveMarker(squareSize / 6.f);
//         moveMarker.setOrigin(moveMarker.getRadius(), moveMarker.getRadius());
//         sf::Vector2f screenPos = boardToScreenPos(pos);
//         moveMarker.setPosition(screenPos.x + squareSize / 2.f, screenPos.y + squareSize / 2.f);
//         moveMarker.setFillColor(sf::Color(0, 0, 0, 80)); // Dark, semi-transparent
//         window.draw(moveMarker);
//     }
// }

// void Graphics::drawUI(const Game& game) {
//     sf::Text uiText;
//     uiText.setFont(font);
//     uiText.setCharacterSize(20);
//     uiText.setFillColor(sf::Color::Black);

//     // Game Status (Hearts, Rounds Won, Current Turn)
//     std::string status = game.getStatusMessage();
//     uiText.setString(status);
//     uiText.setPosition(boardOffset.x, 10.0f);
//     window.draw(uiText);

//     // Power-up button (example)
//     if (game.getCurrentPlayer() && !game.getCurrentPlayer()->getAvailablePowerUps().empty() &&
//         (game.getCurrentRoundState() == RoundState::PLAYING || game.getCurrentRoundState() == RoundState::CHECK)) {
//         sf::RectangleShape powerUpButton(sf::Vector2f(180, 40));
//         powerUpButton.setFillColor(sf::Color::Magenta);
//         powerUpButton.setPosition(boardOffset.x + 8 * squareSize + 20, boardOffset.y + 100);
//         window.draw(powerUpButton);

//         uiText.setString("Use Power-Up");
//         uiText.setCharacterSize(18);
//         uiText.setFillColor(sf::Color::White);
//         uiText.setPosition(boardOffset.x + 8 * squareSize + 30, boardOffset.y + 108);
//         window.draw(uiText);
//     }
//      if (game.getCurrentRoundState() == RoundState::POWERUP_SELECTION || 
//         (game.getCurrentRoundState() == RoundState::ROUND_OVER && !game.isOverallGameOver())) {
//         sf::RectangleShape nextRoundButton(sf::Vector2f(180, 40));
//         nextRoundButton.setFillColor(sf::Color::Green);
//         nextRoundButton.setPosition(boardOffset.x + 8 * squareSize + 20, boardOffset.y + 150);
//         window.draw(nextRoundButton);

//         uiText.setString("Next Round / Cont.");
//         uiText.setCharacterSize(18);
//         uiText.setFillColor(sf::Color::White);
//         uiText.setPosition(boardOffset.x + 8 * squareSize + 25, boardOffset.y + 158);
//         window.draw(uiText);
//     }

// }

// void Graphics::drawPromotionDialog() {
//     if (!showPromotionDialog) return;

//     sf::Text dialogText;
//     dialogText.setFont(font);
//     dialogText.setCharacterSize(24);
//     dialogText.setFillColor(sf::Color::Black);

//     sf::RectangleShape background(sf::Vector2f(200, 250));
//     background.setFillColor(sf::Color(200, 200, 200, 220));
//     background.setOutlineThickness(2);
//     background.setOutlineColor(sf::Color::Black);
//     background.setPosition(window.getSize().x / 2.f - 100, window.getSize().y / 2.f - 125);
//     window.draw(background);

//     dialogText.setString("Promote Pawn to:");
//     dialogText.setPosition(window.getSize().x / 2.f - 90, window.getSize().y / 2.f - 115);
//     window.draw(dialogText);

//     std::string options[] = {"Queen (Q)", "Rook (R)", "Bishop (B)", "Knight (N)"};
//     for (int i = 0; i < 4; ++i) {
//         sf::RectangleShape button(sf::Vector2f(160, 40));
//         button.setFillColor(sf::Color::LightGray);
//         button.setOutlineColor(sf::Color::Black);
//         button.setOutlineThickness(1);
//         button.setPosition(window.getSize().x / 2.f - 80, window.getSize().y / 2.f - 70 + i * 50);
//         window.draw(button);

//         dialogText.setString(options[i]);
//         dialogText.setCharacterSize(20);
//         dialogText.setPosition(window.getSize().x / 2.f - 70, window.getSize().y / 2.f - 65 + i * 50);
//         window.draw(dialogText);
//     }
// }

// void Graphics::drawPowerUpDialog(const Player& player) {
//     if (!showPowerUpDialog || !playerForPowerUp) return;

//     sf::Text dialogText;
//     dialogText.setFont(font);
//     dialogText.setCharacterSize(20); // Smaller for list
//     dialogText.setFillColor(sf::Color::Black);

//     sf::RectangleShape background(sf::Vector2f(300, 200 + player.getAvailablePowerUps().size() * 40.f));
//     background.setFillColor(sf::Color(200, 200, 200, 220));
//     background.setOutlineThickness(2);
//     background.setOutlineColor(sf::Color::Black);
//     float dialogX = boardOffset.x + 8 * squareSize + 10; // Position it to the right of the board
//     float dialogY = boardOffset.y + 50;
//     background.setPosition(dialogX, dialogY);
//     window.draw(background);

//     dialogText.setString(player.getName() + ", choose Power-Up:");
//     dialogText.setCharacterSize(24);
//     dialogText.setPosition(dialogX + 10, dialogY + 10);
//     window.draw(dialogText);

//     const auto& powerUps = player.getAvailablePowerUps();
//     for (size_t i = 0; i < powerUps.size(); ++i) {
//         sf::RectangleShape button(sf::Vector2f(260, 35));
//         button.setFillColor(sf::Color::Cyan);
//         button.setOutlineColor(sf::Color::Black);
//         button.setOutlineThickness(1);
//         button.setPosition(dialogX + 20, dialogY + 60 + i * 45);
//         window.draw(button);

//         dialogText.setString(powerUps[i]->getName());
//         dialogText.setCharacterSize(18);
//         dialogText.setPosition(dialogX + 25, dialogY + 65 + i * 45);
//         window.draw(dialogText);
//     }
//     if (awaitingKnightForBoost) {
//         dialogText.setString("Click on a Knight to boost.");
//         dialogText.setCharacterSize(18);
//         dialogText.setPosition(dialogX + 10, dialogY + 60 + powerUps.size() * 45 + 10);
//         window.draw(dialogText);
//     }
// }


// void Graphics::handleEvents(Game& game) {
//     sf::Event event;
//     while (window.pollEvent(event)) {
//         if (event.type == sf::Event::Closed) {
//             window.close();
//         }
//         if (event.type == sf::Event::MouseButtonPressed) {
//             if (event.mouseButton.button == sf::Mouse::Left) {
//                 sf::Vector2i mousePos = sf::Mouse::getPosition(window);

//                 if (showPromotionDialog) {
//                     // Check clicks on promotion options
//                     // Simplified: Q, R, B, N buttons
//                     PieceType chosenType = PieceType::NONE;
//                     float dialogBaseX = window.getSize().x / 2.f - 80;
//                     float dialogBaseY = window.getSize().y / 2.f - 70;
//                     if (mousePos.x >= dialogBaseX && mousePos.x <= dialogBaseX + 160) {
//                         if (mousePos.y >= dialogBaseY && mousePos.y <= dialogBaseY + 40) chosenType = PieceType::QUEEN;
//                         else if (mousePos.y >= dialogBaseY + 50 && mousePos.y <= dialogBaseY + 90) chosenType = PieceType::ROOK;
//                         else if (mousePos.y >= dialogBaseY + 100 && mousePos.y <= dialogBaseY + 140) chosenType = PieceType::BISHOP;
//                         else if (mousePos.y >= dialogBaseY + 150 && mousePos.y <= dialogBaseY + 190) chosenType = PieceType::KNIGHT;
//                     }
//                     if (chosenType != PieceType::NONE) {
//                         game.processMove(selectedPiecePos, promotionSquare, chosenType); // This is a bit off, needs rework.
//                                                                                          // processMove should be called once with promotion type.
//                                                                                          // The original move that led to promotion needs to be stored.
//                         // For now, assume the move was already made and we just need to set the piece type.
//                         // This part needs careful integration with Game's move processing.
//                         // A better way: Game::processMove returns a "PROMOTION_NEEDED" state.
//                         // Then Graphics collects choice, then calls Game::completePromotion(type).
//                         // Simplified: if game.needsPawnPromotion(lastMove.to), then show dialog.
//                         // For this version, let's assume Game handles this. We just provide the choice.
//                         // This interaction is tricky. The `getPlayerMoveInput` should handle this.
//                         endPromotion();
//                         clearSelection(); // After promotion, clear selection.
//                     }
//                     return; // Consume click
//                 }

//                 if (showPowerUpDialog && playerForPowerUp) {
//                     float dialogX = boardOffset.x + 8 * squareSize + 10;
//                     float dialogY = boardOffset.y + 50;
//                     const auto& powerUps = playerForPowerUp->getAvailablePowerUps();
//                     for (size_t i = 0; i < powerUps.size(); ++i) {
//                         if (mousePos.x >= dialogX + 20 && mousePos.x <= dialogX + 20 + 260 &&
//                             mousePos.y >= dialogY + 60 + i * 45 && mousePos.y <= dialogY + 60 + i * 45 + 35) {
                            
//                             PowerUpType typeToActivate = powerUps[i]->getType();
//                             if (typeToActivate == PowerUpType::KNIGHT_BOOST_QUEEN_MOVE) {
//                                 awaitingKnightForBoost = true;
//                                 std::cout << "Knight Boost selected. Click on one of your knights." << std::endl;
//                                 // Don't activate yet, wait for knight click.
//                             } else {
//                                 Piece* target = nullptr; // For non-knight-boost powerups
//                                 game.attemptActivatePowerUp(playerForPowerUp, typeToActivate, target);
//                                 endPowerUpSelection();
//                             }
//                             return; // Consume click
//                         }
//                     }
//                 }
                
//                 if (awaitingKnightForBoost) {
//                     Position clickedBoardPos = screenToBoardPos(mousePos);
//                     if (clickedBoardPos.isValid()) {
//                         Piece* piece = game.getBoardModifiable().getPieceAtModifiable(clickedBoardPos);
//                         if (piece && piece->getType() == PieceType::KNIGHT && piece->getColor() == playerForPowerUp->getColor()) {
//                             game.attemptActivatePowerUp(playerForPowerUp, PowerUpType::KNIGHT_BOOST_QUEEN_MOVE, piece);
//                             endPowerUpSelection(); // Also sets awaitingKnightForBoost to false
//                         } else {
//                             std::cout << "Invalid target for Knight Boost. Click a Knight." << std::endl;
//                         }
//                     }
//                     return; // Consume click
//                 }


//                 // Power-up button click
//                 sf::FloatRect powerUpButtonRect(boardOffset.x + 8 * squareSize + 20, boardOffset.y + 100, 180, 40);
//                 if (powerUpButtonRect.contains(static_cast<sf::Vector2f>(mousePos))) {
//                     if (game.getCurrentPlayer() && !game.getCurrentPlayer()->getAvailablePowerUps().empty() &&
//                         (game.getCurrentRoundState() == RoundState::PLAYING || game.getCurrentRoundState() == RoundState::CHECK)) {
//                         startPowerUpSelection(game.getCurrentPlayer());
//                         return; // Consume click
//                     }
//                 }
                
//                 // Next Round button click
//                 sf::FloatRect nextRoundButtonRect(boardOffset.x + 8 * squareSize + 20, boardOffset.y + 150, 180, 40);
//                  if (nextRoundButtonRect.contains(static_cast<sf::Vector2f>(mousePos))) {
//                     if (game.getCurrentRoundState() == RoundState::POWERUP_SELECTION || 
//                         (game.getCurrentRoundState() == RoundState::ROUND_OVER && !game.isOverallGameOver())) {
//                         if (showPowerUpDialog) endPowerUpSelection(); // Close dialog if open
//                         game.startNewRound(); // Or a method like game.proceedToNextStage();
//                         return; // Consume click
//                     }
//                 }


//                 // Board interaction
//                 Position clickedBoardPos = screenToBoardPos(mousePos);
//                 if (clickedBoardPos.isValid()) {
//                     if (!selectedPiecePos.isValid()) { // No piece selected yet
//                         const Piece* piece = game.getBoard().getPieceAt(clickedBoardPos);
//                         if (piece && piece->getColor() == game.getCurrentPlayerColor()) {
//                             setSelectedPiece(clickedBoardPos, game.getValidMovesForPieceAt(clickedBoardPos));
//                         }
//                     } else { // A piece is already selected, try to move it
//                         bool moved = false;
//                         for (const auto& validMovePos : highlightedMoves) {
//                             if (validMovePos == clickedBoardPos) {
//                                 // Check for pawn promotion
//                                 const Piece* piece = game.getBoard().getPieceAt(selectedPiecePos);
//                                 bool needsPromo = false;
//                                 if (piece && piece->getType() == PieceType::PAWN) {
//                                     if ((piece->getColor() == PlayerColor::WHITE && clickedBoardPos.row == 0) ||
//                                         (piece->getColor() == PlayerColor::BLACK && clickedBoardPos.row == 7)) {
//                                         needsPromo = true;
//                                     }
//                                 }

//                                 if (needsPromo) {
//                                     startPromotion(game.getCurrentPlayerColor(), clickedBoardPos);
//                                     // The actual move will be processed after promotion choice.
//                                     // Store the 'to' square for promotion.
//                                     promotionSquare = clickedBoardPos; 
//                                 } else {
//                                     game.processMove(selectedPiecePos, clickedBoardPos);
//                                 }
//                                 moved = true;
//                                 break;
//                             }
//                         }
//                         clearSelection(); // Clear selection whether move was valid or not, or if clicked elsewhere
//                         if (!moved && clickedBoardPos.isValid()) { // If clicked on another of own pieces, select it
//                              const Piece* piece = game.getBoard().getPieceAt(clickedBoardPos);
//                              if (piece && piece->getColor() == game.getCurrentPlayerColor()) {
//                                 setSelectedPiece(clickedBoardPos, game.getValidMovesForPieceAt(clickedBoardPos));
//                             }
//                         }
//                     }
//                 } else { // Clicked outside board
//                     clearSelection();
//                     if (showPowerUpDialog) endPowerUpSelection(); // Close power-up dialog if clicked outside
//                 }
//             }
//         }
//     }
// }


// // This function is tricky because input might span multiple frames/event cycles.
// // For simplicity, this version won't be used directly in the main loop's logic.
// // Instead, handleEvents will directly call game.processMove or update selection state.
// std::optional<Move> Graphics::getPlayerMoveInput(const Game& game) {
//     // This function would typically be called in the game loop,
//     // and it would check internal state set by handleEvents.
//     // For this example, the logic is embedded within handleEvents.
//     return std::nullopt;
// }

// std::optional<PieceType> Graphics::getPromotionChoiceInput() {
//     // This would check state set by handleEvents if promotion dialog is active.
//     // For now, handleEvents processes this directly.
//     return std::nullopt;
// }

// std::optional<PowerUpType> Graphics::getPowerUpActivationChoice(Player* player, Piece*& targetPiece) {
//     // This would check state set by handleEvents if power-up dialog is active.
//     // For now, handleEvents processes this directly.
//     return std::nullopt;
// }


// void Graphics::render(const Game& game) {
//     window.clear(sf::Color(180, 180, 180)); // Light gray background
//     drawBoard(game.getBoard());
//     drawPieces(game.getBoard());
//     drawHighlights();
//     drawUI(game);
//     if (showPromotionDialog) {
//         drawPromotionDialog();
//     }
//     if (showPowerUpDialog && playerForPowerUp) {
//         drawPowerUpDialog(*playerForPowerUp);
//     }
//     window.display();
// }

// void Graphics::setSelectedPiece(Position pos, const std::vector<Position>& validMoves) {
//     selectedPiecePos = pos;
//     highlightedMoves = validMoves;
// }

// void Graphics::clearSelection() {
//     selectedPiecePos = {-1, -1};
//     highlightedMoves.clear();
// }

// void Graphics::startPromotion(PlayerColor forPlayer, Position atSquare) {
//     showPromotionDialog = true;
//     promotionPlayerColor = forPlayer;
//     promotionSquare = atSquare; // This is the 'to' square of the promoting move
// }

// void Graphics::startPowerUpSelection(Player* player) {
//     if (player && !player->getAvailablePowerUps().empty()) {
//         playerForPowerUp = player;
//         showPowerUpDialog = true;
//         awaitingKnightForBoost = false; // Reset this
//         knightSelectionPos = {-1,-1};
//     }
// }


// } // namespace PowerChess

// // File: src/main.cpp
// #include "PowerChess/Core/Game.h"
// #include "PowerChess/Graphics/Graphics.h"
// #include <iostream>

// // Make sure SFML headers are included if needed directly, but Graphics.h should suffice
// // #include <SFML/System.hpp> // For sf::Clock, sf::Time if precise timing is needed

// int main() {
//     std::cout << "Starting PowerChess..." << std::endl;

//     PowerChess::Game game;
//     PowerChess::Graphics graphics;

//     // This is a simplified game loop. More complex UI interactions (like pawn promotion choice)
//     // are handled within Graphics::handleEvents and might need better state management.
//     while (graphics.isOpen() && !game.isOverallGameOver()) {
//         graphics.handleEvents(game); // Handles input and updates game state via calls to 'game'

//         // If pawn promotion is needed, Graphics::handleEvents should manage the dialog
//         // and then call game.processMove with the chosen promotion piece.
//         // This requires careful state management in Graphics or Game.
//         // For now, the promotion choice in handleEvents directly calls game.processMove, which is not ideal.
//         // A better flow:
//         // 1. Player makes a move that results in promotion.
//         // 2. game.processMove identifies this, sets a state like 'AWAITING_PROMOTION_CHOICE'.
//         // 3. Graphics displays dialog.
//         // 4. User clicks choice.
//         // 5. Graphics calls game.completePawnPromotion(chosenType).

//         // Similar flow for power-up selection. Game state should reflect this.
//         // game.update() could be added here if there are time-based effects (not in this version)

//         graphics.render(game);
//     }

//     // Game over loop (display final message, wait for close)
//     while (graphics.isOpen() && game.isOverallGameOver()) {
//          sf::Event event;
//          while (graphics.isOpen() && graphics.window.pollEvent(event)) { // graphics.window is not public, need a getter or pass window
//             if (event.type == sf::Event::Closed) {
//                 graphics.close();
//             }
//         }
//         graphics.render(game); // Show final game over screen
//     }
    
//     std::cout << "PowerChess finished." << std::endl;
//     if (game.isOverallGameOver()) {
//         PowerChess::PlayerColor winner = game.getOverallWinner();
//         if (winner == PowerChess::PlayerColor::WHITE) std::cout << "Overall Winner: White!" << std::endl;
//         else if (winner == PowerChess::PlayerColor::BLACK) std::cout << "Overall Winner: Black!" << std::endl;
//         else std::cout << "Overall Game: Draw or Undetermined." << std::endl;
//     }


//     return 0;
// }

