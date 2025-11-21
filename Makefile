# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -D_POSIX_C_SOURCE=200112L
SDL_FLAGS = `sdl2-config --cflags --libs` -lSDL2_ttf

# Diretórios
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Arquivos fonte
SOURCES = $(wildcard $(SRC_DIR)/*.c) \
          $(wildcard $(SRC_DIR)/exit/*.c) \
          $(wildcard $(SRC_DIR)/game/*.c) \
          $(wildcard $(SRC_DIR)/menu/*.c) \
          $(wildcard $(SRC_DIR)/utils/*.c)

# Arquivos objeto
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Nome do executável
TARGET = $(BIN_DIR)/tetris

# Regra padrão
all: $(TARGET)

# Criação do executável
$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CC) $(OBJECTS) -o $@ $(SDL_FLAGS)

# Compilação dos objetos
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Criação dos diretórios
$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/exit
	@mkdir -p $(OBJ_DIR)/game
	@mkdir -p $(OBJ_DIR)/menu
	@mkdir -p $(OBJ_DIR)/utils

# Limpeza
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Recompilar tudo
rebuild: clean all

# Executar o jogo
run: $(TARGET)
	./$(TARGET)

# Debug com valgrind
debug: CFLAGS += -g -DDEBUG
debug: rebuild

# Release
release: CFLAGS += -O2 -DNDEBUG
release: rebuild

# Ajuda
help:
	@echo "Targets disponíveis:"
	@echo "  all      - Compila o projeto (padrão)"
	@echo "  clean    - Remove arquivos gerados"
	@echo "  rebuild  - Recompila tudo"
	@echo "  run      - Compila e executa"
	@echo "  debug    - Compila com informações de debug"
	@echo "  release  - Compila com otimizações"
	@echo "  help     - Mostra esta ajuda"

.PHONY: all clean rebuild run debug release help

# Informações do projeto
info:
	@echo "=== Informações do Projeto Tetris ==="
	@echo "Diretório fonte: $(SRC_DIR)"
	@echo "Diretório objeto: $(OBJ_DIR)"
	@echo "Diretório binário: $(BIN_DIR)"
	@echo "Arquivos fonte: $(words $(SOURCES))"
	@echo "Arquivos objeto: $(words $(OBJECTS))"
	@echo "Executável: $(TARGET)"