# Compilador y flags
CC     = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
INCLUDES = -Iinclude

# Directorios
SRC_DIR    = src
CORE_DIR   = $(SRC_DIR)/core
UTILS_DIR  = $(SRC_DIR)/utils

# Archivos objeto
OBJS = \
    $(SRC_DIR)/main.o \
    $(CORE_DIR)/memory.o \
    $(CORE_DIR)/allocator.o \
    $(CORE_DIR)/blocks.o \
    $(CORE_DIR)/variables.o \
    $(CORE_DIR)/parser.o \
    $(CORE_DIR)/memory_ops.o \
    $(CORE_DIR)/print.o \
    $(UTILS_DIR)/list.o \
    $(UTILS_DIR)/string_utils.o \
    $(UTILS_DIR)/log.o

# Binario final
TARGET = memsim

.PHONY: all build clean

all: build

build: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

# Regla genérica para compilar .c -> .o
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
