#ifndef HARDCHESS_CORE_GAME_H
#define HARDCHESS_CORE_GAME_H

#include "Header/Core/Board.h"
#include "Header/Core/Player.h"
#include "Header/UI/ConsoleUI.h" // Assuming ConsoleUI is used directly by Game

namespace HardChess
{
    enum class RoundState { ONGOING, CHECKMATE_WHITE_WINS, CHECKMATE_BLACK_WINS, STALEMATE };

    class Game
    {
    private:
        Board board;
        Player *player1; // White
        Player *player2; // Black
        Player *currentPlayer;
        ConsoleUI &ui;
        RoundState roundState;

        Position parsePosition(const std::string &s) const;
        bool makeMove(const Position &start, const Position &end, PieceType promotionType = PieceType::NONE);
        bool canPlayerMakeAnyLegalMove(Player *player);
        void switchPlayer();
        void checkForEndOfRound();
        
        // Castling specific checks
        bool isCastlingAttempt(const Piece* piece, Position start, Position end) const;
        bool canCastle(const Piece* king, Position kingStart, Position kingEnd, Position& rookStart, Position& rookEnd);


    public:
        Game(Player *p1, Player *p2, ConsoleUI &consoleUi);
        
        void startRound(); // Initializes board for a new round
        RoundState playRound(); // Plays a single round and returns the result
        
        // These were in your original Game.h, kept for reference or specific needs
        // bool isRoundOver() const;
        // void playTurn(); // playRound now handles this
        // Player* getRoundWinner() const; // RoundState from playRound is more direct
        // RoundState getRoundState() const { return roundState; }
    };
} // namespace HardChess

#endif // HARDCHESS_CORE_GAME_H