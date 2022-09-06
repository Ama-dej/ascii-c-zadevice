#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum direction {STOP, LEFT, RIGHT, UP, DOWN};
enum direction dir = STOP;

#ifdef __unix__
    const char clear_command[] = "clear";
    #include <termios.h>
    #include <unistd.h>
    #include <fcntl.h>

    int kbhit(void) {
    struct termios oldt, newt;
        int ch;
        int oldf;

        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

        ch = getchar();

        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        fcntl(STDIN_FILENO, F_SETFL, oldf);

        if (ch != EOF) {
            ungetc(ch, stdin);
            return 1;
        }

        return 0;
    }
    
    void keyboard_input() {
        if (kbhit()) {
            int ch = getchar();
            switch (ch | 0b00100000) {
                case 'a':
                    if (dir == RIGHT) break;
                    dir = LEFT;
                    break;
                case 'd':
                    if (dir == LEFT) break;
                    dir = RIGHT;
                    break;
                case 'w':
                    if (dir == DOWN) break;
                    dir = UP;
                    break;
                case 's':
                    if (dir == UP) break;
                    dir = DOWN;
                    break;
                default:
                    break;
            }
        }
    }

#elif defined(_WIN32) || defined(WIN32)
    const char clear_command[] = "cls";
    #include <conio.h>

    void keyboard_input() {
        if (_kbhit()) {
            int ch = _getch();
            switch (ch | 0b00100000) {
                case 'a':
                    if (dir == RIGHT) break;
                    dir = LEFT;
                    break;
                case 'd':
                    if (dir == LEFT) break;
                    dir = RIGHT;
                    break;
                case 'w':
                    if (dir == DOWN) break;
                    dir = UP;
                    break;
                case 's':
                    if (dir == UP) break;
                    dir = DOWN;
                    break;
                default:
                    break;
            }
        }
    }

#endif

#define WIDTH 20
#define HEIGHT 20

void delay(int ms);

unsigned char over = 0;
short score = 0;
unsigned char fruit_x, fruit_y;

char field[HEIGHT][WIDTH] = {0};
unsigned char available[HEIGHT][WIDTH];

struct snake {
    unsigned char head_x;
    unsigned char head_y;
    unsigned char tail_x[(HEIGHT - 2) * (WIDTH - 2)];
    unsigned char tail_y[(HEIGHT - 2) * (WIDTH - 2)];
};

struct snake snek = {WIDTH / 2, HEIGHT / 2, 0, 0};

int main() {
    fruit_x = snek.head_x;
    fruit_y = snek.head_y - 4;

    while (!over) {
        keyboard_input();

        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1) {
                    available[i][j] == 0;
                } else {
                    available[i][j] = 1;
                }
            }
        }

        int temp_x = snek.head_x;
        int temp_y = snek.head_y;
        int temp_x2, temp_y2;
        for (int i = 0; i < score / 10; i++) {
            temp_x2 = snek.tail_x[i]; temp_y2 = snek.tail_y[i];
            snek.tail_x[i] = temp_x; snek.tail_y[i] = temp_y;
            available[temp_y][temp_x] = 0;
            temp_x = temp_x2; temp_y = temp_y2;
        }

        switch (dir) {
            case LEFT:
                snek.head_x--;
                break;
            case RIGHT:
                snek.head_x++;
                break;
            case UP:
                snek.head_y--;
                break;
            case DOWN:
                snek.head_y++;
                break;
            default:
                break;
        }

        available[snek.head_y][snek.head_x] = 0;

        if ((snek.head_y == 0 || snek.head_y == HEIGHT - 1) || (snek.head_x == 0 || snek.head_x == WIDTH - 1)) {
            over = 1;
        }

        for (int i = 0; i < (score / 10) + 1; i++) {
            if ((snek.head_y == snek.tail_y[i] && snek.head_x == snek.tail_x[i]) && dir != STOP) {
                over = 1;
            }
        }

        if (snek.head_y == fruit_y && snek.head_x == fruit_x) {
            score += 10;
            time_t t;
            srand((unsigned) time(&t));
            fruit_x = (rand() % (WIDTH - 2)) + 1;
            fruit_y = (rand() % (HEIGHT - 2)) + 1;
            while (!available[fruit_y][fruit_x]) {
                fruit_x = (fruit_x + 1) % 20;
                if(!fruit_x) {
                    fruit_y = (fruit_y + 1) % 20;
                }
            }
        }

        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                if ((i == 0 || i == HEIGHT - 1) || (j == 0 || j == WIDTH - 1)) {
                    field[i][j] = '#';
                } else if (i == snek.head_y && j == snek.head_x) {
                    field[i][j] = 'O';
                } else if (i == fruit_y && j == fruit_x) {
                    field[i][j] = 'F';
                } else {
                    int printed = 0;
                    for (int k = 0; k < score / 10; k++) {
                        if (snek.tail_y[k] == i && snek.tail_x[k] == j) {
                            field[i][j] = 'o';
                            printed = 1;
                        }
                    }
                    if (!printed) {
                        field[i][j] = ' ';
                    }
                }
            }
        }

        delay(200);

        system(clear_command);

        printf("Controls: W, A, S, D\n");
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                printf("%c", field[i][j]);
            }
            printf("\n");
        }
        /*
        //izpise matrico mest primernih za hrano
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                printf("%d", available[i][j]);
            }
            printf("\n");
        }
        */
        printf("Score: %d\n", score);
    }
    printf("Game over!\n");

    return 0;
}

void delay(int ms) {
    long pause;
    clock_t now, then;

    pause = ms * (CLOCKS_PER_SEC / 1000);
    now = then = clock();
    while ((now - then) < pause) {
        now = clock();
    }
}