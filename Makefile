CC=gcc
CFLAGS=-I"C:/msys64/mingw32/include"
LDFLAGS=-L"C:/msys64/mingw32/lib" -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
SRC=main.c
OBJ=$(SRC:.c=.o)
TARGET=sdl_game.exe

build: $(SRC) 
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LDFLAGS) 
