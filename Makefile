CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude # Add -g for debugging
LDFLAGS =

# Source files
SRCDIR_CORE = src/Core
SRCDIR_UI = src/UI
MAIN_SRC = src/main.cpp

CORE_SRCS = $(wildcard $(SRCDIR_CORE)/*.cpp)
UI_SRCS = $(wildcard $(SRCDIR_UI)/*.cpp)

# Object files
OBJDIR = build/obj
CORE_OBJS = $(patsubst $(SRCDIR_CORE)/%.cpp, $(OBJDIR)/%.o, $(CORE_SRCS))
UI_OBJS = $(patsubst $(SRCDIR_UI)/%.cpp, $(OBJDIR)/%.o, $(UI_SRCS))
MAIN_OBJ = $(patsubst src/%.cpp, $(OBJDIR)/%.o, $(MAIN_SRC))

TARGET = build/HardChess

# Default target
all: $(TARGET)

$(TARGET): $(MAIN_OBJ) $(CORE_OBJS) $(UI_OBJS)
	@mkdir -p build
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)
	@echo "Built $(TARGET) successfully."

$(OBJDIR)/%.o: $(SRCDIR_CORE)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR_UI)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.o: src/%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo "Cleaning build files..."
	rm -rf $(OBJDIR) $(TARGET) build

run: all
	./$(TARGET)

.PHONY: all clean run