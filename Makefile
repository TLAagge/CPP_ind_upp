# Makefile for Door Access Card System

CC = C:/msys64/mingw64/bin/gcc.exe
CFLAGS = -Wall -Wextra -std=c11
TARGET = access_system.exe
SOURCES = main.c SafeInput.c
HEADERS = SafeInput.h

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

clean:
	del /Q $(TARGET) 2>NUL || true

run: $(TARGET)
	$(TARGET)

.PHONY: all clean run
