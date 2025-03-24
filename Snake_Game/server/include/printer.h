#ifndef PRINTER_H
#define PRINTER_H

#include "server.h"

int print_to_file(char *filename, const char *format, ...);

void print_to_window(WINDOW *win, int x, int y, char character, bool bold);

void printHelp();

#endif /* PRINTER_H */