#include "Header/Core/Player.h"

namespace HardChess {

    Player::Player(const std::string& playerName, Color playerColor)
        : name(playerName), color(playerColor), hearts(3), score(0) {}

    void Player::loseHeart() {
        if (hearts > 0) {
            hearts--;
        }
    }

    void Player::incrementScore() {
        score++;
    }

    void Player::resetForNewMatch() {
        hearts = 3;
        score = 0;
    }

} // namespace HardChess
