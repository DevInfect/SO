CC = gcc
CFLAGS = -Wall -Wextra -I./include -g -O2
LDFLAGS = -pthread -lrt

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
TEST_DIR = tests/file_tests

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
EXEC = $(BIN_DIR)/safe

.PHONY: all clean test

all: $(EXEC)

$(EXEC): $(OBJS) | $(BIN_DIR)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

test: $(EXEC)
	@echo "Running tests..."
	@for test in $(TEST_DIR)/instances_*.txt; do \
		echo "Testing $$test..."; \
		./$(EXEC) $$test 4 5000; \
	done

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
