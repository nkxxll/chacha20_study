# Compiler
CC := clang

# Directories
SRC_DIR := src
BUILD_DIR := build
INCLUDE_DIR := include
MUNIT := munit
TEST_DIR := test

# Files
SRC_FILES := $(wildcard $(SRC_DIR)/*.c) munit/munit.c
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC_FILES))
TEST_FILES := $(wildcard $(TEST_DIR)/*.c)
TEST_EXEC := $(BUILD_DIR)/test_main

# Flags
CFLAGS := -I$(INCLUDE_DIR) -I$(MUNIT) -Wall -Wextra
LDFLAGS :=

# Targets
.PHONY: all build test clean

all: build

build: $(OBJ_FILES)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TEST_EXEC)
	$(TEST_EXEC)

$(TEST_EXEC): $(OBJ_FILES) $(TEST_FILES)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

clean:
	rm -rf $(BUILD_DIR)/*

