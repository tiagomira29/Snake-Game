#include "server.h"

void removeWasp(WINDOW *my_win, void *publisher, lizard_info_t lizard[], int *n_insects, wasp_info_t *wasp, pthread_mutex_t (*lock_pos)[30]) {
    int pos_x, pos_y;
    pos_x = wasp->pos_x;
    pos_y = wasp->pos_y;
    pthread_mutex_lock(&lock_pos[pos_x][pos_y]);

    // Delete wasp from board
    deleteWaspOldPosition(my_win, publisher, lizard, MAX_LIZARDS, pos_x, pos_y);

    // Delete wasp from memory
    init_wasp(wasp, -1, -10, -10, -1, -2);
    pthread_mutex_unlock(&lock_pos[pos_x][pos_y]);
    (*n_insects)--;
}

void removeWasps(WINDOW *my_win, void *publisher, lizard_info_t lizard[], croach_info_t cockroaches[], int *n_insects, int max_insects, int max_lizards, remote_char_t *m, remote_wasp *wasp_msg, wasp_info_t wasps[], pthread_mutex_t (*lock_pos)[30]) {
    
    int pos_x, pos_y, ch_pos;
    display_msg msg;

    // for each wasp
    for (int i = 0; i < m->number_cr; i++) {
        m->ch;
        m->key;
        //verify its id
        if ((ch_pos = verify_wasp_info(wasps, max_insects, m->ch[i], m->key)) == -1) {
            continue;
        }

        removeWasp(my_win, publisher, lizard, n_insects, &wasps[ch_pos], lock_pos);
        wasp_msg->ids[i] = m->ch[i];

    }
}

void handleRemoveWaspRequest(WINDOW *my_win, remote_char_t *m, croach_info_t cockroaches[], int *n_insects, int max_insects, lizard_info_t lizard[], int max_lizards, main_ports p, wasp_info_t wasps[], pthread_mutex_t (*lock_pos)[30])
{
    remote_wasp wasp_msg;
    display_msg msg;
    int id;
    int pos_x, pos_y;
    int random_key;


    removeWasps(my_win, p.publisher, lizard, cockroaches, n_insects, max_insects, max_lizards, m, &wasp_msg, wasps, lock_pos);

    // Initializes response message to be sent to the machine client
    sendWaspResponse(p.insect_req, m, &wasp_msg);
}

