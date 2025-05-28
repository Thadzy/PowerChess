#include "PowerChess/UI/ConsoleUI.h"
#include "PowerChess/Core/Board.h"
#include "PowerChess/Core/Player.h"
#include <iostream>
#include <limits>
#include <fstream>
#include <iomanip>

namespace PowerChess {

void ConsoleUI::displayWelcomeScreen() const {
    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════════════════╗\n";
    std::cout << "║              ♔ POWER CHESS ♛                    ║\n";
    std::cout << "║          Enhanced Chess Experience! 🎯          ║\n";
    std::cout << "╚══════════════════════════════════════════════════╝\n";
    std::cout << "\n";
}

int ConsoleUI::displayMainMenu() const {
    std::cout << "\n";
    std::cout << "🎮 ═══════════ MAIN MENU ═══════════ 🎮\n";
    std::cout << "│                                      │\n";
    std::cout << "│  1. 🆕 Start New Game               │\n";
    std::cout << "│  2. 💾 Save Current Game            │\n";
    std::cout << "│  3. 📂 Load Saved Game              │\n";
    std::cout << "│  4. ⚙️  Game Settings               │\n";
    std::cout << "│  5. 📊 View Statistics              │\n";
    std::cout << "│  6. ❓ Help & Rules                 │\n";
    std::cout << "│  7. 🚪 Exit Game                    │\n";
    std::cout << "│                                      │\n";
    std::cout << "╚══════════════════════════════════════╝\n";
    std::cout << "\n🎯 Enter your choice (1-7): ";

    int choice;
    while (!(std::cin >> choice) || choice < 1 || choice > 7) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "❌ Invalid choice! Please enter a number between 1-7: ";
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return choice;
}

void ConsoleUI::displayBoard(const Board& board) const {
    std::cout << "\n";
    std::cout << "    🅰 🅱 🅲 🅳 🅴 🅵 🅶 🅷\n";
    std::cout << "  ┌─────────────────────────┐\n";
    for (int r = 0; r < 8; ++r) {
        std::cout << 8 - r << " │ ";
        for (int c = 0; c < 8; ++c) {
            Piece* piece = board.getPiecePtr(Position(r, c));
            if (piece) {
                std::cout << piece->getSymbol() << " ";
            } else {
                std::cout << ((r + c) % 2 == 0 ? " " : "░") << " ";
            }
        }
        std::cout << "│ " << 8 - r << "\n";
    }
    std::cout << "  └─────────────────────────┘\n";
    std::cout << "    🅰 🅱 🅲 🅳 🅴 🅵 🅶 🅷\n";
    std::cout << "\n";
}

void ConsoleUI::displayPlayerStats(const Player& player1, const Player& player2) const {
    std::cout << "\n";
    std::cout << "┌──────────── 📊 PLAYER STATS 📊 ────────────┐\n";
    std::cout << "│                                             │\n";
    std::cout << "│ " << std::left << std::setw(20) << (player1.getName() + " (♔White):")
              << " ❤️ " << player1.getHearts() << "  🏆 " << player1.getScore() << std::setw(10) << " │\n";
    std::cout << "│ " << std::left << std::setw(20) << (player2.getName() + " (♚Black):")
              << " ❤️ " << player2.getHearts() << "  🏆 " << player2.getScore() << std::setw(10) << " │\n";
    std::cout << "│                                             │\n";
    std::cout << "└─────────────────────────────────────────────┘\n";
    std::cout << "\n";
}

void ConsoleUI::displayPlayerTurn(const Player* currentPlayer) const {
    if (currentPlayer) {
        std::string colorEmoji = (currentPlayer->getColorString() == "White") ? "♔" : "♚";
        std::cout << "\n";
        std::cout << "🎯 ═══════════════════════════════════════════ 🎯\n";
        std::cout << "     " << colorEmoji << " " << currentPlayer->getName()
                  << "'s Turn (" << currentPlayer->getColorString() << ") " << colorEmoji << "\n";
        std::cout << "🎯 ═══════════════════════════════════════════ 🎯\n";
        std::cout << "\n";
    }
}

void ConsoleUI::displayMessage(const std::string& message) const {
    std::cout << "ℹ️  " << message << std::endl;
}

void ConsoleUI::displayError(const std::string& error) const {
    std::cout << "❌ ERROR: " << error << std::endl;
}

void ConsoleUI::displaySuccess(const std::string& success) const {
    std::cout << "✅ " << success << std::endl;
}

std::string ConsoleUI::getPlayerMove(const Player& player) const {
    std::string moveStr;
    std::string colorEmoji = (player.getColorString() == "White") ? "♔" : "♚";
    std::cout << colorEmoji << " " << player.getName()
              << ", enter your move (e.g., e2e4 or a7a8q for promotion): ";
    std::cin >> moveStr;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return "";
    }
    return moveStr;
}

std::string ConsoleUI::promptForPieceSelection(const std::string& promptMessage) const {
    std::string posStr;
    std::cout << "🎯 " << promptMessage;
    std::cin >> posStr;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return "";
    }
    return posStr;
}

void ConsoleUI::displayRoundResult(Player* winner) const {
    std::cout << "\n";
    std::cout << "🎊 ═══════════════════════════════════════ 🎊\n";
    if (winner) {
        std::string colorEmoji = (winner->getColorString() == "White") ? "♔" : "♚";
        std::cout << "    🏆 ROUND OVER! WINNER: " << winner->getName()
                  << " " << colorEmoji << " (" << winner->getColorString() << ") 🏆\n";
    } else {
        std::cout << "    🤝 ROUND OVER! IT'S A DRAW (STALEMATE) 🤝\n";
    }
    std::cout << "🎊 ═══════════════════════════════════════ 🎊\n";
    std::cout << "\n";
}

