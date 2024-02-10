# Compiler options
CC = mpicc
CFLAGS = -Wall -Wextra -std=c99 -O3
LDFLAGS = -lm

# Directories
SRC_DIR = .
BUILD_DIR = build
BIN_DIR = bin
DOC_DIR = doc

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))
EXEC = $(BIN_DIR)/life3d-mpi

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
