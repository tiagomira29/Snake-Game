#include "server.h"

#include <stdarg.h>

// prints to a file
int print_to_file(char *filename, const char *format, ...) {
    FILE *fp = fopen(filename, "a");

    if (fp == NULL) {
        printf("Error opening the file.\n");
        return 1;
    }

    va_list args;
    va_start(args, format);
    vfprintf(fp, format, args);
    va_end(args);

    fclose(fp);

    return 0;
}

// prints to the window!!
void print_to_window(WINDOW *win, int x, int y, char character, bool bold){
    if(bold){
        wmove(win, x, y);
        waddch(win,character| A_BOLD);
    } else {
        wmove(win, x, y);
        waddch(win,character);
    }

    
    
}

