#ifndef POWERCHESS_CORE_PLAYER_H
#define POWERCHESS_CORE_PLAYER_H

#include "Header/Core/CommonTypes.h"
#include <string>
#include <memory> // For std::unique_ptr

namespace HardChess
{

  class Player
  {
  private:
    std::string name;
    Color color;
    int hearts;
    int score; // Rounds won

  public:
    // Constructor to initialize a Player with name and color
    Player(const std::string &name, Color c);

    // Deleted copy constructor and assignment operator to prevent copying
    const std::string &getName() const { return name; }
    Color getColor() const { return color; }
    int getHearts() const { return hearts; }
    int getScore() const { return score; }

    // Function to get the color as a string
    std::string getColorString() const
    {
      return (color == Color::WHITE) ? "White" : "Black";
    }

    // Function to display player information
    void loseHeart();
    void incrementScore();
    void resetForNewMatch();

    // Add to Player class if not already present
    void setHearts(int hearts) { this->hearts = hearts; }
    void setScore(int score) { this->score = score; }

  };

} // namespace PowerChess

#endif // POWERCHESS_CORE_PLAYER_H