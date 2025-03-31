# Configuración básica
TARGET = main
CC = g++
CFLAGS = -g -Wall -Wextra -std=c++17 -Iinclude
SRC_DIR = src
BUILD_DIR = build

# Archivos fuente
SOURCES = $(wildcard $(SRC_DIR)/*.cpp) main.cpp
OBJECTS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(notdir $(SOURCES)))

# Crear directorio build si no existe
$(shell mkdir -p $(BUILD_DIR))

.PHONY: all clean

all: $(TARGET)

Yellow=\033[1;33m
Green=\033[1;32m
Blue = \033[1;34m
Red = \033[1;31m
Orange=\033[1;38;5;208m
EColor=\033[0m
# Regla para el ejecutable principal
$(TARGET): $(OBJECTS)
	@echo "$(Blue)» Enlazando$(EColor) $(Orange)$@$(EColor)..."
	@$(CC) $(CFLAGS) $^ -o $@

# Regla para archivos objeto de src
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "$(Yellow)› Compilando$(EColor) $(Green)$<$(EColor)..."
	@$(CC) $(CFLAGS) -c $< -o $@

# Regla para el objeto de main.cpp
$(BUILD_DIR)/main.o: main.cpp
	@echo "$(Yellow)› Compilando$(EColor) $(Orange)$<$(EColor)..."
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "$(Red)» Limpiando...$(EColor)"
	@rm -rf $(BUILD_DIR) $(TARGET)
