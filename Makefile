# Compiler and flags
CC = gcc
CFLAGS = -Wall -g

# Directories
SRC_DIR = src
TEST_DIR = test
OBJ_DIR = obj

# Source files and object files for the main program
SRC = $(filter-out $(SRC_DIR)/main.c, $(wildcard $(SRC_DIR)/*.c))  # Exclude main.c
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Test source files and object files
TEST_SRC = $(wildcard $(TEST_DIR)/*.c)
TEST_OBJ = $(TEST_SRC:$(TEST_DIR)/%.c=$(OBJ_DIR)/%.o)

# Main program executable
TARGET = elevator_program

# Test executable
TEST_TARGET = test_runner

# Default target: build and run the program
all: $(TARGET)
	./$(TARGET)

# Build the program
$(TARGET): $(OBJ) $(OBJ_DIR)/main.o
	$(CC) $(OBJ) $(OBJ_DIR)/main.o -o $(TARGET)

# Rule for compiling source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule for compiling test files into object files
$(OBJ_DIR)/%.o: $(TEST_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Build and run tests
run_tests: $(TEST_TARGET)
	./$(TEST_TARGET)

# Build the test runner
$(TEST_TARGET): $(OBJ) $(TEST_OBJ)
	$(CC) $(OBJ) $(TEST_OBJ) -o $(TEST_TARGET)

# Clean generated files
clean:
	rm -rf $(OBJ_DIR)/*.o $(TARGET) $(TEST_TARGET)
