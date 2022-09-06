//unix only

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void delay(int milliseconds);
void print_segment(int num, int index);
void print_clock();
void convert_to_time_arr();

char numbers[10][5][6] = {
    {{"######"},
     {"#    #"},
     {"#````#"},
     {"#    #"},
     {"######"}},
    
    {{"`````#"},
     {"`    #"},
     {"`````#"},
     {"`    #"},
     {"`````#"},},

    {{"######"},
     {"`    #"},
     {"######"},
     {"#    `"},
     {"######"},},

    {{"######"},
     {"`    #"},
     {"######"},
     {"`    #"},
     {"######"},},

    {{"#````#"},
     {"#    #"},
     {"######"},
     {"`    #"},
     {"`````#"},},

    {{"######"},
     {"#    `"},
     {"######"},
     {"`    #"},
     {"######"},},

    {{"######"},
     {"#    `"},
     {"######"},
     {"#    #"},
     {"######"},},

    {{"######"},
     {"`    #"},
     {"`````#"},
     {"`    #"},
     {"`````#"},},

    {{"######"},
     {"#    #"},
     {"######"},
     {"#    #"},
     {"######"},},

    {{"######"},
     {"#    #"},
     {"######"},
     {"`    #"},
     {"######"},}
};

int time_arr[6];

time_t rawtime;
struct tm * timeinfo;

int main() {
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    convert_to_time_arr();

    while (1) {
        system("clear");
        print_clock();

        time(&rawtime);
        timeinfo = localtime(&rawtime);
        convert_to_time_arr();
        delay(1000);
    }

    return 0;
}

void delay(int milliseconds) {
    long pause;
    clock_t now, then;

    pause = milliseconds * (CLOCKS_PER_SEC / 1000);
    now = then = clock();
    while ((now - then) < pause) {
        now = clock();
    }
}

void print_segment(int num, int index) {
    for (int j = 0; j < 6; j++) {
        printf("%c",  numbers[num][index][j]);
    }
}

void print_clock() {
    printf("//");
    for (int i = 0; i < 47; i++) {
        printf("%c", '=');
    }
    printf("\\\\\n");

    for (int i = 0; i < 5; i++) {
        printf("|| ");
        print_segment(time_arr[0], i);
        printf(" ");
        print_segment(time_arr[1], i);
        printf(" | ");
        print_segment(time_arr[2], i);
        printf(" ");
        print_segment(time_arr[3], i);
        printf(" | ");
        print_segment(time_arr[4], i);
        printf(" ");
        print_segment(time_arr[5], i);
        printf(" ||\n");
    }

    printf("\\\\");
    for (int i = 0; i < 47; i++) {
        printf("%c", '=');
    }
    printf("//\n");
}

void convert_to_time_arr() {
    time_arr[0] = timeinfo -> tm_hour / 10 % 10;
    time_arr[1] = timeinfo -> tm_hour % 10;
    time_arr[2] = timeinfo -> tm_min / 10 % 10;
    time_arr[3] = timeinfo -> tm_min % 10;
    time_arr[4] = timeinfo -> tm_sec / 10 % 10;
    time_arr[5] = timeinfo -> tm_sec % 10;
}