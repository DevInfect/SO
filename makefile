# Nome do executável
EXEC = projeto_so

# Compilador e flags de compilação
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11
LDFLAGS = -pthread

# Diretórios
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build
TEST_DIR = tests/file_tests

# Ficheiros fonte e objetos
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

# Incluir ficheiros de cabeçalho do diretório include
INCLUDES = -I$(INC_DIR)

# Criar diretório build se não existir
$(shell mkdir -p $(BUILD_DIR))

# Regra principal
all: $(EXEC)

# Regra para compilar o executável
$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# Regra para compilar cada ficheiro fonte em objeto
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Limpeza dos ficheiros objetos e executáveis
clean:
	rm -rf $(BUILD_DIR) $(EXEC)

# Regra para executar o programa com um ficheiro de teste
run: $(EXEC)
	./$(EXEC) $(TEST_DIR)/instances_2.txt 11 1

# Regra para limpeza e recompilação completa
rebuild: clean all

# Regra para verificar memory leaks com valgrind
memcheck: $(EXEC)
	valgrind --leak-check=full --show-leak-kinds=all ./$(EXEC) $(TEST_DIR)/instances_2.txt 11 1

.PHONY: all clean rebuild run memcheck