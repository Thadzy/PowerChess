#ifndef HARDCHESS_CORE_GAME_H
#define HARDCHESS_CORE_GAME_H

#include "Header/Core/Board.h"
#include "Header/Core/Player.h"
#include "Header/UI/ConsoleUI.h"

// enum is a special type that allows you to define a set of named constants 

namespace HardChess {
  enum class RoundState { ONGOING, CHECKMATE_WHITE_WINS, CHECKMATE_BLACK_WINS, STALEMATE};

  // Game class represents a chess game
  class Game {
    private:
      Board board;
      Player* player1; // White player
      Player* player2; // Black player
      Player* currentPlayer; // Pointer to the player whose turn it is
      ConsoleUI& ui; // Reference to the ConsoleUI for displaying the game state
      RoundState roundState; // Current state of the round

      Position parsePosition(const std::string& s) const; // Parse a string to get a Position object
      bool makeMove(const Position& start, const Position& end, PieceType promotionType = PieceType::NONE); // Make a move on the board
      bool canPlayerMakeAnyLegalMove(Player* player); // Check if the player can make any legal move
      void switchPlayer(); // Switch the turn to the next player
      void checkForEndOfRound(); // Check if the round has ended (checkmate, stalemate, etc.)

    public:
    Game(Player* player1, Player* player2, ConsoleUI& ui);
    
    void startRound();         // Start a new round
    void play();               // Main game loop
    bool isRoundOver() const;  // Fix return type to bool
    void playTurn();           // Add declaration for playTurn()
    Player* getRoundWinner() const;
    RoundState getRoundState() const { return roundState; }
  };
}

#endif // PowerChess_CORE_GAME_H