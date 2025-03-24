#ifndef LIZARD_H
#define LIZARD_H

#include <stdlib.h>

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
#include <remote-char.h>

typedef struct point
{
    int x;
    int y;
} point;

typedef struct lizard_tail
{
    point point[5];
} lizard_tail;


// STEP 1
typedef struct lizard_info_t
{
    int ch;
    int pos_x, pos_y;
    int value;
    direction_t direction;
    lizard_tail tail;
    int key;
    time_t last_response;
} lizard_info_t;

/* **************************
    FUNCTIONS' PROTOTYPES
************************** */

int find_lizard_info(lizard_info_t lizard[], int n_char, int ch);

int find_lizard_pos(lizard_info_t lizard[], int n_char, int pos_x, int pos_y, bool head_but);

int find_point_lizard(lizard_info_t lizard[], int n_char, int pos_x, int pos_y);

int find_point_other_lizard(lizard_info_t lizard[], int max_lizards, int pos_x, int pos_y, int ch_pos);

int find_lizard_free_index(lizard_info_t lizard[], int max_lizards, int *id);

void atualize_tail(lizard_info_t *lizard);

void init_lizard(lizard_info_t *lizard, int ch, int pos_x, int pos_y, int value, int direction, int key);

int point_in_coord(lizard_info_t *lizards, int n_lizards, int x, int y);

int verify_lizard_info(lizard_info_t lizard[], int n_char, int received_id, int received_key);

void initializeLizards(lizard_info_t lizard[], int *n_lizards);

void update_min_response_lizard(int max_lizards, lizard_info_t lizards[], time_t *min_response_time, int *ch_pos, int *type);
    
#endif /* LIZARD_H */