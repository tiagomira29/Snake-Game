#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>  
#include "zhelpers.h"
#include <assert.h>
#include <unistd.h>
#include <time.h>

typedef struct main_ports
{
    int requester_port;
    int publisher_port;
    void *requester;
    void *publisher;
    void *context;
} main_ports;

void printHelp() {
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();

    mvprintw(0, 0, "Available arguments:");
    mvprintw(1, 0, "-h or --help, requires an argument and it will be the server address");
    mvprintw(2, 0, "-i or --ip, requires an argument and it will be the server address, default value '127.0.0.1'");
    mvprintw(3, 0, "-r or --requester_port, requires an argument and it will be the port used to communicate with the clients, default value '5555'");
    mvprintw(4, 0, "-p or --publisher_port, requires an argument and it will be the port used to communicate with the displays, default value '5556'");

    mvprintw(6, 0, "PRESS ENTER TO LEAVE");

    refresh();
    int ch;
    do {
        ch = getch();
    } while (ch != '\n');
    endwin();
}

void parseCommandLineArguments(int argc, char *argv[], char *ip, main_ports *server_ports) {
    struct option long_options[] = {
        {"ip", required_argument, 0, 'i'},
        {"requester_port", required_argument, 0, 'r'},
        {"publisher_port", required_argument, 0, 'p'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0} // Termination element for options
    };

    int opt;
    int option_index = 0;

    while ((opt = getopt_long(argc, argv, "hi:p:r:", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'i':
                strcpy(ip, optarg);
                break;
            case 'r':
                if (atoi(optarg) != -1) // verifica se e um inteiro
                    server_ports->requester_port = atoi(optarg);
                else {
                    perror("Port numbers must be integers");
                    exit(0);
                }
                break;
            case 'p':
                if (atoi(optarg) != -1) // verifica se e um inteiro
                    server_ports->publisher_port = atoi(optarg);
                else {
                    perror("Port numbers must be integers");
                    exit(0);
                }
                break;
            case 'h':
                printHelp();
                exit(0);
                break;
            default:
                printf("-h flag for help");
                break;
        }
    }
}