#ifndef WASP_H
#define WASP_H

#include <stdlib.h>
#include <time.h>
typedef struct wasp_info_t
{
    int ch;
    int pos_x, pos_y;
    int id;
    int key;
    time_t last_response;
} wasp_info_t;

/* ************************
    FUNCTION PROTOTYPES
************************ */

int find_wasp_info(wasp_info_t wasps[], int max_insects, int id);

int find_wasp_free_index(wasp_info_t wasps[], int max_insects);

int verify_wasp_info(wasp_info_t wasps[], int max_insects, int received_id, int received_key);

int find_wasp_pos(wasp_info_t wasps[], int max_insects, int pos_x, int pos_y);

void init_wasp(wasp_info_t *wasp, int ch, int x, int y, int id, int key);

int adjustWaspCount(int *number_wasps, int *n_insects, int max_insects);

void initializeWasps(wasp_info_t wasps[], int *n_insects);

void initializeWasps(wasp_info_t wasps[], int *n_insects);

void update_min_response_wasp(int max_insects, wasp_info_t wasps[], time_t *min_response_time, int *ch_pos, int *type);


#endif /* WASP_H */