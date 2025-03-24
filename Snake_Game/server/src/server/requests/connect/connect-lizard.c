#include "server.h"

void handleConnectLizardRequest(WINDOW *my_win, remote_char_t *m, lizard_info_t lizard[], int *n_lizards, int max_lizards, main_ports *p, croach_info_t cockroaches[], int max_insects, wasp_info_t wasps[], remote_lizard *lizard_msg){
    


    int id;
    display_msg msg;
    int pos_x, pos_y;


    // Find the first free lizard slot
    if (find_lizard_free_index(lizard, max_lizards, &id) == -1) {
        sendLizardErrorMessage(lizard_msg);
        
        return;
    }
    // Find a free position on the board
    findValidPosition(cockroaches, wasps, max_insects, lizard, max_lizards, &pos_x, &pos_y);

    // Initialize new lizard values
    init_lizard(&(lizard[id]), -1, pos_x, pos_y, 0, random_dir(), generateRandomKey());

    print_to_window(my_win, lizard[id].pos_x, lizard[id].pos_y, lizard[id].ch, TRUE);

    // mutex
    sendDisplayMessage(p->publisher, cockroaches[id].ch, cockroaches[id].pos_x, cockroaches[id].pos_y);

    sendLizardResponse(lizard_msg, 0, (char)lizard[id].ch, lizard[id].key);

    (*n_lizards)++;

}   

