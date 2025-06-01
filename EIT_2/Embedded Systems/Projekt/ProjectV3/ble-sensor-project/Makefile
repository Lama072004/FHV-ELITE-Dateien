PROJECT_NAME := ble-sensor-project
PROJECT_VERSION := 1.0

# Compiler and flags
CC := gcc
CFLAGS := -I./main -Wall -Wextra -std=c99

# Source files
SRC := main/ble.c main/heart.c main/main.c

# Output binary
OUTPUT := $(PROJECT_NAME)

# Build rules
all: $(OUTPUT)

$(OUTPUT): $(SRC)
	$(CC) $(CFLAGS) -o $(OUTPUT) $(SRC)

clean:
	rm -f $(OUTPUT)