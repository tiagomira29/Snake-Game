#include "server.h"


void generateNewCockroaches(WINDOW *my_win, void *publisher, lizard_info_t lizard[], croach_info_t cockroaches[], int *n_insects, int max_insects, int max_lizards, remote_char_t *m, remote_cockroach *cockroach_msg, wasp_info_t wasps[]) {
    
    int pos_x, pos_y, id;
    cockroach_msg->key = generateRandomKey(); // Random key associated with all cockroaches of given client
    display_msg msg;

    // for each cockroach
    for (int i = 0; i < m->number_cr; i++) {

        findValidPosition(cockroaches, wasps, max_insects, lizard, max_lizards, &pos_x, &pos_y);

        if ((id = find_cockroach_free_index(cockroaches, max_insects)) == -1) {
            // Find a free cockroach slot
            continue;
        }

        // Initializes new cockroach values
        init_cockroach(&(cockroaches[id]), m->ch[i], pos_x, pos_y, -1, 1, cockroach_msg->key);

        // Initializes new cockroach on the board
        print_to_window(my_win, cockroaches[id].pos_x, cockroaches[id].pos_y, cockroaches[id].ch, TRUE);

        sendDisplayMessage(publisher, cockroaches[id].ch, cockroaches[id].pos_x, cockroaches[id].pos_y);

        // saves the id in the msg to respond to client
        cockroach_msg->ids[i] = cockroaches[id].id;

        (*n_insects)++;
    }
}

// Function to handle the error when m->number_cr is less than or equal to 0
void handleCockroachCountError(void *resquester, remote_cockroach *cockroach_msg) {
    // Initialize the cockroach_msg structure with error values
    for (int i = 0; i < 10; i++) {
        cockroach_msg->ids[i] = -2;
    }

    init_cockroach_msg(cockroach_msg, -2, cockroach_msg->ids, -2);

    // Send the error message using zmq_send
    if (zmq_send(resquester, cockroach_msg, sizeof(*cockroach_msg), 0) == -1) {
        perror("zmq_send"); // Handle the send error if needed
    }
}
void handleConnectCockroachRequest(WINDOW *my_win, remote_char_t *m, croach_info_t cockroaches[], int *n_insects, int max_insects, lizard_info_t lizard[], int max_lizards, main_ports p, wasp_info_t wasps[])
{
    
    remote_cockroach cockroach_msg;
    display_msg msg;
    int id;
    int pos_x, pos_y;
    int random_key;
    // Certify there is still space for more cockroaches
    adjustCockroachCount(&m->number_cr, n_insects, max_insects);

    // Check if m->number_cr is less than or equal to 0
    if (m->number_cr <= 0) {
        handleCockroachCountError(p.insect_req, &cockroach_msg);
        return;
    }

    generateNewCockroaches(my_win, p.publisher, lizard, cockroaches, n_insects, max_insects, max_lizards, m, &cockroach_msg, wasps);

    // Initializes response message to be sent to the machine client
    sendCockroachResponse(p.insect_req, m, &cockroach_msg);
}