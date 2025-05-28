#ifndef HARDCHESS_CORE_PLAYER_H
#define HARDCHESS_CORE_PLAYER_H

#include "Header/Core/CommonTypes.h"
#include <string>
#include <memory>

namespace HardChess
{
    class Player
    {
    private:
        std::string name;
        Color color;
        int hearts;
        int score; // Rounds won in a match

    public:
        Player(const std::string &name, Color c, int initialHearts = 3);

        const std::string &getName() const { return name; }
        Color getColor() const { return color; }
        int getHearts() const { return hearts; }
        int getScore() const { return score; }

        std::string getColorString() const
        {
            return (color == Color::WHITE) ? "White" : "Black";
        }

        void loseHeart();
        void incrementScore();
        void resetForNewMatch(int initialHearts = 3); // Resets score and hearts for a new match
        // void resetForNewRound(); // If anything specific needed between rounds of same match

        void setHearts(int h) { hearts = h; } // For potential game loading later
        void setScore(int s) { score = s; }   // For potential game loading later
    };
} // namespace HardChess

#endif // HARDCHESS_CORE_PLAYER_H