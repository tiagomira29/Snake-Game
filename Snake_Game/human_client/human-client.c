#define _GNU_SOURCE

#include <getopt.h>

#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>  
#include <ctype.h> 
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#include "remote-char.h"
#include "zhelpers.h"

#define WINDOW_SIZE 30

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

typedef struct display_args {
    WINDOW *win;
    void *subscriber;
    void * puller;
    void * sock_recv;
    WINDOW *display_window;
} display_args;

typedef struct client_args {
    WINDOW *win;
    void *requester;
    remote_char_t m;
    WINDOW *display_window;
    void * puller;
} client_args;

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

void delete_line(WINDOW *win, int pos_x){
    int size_of_line = getmaxx(win);

    for(int i=0; i<size_of_line; i++){
        wmove(win, pos_x, i);
        waddch(win, ' ');
    }
    wrefresh(win);
}

void print_string(WINDOW *win, char *str, int pos_x, int pos_y){
    
    int l = strlen(str);

    delete_line(win, pos_x);

    for(int i=0; i<l; i++, pos_y++){
        wmove(win, pos_x, pos_y);
        waddch(win,str[i]);
    }
    wrefresh(win);
}

void *display(void *arg){

    display_msg msg;
    display_args args = *((display_args *) arg);

    while (1)
    {
        pthread_mutex_lock(&mutex2);
        zmq_recv (args.subscriber, &msg, sizeof(msg), 0);
        pthread_mutex_unlock(&mutex2);
        pthread_mutex_lock(&mutex);
        wmove(args.win, msg.pos_x, msg.pos_y);
        waddch(args.win,msg.ch| A_BOLD);
        wrefresh(args.win);
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}

void *display2(void *arg){

    display_msg msg;
    display_args args = *((display_args *) arg);
    /* puller_msg_t msg; */
    while (1)
    {
        
        zmq_recv (args.subscriber, &msg, sizeof(msg), 0);
        /* pthread_mutex_lock(&mutex); */
        
        zmq_send(args.puller, &msg, sizeof(msg), 0);
        /* pthread_mutex_unlock(&mutex); */
    }

    pthread_exit(NULL);
}

void *thread_puller( void *arg ){

    display_args args = *((display_args *) arg);;
    /* puller_msg_t msg; */
    display_msg msg;
    char value_message[40];
    while (1) {
        zmq_recv (args.sock_recv, &msg, sizeof(msg), 0);
        
        /* switch (msg.type)
        {
        case 1:        
            wmove(args.win, msg.pos_x, msg.pos_y);
            waddch(args.win,msg.ch| A_BOLD);
            break;
        case 2:
            wmove(args.win, 1, 0);
            waddch(args.win,msg.value| A_BOLD);
            break;
        } */
        if(msg.pos_x==-1){
            sprintf(value_message, "%d Value", msg.ch);
            wmove(args.display_window, 1, 0);
            print_string(args.display_window, value_message, 1, 0);
            wrefresh(args.display_window);
            wrefresh(args.win);
            continue;
        }
            
        

        wmove(args.win, msg.pos_x, msg.pos_y);
        waddch(args.win,msg.ch| A_BOLD);
        wrefresh(args.win);
        wrefresh(args.display_window);
        box(args.win,0 ,0);
        

    }
    return 0;
}



void *client(void *arg){
    
    client_args args = *((client_args *) arg);
    display_msg msg;
    msg.pos_x=2;
    msg.pos_y=2;
    remote_lizard r;

    

    int key;
    int count=0;
    int pressed_key;
    do
    {   
        
        
        pressed_key = getch();
        
        
        
        switch (pressed_key)
        {
        case KEY_LEFT:        
            args.m.direction = LEFT;
            break;
        case KEY_RIGHT:
            args.m.direction = RIGHT;
            break;
        case KEY_DOWN:
            args.m.direction = DOWN;
            break;
        case KEY_UP:
            args.m.direction = UP;
            break;
        case 'q': case 'Q':
            //disconect message
            args.m.msg_type = 4;
            zmq_send (args.requester, &args.m, sizeof(args.m), 0);
            zmq_recv (args.requester, &r, sizeof(r), 0);
            pthread_exit(NULL);
            break;

        default:
            pressed_key = 'x';
            break;
        }

        //TODO_10
        //send the movement message
        if (pressed_key != 'x'){
            /* pthread_mutex_lock(&mutex); */
            zmq_send (args.requester, &args.m, sizeof(args.m), 0);
            //meter numa thread

            zmq_recv (args.requester, &r, sizeof(r), 0);
            
            msg.ch = r.value;
            msg.pos_x = -1;
            zmq_send(args.puller, &msg, sizeof(msg), 0); 
            count++;
        }
        
        
    }while(pressed_key != 27);

    pthread_exit(NULL);
}

int main(int argc, char *argv[]){

    /**** Argument Handling ****/

    struct option long_options[] = {
        {"ip", required_argument, 0, 'i'},
        {"requester_port", required_argument, 0, 'r'},
        {"help", required_argument, 0, 'h'},
        {0, 0, 0, 0} // Termination element for options
    };
    
    int opt;
    int option_index=0;

    char ip[15] = "127.0.0.1";
    int requester_port = 5555;
    int publisher_port = 5556;

    while( (opt=getopt_long(argc, argv, "hi:r:", long_options, &option_index)) != -1 ){
        int ch;
        switch (opt)
        {
        case 'i':
            strcpy(ip,optarg);
            break;
        case 'r':
            if(stoi(optarg) != -1){//verifica se e um inteiro
                requester_port = stoi(optarg);
            } else {
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
            mvprintw(3,0,"-r or --requester_port, requires an argument and it will be the port used to comunicate with the clients, default value '5555'");

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

    /**** Assign client sockets ****/

    void *context = zmq_ctx_new();
    if (context == NULL) {
        perror("zmq_ctx_new");
        // Handle the error
    }
    void *pusher = zmq_socket (context, ZMQ_PUSH);
    int rc1 = zmq_connect (pusher, "inproc://xxxx");
    assert (rc1 == 0);
    void *sock_recv = zmq_socket (context, ZMQ_PULL);
    int rc = zmq_bind (sock_recv, "inproc://xxxx");
    assert (rc == 0);
    void *requester = zmq_socket(context, ZMQ_REQ);
    if (requester == NULL) {
        perror("zmq_socket");
        // Handle the error
    }

    char requester_info[40]; for(int i=0; i<40; i++) requester_info[i]='\000';
    sprintf(requester_info, "tcp://%s:%d", ip, requester_port);

    int connect_result = zmq_connect(requester, requester_info);
    if (connect_result == -1) {
        perror("zmq_connect");
        // Handle the error
    }

    /**** Assign display sockets ****/
    
    void *display_context = zmq_ctx_new();
    if (display_context == NULL) {
        perror("zmq_ctx_new: display");
    }

    void *display_subscriber = zmq_socket(display_context, ZMQ_SUB);
    if (display_subscriber == NULL) {
        perror("zmq_socket: display socket");
    }

    char publisher_info[40]; for(int i=0; i<40; i++) publisher_info[i]='\000';
    sprintf(publisher_info, "tcp://%s:%d", ip, publisher_port);

    if(zmq_connect(display_subscriber, publisher_info) == -1) {
        perror("zmq_connect: display socket");
    }

    zmq_setsockopt(display_subscriber, ZMQ_SUBSCRIBE, NULL, 0);


    int count=0;
    int key = -2;
    char ch = 'a';
    
    // send connection message
    remote_char_t m;
    remote_lizard r;

    init_remote_char_msg(&m,-1,0,ch,-1,UP, key);

    if (zmq_send(requester, &m, sizeof(m), 0) == -1) {
        perror("zmq_send");
    }

    if (zmq_recv(requester, &r, sizeof(r), 0) == -1) {
        perror("zmq_recv");
    }

    ch = r.ch;
    key = r.key;
    if(key==-2){
        printf("Server is full :(\n");
        return 0;
    }
    init_remote_char_msg(&m,-1,2,r.ch,-1,UP, r.key);

    /**** Start curses mode ****/

	initscr();			/* Start curses mode 		*/
	cbreak();				/* Line buffering disabled	*/
	keypad(stdscr, TRUE);		/* We get F1, F2 etc..		*/
	noecho();			/* Don't echo() while we do getch */

    WINDOW *display_win = newwin(WINDOW_SIZE, WINDOW_SIZE, 2, 0);
    WINDOW *client_win = newwin(2, WINDOW_SIZE, 0, 0);
    box(display_win, 0 , 0);
	
    wrefresh(client_win);
    wrefresh(display_win);
    
    char message_to_print[40];
    sprintf(message_to_print,"Your character is %c", ch);

    print_string(client_win, message_to_print, 0, 0);
    
    /////////////////////////////////////////////////////////
    int print_value = 'v';
    wmove(client_win, 4, 0);
    waddch(client_win,print_value| A_BOLD);
    print_value = 'a';
    wmove(client_win, 5, 0);
    waddch(client_win,print_value| A_BOLD);
    print_value = 'l';
    wmove(client_win, 6, 0);
    waddch(client_win,print_value| A_BOLD);
    print_value = 'u';
    wmove(client_win, 7, 0);
    waddch(client_win,print_value| A_BOLD);
    print_value = 'e';
    wmove(client_win, 8, 0);
    waddch(client_win,print_value| A_BOLD);
    wrefresh(client_win);
    box(display_win, 0 , 0);
    wrefresh(display_win);
    
    /////////////////////////////////////////////////////////


    /**** Create a thread to handle display ****/

    pthread_t thread_display, thread_client, thread_puller1;
    int ret;
    display_args args_d = {display_win, display_subscriber, pusher, sock_recv, client_win};
    client_args args_c = {client_win, requester, m, display_win, pusher};

    ret = pthread_create(&thread_puller1, NULL, thread_puller, (void *) (&args_d));
    if (ret != 0) {
        printf("Failed to create thread\n");
        return 1;
    }
    ret = pthread_create(&thread_display, NULL, display2, (void *) (&args_d));
    if (ret != 0) {
        printf("Failed to create thread\n");
        return 1;
    }

    ret = pthread_create(&thread_client, NULL, client, (void *) (&args_c));
    if (ret != 0) {
        printf("Failed to create thread\n");
        return 1;
    }

    pthread_join(thread_client, NULL);
    pthread_exit(NULL);
    
    exit:
    zmq_send (requester, &m, sizeof(m), 0);
    zmq_recv (requester, &r, sizeof(r), 0);
    
    ret = pthread_cancel(thread_display);
    if (ret != 0) {
        perror("Failed to cancel thread\n");
        return 1;
    }

    refresh();
  	endwin();			/* End curses mode		  */

	return 0;
}