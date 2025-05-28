#ifndef POWERCHESS_CORE_PLAYER_H
#define POWERCHESS_CORE_PLAYER_H

#include "PowerChess/Core/CommonTypes.h"
#include <string>
#include <memory> // For std::unique_ptr

namespace PowerChess
{

  class Player
  {
  private:
    std::string name;
    Color color;
    int hearts;
    int score; // Rounds won
    // std::unique_ptr<PowerUp> activePowerUp;

  public:
    Player(const std::string &name, Color c);

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
    void resetForNewMatch();

    // Add to Player class if not already present
    void setHearts(int hearts) { this->hearts = hearts; }
    void setScore(int score) { this->score = score; }

    // void setActivePowerUp(std::unique_ptr<PowerUp> pu);
    // bool hasActivePowerUp() const;
    // std::unique_ptr<PowerUp> consumeActivePowerUp(); // Gets and clears the power-up
  };

} // namespace PowerChess

#endif // POWERCHESS_CORE_PLAYER_H