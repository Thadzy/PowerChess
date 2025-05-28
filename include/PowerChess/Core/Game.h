#ifndef POWERCHESS_CORE_GAME_H
#define POWERCHESS_CORE_GAME_H

#include "PowerChess/Core/Board.h"
#include "PowerChess/Core/Player.h"
#include "PowerChess/UI/ConsoleUI.h" // Game needs UI to interact

namespace PowerChess {

    enum class RoundState { ONGOING, CHECKMATE_WHITE_WINS, CHECKMATE_BLACK_WINS, STALEMATE };

    class Game {
      private:
        Board board;
        Player* player1; // White
        Player* player2; // Black
        Player* currentPlayer;
        ConsoleUI& ui;
        RoundState roundState;

        Position parsePosition(const std::string& s) const;
        bool makeMove(Position start, Position end, PieceType promotionType = PieceType::NONE);
        bool canPlayerMakeAnyLegalMove(Player* player);
        void switchPlayer();
        void checkForEndOfRound();


      public:
        Game(Player* p1, Player* p2, ConsoleUI& consoleUi);
        
        void startRound();
        void playTurn();
        bool isRoundOver() const;
        Player* getRoundWinner() const; // nullptr if draw or ongoing
        RoundState getRoundState() const { return roundState; }
    };

} // namespace PowerChess

#endif // POWERCHESS_CORE_GAME_H