#include "PowerChess/Core/Player.h"


namespace PowerChess {

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
        // activePowerUp = nullptr;
    }

    // void Player::setActivePowerUp(std::unique_ptr<PowerUp> pu) {
    //     activePowerUp = std::move(pu);
    // }

    // bool Player::hasActivePowerUp() const {
    //     return activePowerUp != nullptr;
    // }

    // std::unique_ptr<PowerUp> Player::consumeActivePowerUp() {
    //     return std::move(activePowerUp); // activePowerUp will become nullptr after this
    // }

} // namespace PowerChess