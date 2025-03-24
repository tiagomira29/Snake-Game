#define _GNU_SOURCE

#include <getopt.h>

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include "zhelpers.h"
#include <assert.h>
#include <unistd.h>
#include <time.h>
#include <ncurses.h>

#include "remote-char.h"

/*
    Utility functions
*/

int integer(char c){

    switch (c){
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
            return 0;
        
        default:
            return -1;
    }

}

time_t smaller_time(){
    return time(NULL);
}

int stoi(char *s){
    int n=0;
    for(int i=0; s[i]!='\000'; i++){
        if(s[i] >= '0' && s[i] <= '9'){
            n *= 10;
            n += (int)(s[i] - '0');
        } else {
            return -1;
        }
    }
    return n;
}

unsigned long long get_time_in_ms() {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts); // Gets the current time
    return (unsigned long long)(ts.tv_sec) * 1000 + (unsigned long long)(ts.tv_nsec) / 1000000;
}
bool received_termination_signal = false;
void handle_termination(int signum) {
    received_termination_signal = true;
}
int main(int argc, char *argv[])
{	 
    signal(SIGINT, handle_termination);
    signal(SIGTERM, handle_termination);
    struct option long_options[] = {
        {"ip", required_argument, 0, 'i'},
        {"requester_port", required_argument, 0, 'r'},
        {"help", required_argument, 0, 'h'},
        {"cockroaches", required_argument, 0, 'n'},
        {0, 0, 0, 0} // Termination element for options
    };
    
    int opt;
    int option_index=0;

    char ip[15] = "127.0.0.1";
    int requester_port = 5553;
    int number_cr = 1;
    bool recieved_number_cr = FALSE;

    while( (opt=getopt_long(argc, argv, "hi:n:r:", long_options, &option_index)) != -1 ){
        int ch;
        switch (opt)
        {
        case 'i':
            strcpy(ip,optarg);
            break;
        case 'n':
            if(stoi(optarg) != -1){//verifica se e um inteiro
                number_cr = stoi(optarg);
                if(number_cr < 1 || number_cr >10){
                    perror("Cockroach number must be between 1 and 10");
                    exit(0);
                }
                recieved_number_cr = TRUE;
            } else {
                perror("Cockroach number must be an integer");
                exit(0);
            }
            break;
        case 'r':
            if(stoi(optarg) != -1)//verifica se e um inteiro
                requester_port = stoi(optarg);
            else{
                perror("Port numbers must be integers");
                exit(0);
            }
            break;
        case 'h':
            initscr();			/* Start curses mode 		*/
            cbreak();				/* Line buffering disabled	*/
            keypad(stdscr, TRUE);		/* We get F1, F2 etc..		*/
            noecho();			/* Don't echo() while we do getch */

            mvprintw(0,0,"Available arguments:");
            mvprintw(1,0,"-h or --help, requires an argument and it will be the server address");
            mvprintw(2,0,"-i or --ip, requires an argument and it will be the server address, default value '127.0.0.1'");
            mvprintw(3,0,"-n or --cockroaches, requires an argument and it will be the number of cockroaches, default value 1, must be between 1 and 10");
            mvprintw(4,0,"-r or --requester_port, requires an argument and it will be the port used to comunicate with the clients, default value '5555'");

            mvprintw(6,0,"PRESS ENTER TO LEAVE");

            refresh();
            do {
                ch = getch();
            } while (ch != '\n');
            endwin();			/* End curses mode		  */
            return 0;
            break;
        default:
            printf("-h flag for help");
            break;
        }
    }

    int fd;
    int ids[10]; for(int i=0; i<10; i++) ids[i] = -1;
    int ch[10]; for(int i=0; i<10; i++) ch[i] = 0;
    time_t timers[10];
    int key;
    int sleep_delay, random_interval = 3;
    remote_char_t message;
    remote_cockroach cr_m;
    direction_t direction;

    void *context = zmq_ctx_new (); // create context
    void *requester = zmq_socket (context, ZMQ_REQ);

    char requester_info[40]; for(int i=0; i<40; i++) requester_info[i]='\000';
    sprintf(requester_info, "tcp://%s:%d", ip, requester_port);

    zmq_connect (requester, requester_info); // connect to server

    if(!recieved_number_cr) number_cr = rand() % 10 + 1;

    for(int i=0; i<number_cr;i++){
        ch[i] = rand() % 5 + '1';
        //printf("%d-%c\t", ch[i], ch[i]);
    }
    
    init_remote_char_msg(&message, -1, 5, ch, number_cr, UP, -5);
    zmq_send (requester, &message, sizeof(message), 0); // send connection message
    zmq_recv (requester, &cr_m, sizeof(cr_m), 0); // recieve confirmation message
    key = cr_m.key;
    printf("number_cr:%d\n", number_cr);
    printf("Succesful entering!\n");
    number_cr = cr_m.number_cr;
    for(int i=0; i<number_cr; i++){
        ids[i] = cr_m.ids[i];
        printf("ids:%d\n", ids[i]);
    }
    printf("%d\n",number_cr);

    // alocates time vector
    //time_t *timers = malloc();

    for(int i=0; i<number_cr; i++){
        timers[i] = time(NULL) + rand() % 3 + 1;
    }

    for(int n=0; 1; n++)
    {
        //usleep(random_tempo - tempo_atual)
        
        for(int i=0; i<number_cr;i++){

            if(timers[i]>time(NULL)){
                continue;
            }
            if (received_termination_signal) {
                // Send Roaches_disconnect message to the server before exiting
                init_remote_char_msg(&message, -1, 8, ids, number_cr, UP, key);
                zmq_send(requester, &message, sizeof(message), 0);

                // Cleanup resources before exiting
                zmq_close(requester);
                zmq_ctx_destroy(context);

                exit(0);
            }
            direction = random()%4; // chooses random direction
            
            init_remote_char_msg(&message, ids[i], 6, ch, number_cr, direction, key); // initializes movement cr_m

            switch (direction)
            {
            case LEFT:
                printf("%d Going Left   \n", i);
                break;
            case RIGHT:
                printf("%d Going Right   \n", i);
                break;
            case DOWN:
                printf("%d Going Down   \n", i);
                break;
            case UP:
                printf("%d Going Up    \n", i);
                break;
            }

            //send the movement cr_m
            zmq_send (requester, &message, sizeof(message), 0);
            zmq_recv (requester, &cr_m, sizeof(cr_m), 0);

            timers[i] = time(NULL) + rand() % 3 + 1;
        }

        
    }

 
	return 0;
}