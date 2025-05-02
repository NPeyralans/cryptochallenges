CC=gcc
CFLAGS=-Wall -Wextra -pedantic

SRC=src/6.c
OBJ_MAIN=src/6.o
OBJ_FUNC=programs/functions.o

TARGET=6

all: $(TARGET)

$(TARGET): $(OBJ_MAIN) $(OBJ_FUNC) 
	$(CC) $(OBJ_MAIN) $(OBJ_FUNC) -o $(TARGET)

$(OBJ_FUNC): programs/functions.c programs/functions.h
	$(CC) $(CFLAGS) -c programs/functions.c -o $(OBJ_FUNC)

clean:
	rm -f src/*.o programs/*.o $(TARGET)
