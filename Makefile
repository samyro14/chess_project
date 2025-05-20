CC = gcc -Wall -Wextra -O2
CFLAGS = -I"C:/msys64/mingw32/include" -Iinclude
LDFLAGS = -L"C:/msys64/mingw32/lib" -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lws2_32
SRC = src/main.c src/moves.c src/board.c src/socket_utils.c src/pawn.c src/knight.c src/bishop.c src/rook.c src/queen.c src/king.c
OBJ= $(SRC:.c=.o)
TARGET= sdl_game.exe
git:
	git add .
	git commit -m "Updated files"
	git push
pull:
	git pull origin main
status:
	git status

build: $(SRC) 
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LDFLAGS) 
run:
	./$(TARGET)
clean:
	rm -f $(OBJ) $(TARGET)
	@echo "Cleaned up object files and executable."