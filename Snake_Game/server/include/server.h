#ifndef SERVER_H
#define SERVER_H

#define _GNU_SOURCE

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
#include <pthread.h>

#include "remote-char.h"

#include "lizard.h"
#include "wasp.h"
#include "cockroach.h"

#include "randoms.h"
#include "printer.h"

#include "connect.h"
#include "move.h"

#define WINDOW_SIZE 30
#define MAX_LIZARDS 26
#define MAX_INSECTS 300

#define MAX_LIZARD_THREADS 4

#define ON_BORDER(x, y) ((x) >= (WINDOW_SIZE - 1) || (y) >= (WINDOW_SIZE - 1) || (x) <= 0 || (y) <= 0)
#define INTEGER(c) ((c > 0 && c < 6) ? 0 : -1)

typedef struct timeout_t {
    int type;
    int ch_pos;
    int time;
}timeout_t;

typedef struct args_t {
    /*window*/
    WINDOW *my_win;
    /*lizard*/
    lizard_info_t *lizard;
    int *n_lizards;
    /*cockroaches*/
    croach_info_t *cockroaches;
    int *n_insects;
    /*wasps*/
    wasp_info_t *wasps;
    /*remote*/
    main_ports *mports;
    pthread_mutex_t (*lock_pos)[30];
} args_t;

typedef struct thread_info_t {
    args_t *args;

} thread_info_t;

/* **************************
    FUNCTIONS' PROTOTYPES
************************** */

void findValidPosition(croach_info_t cockroaches[], wasp_info_t wasps[], int max_insects, lizard_info_t lizard[], int max_lizards, int *pos_x, int *pos_y);

bool noVisibleAnimal(lizard_info_t lizard[], int max_lizards, croach_info_t cockroaches[], int max_insects, int xpos, int ypos, wasp_info_t wasps[]);

void new_position(int* x, int *y, direction_t direction);

/*
    Functions arguments.c
*/
void parseCommandLineArguments(int argc, char *argv[], char *ip, main_ports *server_ports);

/*
    Functions window-manager.c
*/
void initializeWindow(WINDOW **my_win);

void deleteCockroachOldPosition(WINDOW *my_win, void *publisher, croach_info_t cockroaches[], int max_insects, lizard_info_t lizard[], int max_lizards, int old_x, int old_y);

void deleteWaspOldPosition(WINDOW *my_win, void *publisher, lizard_info_t lizard[], int max_lizards, int old_x, int old_y);

void deleteDeadLizardOldPosition(WINDOW *my_win, void *publisher, lizard_info_t lizard[], int max_lizards, int old_x, int old_y);

void deleteOldPoints(WINDOW *my_win, void *publisher, lizard_info_t lizard[], int max_lizards, croach_info_t cockroaches[], int max_insects, int ch_pos, wasp_info_t wasps[]);

void writeNewPoints(WINDOW *my_win, void *publisher, lizard_info_t lizard[], int max_lizards, croach_info_t cockroaches[], int max_insects, int ch_pos, wasp_info_t wasps[]);

void cleanup(WINDOW *my_win, void *publisher, void *resquester, void *context);

/*
    Generate
*/

void generateNewCockroaches(WINDOW *my_win, void *publisher, lizard_info_t lizard[], croach_info_t cockroaches[], int *n_insects, int max_insects, int max_lizards, remote_char_t *m, remote_cockroach *cockroach_msg, wasp_info_t wasps[]);

void generateNewWasp(WINDOW *my_win, void *publisher, lizard_info_t lizard[], croach_info_t cockroaches[], wasp_info_t wasps[], int *n_insects, int max_insects, int max_lizards, remote_char_t *m, remote_wasp *wasp_msg);
/*
    Initialize
*/
args_t *init_args(WINDOW *my_win, lizard_info_t *lizard, int *n_lizards, croach_info_t *cockroaches, int *n_insects, main_ports *mports, wasp_info_t *wasps, pthread_mutex_t (*lock_pos)[30]);
void initializeServer(char *ip, main_ports *mports, lizard_info_t lizard[], int *n_lizards, croach_info_t cockroaches[], int *n_insects, WINDOW **my_win, wasp_info_t wasps[]);
void messageHandlingLoop(WINDOW *my_win, lizard_info_t *lizard, int *n_lizards, croach_info_t *cockroaches, int *n_insects, main_ports *server_ports, wasp_info_t wasps[]);

