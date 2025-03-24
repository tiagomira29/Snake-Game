#include "server.h"

void removeCockroach(WINDOW *my_win, void *publisher, lizard_info_t lizard[], croach_info_t cockroaches[], int *n_insects, int max_insects, int max_lizards, wasp_info_t wasps[], int ch_pos, pthread_mutex_t (*lock_pos)[30]) {

    int old_x = cockroaches[ch_pos].pos_x;
    int old_y = cockroaches[ch_pos].pos_y;
    //para o mutex.....................
    pthread_mutex_lock(&lock_pos[old_x][old_y]);
    //para o mutex.....................
    
    //se for visivel remove do board
    if (is_visible(cockroaches[ch_pos]))
    {
        
        init_cockroach(&(cockroaches[ch_pos]), -10, -10, -1, -1, -2, -2);
        // Delete cockroach from board
        // deleteCockroachOldPosition(my_win, publisher, cockroaches, max_insects, lizard, MAX_LIZARDS, old_x, old_y);
        print_to_window(my_win, old_x, old_y, ' ', TRUE);
        sendDisplayMessage(publisher, ' ', old_x, old_y);
    }

    // Delete cockroach from memory
    init_cockroach(&(cockroaches[ch_pos]), -1000000, -10, -1, -1, -2, -2);
    pthread_mutex_unlock(&lock_pos[old_x][old_y]);
    (*n_insects)--;
}


void removeCockroaches(WINDOW *my_win, void *publisher, lizard_info_t lizard[], croach_info_t cockroaches[], int *n_insects, int max_insects, int max_lizards, remote_char_t *m, remote_cockroach *cockroach_msg, wasp_info_t wasps[], pthread_mutex_t (*lock_pos)[30]) {
    
    int pos_x, pos_y, ch_pos;
    display_msg msg;

    // for each cockroach
    for (int i = 0; i < m->number_cr; i++) {

        // Certify there is still space for more cockroaches

        if ((ch_pos = verify_cockroach_info(cockroaches, max_insects, m->ch[i], m->key)) == -1) {
            continue;
        }



        removeCockroach(my_win, publisher, lizard, cockroaches, n_insects, max_insects, max_lizards, wasps, ch_pos, lock_pos);

        // saves the id in the msg to respond to client
        cockroach_msg->ids[i] = m->ch[i];
    }
}
void handleRemoveCockroachRequest(WINDOW *my_win, remote_char_t *m, croach_info_t cockroaches[], int *n_insects, int max_insects, lizard_info_t lizard[], int max_lizards, main_ports p, wasp_info_t wasps[], pthread_mutex_t (*lock_pos)[30])
{
    remote_cockroach cockroach_msg;
    display_msg msg;
    int id;
    int pos_x, pos_y;
    int random_key;


    removeCockroaches(my_win, p.publisher, lizard, cockroaches, n_insects, max_insects, max_lizards, m, &cockroach_msg, wasps, lock_pos);

    // Initializes response message to be sent to the machine client
    sendCockroachResponse(p.insect_req, m, &cockroach_msg);
}
