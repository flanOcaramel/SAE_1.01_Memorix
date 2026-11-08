# Compilateur et options
CC      = gcc
CFLAGS  = -Wall -Wextra -std=gnu99 -Iinclude

# Dossiers
BUILD   = build
SRC     = src
INC     = include

# Fichiers du projet
SOURCES = $(SRC)/main.c $(SRC)/jeu.c $(SRC)/creation.c $(SRC)/ui.c
OBJECTS = $(BUILD)/main.o $(BUILD)/jeu.o $(BUILD)/creation.o $(BUILD)/ui.o
TARGET  = memorix

# Cible par defaut : construire le jeu
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD)/%.o: $(SRC)/%.c | $(BUILD)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD):
	mkdir -p $(BUILD)

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean
