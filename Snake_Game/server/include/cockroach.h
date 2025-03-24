#ifndef COCKROACH_H
#define COCKROACH_H

/* #include "server.h" */
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

typedef struct croach_info_t
{
    int ch;
    int pos_x, pos_y;
    int id;
    bool visible;
    time_t respawnTime;
    int key;
    time_t last_response;
} croach_info_t;


/* **************************
    FUNCTIONS' PROTOTYPES
************************** */

int find_cockroach_free_index(croach_info_t cockroach[], int max_cockroachs);

int verify_cockroach_info(croach_info_t cockroach[], int max_insects, int received_id, int received_key);

int find_cr_info(croach_info_t cockroach[], int max_cr, int id);

int find_cr_pos(croach_info_t cockroach[], int max_cr, int pos_x, int pos_y, bool food);

bool is_visible(croach_info_t cockroach);

void init_cockroach(croach_info_t *cr, int ch, int x, int y, int id, int visibility, int key);

void kill_cockroach(croach_info_t *cockroach);

void respawn(croach_info_t *cockroach);

void check_respawn_all_cr(croach_info_t cockroach[], int max_cr);

int adjustCockroachCount(int *number_cr, int *n_insects, int max_insects);


bool no_visible_cr_pos(croach_info_t cockroaches[], int max_insects, int xpos, int ypos);

void initializeCockroaches(croach_info_t cockroaches[], int *n_insects);

void update_min_response_cockroach(int max_insects, croach_info_t cockroaches[], time_t *min_response_time, int *ch_pos, int *type);

#endif /* COCKROACH_H */