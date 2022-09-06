#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define HEIGHT 20
#define WIDTH 30
#define RACKET_LENGHT 4

enum direction {STOP, LEFTUP, RIGHTUP, LEFTDOWN, RIGHTDOWN};
enum direction dir = STOP;

struct racket {
    unsigned char y;
    unsigned char x;
    unsigned char y_pos[RACKET_LENGHT];
    unsigned short score;
};

struct Ball {
    unsigned char y;
    unsigned char x;
};

struct racket ping = {HEIGHT / 2 - (RACKET_LENGHT / 2), 1, 0, 0};
struct racket pong = {HEIGHT / 2 - (RACKET_LENGHT / 2), WIDTH - 2, 0, 0};

struct Ball ball = {HEIGHT / 2, WIDTH / 2};
unsigned int freq = 0;

#ifdef __unix__
    #include <termios.h>
    #include <unistd.h>
    #include <fcntl.h>

    #define CLOCKY 2
    #define DELAYT 40

    const char clear[] = "clear";

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
            if (dir == STOP) {
                time_t t;
                srand((unsigned) time(&t));
                dir = (rand() % 4) + 1;
            }

            switch (getchar() | 0b00100000) {
                case 'w':
                    if (ping.y > 1) {
                        ping.y--;
                    }
                    break;
                case 's':
                    if (ping.y + RACKET_LENGHT < HEIGHT - 1) {
                        ping.y++;
                    }
                    break;
                case 'i':
                    if (pong.y > 1) {
                        pong.y--;
                    }
                    break;
                case 'k':
                    if (pong.y + RACKET_LENGHT < HEIGHT - 1) {
                        pong.y++;
                    }
                    break;
                default:
                    break;
            }
        }
    }

#elif defined(_WIN32) || defined(WIN32)
    #include <conio.h>

    #define CLOCKY 1
    #define DELAYT 35
    
    const char clear[] = "cls";
    
    void keyboard_input() {
        if (kbhit()) {
            if (dir == STOP) {
                time_t t;
                srand((unsigned) time(&t));
                dir = (rand() % 4) + 1;
            }

            switch (_getch() | 0b00100000) {
                case 'w':
                    if (ping.y > 1) {
                        ping.y--;
                    }
                    break;
                case 's':
                    if (ping.y + RACKET_LENGHT < HEIGHT - 1) {
                        ping.y++;
                    }
                    break;
                case 'i':
                    if (pong.y > 1) {
                        pong.y--;
                    }
                    break;
                case 'k':
                    if (pong.y + RACKET_LENGHT < HEIGHT - 1) {
                        pong.y++;
                    }
                    break;
                default:
                    break;
            }
        }
    }

#endif

void delay(long ms);

unsigned char over = 0;
char field[HEIGHT][WIDTH];

int main() {
    while (!over) {
        freq++;

        if (freq % CLOCKY == 0) {
            unsigned char exit = 0;
            switch (dir) {
                case LEFTUP:
                    ball.y--;
                    ball.x--;
                    break;
                case RIGHTUP:
                    ball.y--;
                    ball.x++;
                    break;
                case LEFTDOWN:
                    ball.y++;
                    ball.x--;
                    break;
                case RIGHTDOWN:
                    ball.y++;
                    ball.x++;
                    break;
                default:
                    break;
            }

            switch (ball.y) {
                case 1:
                    if (dir == LEFTUP) {
                        dir = LEFTDOWN;
                    } else {
                        dir = RIGHTDOWN;
                    }
                    break;
                case HEIGHT - 2:
                    if (dir == LEFTDOWN) {
                        dir = LEFTUP;
                    } else {
                        dir = RIGHTUP;
                    }
                    break;
                default:
                    break;
            }

            if (ball.x == ping.x) {
                pong.score += 10;
                ball.y = HEIGHT / 2;
                ball.x = WIDTH / 2;
                dir = STOP;
            } else if (ball.x == pong.x) {
                ping.score += 10;
                ball.y = HEIGHT / 2;
                ball.x = WIDTH / 2;
                dir = STOP;
            }
        }
        
        keyboard_input();

        for (int i = 0; i < RACKET_LENGHT; i++) {
            ping.y_pos[i] = ping.y + i;
            pong.y_pos[i] = pong.y + i;
        }

        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                unsigned char printed = 0;
                for (int k = 0; k < RACKET_LENGHT; k++) {
                    if (ping.y_pos[k] == i && ping.x == j) {
                        field[i][j] = ']';
                        printed = 1;
                        if (ping.y_pos[k] == ball.y && ping.x + 1 == ball.x) {
                            switch (dir) {
                                case LEFTUP:
                                    dir = RIGHTUP;
                                    break;
                                case LEFTDOWN:
                                    dir = RIGHTDOWN;
                                    break;
                                default:
                                    break;
                            }
                        }
                    } else if (pong.y_pos[k] == i && pong.x == j) {
                        field[i][j] = '[';
                        printed = 1;
                        if (pong.y_pos[k] == ball.y && pong.x - 1 == ball.x) {
                            switch (dir) {
                                case RIGHTUP:
                                    dir = LEFTUP;
                                    break;
                                case RIGHTDOWN:
                                    dir = LEFTDOWN;
                                    break;
                                default:
                                    break;
                            }
                        }
                    }
                }
                if (!printed) {              
                    if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1) {
                        field[i][j] = '#';
                    } else if (ball.y == i && ball.x == j) {
                        field[i][j] = '0';
                    } else {
                        field[i][j] = ' ';
                    }
                }    
            }
        }

        delay(DELAYT);

        system(clear);

        printf("Controls: P1: W, S | P2: I, K\n");
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                printf("%c", field[i][j]);
            }
            printf("\n");
        }
        printf("| P1: %d | P2: %d |\n", ping.score, pong.score);

        if (ping.score >= 100) {
            printf("P1 wins!\n");
            over = 1;
        } else if (pong.score >= 100) {
            printf("P2 wins!\n");
            over = 1;
        }
    }

    return 0;
}

void delay(long ms) {
    long pause;
    clock_t now, then;

    pause = ms * (CLOCKS_PER_SEC / 1000);
    now = then = clock();
    while ((now - then) < pause) {
        now = clock();
    }
}