void ConsoleUI::displayMatchResult(Player* winner) const {
    std::cout << "\n";
    std::cout << "🎆 ╔══════════════════════════════════════════════╗ 🎆\n";
    std::cout << "🎆 ║                                              ║ 🎆\n";
    if (winner) {
        std::string colorEmoji = (winner->getColorString() == "White") ? "♔" : "♚";
        std::cout << "🎆 ║     🏆 MATCH OVER! CONGRATULATIONS! 🏆      ║ 🎆\n";
        std::cout << "🎆 ║        " << std::setw(20) << (winner->getName() + " " + colorEmoji + " WINS!")
                  << "             ║ 🎆\n";
    } else {
        std::cout << "🎆 ║       🤝 MATCH OVER! IT'S A DRAW! 🤝        ║ 🎆\n";
    }
    std::cout << "🎆 ║                                              ║ 🎆\n";
    std::cout << "🎆 ╚══════════════════════════════════════════════╝ 🎆\n";
    std::cout << "\n";
}

std::string ConsoleUI::formatPosition(Position pos) const {
    if (!pos.isValid()) return "❓❓";
    char file = 'a' + pos.col;
    char rank = '1' + (7 - pos.row);
    return std::string(1, file) + std::string(1, rank);
}

PieceType ConsoleUI::promptPawnPromotionChoice(const Player& player) const {
    std::cout << "\n";
    std::cout << "🎉 ═══════════ PAWN PROMOTION! ═══════════ 🎉\n";
    std::cout << "│                                           │\n";
    std::cout << "│ " << player.getName() << ", your pawn can be promoted! 👑 │\n";
    std::cout << "│                                           │\n";
    std::cout << "│ Choose piece to promote to:               │\n";
    std::cout << "│  Q - ♕/♛ Queen   (Most powerful!)        │\n";
    std::cout << "│  R - ♖/♜ Rook    (Castle power!)         │\n";
    std::cout << "│  B - ♗/♝ Bishop  (Diagonal master!)      │\n";
    std::cout << "│  N - ♘/♞ Knight  (Horse jumper!)         │\n";
    std::cout << "│                                           │\n";
    std::cout << "╚═══════════════════════════════════════════╝\n";
    std::cout << "\n👑 Enter your choice (Q/R/B/N): ";

    char choiceChar;
    while (true) {
        std::cin >> choiceChar;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        switch (tolower(choiceChar)) {
            case 'q':
                std::cout << "✅ Promoted to Queen! ♕/♛\n";
                return PieceType::QUEEN;
            case 'r':
                std::cout << "✅ Promoted to Rook! ♖/♜\n";
                return PieceType::ROOK;
            case 'b':
                std::cout << "✅ Promoted to Bishop! ♗/♝\n";
                return PieceType::BISHOP;
            case 'n':
                std::cout << "✅ Promoted to Knight! ♘/♞\n";
                return PieceType::KNIGHT;
            default:
                std::cout << "❌ Invalid choice! Enter Q, R, B, or N: ";
        }
    }
}

void ConsoleUI::displayGameSaved(const std::string& filename) const {
    std::cout << "\n💾 Game saved successfully to: " << filename << " ✅\n";
}

void ConsoleUI::displayGameLoaded(const std::string& filename) const {
    std::cout << "\n📂 Game loaded successfully from: " << filename << " ✅\n";
}

void ConsoleUI::displayHelp() const {
    std::cout << "\n";
    std::cout << "❓ ═══════════════ HELP & RULES ═══════════════ ❓\n";
    std::cout << "│                                                │\n";
    std::cout << "│ 🎯 HOW TO PLAY:                               │\n";
    std::cout << "│  • Enter moves like 'e2e4' (from-to format)   │\n";
    std::cout << "│  • For pawn promotion: 'a7a8q' (q=queen)      │\n";
    std::cout << "│  • Capture opponent pieces to score points!   │\n";
    std::cout << "│                                                │\n";
    std::cout << "│ 🏆 SCORING:                                    │\n";
    std::cout << "│  • Win rounds to earn points!                 │\n";
    std::cout << "│  • First to lose all hearts loses match!      │\n";
    std::cout << "│                                                │\n";
    std::cout << "╚════════════════════════════════════════════════╝\n";
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}

std::string ConsoleUI::getSaveFileName() const {
    std::string filename;
    std::cout << "\n💾 Enter filename to save (without extension): ";
    std::cin >> filename;
    return filename + ".pcg";
}

std::string ConsoleUI::getLoadFileName() const {
    std::string filename;
    std::cout << "\n📂 Enter filename to load (without extension): ";
    std::cin >> filename;
    return filename + ".pcg";
}

void ConsoleUI::displayGoodbye() const {
    std::cout << "\n";
    std::cout << "🎮 ════════════════════════════════════════════ 🎮\n";
    std::cout << "│                                                │\n";
    std::cout << "│      👋 Thanks for playing Power Chess! 👋    │\n";
    std::cout << "│           Come back soon for more! 🎯         │\n";
    std::cout << "│                                                │\n";
    std::cout << "🎮 ════════════════════════════════════════════ 🎮\n";
    std::cout << "\n";
}

void ConsoleUI::clearScreen() const {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void ConsoleUI::pauseForUser() const {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore();
    std::cin.get();
}

} // namespace PowerChess