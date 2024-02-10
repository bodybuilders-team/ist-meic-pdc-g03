# Compiler options
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O3
LDFLAGS = -lm

# Directories
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin
DOC_DIR = doc

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))
EXEC_SERIAL = $(BIN_DIR)/life3d
EXEC_OMP = $(BIN_DIR)/life3d-omp
EXEC_MPI = $(BIN_DIR)/life3d-mpi

# Compiler rules
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Targets
all: $(EXEC_SERIAL) $(EXEC_OMP) $(EXEC_MPI)

$(EXEC_SERIAL): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(EXEC_OMP): $(OBJS)
	$(CC) $(CFLAGS) -fopenmp $^ -o $@ $(LDFLAGS)

$(EXEC_MPI): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Phony targets
.PHONY: clean run_serial run_omp run_mpi

clean:
	rm -rf $(BUILD_DIR)/*.o $(EXEC_SERIAL) $(EXEC_OMP) $(EXEC_MPI)

run_serial: $(EXEC_SERIAL)
	./$(EXEC_SERIAL) < /dev/null

run_omp: $(EXEC_OMP)
	./$(EXEC_OMP) < /dev/null

run_mpi: $(EXEC_MPI)
	mpirun -np <num_processes> ./$(EXEC_MPI) < /dev/null
