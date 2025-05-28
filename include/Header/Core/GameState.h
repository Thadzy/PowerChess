#ifndef HARDCHESS_CORE_GAME_STATE_H
#define HARDCHESS_CORE_GAME_STATE_H

#include <string>
#include <memory>
#include <vector> 

enum class GamePhase {
    MENU, PLAYING, GAME_OVER, PAUSED
};

class GameState {
    private:
        GamePhase currentPhase; // Current phase of the game
        int roundNumber; // Current round number
        std::string lastMoveDEscription; // Description of the last move made
        bool isCheck;
        bool isCheckmate;
        std::vector<std::string> gameHistory; // History of moves made in the game

    public:
        GameState();

        // Phase management
        GamePhase getCurrentPhase() const { return currentPhase; };
        void setPhase(GamePhase phase) { currentPhase = phase; };

        // Round management
        int getRoundNumber() const { return roundNumber; };
        void incrementRound() { roundNumber++; };
        void resetRound() { roundNumber = 0; };

        // Game status
        void setCheck(bool check) { isCheck = check; };
        void setCheckmate(bool checkmate) { isCheckmate = checkmate; };
        bool getCheck() const { return isCheck; };
        bool getCheckmate() const { return isCheckmate; };

        // Move history
        void addMove(const std::string& move);
        std::vector<std::string> getHistory() const { return gameHistory; };
        void clearHistory() { gameHistory.clear(); };

        // Utility 
        std::string getPhaseString() const;
};

#endif // HARDCHESS_GAME_STATE_H