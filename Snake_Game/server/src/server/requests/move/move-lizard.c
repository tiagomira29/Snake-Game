#include "server.h"

void handleMoveLizardRequest(WINDOW *my_win, remote_char_t *m, lizard_info_t lizard[], int n_lizards, int max_lizards, croach_info_t cockroaches[], int max_insects, main_ports p, wasp_info_t wasps[], remote_lizard *lizard_msg, pthread_mutex_t (*lock_pos)[30]){
    int ch_pos, value, x_print, y_print, id;
    display_msg msg;


    //first verify identity
    if ((ch_pos = verify_lizard_info(lizard, 26, m->ch[0], m->key)) == -1)
    {
        sendLizardErrorMessage(lizard_msg);
        return;
    }

    if (lizard[ch_pos].value<0){ // if lizard is dead
        handleDeathLizard(my_win, m, lizard, n_lizards, max_lizards, cockroaches, max_insects, p, wasps, ch_pos, lizard_msg);
        return;
    }
    x_print = lizard[ch_pos].pos_x;
    y_print = lizard[ch_pos].pos_y;
    new_position(&x_print, &y_print, m->direction);
    pthread_mutex_lock(&lock_pos[x_print][y_print]);
    if (updateLizardValueAndCheckCollision(lizard, max_lizards, cockroaches, max_insects, ch_pos, m, wasps, my_win, p.publisher)) {
        // it found a lizard in new position, return new value
        pthread_mutex_unlock(&lock_pos[x_print][y_print]);
        sendLizardResponse(lizard_msg, lizard[ch_pos].value, (char)lizard[ch_pos].ch, lizard[ch_pos].key);
        return;
    }

    //delete old head
    print_to_window(my_win, lizard[ch_pos].pos_x, lizard[ch_pos].pos_y, ' ', TRUE);
    sendDisplayMessage(p.publisher, ' ', lizard[ch_pos].pos_x, lizard[ch_pos].pos_y);

    deleteOldPoints(my_win, p.publisher, lizard, max_lizards, cockroaches, max_insects, ch_pos, wasps);
    //update position
    new_position(&lizard[ch_pos].pos_x, &lizard[ch_pos].pos_y, m->direction);

    init_lizard(&(lizard[ch_pos]), -1, -1, -1, -1, (int)m->direction, -1);
    pthread_mutex_unlock(&lock_pos[x_print][y_print]);
    //write new head
    print_to_window(my_win, lizard[ch_pos].pos_x, lizard[ch_pos].pos_y, lizard[ch_pos].ch, TRUE);
    sendDisplayMessage(p.publisher, lizard[ch_pos].ch, lizard[ch_pos].pos_x, lizard[ch_pos].pos_y);

    writeNewPoints(my_win, p.publisher, lizard, max_lizards, cockroaches, max_insects, ch_pos, wasps);

    sendLizardResponse(lizard_msg, lizard[ch_pos].value, (char)lizard[ch_pos].ch, lizard[ch_pos].key);
}

void handleDeathLizard(WINDOW *my_win, remote_char_t *m, lizard_info_t lizard[], int n_lizards, int max_lizards, croach_info_t cockroaches[], int max_insects, main_ports p, wasp_info_t wasps[], int ch_pos, remote_lizard *lizard_msg){
    int new_x = lizard[ch_pos].pos_x;
    int new_y = lizard[ch_pos].pos_y;
    new_position(&new_x, &new_y, m->direction);
    //se tiver um animal na nova posiçao retorna
    if(!noVisibleAnimal(lizard, max_lizards, cockroaches, max_insects, new_x, new_y, wasps)) {
        sendLizardResponse(lizard_msg, lizard[ch_pos].value, (char)lizard[ch_pos].ch, lizard[ch_pos].key);    
        return;
    }
    int old_x = lizard[ch_pos].pos_x;
    int old_y = lizard[ch_pos].pos_y;

    init_lizard(&(lizard[ch_pos]), -1, new_x, new_y, -1, (int)m->direction, -1);
    if(noVisibleAnimal(lizard, max_lizards, cockroaches, max_insects, old_x, old_y, wasps)) {
        deleteDeadLizardOldPosition(my_win, p.publisher, lizard, max_lizards, old_x, old_y);
    }



    print_to_window(my_win, lizard[ch_pos].pos_x, lizard[ch_pos].pos_y, lizard[ch_pos].ch, TRUE);
    sendDisplayMessage(p.publisher, lizard[ch_pos].ch, lizard[ch_pos].pos_x, lizard[ch_pos].pos_y);
    
    sendLizardResponse(lizard_msg, lizard[ch_pos].value, (char)lizard[ch_pos].ch, lizard[ch_pos].key);
}

int updateLizardValueAndCheckCollision(lizard_info_t lizard[], int max_lizards, croach_info_t cockroaches[], int max_insects, int ch_pos, remote_char_t *m, wasp_info_t wasps[], WINDOW *mywin, void *publisher) {
    int value;
    int x_pos = lizard[ch_pos].pos_x;
    int y_pos = lizard[ch_pos].pos_y;
    new_position(&x_pos, &y_pos, m->direction);

    //if it finds a lizard, update value and return 1, meaning it cant move there
    if ((value = find_lizard_pos(lizard, max_lizards, x_pos, y_pos, FALSE)) != -1) {
        if(lizard[value].value > 0){
            lizard[value].value = (lizard[value].value + lizard[ch_pos].value) / 2;
            lizard[ch_pos].value = lizard[value].value;
        }
        return 1;
    }
    //if it finds a wasp, update value and return 1, meaning it cant move there
    if ((find_wasp_pos(wasps, max_insects, x_pos, y_pos)) != -1) {
        lizard[ch_pos].value -= 10;
        if(lizard[ch_pos].value < 0){
            deleteOldPoints(mywin, publisher, lizard, max_lizards, cockroaches, max_insects, ch_pos, wasps);
        }
        return 1;
    } 
    //if it finds a cr, update value and kill cr
    if ((value = find_cr_pos(cockroaches, max_insects, x_pos, y_pos, FALSE)) != -1) {
        if (is_visible(cockroaches[value])) {
            lizard[ch_pos].value += cockroaches[value].ch - '0';
            kill_cockroach(&cockroaches[value]);
        }
    }

    return 0;
}