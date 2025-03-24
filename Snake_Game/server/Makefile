# Compiler and flags
CC = gcc
CFLAGS = -g -lzmq -lcurses -Iinclude -pthread #-Wall

# Directories
SRC_DIR := src
OBJ_DIR := obj
INC_DIR := include

# Find all source files in SRC_DIR recursively
SOURCES := $(shell find $(SRC_DIR) -type f -name "*.c")
# Generate object file names from source files
OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))

# Targets
EXECUTABLE = server

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(foreach obj,$(OBJECTS),$(OBJ_DIR)/$(notdir $(obj))) -o $@ $(CFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) -c $< -o $(OBJ_DIR)/$(patsubst %.c,%.o,$(notdir $<)) $(CFLAGS)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) $(EXECUTABLE)

print-%:
	@echo $($*)
