# PowerChess

**Hard Chess** is a two-player, turn-based chess program developed in C++ with a clean project structure, using terminal-based ASCII graphics. The focus is on classic chess gameplay without any AI or advanced graphical libraries.

---

## 📖 Project Description

Hard Chess allows two players to alternate turns, moving chess pieces rendered as ASCII characters in the terminal. The game continues until a Checkmate, Stalemate, or player resignation.

### 🎯 Functional Requirements

* **Chessboard Display**: Render an 8×8 board in the terminal using ASCII characters (e.g., r for rook, n for knight, uppercase for White, lowercase for Black).
* **Piece Movement**: Support legal moves for all piece types (pawn, rook, knight, bishop, queen, king).
* **Captures**: Enable capturing opponent pieces.
* **Game End Conditions**: Detect and display Checkmate, Stalemate, or Resignation.

### ⚙️ Non-Functional Requirements

* **Performance**: Command response time must be under 0.5 seconds.
* **Stability**: No crashes during normal play.
* **Maintainability**: Code must be modular and well-documented.

---

## 🗂️ Project Structure

```PowerChess/
├── src/                      # Source files (.cpp)
│   ├── Core/                 # Game logic
│   │   ├── Piece.cpp
│   │   ├── Pawn.cpp
│   │   ├── Rook.cpp
│   │   ├── Knight.cpp
│   │   ├── Bishop.cpp
│   │   ├── Queen.cpp
│   │   ├── King.cpp
│   │   ├── Board.cpp         # ASCII board rendering
│   │   ├── Player.cpp
│   │   └── Game.cpp
│   └── main.cpp              # Entry point, input loop & rendering
│
├── include/                  # Headers (.h)
│   └── PowerChess/
│       ├── Core/
│       │   ├── CommonTypes.h
│       │   ├── Piece.h
│       │   ├── Pawn.h
│       │   ├── Rook.h
│       │   ├── Knight.h
│       │   ├── Bishop.h
│       │   ├── Queen.h
│       │   ├── King.h
│       │   ├── Board.h
│       │   ├── Player.h
│       │   └── Game.h
│
├── doc/                      # Documentation
│   └── README.md             # This file
├── Makefile                  # Build 
```
---

## 🛠️ Build & Run Instructions

This project uses only standard C++ (C++11 or later) and requires no external libraries.

### Using Makefile

1. **Build**

   
bash
   make
   

2. **Run**

   
bash
   ./PowerChess
   

---

## 🔗 Related Links

* **GitHub Repository**: [https://github.com/Thadzy/PowerChess](https://github.com/Thadzy/PowerChess)

---
