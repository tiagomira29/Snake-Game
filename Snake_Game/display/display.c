#define _GNU_SOURCE

#include <getopt.h>

#include <ncurses.h>
#include "remote-char.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>  
#include <stdlib.h>
#include "zhelpers.h"
#include <assert.h>
#include <unistd.h>
#define WINDOW_SIZE 30

// STEP 1x
typedef struct ch_info_t
{
    int ch;
    int pos_x, pos_y;
} ch_info_t;

direction_t random_direction(){
    return  random()%4;

}

void new_position(int* x, int *y, direction_t direction){
        switch (direction)
        {
        case UP:
            (*x) --;
            if(*x ==0)
                *x = 2;
            break;
        case DOWN:
            (*x) ++;
            if(*x ==WINDOW_SIZE-1)
                *x = WINDOW_SIZE-3;
            break;
        case LEFT:
            (*y) --;
            if(*y ==0)
                *y = 2;
            break;
        case RIGHT:
            (*y) ++;
            if(*y ==WINDOW_SIZE-1)
                *y = WINDOW_SIZE-3;
            break;
        default:
            break;
        }
}

int find_ch_info(ch_info_t char_data[], int n_char, int ch){

    for (int i = 0 ; i < n_char; i++){
        if(ch == char_data[i].ch){
            return i;
        }
    }
    return -1;
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

int main(int argc, char *argv[])
{
    /**** Argument Handling ****/

    /* Arguments expected */
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
    int publisher_port = 5556;

    while( (opt=getopt_long(argc, argv, "hi:n:p:", long_options, &option_index)) != -1 ){
        int ch;
        switch (opt)
        {
        case 'i':
            strcpy(ip,optarg);
            break;
        case 'p':
            if(stoi(optarg) != -1)//verifica se e um inteiro
                publisher_port = stoi(optarg);
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
            mvprintw(3,0,"-p or --publisher_port, requires an argument and it will be the port used to comunicate with the server, default value '5556'");

            mvprintw(5,0,"PRESS ENTER TO LEAVE");

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
    

    int n_chars = 0;
    display_msg msg;

    void *context = zmq_ctx_new ();
    void *subscriber = zmq_socket (context, ZMQ_SUB);

    char publisher_info[40]; for(int i=0; i<40; i++) publisher_info[i]='\000';
    sprintf(publisher_info, "tcp://%s:%d", ip, publisher_port);

    zmq_connect (subscriber, publisher_info);

    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, NULL, 0);
	initscr();
	cbreak();
    keypad(stdscr, TRUE);
	noecho();

    /* creates a window and draws a border */
    WINDOW * my_win = newwin(WINDOW_SIZE, WINDOW_SIZE, 0, 0);
    box(my_win, 0 , 0);	
	wrefresh(my_win);

    int ch;
    int pos_x;
    int pos_y;

    direction_t  direction;
    while (1)
    {

        /* read(fd, &m, sizeof(remote_char_t)); */
        zmq_recv (subscriber, &msg, sizeof(msg), 0);
        /* zmq_send (resquester, &msg, sizeof(msg), 0); */
        /* sleep(5); */
        
        /* draw mark on new position */
        
        wmove(my_win, msg.pos_x, msg.pos_y);
        waddch(my_win,msg.ch| A_BOLD);
        wrefresh(my_win);
    }
  	endwin();			/* End curses mode */

	return 0;
}