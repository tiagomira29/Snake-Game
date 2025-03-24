#include "server.h"

void removeLizard(WINDOW *my_win, main_ports p, lizard_info_t lizard[], int *n_lizards, int ch_pos, croach_info_t cockroaches[], int max_insects, wasp_info_t wasps[], pthread_mutex_t (*lock_pos)[30]) {
    int x_print = lizard[ch_pos].pos_x;
    int y_print = lizard[ch_pos].pos_y;
    pthread_mutex_lock(&lock_pos[x_print][y_print]);
    // Delete old head
    print_to_window(my_win, lizard[ch_pos].pos_x, lizard[ch_pos].pos_y, ' ', TRUE);
    sendDisplayMessage(p.publisher, ' ', lizard[ch_pos].pos_x, lizard[ch_pos].pos_y);

    // Delete old points
    deleteOldPoints(my_win, p.publisher, lizard, MAX_LIZARDS, cockroaches, max_insects, ch_pos, wasps);

    // Remove lizard from memory
    init_lizard(&(lizard[ch_pos]), lizard[ch_pos].ch, -2, -2, 0, -1, -2);
    pthread_mutex_unlock(&lock_pos[x_print][y_print]);
    (*n_lizards)--;
}

void handleRemoveLizardRequest(WINDOW *my_win, remote_char_t *m, lizard_info_t lizard[], int *n_lizards, int max_lizards, croach_info_t cockroaches[], int max_insects, main_ports p, wasp_info_t wasps[], remote_lizard *lizard_msg, pthread_mutex_t (*lock_pos)[30])
{
    display_msg msg;
    int ch_pos, value;

    //first verify key
    if ((ch_pos = verify_lizard_info(lizard, 26, m->ch[0], m->key)) == -1)
    {
        sendLizardErrorMessage(lizard_msg);
        return;
    }

    removeLizard(my_win, p, lizard, n_lizards, ch_pos, cockroaches, max_insects, wasps, lock_pos);

    sendLizardErrorMessage(lizard_msg);
    return;
}
