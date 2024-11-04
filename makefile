# Nome do executável
EXEC = projeto_so

# Compilador e flags de compilação
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11

# Diretórios para os ficheiros fonte e cabeçalho
SRC_DIR = src
INC_DIR = include

# Ficheiros fonte e objetos
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:.c=.o)

# Incluir ficheiros de cabeçalho do diretório include
INCLUDES = -I$(INC_DIR)

# Regra para compilar o executável
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(EXEC) $(OBJS)

# Regra para compilar cada ficheiro fonte em objeto
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Limpeza dos ficheiros objetos e executáveis
clean:
	rm -f $(SRC_DIR)/*.o $(EXEC)

# Regra padrão para compilar tudo
all: $(EXEC)

# Regra para executar o programa com um ficheiro de teste (opcional)
run: $(EXEC)
	./$(EXEC) tests/file_tests/instances_2.txt 11 1


# Regra para limpeza e recompilação completa
rebuild: clean all
