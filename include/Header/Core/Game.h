#ifndef HARDCHESS_CORE_GAME_H
#define HARDCHESS_CORE_GAME_H

#include "Header/Core/Board.h"
#include "Header/Core/Player.h"
#include "Header/Core/ConsoleUI.h"

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

      Postion parsePostion(const std::string& s) const; // Parse a string to get a Position object
      bool makeMove(cosnt Postion& start, const Position& end, PieceType promotionType = PieceType::NONE); // Make a move on the board
      bool canPlayerMakeAnyLegalMove(Player* player); // Check if the player can make any legal move
      void switchPlayer(); // Switch the turn to the next player
      void checkForEndOfRound(); // Check if the round has ended (checkmate, stalemate, etc.)

    public:
      Game(Player* player1, Player* player2, ConsoleUI& ui); // Constructor to initialize the game with two players and a UI
      
      void startRound(); // Start a new round of the game
      void play(); // Main game loop to play the game
      void isRodundOver() const; // Check if the round is over and display the result
      Player* getRoundWinner() const; // Get the player who won the round
      RoundState getRoundState() const { return roundState; } // Get the current state of the round
  };
}

#endif // PowerChess_CORE_GAME_H