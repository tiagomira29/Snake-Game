#ifndef MOVE_H
#define MOVE_H

#include "server.h"

/* ************************
    FUNCTION PROTOTYPES
************************ */

/*
    Cockroach movement
*/
// void handleMoveCockroachRequest(WINDOW *my_win, remote_char_t *m, croach_info_t cockroaches[], int max_insects, lizard_info_t lizard[], int max_lizards, main_ports p, wasp_info_t wasps[]);

int verify_cr_move(lizard_info_t lizard[], int max_lizards, int x, int y, remote_char_t *m, wasp_info_t wasps[], int max_insects);

int handleCockroachMoveError(WINDOW *my_win, main_ports p, croach_info_t cockroaches[], int max_insects, lizard_info_t lizard[], int max_lizards, remote_char_t *m, wasp_info_t wasps[]);

/*
    Lizard movement
*/

//void handleMoveLizardRequest(WINDOW *my_win, remote_char_t *m, lizard_info_t lizard[], int n_lizards, int max_lizards, croach_info_t cockroaches[], int max_insects, main_ports p, wasp_info_t wasps[]);

/*
    Wasp movement
*/

int verify_wasp_move(lizard_info_t lizard[], int max_lizards, int x, int y, remote_char_t *m, wasp_info_t wasps[], int max_insects, croach_info_t cockroach[], WINDOW *my_win, main_ports p);

int handleWaspMoveError(WINDOW *my_win, main_ports p, croach_info_t cockroaches[], int max_insects, lizard_info_t lizard[], int max_lizards, remote_char_t *m, wasp_info_t wasps[]);

#endif /* MOVE_H */