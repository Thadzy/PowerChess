#ifndef POWERCHESS_POWERUPS_POWERUP_H
#define POWERCHESS_POWERUPS_POWERUP_H

#include <string>
#include "PowerChess/Core/CommonTypes.h" // For Player, Board, ConsoleUI forward declarations

// Forward declare Player, Board, ConsoleUI to avoid circular dependencies in headers
namespace PowerChess {
    class Player;
    class Board;
    class ConsoleUI;


    enum class PowerUpType {
        KNIGHT_BOOST
        // Add more types here
    };

    class PowerUp {
      protected:
        std::string name;
        std::string description;
        PowerUpType type;


      public:
        // Ensure constructor initialization order matches declaration order
        PowerUp(PowerUpType type, const std::string& name, const std::string& description)
            : name(name), description(description), type(type) {} 
        virtual ~PowerUp() = default;

        std::string getName() const { return name; }
        std::string getDescription() const { return description; }
        PowerUpType getType() const { return type; }

        // Player& player: The player using the power-up
        // Board& board: The game board, to modify pieces if needed
        // ConsoleUI& ui: For any interaction needed during application (e.g., selecting a piece)
        virtual void apply(Player& player, Board& board, ConsoleUI& ui) = 0;
        virtual std::unique_ptr<PowerUp> clone() const = 0;
    };

} // namespace PowerChess


#endif // POWERCHESS_POWERUPS_POWERUP_H