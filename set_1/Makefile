CC=gcc
CFLAGS=-Wall -Wextra -pedantic

SRC=src/repeated_xor.c
OBJ_MAIN=src/repeated_xor.o
OBJ_FUNC=programs/functions.o

TARGET=repeated_xor

all: $(TARGET)

$(TARGET): $(OBJ_MAIN) $(OBJ_FUNC) 
	$(CC) $(OBJ_MAIN) $(OBJ_FUNC) -o $(TARGET)

$(OBJ_FUNC): programs/functions.c programs/functions.h
	$(CC) $(CFLAGS) -c programs/functions.c -o $(OBJ_FUNC)

clean:
	rm -f src/*.o programs/*.o $(TARGET)
