all: snake

snake: snake.c
	gcc snake.c -o snake -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2

assembly:
	gcc -S snake.c -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2

clean:
	rm -f snake
