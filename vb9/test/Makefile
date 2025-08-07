# Makefile for testing VB9 concepts on Linux

CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -O2
TARGET=test-vb9
SRCDIR=.

all: $(TARGET)

$(TARGET): $(SRCDIR)/test-vb9.c
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCDIR)/test-vb9.c

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean