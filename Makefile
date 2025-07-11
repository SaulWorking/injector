# Compiler and flags
CXX := g++
CXXFLAGS := -W

# Folders
SRC_DIR := src
BIN_DIR := output/bin

# Target executable
TARGET := $(BIN_DIR)/main

# Find all .cpp files in the source directory
SOURCES := $(wildcard $(SRC_DIR)/*.cpp)

# Default rule
all: $(TARGET)

$(TARGET): | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $@

# Create binary directory if it doesn't exist
$(BIN_DIR):
	mkdir -p $(BIN_DIR)