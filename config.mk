CC = gcc
CFLAGS = -Wall -ansi -pedantic -std=gnu99

LD = ld -r -o

ifeq ($(OS),Windows_NT)
    RM = del /Q
else
    RM = rm -f
endif