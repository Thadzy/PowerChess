#include "Header/Core/Player.h"

namespace HardChess
{
    Player::Player(const std::string &playerName, Color playerColor, int initialHearts)
        : name(playerName), color(playerColor), hearts(initialHearts), score(0) {}

    void Player::loseHeart()
    {
        if (hearts > 0)
        {
            hearts--;
        }
    }

    void Player::incrementScore()
    {
        score++;
    }

    void Player::resetForNewMatch(int initialHearts)
    {
        hearts = initialHearts;
        score = 0;
    }
} // namespace HardChess