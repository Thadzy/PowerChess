# PowerChess ♟️⚡

**Version:** 1.0 (Initial Draft)

## Table of Contents
- [PowerChess ♟️⚡](#powerchess-️)
  - [Table of Contents](#table-of-contents)
  - [1. Game Overview](#1-game-overview)
  - [2. Game Manual \& Rules](#2-game-manual--rules)
    - [Objective](#objective)
    - [Setup](#setup)
    - [Gameplay](#gameplay)
    - [Piece Movements](#piece-movements)
    - [Special Rules](#special-rules)
    - [Power-Ups](#power-ups)
    - [Winning a Round](#winning-a-round)
    - [Winning the Game](#winning-the-game)
  - [3. Requirements](#3-requirements)
    - [Functional Requirements](#functional-requirements)
    - [Non-Functional Requirements](#non-functional-requirements)
    - [OOP Specific Requirements Met](#oop-specific-requirements-met)
  - [4. User Manual](#4-user-manual)
    - [System Requirements](#system-requirements)
    - [Installation/Setup](#installationsetup)
    - [How to Play](#how-to-play)
  - [5. Development \& Code Structure](#5-development--code-structure)
    - [Folder Structure](#folder-structure)
    - [Key Classes](#key-classes)

---

## 1. Game Overview

PowerChess is a thrilling twist on the classic game of chess. Each player starts with 3 hearts. The ultimate goal is to outwit your opponent and win 2 out of 3 rounds of chess. While the first round is standard chess, losing a round grants the defeated player special powers for subsequent rounds, adding a dynamic layer of strategy and comeback potential. This version is a 2-player local game.

---

## 2. Game Manual & Rules

### Objective
Be the first player to win 2 rounds of chess. Each player has 3 hearts. If a player loses all their hearts, they can no longer win rounds, effectively losing the match if their opponent has more rounds won or subsequently wins another round.

### Setup
* The game starts with a standard 8x8 chessboard setup.
* Player 1 (White) and Player 2 (Black).
* Each player begins with 3 hearts.
* The first round is standard chess.

### Gameplay
1.  **Turns:** White moves first, then players alternate turns.
2.  **Moving Pieces:** On your turn, select one of your pieces and move it to a valid square.
    * You can move to an empty square.
    * You can capture an opponent's piece by moving to its square. The captured piece is removed from the board.
3.  **Rounds:** A game consists of up to 3 rounds (or more if draws occur and no one has won 2 rounds yet).

### Piece Movements
Standard chess piece movements apply:
* **Pawn:** Moves one square forward (or two on its first move). Captures diagonally one square forward. Can be promoted to a Queen, Rook, Bishop, or Knight if it reaches the opponent's back rank.
* **Rook:** Moves any number of squares horizontally or vertically.
* **Knight:** Moves in an "L" shape (two squares in one direction, then one square perpendicular). Can jump over other pieces.
* **Bishop:** Moves any number of squares diagonally.
* **Queen:** Moves any number of squares horizontally, vertically, or diagonally.
* **King:** Moves one square in any direction. Cannot move into check.

### Special Rules
* **Check:** If a King is attacked by an opponent's piece, it is in "check." The player whose King is in check must make a move that removes the check (move the King, block the check, or capture the attacking piece).
* **Checkmate:** If a King is in check and there is no legal move to remove the check, it is "checkmate." The player delivering checkmate wins the round.
* **Stalemate:** If a player has no legal moves, and their King is *not* in check, the round is a "stalemate" (a draw). In PowerChess, a stalemate results in the round being a draw, and no hearts are lost. The round effectively counts for neither player.
* **Pawn Promotion:** When a pawn reaches the farthest rank from its starting position, it must be promoted to a Queen, Rook, Bishop, or Knight of the same color.

### Power-Ups
* **Gaining Power-Ups:** When a player loses a round (by checkmate) and still has hearts remaining, they receive a special power-up to use in a subsequent round.
* **Current Power-Up(s):**
    * **Knight's Royal Dance:**
        * **Effect:** Allows one of the player's Knights to move like a Queen for its *next single move only*.
        * **Activation:** Before moving a Knight, the player can choose to activate this power-up. The chosen Knight will then have Queen-like movement options for that immediate turn. After the move, the power-up is consumed.
* *(More power-ups can be conceptualized and added).*

### Winning a Round
A player wins a round by:
* Checkmating the opponent's King.
* The opponent resigning (not implemented in this version).

If a round is won, the winner gains 1 point towards the "best of 3" goal. The loser of the round loses 1 heart.

### Winning the Game
The first player to achieve **2 round wins** wins the PowerChess match.
Alternatively, if a player loses all 3 of their hearts, they are at a significant disadvantage. If their opponent has more rounds won at this point, or subsequently wins another round, the opponent wins the match.

---

## 3. Requirements

### Functional Requirements
1.  **FR1:** The system shall implement a 2-player chess game on an 8x8 board.
2.  **FR2:** Standard chess piece movements (Pawn, Rook, Knight, Bishop, Queen, King) shall be correctly implemented.
3.  **FR3:** The game shall correctly identify and handle Check, Checkmate, and Stalemate conditions.
4.  **FR4:** Each player shall start with 3 hearts. Losing a round (via checkmate) results in the loss of one heart for the loser.
5.  **FR5:** The game shall be played in rounds. The first player to win 2 rounds wins the overall game.
6.  **FR6:** If a player loses a round and has hearts remaining, they shall receive a power-up.
    * **FR6.1:** Implement "Knight's Royal Dance" power-up: a chosen Knight can move like a Queen for one turn.
7.  **FR7:** The game shall allow pawn promotion when a pawn reaches the opponent's back rank.
8.  **FR8:** The game interface shall display the board, pieces, player hearts, rounds won, and current turn.
9.  **FR9:** Players shall be able to select their pieces and move them using mouse clicks.
10. **FR10:** The system shall visually indicate valid moves for a selected piece.

### Non-Functional Requirements
1.  **NFR1:** The game should be developed in C++.
2.  **NFR2:** The game should use the SFML library (version 3.x) for graphics and user interaction.
3.  **NFR3:** The code should be well-commented and follow good OOP principles.
4.  **NFR4:** The game should be playable on macOS.
5.  **NFR5:** The user interface should be intuitive for playing chess.
6.  **NFR6:** The game state (board, scores, hearts) should be clearly visible.

### OOP Specific Requirements Met
* **Appropriate OOP Use (4 pts):**
    * **Encapsulation:** Data members are private/protected with public interfaces (e.g., `Piece`, `Board`, `Player`, `Game`).
    * **Abstraction:** Complex systems are represented by simpler interfaces (e.g., `Game` class manages overall flow, `Piece` class abstracts piece behavior).
    * **Polymorphism:** Achieved through virtual functions in the `Piece` hierarchy (e.g., `getValidMoves()`, `getSymbol()`) and `PowerUp` hierarchy (e.g., `apply()`).
* **At least 2 Additional Classes (2 pts each = 4 pts):**
    * `Player`: Manages player-specific data like hearts, rounds won, and power-ups.
    * `Game`: Manages the overall game state, rounds, turns, and rules.
    * `PowerUp` (and its derivatives like `KnightBoostPowerUp`): Manages special abilities.
    * `Graphics`: Handles SFML rendering and UI.
    * *(This fulfills the requirement with multiple additional classes beyond basic pieces and board).*
* **Have Inheritance at least 2 times (2 pts each = 4 pts):**
    * **1. Piece Hierarchy:** `Pawn`, `Rook`, `Knight`, `Bishop`, `Queen`, `King` all inherit from the abstract base class `Piece`.
    * **2. PowerUp Hierarchy:** `KnightBoostPowerUp` inherits from the abstract base class `PowerUp`. (Further power-ups would also inherit from `PowerUp`).
* **Have edited data on the board/game (3 pts):**
    * Pieces are moved on the `Board` (data edited).
    * Pieces are captured and removed from the `Board` (data edited).
    * Player hearts are decremented (`Player` data edited).
    * Player rounds won are incremented (`Player` data edited).
    * Power-ups are acquired and consumed (`Player` data edited, potentially piece state temporarily modified).
    * Pawn promotion changes a piece type on the `Board` (data edited).
    * `Piece::hasMoved` flag is updated.

---

## 4. User Manual

### System Requirements
* **Operating System:** macOS (tested on [Your macOS Version, e.g., Sonoma 14.x])
* **Compiler:** C++17 compatible (e.g., Clang, GCC)
* **Libraries:**
    * SFML 3.0.1 (Graphics, Window, System modules)
    * CMake (for building)
* **Hardware:** Basic modern Mac capable of running SFML applications.

### Installation/Setup
1.  **Prerequisites:**
    * Ensure you have a C++ compiler (Xcode Command Line Tools usually provide Clang).
    * Install Homebrew (if not already installed): `/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"`
    * Install CMake: `brew install cmake`
    * Install SFML: `brew install sfml` (This should install SFML 3.x)
2.  **Get the Code:**
    * Download or clone the PowerChess project repository/files.
3.  **Compile the Game:**
    * Open your terminal and navigate to the root directory of the PowerChess project (e.g., `cd /path/to/PowerChess/`).
    * Create a build directory: `mkdir build`
    * Navigate into the build directory: `cd build`
    * Run CMake to configure the project: `cmake ..`
        * If SFML is not found, you may need to specify its location: `cmake -DSFML_DIR="/opt/homebrew/Cellar/sfml/3.0.1/lib/cmake/SFML" ..` (adjust path if your SFML installation is different).
    * Compile the project: `make`
4.  **Locate Assets:**
    * Ensure you have an `arial.ttf` font file (or similar) located at `PowerChess/assets/fonts/arial.ttf`. The game needs this font to display text.

### How to Play
1.  **Run the Game:**
    * After successful compilation, the executable `PowerChess` will be in the `PowerChess/build/` directory.
    * Navigate to the `build` directory in your terminal and run: `./PowerChess`
    * *(If an .app bundle was created, you can run that instead, but ensure assets are correctly bundled or accessible).*
2.  **Gameplay Interface:**
    * The game window will display the chessboard, pieces, and UI elements showing player hearts, rounds won, and whose turn it is.
3.  **Making a Move:**
    * **Select a Piece:** Click on one ofyour pieces. Valid moves for that piece should be highlighted.
    * **Move the Piece:** Click on one of the highlighted valid destination squares.
    * If you select the wrong piece or want to deselect, click outside the board or on the same piece again (current implementation clears selection on any click after first selection if not a valid move target, or if you click another of your own pieces it reselects).
4.  **Pawn Promotion:**
    * If your pawn reaches the opponent's back rank, a dialog will appear. Click on the piece type (Queen, Rook, Bishop, or Knight) you wish to promote your pawn to.
5.  **Using Power-Ups (e.g., Knight's Royal Dance):**
    * If you have a power-up available (e.g., after losing a round), a "Use Power-Up" button might appear, or it will be indicated in the UI.
    * Click the "Use Power-Up" button. A dialog may appear listing your available power-ups.
    * Select the "Knight's Royal Dance."
    * The game will then prompt you to click on one of your Knights on the board.
    * After selecting a Knight, its next move will allow Queen-like movement options. Make the move as usual. The power-up is consumed after this move.
6.  **Winning:**
    * Play until one player wins 2 rounds or the game ends due to heart depletion.
    * The game status will indicate checkmates, stalemates, and round/game winners.
    * A "Next Round / Cont." button will appear after a round ends or after power-up selection to proceed.

---

## 5. Development & Code Structure

### Folder Structure

PowerChess/
├── src/                      # Source (.cpp) files
│   ├── Core/                 # Core game logic (Piece, Board, Player, Game)
│   ├── PowerUps/             # Power-up system
│   └── Graphics/             # SFML graphics handling
│   └── main.cpp              # Main application entry point
│
├── include/                  # Header (.h) files
│   ├── PowerChess/           # Namespace/project specific
│   │   ├── Core/
│   │   ├── PowerUps/
│   │   └── Graphics/
│
├── assets/                   # Game assets
│   ├── fonts/                # (e.g., arial.ttf)
│   └── images/               # (For piece sprites, if added later)
│
├── doc/                      # Documentation (like this README)
│   └── README.md
│
├── build/                    # Build output (created by CMake)
│
└── CMakeLists.txt            # CMake build script


### Key Classes
* **`CommonTypes.h`**: Defines shared enums (`PlayerColor`, `PieceType`, `RoundState`, `PowerUpType`) and structs (`Position`, `Move`).
* **`Piece` (and derivatives)**: Abstract base for chess pieces, with derived classes (`Pawn`, `Rook`, etc.) implementing specific movement logic.
* **`Board`**: Represents the chessboard, manages piece positions, and provides utility functions for move validation.
* **`Player`**: Stores player-specific data including color, hearts, rounds won, and available/active power-ups.
* **`PowerUp` (and derivatives)**: Abstract base for power-ups, with derived classes like `KnightBoostPowerUp` implementing specific power effects.
* **`Game`**: The central orchestrator. Manages game rounds, player turns, win/loss conditions, power-up distribution, and overall game state.
* **`Graphics`**: Handles all SFML-related tasks: window creation, event handling (mouse clicks), rendering the board, pieces, and UI elements.
* **`main.cpp`**: Entry point of the application, initializes the `Game` and `Graphics` objects, and contains the main game loop.

---
