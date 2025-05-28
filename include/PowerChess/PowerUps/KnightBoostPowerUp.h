#ifndef POWERCHESS_POWERUPS_KNIGHTBOOSTPOWERUP_H
#define POWERCHESS_POWERUPS_KNIGHTBOOSTPOWERUP_H

#include "PowerChess/PowerUps/PowerUp.h"

namespace PowerChess {

    class KnightBoostPowerUp : public PowerUp {
      public:
        KnightBoostPowerUp();
        void apply(Player& player, Board& board, ConsoleUI& ui) override;
        std::unique_ptr<PowerUp> clone() const override;
    };

} // namespace PowerChess

#endif // POWERCHESS_POWERUPS_KNIGHTBOOSTPOWERUP_H