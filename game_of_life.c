#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ROWS 25
#define COLUMNS 80

void draw_start_screen();
void draw_end_screen();

void init_screen();

int get_game_state(int **cur_gen, int **next_gen, int n, int m);
int is_matrices_equal(int **matr_1, int **matr_2, int n, int m);
int sum_of_elements(int **matrix, int n, int m);

void draw_field(int **gen, int n, int m);

int num_around_cell(int **gen, int i, int j);
void get_next_gen(int **gen, int n, int m, int **res);

void free_matrix(int ***matrix, int n);
void init_matrix(int ***matrix, int n, int m);

void read_gen(int ***gen, int n, int m);

void copy_matrix(int **m_1, int **m_2, int n, int m);

void increase_speed(int *speed);
void decrease_speed(int *speed);

void input_controller(int *speed, int *state);

int main() {
    int game_speed = 100000;
    int **cur_gen;
    int **next_gen;
    int game_is_on = 1;

    read_gen(&cur_gen, ROWS, COLUMNS);

    init_matrix(&next_gen, ROWS, COLUMNS);

    init_screen();
    clear();
    draw_start_screen();
    refresh();
    usleep(3000000);

    while (game_is_on) {
        usleep(game_speed);

        input_controller(&game_speed, &game_is_on);
        if (!game_is_on) {
            break;
        }
        get_next_gen(cur_gen, ROWS, COLUMNS, next_gen);
        game_is_on = get_game_state(cur_gen, next_gen, ROWS, COLUMNS);

        clear();
        draw_field(cur_gen, ROWS, COLUMNS);
        refresh();

        copy_matrix(cur_gen, next_gen, ROWS, COLUMNS);
    }

    usleep(2000000);
    endwin();
    draw_end_screen();
    free_matrix(&next_gen, ROWS);
    free_matrix(&cur_gen, ROWS);
}

void init_screen() {
    initscr();
    cbreak();
    raw();
    nodelay(stdscr, TRUE);
    curs_set(0);
    noecho();
}

void read_gen(int ***gen, int n, int m) {
    printf("Enter the first generation(%dx%d):\n", ROWS, COLUMNS);
    char c;
    init_matrix(gen, ROWS, COLUMNS);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= m; j++) {
            c = getchar();
            if (c == 'o') {
                (*gen)[i][j] = 1;
            } else {
                (*gen)[i][j] = 0;
            }
        }
    }
    FILE *tty;
    tty = freopen("/dev/tty", "r", stdin);
    if (tty == NULL) {
        printf("Unable to open terminal for reading");
    }
}

void draw_field(int **gen, int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (gen[i][j] == 0) {
                printw(".");
            } else if (gen[i][j] == 1) {
                printw("o");
            }
            if (i != n) {
                printw(" ");
            }
        }
        printw("\n");
    }
}

void init_matrix(int ***matrix, int n, int m) {
    *matrix = malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        (*matrix)[i] = malloc(m * sizeof(int));
    }
}

void free_matrix(int ***matrix, int n) {
    for (int i = 0; i < n; i++) {
        free((*matrix)[i]);
    }
    free(*matrix);
}

int num_around_cell(int **gen, int i, int j) {
    int num;
    num = gen[(i + ROWS - 1) % ROWS][(j + COLUMNS - 1) % COLUMNS] +
          gen[(i + ROWS - 1) % ROWS][(j + COLUMNS) % COLUMNS] +
          gen[(i + ROWS - 1) % ROWS][(j + COLUMNS + 1) % COLUMNS] +
          gen[(i + ROWS) % ROWS][(j + COLUMNS - 1) % COLUMNS] +
          gen[(i + ROWS) % ROWS][(j + COLUMNS + 1) % COLUMNS] +
          gen[(i + ROWS + 1) % ROWS][(j + COLUMNS - 1) % COLUMNS] +
          gen[(i + ROWS + 1) % ROWS][(j + COLUMNS) % COLUMNS] +
          gen[(i + ROWS + 1) % ROWS][(j + COLUMNS + 1) % COLUMNS];
    return num;
}

void get_next_gen(int **gen, int n, int m, int **res) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            int num = num_around_cell(gen, i, j);
            if (num >= 2 && num <= 3 && gen[i][j] == 1) {
                res[i][j] = 1;
            } else if (num == 3) {
                res[i][j] = 1;
            } else {
                res[i][j] = 0;
            }
        }
    }
}

int get_game_state(int **cur_gen, int **next_gen, int n, int m) {
    int res = 1;
    if (is_matrices_equal(cur_gen, next_gen, n, m) ||
        sum_of_elements(cur_gen, n, m) == 0) {
        res = 0;
    }
    return res;
}

int is_matrices_equal(int **matr_1, int **matr_2, int n, int m) {
    int res = 1;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (matr_1[i][j] != matr_2[i][j]) {
                res = 0;
                break;
            }
        }
        if (!res) {
            break;
        }
    }
    return res;
}

int sum_of_elements(int **matrix, int n, int m) {
    int res = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            res += matrix[i][j];
        }
    }
    return res;
}

void copy_matrix(int **m_1, int **m_2, int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            m_1[i][j] = m_2[i][j];
        }
    }
}

void increase_speed(int *speed) {
    if (*speed > 50000) {
        *speed -= 10000;
    }
}
void decrease_speed(int *speed) {
    if (*speed < 250000) {
        *speed += 10000;
    }
}

void input_controller(int *speed, int *state) {
    char command = getch();
    if (command != ERR) {
        if (command == 'a' || command == 'A') {
            increase_speed(speed);
        } else if (command == 'z' || command == 'Z') {
            decrease_speed(speed);
        } else if (command == 'q' || command == 'Q') {
            *state = 0;
        } else if (command == EOF) {
            *state = 0;
        }
    }
}

void draw_start_screen() {
    printw(
        "  _____   _  _   ___      ___     _     __  __   ___      ___    ___  "
        "   _      ___   ___   ___ \n");
    printw(
        " |_   _| | || | | __|    / __|   /_\\   |  \\/  | | __|    / _ \\  | "
        "__|   | |    |_ _| | __| | __|\n");
    printw(
        "   | |   | __ | | _|    | (_ |  / _ \\  | |\\/| | | _|    | (_) | | "
        "_|    | |__   | |  | _|  | _| \n");
    printw(
        "   |_|   |_||_| |___|    \\___| /_/ \\_\\ |_|  |_| |___|    \\___/  "
        "|_|     |____| |___| |_|   |___|\n");

    printw(
        "     ____                                ____                         "
        "      ____ \n");
    printw(
        "    ||A ||                              ||Z ||                        "
        "     ||Q ||\n");
    printw(
        "    ||__|| - increase simulation speed  ||__|| - decrease simlation "
        "speed  ||__|| - quit game\n");
    printw(
        "    |/__\\|                              |/__\\|                      "
        "       |/__\\|\n");
}

void draw_end_screen() {
    printf("\033[0d\033[2J");
    printf(
        "                    ___     _     __  __   ___      ___   __   __  "
        "___   ___                     \n");
    printf(
        "                   / __|   /_\\   |  \\/  | | __|    / _ \\  \\ \\ / "
        "/ | __| | _ \\                     \n");
    printf(
        "                  | (_ |  / _ \\  | |\\/| | | _|    | (_) |  \\ V /  "
        "| _|  |   /                    \n");
    printf(
        "                   \\___| /_/ \\_\\ |_|  |_| |___|    \\___/    \\_/  "
        " |___| |_|_\\                   \n");
}
