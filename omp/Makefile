# Compiler options
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -fopenmp -I$(INCLUDE_DIR) -g
LDFLAGS = -lm

# Directories
INCLUDE_DIR = include
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = .#bin
DOC_DIR = doc

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))
EXEC = $(BIN_DIR)/life3d

# Create the build and bin directories if any of them doesn't exist
$(shell mkdir -p $(BUILD_DIR) $(BIN_DIR))

# Compiler rules
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Targets
all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Phony targets
.PHONY: clean

clean:
	rm -rf $(BUILD_DIR)/*.o $(EXEC)
