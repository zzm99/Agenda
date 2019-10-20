CC := g++
FLAGS := -std=c++2a -w -g -pthread
INC_DIR := include
SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin
TEST_DIR := test
INCLUDE := -I./$(INC_DIR) 
TEST_LIBRARY := -lgtest -lgtest_main

SOURCE_FILES=$(shell find $(SRC_DIR) -name '*.cpp')
OBJS=$(patsubst $(SOURCE_FILES)/%.cpp,$(BUILD_DIR)/%.o,$(SOURCE_FILES))

run: $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(FLAGS) $(INCLUDE) $^ -o $@ $(TEST_LIBRARY)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CC) $(FLAGS) $(INCLUDE) -c -o $@ $<