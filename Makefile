CC=gcc
FLAGS=-Wall -Wextra -Werror

all:
	$(CC) $(FLAGS) game_of_life.c -o game_of_life -lncurses

clean:
	rm game_of_life