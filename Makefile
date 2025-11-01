# Makefile para compilar el juego Coloretto

CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -g -std=c++17 -Iinclude
LDFLAGS =

# Directorios del proyecto
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj

# Archivos fuente y objetos
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
TARGET = coloretto

# Colores para que se vea mejor en la terminal
COLOR_GREEN = \033[0;32m
COLOR_BLUE = \033[0;34m
COLOR_RESET = \033[0m

.PHONY: all clean run valgrind help

all: $(TARGET)

# Enlazar todos los archivos objeto para crear el ejecutable
$(TARGET): $(OBJECTS)
	@echo "$(COLOR_GREEN)Enlazando $(TARGET)...$(COLOR_RESET)"
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
	@echo "$(COLOR_GREEN)Compilacion exitosa!$(COLOR_RESET)"

# Compilar cada archivo .c a .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@echo "$(COLOR_BLUE)Compilando $<...$(COLOR_RESET)"
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Crear la carpeta obj si no existe
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# Compilar y ejecutar el juego
run: $(TARGET)
	@echo "$(COLOR_GREEN)Ejecutando Coloretto...$(COLOR_RESET)"
	@echo ""
	./$(TARGET)

# Revisar si hay fugas de memoria con Valgrind
valgrind: $(TARGET)
	@echo "$(COLOR_BLUE)Analizando memoria con Valgrind...$(COLOR_RESET)"
	valgrind --leak-check=full \
	         --show-leak-kinds=all \
	         --track-origins=yes \
	         --verbose \
	         --log-file=valgrind-report.txt \
	         ./$(TARGET)
	@echo ""
	@echo "$(COLOR_GREEN)Reporte guardado en valgrind-report.txt$(COLOR_RESET)"

# Limpiar todos los archivos compilados
clean:
	@echo "$(COLOR_BLUE)Limpiando archivos...$(COLOR_RESET)"
	rm -rf $(OBJ_DIR)
	rm -f $(TARGET)
	rm -f valgrind-report.txt
	@echo "$(COLOR_GREEN)Limpieza completa$(COLOR_RESET)"

# Mostrar ayuda con los comandos disponibles
help:
	@echo "$(COLOR_GREEN)========================================$(COLOR_RESET)"
	@echo "$(COLOR_GREEN)        COLORETTO - MAKEFILE HELP       $(COLOR_RESET)"
	@echo "$(COLOR_GREEN)========================================$(COLOR_RESET)"
	@echo ""
	@echo "Comandos disponibles:"
	@echo "  $(COLOR_BLUE)make$(COLOR_RESET)           - Compila el proyecto"
	@echo "  $(COLOR_BLUE)make run$(COLOR_RESET)       - Compila y ejecuta el juego"
	@echo "  $(COLOR_BLUE)make clean$(COLOR_RESET)     - Elimina archivos compilados"
	@echo "  $(COLOR_BLUE)make valgrind$(COLOR_RESET)  - Analiza memoria con Valgrind"
	@echo "  $(COLOR_BLUE)make help$(COLOR_RESET)      - Muestra esta ayuda"
	@echo ""
	@echo "$(COLOR_GREEN)========================================$(COLOR_RESET)"

# Dependencias entre archivos (para que recompile si cambio un .h)
$(OBJ_DIR)/main.o: $(INC_DIR)/game_logic.h $(INC_DIR)/player.h $(INC_DIR)/deck.h $(INC_DIR)/common.h
$(OBJ_DIR)/game_logic.o: $(INC_DIR)/game_logic.h $(INC_DIR)/player.h $(INC_DIR)/deck.h $(INC_DIR)/common.h
$(OBJ_DIR)/player.o: $(INC_DIR)/player.h $(INC_DIR)/deck.h $(INC_DIR)/common.h
$(OBJ_DIR)/deck.o: $(INC_DIR)/deck.h $(INC_DIR)/common.h

