#include "server.h"

int main(int argc, char *argv[]) {
    char ip[15] = "127.0.0.1";
    main_ports server_ports;
    int n_lizards = 26;
    int n_insects = 300;
    lizard_info_t lizard[n_lizards];
    croach_info_t cockroaches[n_insects];
    wasp_info_t wasps[n_insects];
    WINDOW *my_win;
    display_msg msg;
    void *resquester, *publisher, *context;
    void *lock_pos[30][30];
    
    server_ports.insect_req_port = 5553;
    server_ports.backend_port = 5554;
    server_ports.frontend_port = 5555;
    server_ports.publisher_port = 5556;

    // Parse command-line arguments
    parseCommandLineArguments(argc, argv, ip, &server_ports);

    // Initialize server
    initializeServer(ip, &server_ports, lizard, &n_lizards, cockroaches, &n_insects, &my_win, wasps);

    // Seed random number generator
    srand((unsigned int)time(NULL));

    // Call the message handling loop function
    messageHandlingLoop(my_win, lizard, &n_lizards, cockroaches, &n_insects, &server_ports, wasps);

    // Cleanup and exit curses mode
    cleanup(my_win, publisher, resquester, context);

    return 0;
}

