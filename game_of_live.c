//----------------------------------------------------//
//                    Game of life                    //
//                    Field 25x80                     //
//    Control: 'c' - clear; 'r' - random generate;    //
//                    'q' - exit;                     //
//    '+' - increase speed; '-' - decrease speed      //
// Compiller: gcc -other_flags game_of_life.c -lncurses//
//         Run: cat *.txt | ./a.out < /dev/tty        //
//----------------------------------------------------//

#include <ncurses.h>
#include <stdio.h>

#define WIDTH 80
#define HEIGHT 25

void random_generate(int beforeLifeCells[WIDTH][HEIGHT], int *next);
void clear_board(int beforeLifeCells[WIDTH][HEIGHT]);
void array_copy(int lifeCells[WIDTH][HEIGHT], int beforeLifeCells[WIDTH][HEIGHT]);
int check(int x, int y, int lifeCells[WIDTH][HEIGHT]);

void display(int lifeCells[WIDTH][HEIGHT]);
void update_life(int lifeCells[WIDTH][HEIGHT], int beforeLifeCells[WIDTH][HEIGHT]);
void handle_input(int ch, int *running, int *speed, int beforeLifeCells[WIDTH][HEIGHT], int *next);
void start_generate(int beforeLifeCells[WIDTH][HEIGHT]);
int randomizer(int *next);

int main() {
    int next = 1;

    int running = 1;
    int speed = 100;
    int lifeCells[WIDTH][HEIGHT];
    int beforeLifeCells[WIDTH][HEIGHT];

    start_generate(beforeLifeCells);
    // if (freopen("/dev/tty", "r", stdin) == NULL) running = 0;

    initscr();
    noecho();
    cbreak();
    curs_set(FALSE);
    timeout(100);

    keypad(stdscr, TRUE);

    while (running) {
        array_copy(lifeCells, beforeLifeCells);
        display(lifeCells);
        update_life(lifeCells, beforeLifeCells);
        handle_input(getch(), &running, &speed, beforeLifeCells, &next);
        napms(speed);
    }

    endwin();
    return 0;
}

void random_generate(int beforeLifeCells[WIDTH][HEIGHT], int *next) {
    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            beforeLifeCells[x][y] = (randomizer(next) % 5 == 0) ? 1 : 0;
        }
    }
}

void start_generate(int beforeLifeCells[WIDTH][HEIGHT]) {
    char c;
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (scanf(" %c", &c) == 1) {
                beforeLifeCells[x][y] = (c == '1') ? 1 : 0;
            }
        }
    }
}

void handle_input(int ch, int *running, int *speed, int beforeLifeCells[WIDTH][HEIGHT], int *next) {
    switch (ch) {
        case 'r':
            random_generate(beforeLifeCells, next);
            break;
        case 'c':
            clear_board(beforeLifeCells);
            break;
        case 'q':
            *running = 0;
            break;
        case '+':
            if (*speed < 1001 && *speed - 100 > 99) {
                *speed -= 100;
            }
            break;
        case '-':
            if (*speed + 100 < 1001 && *speed > 99) {
                *speed += 100;
            }
            break;
        default:
            break;
    }
}

void clear_board(int beforeLifeCells[WIDTH][HEIGHT]) {
    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            beforeLifeCells[x][y] = 0;
        }
    }
}

void array_copy(int lifeCells[WIDTH][HEIGHT], int beforeLifeCells[WIDTH][HEIGHT]) {
    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            lifeCells[x][y] = beforeLifeCells[x][y];
        }
    }
}

int check(int x, int y, int lifeCells[WIDTH][HEIGHT]) {
    int alive = 0;
    alive += lifeCells[(x + WIDTH - 1) % WIDTH][(y + HEIGHT - 1) % HEIGHT];
    alive += lifeCells[(x + WIDTH) % WIDTH][(y + HEIGHT - 1) % HEIGHT];
    alive += lifeCells[(x + WIDTH + 1) % WIDTH][(y + HEIGHT - 1) % HEIGHT];
    alive += lifeCells[(x + WIDTH - 1) % WIDTH][(y + HEIGHT) % HEIGHT];
    alive += lifeCells[(x + WIDTH + 1) % WIDTH][(y + HEIGHT) % HEIGHT];
    alive += lifeCells[(x + WIDTH - 1) % WIDTH][(y + HEIGHT + 1) % HEIGHT];
    alive += lifeCells[(x + WIDTH) % WIDTH][(y + HEIGHT + 1) % HEIGHT];
    alive += lifeCells[(x + WIDTH + 1) % WIDTH][(y + HEIGHT + 1) % HEIGHT];
    return alive;
}

void display(int lifeCells[WIDTH][HEIGHT]) {
    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            if (lifeCells[x][y] == 1) {
                mvprintw(y, x, "*");
            } else {
                mvprintw(y, x, " ");
            }
        }
    }
    refresh();
}

void update_life(int lifeCells[WIDTH][HEIGHT], int beforeLifeCells[WIDTH][HEIGHT]) {
    int alive;
    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            alive = check(x, y, lifeCells);
            if (alive == 3) {
                beforeLifeCells[x][y] = 1;
            } else if (alive == 2 && lifeCells[x][y] == 1) {
                beforeLifeCells[x][y] = 1;
            } else {
                beforeLifeCells[x][y] = 0;
            }
        }
    }
}

int randomizer(int *next) {
    *next = *next * 1103515245;
    return (unsigned int)(*next / 65536) * 2768;
}