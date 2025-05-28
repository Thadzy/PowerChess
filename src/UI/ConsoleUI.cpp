#include "Header/UI/ConsoleUI.h"
#include "Header/Core/Board.h"   // For displayBoard
#include "Header/Core/Player.h"  // For displayPlayerStats, etc.
#include <iostream>
#include <limits>   // For std::numeric_limits
#include <cctype>   // For std::tolower
#include <cstdlib>  // For system("cls") / system("clear")

namespace HardChess
{
    void ConsoleUI::displayWelcomeScreen() const
    {
        std::cout << "\n";
        std::cout << "=============================================\n";
        std::cout << "             WELCOME TO HARDCHESS            \n";
        std::cout << "         An Enhanced Chess Experience        \n";
        std::cout << "=============================================\n\n";
    }

    int ConsoleUI::displayMainMenu() const
    {
        std::cout << "\n============== MAIN MENU ==============\n";
        std::cout << "|                                     |\n";
        std::cout << "|         1. Start New Match          |\n";
        std::cout << "|         2. Help & Rules             |\n";
        std::cout << "|         3. Exit Game                |\n";
        std::cout << "|                                     |\n";
        std::cout << "=======================================\n";
        std::cout << "Enter your choice (1-3): ";

        int choice;
        while (!(std::cin >> choice) || choice < 1 || choice > 3)
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid choice! Please enter a number between 1 and 3: ";
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Consume newline
        return choice;
    }

    void ConsoleUI::displayBoard(const Board &board) const
    {
        std::cout << "\n";
        std::cout << "    a  b  c  d  e  f  g  h\n";
        std::cout << "   -------------------------\n";
        for (int r = 0; r < 8; ++r)
        {
            std::cout << 8 - r << " |";
            for (int c = 0; c < 8; ++c)
            {
                Piece *piece = board.getPiecePtr(Position(r, c));
                if (piece)
                {
                    std::cout << " " << piece->getSymbol() << " ";
                }
                else
                {
                    // Alternating characters for empty squares
                    std::cout << ((r + c) % 2 == 0 ? " . " : "   ");
                }
            }
            std::cout << "| " << 8 - r << "\n";
        }
        std::cout << "   -------------------------\n";
        std::cout << "    a  b  c  d  e  f  g  h\n\n";
    }

    void ConsoleUI::displayPlayerStats(const Player &player1, const Player &player2) const
    {
        std::cout << "--------------------------------------------------\n";
        std::cout << player1.getName() << " (White) - Score: " << player1.getScore() << ", Hearts: " << player1.getHearts() << "\n";
        std::cout << player2.getName() << " (Black) - Score: " << player2.getScore() << ", Hearts: " << player2.getHearts() << "\n";
        std::cout << "--------------------------------------------------\n";
    }

    void ConsoleUI::displayCurrentPlayerTurn(const Player &player) const
    {
        std::cout << "\n>>> " << player.getName() << "'s Turn (" << player.getColorString() << ") <<<\n";
    }

    void ConsoleUI::displayMessage(const std::string &message) const
    {
        std::cout << message << std::endl;
    }

    void ConsoleUI::displayError(const std::string &error) const
    {
        std::cout << "ERROR: " << error << std::endl;
    }

    void ConsoleUI::displaySuccess(const std::string &success) const
    {
        std::cout << "SUCCESS: " << success << std::endl;
    }

    void ConsoleUI::displayRoundResult(RoundState result, const std::string& winnerName) const {
        std::cout << "\n========== ROUND OVER ==========\n";
        switch (result) {
            case RoundState::CHECKMATE_WHITE_WINS:
            case RoundState::CHECKMATE_BLACK_WINS:
                std::cout << "CHECKMATE! " << winnerName << " wins the round!\n";
                break;
            case RoundState::STALEMATE:
                std::cout << "STALEMATE! The round is a draw.\n";
                break;
            default: // ONGOING or other
                std::cout << "Round concluded.\n"; // Should not happen if called at end
                break;
        }
        std::cout << "==============================\n";
    }
    
    void ConsoleUI::displayMatchResult(const Player* winner, const Player& p1, const Player& p2) const {
        std::cout << "\n\n|||||||||||| MATCH OVER ||||||||||||\n";
        if (winner) {
            std::cout << "CONGRATULATIONS! " << winner->getName() << " (" << winner->getColorString() << ") wins the match!\n";
        } else {
            std::cout << "The match is a DRAW!\n";
        }
        std::cout << "Final Scores:\n";
        std::cout << "  " << p1.getName() << ": " << p1.getScore() << " rounds won, " << p1.getHearts() << " hearts left.\n";
        std::cout << "  " << p2.getName() << ": " << p2.getScore() << " rounds won, " << p2.getHearts() << " hearts left.\n";
        std::cout << "|||||||||||||||||||||||||||||||||||\n\n";
    }


