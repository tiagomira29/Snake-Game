#include "server.h"

void handleMoveWaspRequest(WINDOW *my_win, remote_char_t *m, croach_info_t cockroaches[], int max_insects, lizard_info_t lizard[], int max_lizards, main_ports p, wasp_info_t wasps[], pthread_mutex_t (*lock_pos)[30])
{
    remote_wasp wasp_msg;
    display_msg msg;
    int ch_pos, old_x, old_y, id, value, x_print, y_print;

    ch_pos = verify_wasp_info(wasps, max_insects, m->id, m->key);
    if (ch_pos == -1) {
        
        init_wasp_msg(&wasp_msg, -2, wasp_msg.ids, -2);
        zmq_send(p.insect_req, &wasp_msg, sizeof(wasp_msg), 0);
        return; // Error
    }
    //para o mutex.....................
    x_print = wasps[ch_pos].pos_x;
    y_print = wasps[ch_pos].pos_y;
    new_position(&x_print, &y_print, m->direction);
    pthread_mutex_lock(&lock_pos[x_print][y_print]);
    //para o mutex.....................

    //verify it can move, has right key, and is visible
    if ((ch_pos = handleWaspMoveError(my_win, p, cockroaches, max_insects, lizard, MAX_LIZARDS, m, wasps)) == -1) {
        
        return;
    }


    init_wasp(&wasps[ch_pos], -1, -1, -1, -1, -1);
    old_x = wasps[ch_pos].pos_x;
    old_y = wasps[ch_pos].pos_y;
    //get new position
    new_position(&wasps[ch_pos].pos_x, &wasps[ch_pos].pos_y, m->direction);
    //delete old position
    pthread_mutex_unlock(&lock_pos[x_print][y_print]);
    deleteWaspOldPosition(my_win, p.publisher, lizard, MAX_LIZARDS, old_x, old_y);
    
    //write new position
    print_to_window(my_win, wasps[ch_pos].pos_x, wasps[ch_pos].pos_y, '#', TRUE);
    sendDisplayMessage(p.publisher, '#', wasps[ch_pos].pos_x, wasps[ch_pos].pos_y);

    init_wasp_msg(&wasp_msg, max_insects, wasp_msg.ids, -1);
    
    zmq_send(p.insect_req, &wasp_msg, sizeof(wasp_msg), 0);
}

int verify_wasp_move(lizard_info_t lizard[], int max_lizards, int x, int y, remote_char_t *m, wasp_info_t wasps[], int max_insects, croach_info_t cockroach[], WINDOW *my_win, main_ports p) 
{
    int value;
    int x_pos = x;
    int y_pos = y;
    new_position(&x_pos, &y_pos, m->direction);

    //if it finds a lizard, update lizard value and return 1, meaning it cant move there
    if ((value = find_lizard_pos(lizard, max_lizards, x_pos, y_pos, FALSE)) != -1) {
        lizard[value].value -= 10;
        if(lizard[value].value < 0){
            deleteOldPoints(my_win, p.publisher, lizard, max_lizards, cockroach, max_insects, value, wasps);
        }
        return 1;
    } 
    //if it finds a wasp, return 1, meaning it cant move there
    if ((find_wasp_pos(wasps, max_insects, x_pos, y_pos)) != -1) {
        return 1;
    } 
    //if it finds a cr, return 1, meaning it cant move there
    if ((value = find_cr_pos(cockroach, max_insects, x_pos, y_pos, FALSE)) != -1) {
        return 1;
    }
    return 0;
}

int handleWaspMoveError(WINDOW *my_win, main_ports p, croach_info_t cockroaches[], int max_insects, lizard_info_t lizard[], int max_lizards, remote_char_t *m, wasp_info_t wasps[]) 
{
    
    remote_wasp wasp_msg;

    // Verify identity
    int ch_pos = verify_wasp_info(wasps, max_insects, m->id, m->key);
    if (ch_pos == -1) {
        
        init_wasp_msg(&wasp_msg, -2, wasp_msg.ids, -2);
        zmq_send(p.insect_req, &wasp_msg, sizeof(wasp_msg), 0);
        return -1; // Error
    }
    //se nao for visivel ou falhar a verificacao do move da erro
    if (verify_wasp_move(lizard, max_lizards, wasps[ch_pos].pos_x, wasps[ch_pos].pos_y, m, wasps, max_insects, cockroaches, my_win, p)) {
        
        init_wasp_msg(&wasp_msg, max_insects, wasp_msg.ids, -1);
        zmq_send(p.insect_req, &wasp_msg, sizeof(wasp_msg), 0);
        return -1; // Error
    }

    return ch_pos; // No error
}
