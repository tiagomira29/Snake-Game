#include "server.h"


int verify_cr_move(lizard_info_t lizard[], int max_lizards, int x, int y, remote_char_t *m, wasp_info_t wasps[], int max_insects) 
{
    int value;
    int x_pos = x;
    int y_pos = y;
    new_position(&x_pos, &y_pos, m->direction);

    //if it finds a lizard or a wasp, update value and return 0, meaning it cant move there
    return ((find_lizard_pos(lizard, max_lizards, x_pos, y_pos, FALSE) == -1)&&
            (find_wasp_pos(wasps, max_insects, x_pos, y_pos) == -1));
}

int handleCockroachMoveError(WINDOW *my_win, main_ports p, croach_info_t cockroaches[], int max_insects, lizard_info_t lizard[], int max_lizards, remote_char_t *m, wasp_info_t wasps[]) 
{
    remote_cockroach cockroach_msg;
    /* exit(0); */
    // Verify identity
    int ch_pos = verify_cockroach_info(cockroaches, max_insects, m->id, m->key);
    if (ch_pos == -1) {
        init_cockroach_msg(&cockroach_msg, -2, cockroach_msg.ids, -2);
        zmq_send(p.insect_req, &cockroach_msg, sizeof(cockroach_msg), 0);
        return -1; // Error
    }
    //se nao for visivel ou falhar a verificacao do move da erro
    if (!is_visible(cockroaches[ch_pos]) || !verify_cr_move(lizard, max_lizards, cockroaches[ch_pos].pos_x, cockroaches[ch_pos].pos_y, m, wasps, max_insects)) {
        init_cockroach(&(cockroaches[ch_pos]), -1, -1, -1, -1, -1, -1);
        init_cockroach_msg(&cockroach_msg, max_insects, cockroach_msg.ids, -1);
        zmq_send(p.insect_req, &cockroach_msg, sizeof(cockroach_msg), 0);
        return -1; // Error
    }
    return ch_pos; // No error
}

void handleMoveCockroachRequest(WINDOW *my_win, remote_char_t *m, croach_info_t cockroaches[], int max_insects, lizard_info_t lizard[], int max_lizards, main_ports p, wasp_info_t wasps[], pthread_mutex_t (*lock_pos)[30])
{
    remote_cockroach cockroach_msg;
    display_msg msg;
    int ch_pos, old_x, old_y, id, value, x_print, y_print;

    ch_pos = verify_cockroach_info(cockroaches, max_insects, m->id, m->key);
    if (ch_pos == -1) {
        init_cockroach_msg(&cockroach_msg, -2, cockroach_msg.ids, -2);
        zmq_send(p.insect_req, &cockroach_msg, sizeof(cockroach_msg), 0);
        return; // Error
    }
    //para o mutex.....................
    x_print = cockroaches[ch_pos].pos_x;
    y_print = cockroaches[ch_pos].pos_y;
    new_position(&x_print, &y_print, m->direction);
    pthread_mutex_lock(&lock_pos[x_print][y_print]);
    //para o mutex.....................

    //verify it can move, has right key, and is visible
    if ((ch_pos = handleCockroachMoveError(my_win, p, cockroaches, max_insects, lizard, MAX_LIZARDS, m, wasps)) == -1) {
        return;
    }
    init_cockroach(&(cockroaches[ch_pos]), -1, -1, -1, -1, -1, -1);

    old_x = cockroaches[ch_pos].pos_x;
    old_y = cockroaches[ch_pos].pos_y;
    new_position(&cockroaches[ch_pos].pos_x, &cockroaches[ch_pos].pos_y, m->direction);
    //depois de atualizar posso dar unlock....
    pthread_mutex_unlock(&lock_pos[x_print][y_print]);


    deleteCockroachOldPosition(my_win, p.publisher, cockroaches, max_insects, lizard, MAX_LIZARDS, old_x, old_y);

    //write new position
    print_to_window(my_win, cockroaches[ch_pos].pos_x, cockroaches[ch_pos].pos_y, cockroaches[ch_pos].ch, TRUE);
    sendDisplayMessage(p.publisher, cockroaches[ch_pos].ch, cockroaches[ch_pos].pos_x, cockroaches[ch_pos].pos_y);
    init_cockroach_msg(&cockroach_msg, max_insects, cockroach_msg.ids, -1);
    zmq_send(p.insect_req, &cockroach_msg, sizeof(cockroach_msg), 0);
}

