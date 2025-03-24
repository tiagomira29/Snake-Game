#include "server.h"


void generateNewWasp(WINDOW *my_win, void *publisher, lizard_info_t lizard[], croach_info_t cockroaches[], wasp_info_t wasps[], int *n_insects, int max_insects, int max_lizards, remote_char_t *m, remote_wasp *wasp_msg) {
    
    int pos_x, pos_y, id;
    wasp_msg->key = generateRandomKey(); // Random key associated with all wasps of given client
    display_msg msg;

    // for each wasp
    for (int i = 0; i < m->number_cr; i++) {

        findValidPosition(cockroaches, wasps, max_insects, lizard, max_lizards, &pos_x, &pos_y);

        if ((id = find_wasp_free_index(wasps, max_insects)) == -1) {
            // Find a free wasp slot
            continue;
        }

        // Initializes new wasp values
        init_wasp(&(wasps[id]), m->ch[i], pos_x, pos_y, -1, wasp_msg->key);

        // Initializes new wasp on the board
        print_to_window(my_win, wasps[id].pos_x, wasps[id].pos_y, '#', TRUE);

        sendDisplayMessage(publisher, '#', wasps[id].pos_x, wasps[id].pos_y);

        // saves the id in the msg to respond to client
        wasp_msg->ids[i] = wasps[id].id;

        (*n_insects)++;
    }
}

// Function to handle the error when m->number_wasps is less than or equal to 0
void handleWaspCountError(void *resquester, remote_wasp *wasp_msg) {
    // Initialize the cockroach_msg structure with error values
    for (int i = 0; i < 10; i++) {
        wasp_msg->ids[i] = -2;
    }

    init_wasp_msg(wasp_msg, -2, wasp_msg->ids, -2);

    // Send the error message using zmq_send
    if (zmq_send(resquester, wasp_msg, sizeof(*wasp_msg), 0) == -1) {
        perror("zmq_send"); // Handle the send error if needed
    }
}

void handleConnectWaspRequest(WINDOW *my_win, remote_char_t *m, croach_info_t cockroaches[], wasp_info_t wasps[], int *n_insects, int max_insects, lizard_info_t lizard[], int max_lizards, main_ports p) {
    remote_wasp wasp_msg;
    display_msg msg;
    int id;
    int pos_x, pos_y;
    int random_key;

    // Certify there is still space for more cockroaches
    adjustWaspCount(&m->number_cr, n_insects, max_insects);

    // Check if m->number_cr is less than or equal to 0
    if (m->number_cr <= 0) {
        handleWaspCountError(p.insect_req, &wasp_msg);
        return;
    }

    generateNewWasp(my_win, p.publisher, lizard, cockroaches, wasps, n_insects, max_insects, max_lizards, m, &wasp_msg);

    // Initializes response message to be sent to the machine client
    sendWaspResponse(p.insect_req, m, &wasp_msg);
}