    void ConsoleUI::getMoveInput(const Player& player, std::string& startStr, std::string& endStr) const {
        std::cout << player.getName() << ", enter your move (e.g., e2 e4): ";
        std::cin >> startStr >> endStr;
        // Basic validation or error handling for cin failure could be added here
        while (std::cin.fail() || startStr.length() != 2 || endStr.length() != 2) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            displayError("Invalid input format. Use two positions like 'e2 e4'.");
            std::cout << player.getName() << ", enter your move (e.g., e2 e4): ";
            std::cin >> startStr >> endStr;
        }
    }


    PieceType ConsoleUI::promptPawnPromotionChoice(const Player &player) const
    {
        std::cout << "\n--- Pawn Promotion for " << player.getName() << " ---\n";
        std::cout << "Choose piece to promote to:\n";
        std::cout << "  Q - Queen\n";
        std::cout << "  R - Rook\n";
        std::cout << "  B - Bishop\n";
        std::cout << "  N - Knight\n";
        std::cout << "Enter your choice (Q/R/B/N): ";

        char choiceChar;
        while (true)
        {
            std::cin >> choiceChar;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Consume newline
            choiceChar = std::tolower(choiceChar);
            switch (choiceChar)
            {
            case 'q': displayMessage("Promoted to Queen!"); return PieceType::QUEEN;
            case 'r': displayMessage("Promoted to Rook!"); return PieceType::ROOK;
            case 'b': displayMessage("Promoted to Bishop!"); return PieceType::BISHOP;
            case 'n': displayMessage("Promoted to Knight!"); return PieceType::KNIGHT;
            default:
                std::cout << "Invalid choice! Enter Q, R, B, or N: ";
            }
        }
    }
    
    std::string ConsoleUI::getPlayerName(const std::string& prompt) const {
        std::string name;
        std::cout << prompt;
        std::getline(std::cin, name);
        if (name.empty()) {
            return (prompt.find("White") != std::string::npos) ? "Player 1 (White)" : "Player 2 (Black)";
        }
        return name;
    }


    void ConsoleUI::displayHelp() const
    {
        clearScreen();
        std::cout << "\n========== HELP & RULES ==========\n";
        std::cout << "HOW TO PLAY:\n";
        std::cout << "  - Chess is a two-player strategy board game played on a checkered board.\n";
        std::cout << "  - Each player begins with 16 pieces: one king, one queen, two rooks,\n";
        std::cout << "    two knights, two bishops, and eight pawns.\n";
        std::cout << "  - The objective is to checkmate the opponent's king, whereby the king\n";
        std::cout << "    is under immediate attack (in 'check') and there is no way for it\n";
        std::cout << "    to escape.\n\n";
        std::cout << "ENTERING MOVES:\n";
        std::cout << "  - Use algebraic notation for squares (e.g., 'e2', 'h8').\n";
        std::cout << "  - Enter the starting square followed by the ending square, separated by a space.\n";
        std::cout << "    Example: 'e2 e4' moves the piece from e2 to e4.\n\n";
        std::cout << "SPECIAL MOVES:\n";
        std::cout << "  - Pawn Promotion: If a pawn reaches the opponent's back rank, you will be\n";
        std::cout << "    prompted to choose a Queen, Rook, Bishop, or Knight to replace it.\n";
        std::cout << "  - Castling: A special king and rook move. Type the king's start and end\n";
        std::cout << "    squares (e.g., 'e1 g1' for white kingside castle).\n";
        std::cout << "  - En Passant: A special pawn capture.\n\n";
        std::cout << "MATCHES & HEARTS:\n";
        std::cout << "  - A match consists of several rounds (games).\n";
        std::cout << "  - Each player starts with a set number of hearts (e.g., 3).\n";
        std::cout << "  - Winning a round earns you a point.\n";
        std::cout << "  - Losing a round costs you a heart.\n";
        std::cout << "  - The match ends if a player reaches the target number of wins OR\n";
        std::cout << "    if a player loses all their hearts.\n";
        std::cout << "==================================\n";
        pauseForUser();
    }

    void ConsoleUI::displayGoodbye() const
    {
        std::cout << "\n=============================================\n";
        std::cout << "        Thanks for playing HardChess!        \n";
        std::cout << "              Come back soon!                \n";
        std::cout << "=============================================\n\n";
    }

    void ConsoleUI::clearScreen() const
    {
        #ifdef _WIN32
            std::system("cls");
        #else
            std::system("clear"); // Works on Linux/macOS
        #endif
    }

    void ConsoleUI::pauseForUser() const
    {
        std::cout << "\nPress Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        // If previous input was line-based (getline), an extra ignore might not be needed
        // or std::cin.get() might be better. For now, this handles typical cases after >>.
    }
    
    std::string ConsoleUI::formatPosition(Position pos) const {
        if (!pos.isValid()) return "??";
        char file = 'a' + pos.col;
        char rank = '1' + (7 - pos.row); // '1' is row 7, '8' is row 0
        return std::string(1, file) + std::string(1, rank);
    }

} // namespace HardChess