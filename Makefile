# Compiler and Flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

# Linker Flags (add -fsanitize=address -g for debugging)
LDFLAGS =

# Source Files
SOURCES = src/main.cpp \
          src/Core/Piece.cpp \
          src/Core/Pawn.cpp \
          src/Core/Rook.cpp \
          src/Core/Knight.cpp \
          src/Core/Bishop.cpp \
          src/Core/Queen.cpp \
          src/Core/King.cpp \
          src/Core/Board.cpp \
          src/Core/Player.cpp \
          src/Core/Game.cpp \
          src/UI/ConsoleUI.cpp

# Object Files (replace .cpp with .o)
OBJECTS = $(SOURCES:.cpp=.o)

# Output Executable
EXECUTABLE = HardChess

# Default Target
all: $(EXECUTABLE)

# Linking the executable
$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(EXECUTABLE) $(LDFLAGS)

# Compiling each .cpp into .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run the program
run: $(EXECUTABLE)
	./$(EXECUTABLE)

# Clean build artifacts
clean:
	rm -f $(EXECUTABLE) $(OBJECTS) PowerChess.dSYM