void handleConnectCockroachRequest(WINDOW *my_win, remote_char_t *m, croach_info_t cockroaches[], int *n_insects, int max_insects, lizard_info_t lizard[], int max_lizards, main_ports p, wasp_info_t wasps[]);
void handleConnectLizardRequest(WINDOW *my_win, remote_char_t *m, lizard_info_t lizard[], int *n_lizards, int max_lizards, main_ports *p, croach_info_t cockroaches[], int max_insects, wasp_info_t wasps[], remote_lizard *lizard_msg);
void handleConnectWaspRequest(WINDOW *my_win, remote_char_t *m, croach_info_t cockroaches[], wasp_info_t wasps[], int *n_insects, int max_insects, lizard_info_t lizard[], int max_lizards, main_ports p);

void handleMoveCockroachRequest(WINDOW *my_win, remote_char_t *m, croach_info_t cockroaches[], int max_insects, lizard_info_t lizard[], int max_lizards, main_ports p, wasp_info_t wasps[], pthread_mutex_t (*lock_pos)[30]);
void handleMoveLizardRequest(WINDOW *my_win, remote_char_t *m, lizard_info_t lizard[], int n_lizards, int max_lizards, croach_info_t cockroaches[], int max_insects, main_ports p, wasp_info_t wasps[], remote_lizard *lizard_msg, pthread_mutex_t (*lock_pos)[30]);
void handleMoveWaspRequest(WINDOW *my_win, remote_char_t *m, croach_info_t cockroaches[], int max_insects, lizard_info_t lizard[], int max_lizards, main_ports p, wasp_info_t wasps[], pthread_mutex_t (*lock_pos)[30]);

void handleRemoveCockroachRequest(WINDOW *my_win, remote_char_t *m, croach_info_t cockroaches[], int *n_insects, int max_insects, lizard_info_t lizard[], int max_lizards, main_ports p, wasp_info_t wasps[], pthread_mutex_t (*lock_pos)[30]);
void handleRemoveLizardRequest(WINDOW *my_win, remote_char_t *m, lizard_info_t lizard[], int *n_lizards, int max_lizards, croach_info_t cockroaches[], int max_insects, main_ports p, wasp_info_t wasps[], remote_lizard *lizard_msg, pthread_mutex_t (*lock_pos)[30]);
void handleRemoveWaspRequest(WINDOW *my_win, remote_char_t *m, croach_info_t cockroaches[], int *n_insects, int max_insects, lizard_info_t lizard[], int max_lizards, main_ports p, wasp_info_t wasps[], pthread_mutex_t (*lock_pos)[30]);

void removeCockroach(WINDOW *my_win, void *publisher, lizard_info_t lizard[], croach_info_t cockroaches[], int *n_insects, int max_insects, int max_lizards, wasp_info_t wasps[], int ch_pos, pthread_mutex_t (*lock_pos)[30]);
void removeLizard(WINDOW *my_win, main_ports p, lizard_info_t lizard[], int *n_lizards, int ch_pos, croach_info_t cockroaches[], int max_insects, wasp_info_t wasps[], pthread_mutex_t (*lock_pos)[30]);
void removeWasp(WINDOW *my_win, void *publisher, lizard_info_t lizard[], int *n_insects, wasp_info_t *wasp, pthread_mutex_t (*lock_pos)[30]);

int updateLizardValueAndCheckCollision(lizard_info_t lizard[], int max_lizards, croach_info_t cockroaches[], int max_insects, int ch_pos, remote_char_t *m, wasp_info_t wasps[], WINDOW *mywin, void *publisher);

void handleDeathLizard(WINDOW *my_win, remote_char_t *m, lizard_info_t lizard[], int n_lizards, int max_lizards, croach_info_t cockroaches[], int max_insects, main_ports p, wasp_info_t wasps[], int ch_pos, remote_lizard *lizard_msg);

/*
    Threads
*/
void *timeOutThread(void *arguments);
void *LizardThread(void *arguments);
void *cockroachWaspThread(void *arguments);
int find_min_response_entity(lizard_info_t lizards[], int max_lizards,
                              croach_info_t cockroaches[], int max_insects,
                              wasp_info_t wasps[],
                              int *ch_pos, int *type);

#endif /* SERVER_H */
/*
    Remove
*/
/* void removeCockroaches(WINDOW *my_win, void *publisher, lizard_info_t lizard[], croach_info_t cockroaches[], int *n_insects, int max_insects, int max_lizards, remote_char_t *m, remote_cockroach *cockroach_msg, wasp_info_t wasps[]) {
 */