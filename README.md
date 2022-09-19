# Game of Life on C

> The Game of Life, also known simply as Life, is a cellular automaton devised by the British mathematician John Horton Conway in 1970. It is a zero-player game, meaning that its evolution is determined by its initial state, requiring no further input. One interacts with the Game of Life by creating an initial configuration and observing how it evolves. It is Turing complete and can simulate a universal constructor or any other Turing machine. 

The game is written in C using the ncurses library.

**Controls:**

* ***A*** to increase game speed
* ***Z*** to decrease game speed
* ***A*** to quit game

![giphy gif](https://github.com/dyakov-kirill/terminal-game-of-life/blob/ede7af702742b5bf047c764013dd6c92ef2bd8da/res/presentation.gif)

Before compilation you may need to download ncurses

> sudo apt-get install lib32ncurses5-dev

Compilation

> make

Run

> ./game_of_life

or

> ./game_of_life < file_with_generation.txt
