# Makefile pour le projet Pokemon_Arena

# Compilateur et options
CXX = C:/msys64/ucrt64/bin/g++.exe
CXXFLAGS = -Wall -Wextra -std=c++11 -g -Wfatal-errors

# Dossiers
INC_DIR = include
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Fichiers source et objets
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Exécutable
TARGET = $(BIN_DIR)/Pokemon_Arena.exe

# Règle par défaut
all: directories $(TARGET)

# Création des répertoires
directories:
	@if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)
	@if not exist $(BIN_DIR) mkdir $(BIN_DIR)

# Règle pour l'exécutable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Règle pour les fichiers objets
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

# Nettoyage
clean:
	@if exist $(OBJ_DIR)\*.o del /Q $(OBJ_DIR)\*.o
	@if exist $(BIN_DIR)\*.exe del /Q $(BIN_DIR)\*.exe

# Exécution
run: all
	$(TARGET)

# Règles pour les tests
TESTS_DIR = tests
TEST_TARGET = $(BIN_DIR)/Tests.exe

tests: directories $(TEST_TARGET)

$(TEST_TARGET): $(TESTS_DIR)/TestSuite.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -o $@ $^

run-tests: tests
	$(TEST_TARGET)

.PHONY: all clean run directories tests run-tests
