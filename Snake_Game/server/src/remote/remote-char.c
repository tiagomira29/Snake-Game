#include "remote-char.h"

// initializes display message
void init_display_msg(display_msg *msg, int ch, int pos_x, int pos_y){
    msg->ch = ch;
    msg->pos_x = pos_x;
    msg->pos_y = pos_y;
}

// initializes base message
void init_remote_char_msg(remote_char_t *msg, int id, int msg_type, int ch[10], int number_cr, direction_t direction, int key){
    msg->id = id;
    msg->msg_type = msg_type;
    for(int i=0; i<10; i++){
        msg->ch[i] = ch[i];
    }
    msg->number_cr = number_cr;
    msg->direction = direction;
    msg->key = key;
}

void init_lizard_msg(remote_lizard *msg, int value, char ch, int key){
    msg->value = value;
    msg->ch = ch;
    msg->key = key;
}

void init_cockroach_msg(remote_cockroach *msg, int number_cr, int ids[10], int key){
    msg->number_cr = number_cr;
    for(int i=0; i<10; i++){
        msg->ids[i] = ids[i];
    }
    msg->key = key;
}

void init_wasp_msg(remote_wasp *msg, int number_wasp, int ids[10], int key){
    msg->number_wasp = number_wasp;
    for(int i=0; i<10; i++){
        msg->ids[i] = ids[i];
    }
    msg->key = key;
}

void sendLizardResponse(remote_lizard *lizard_msg, int value1, int value2, int value3) {
    // Initializes lizard message
    init_lizard_msg(lizard_msg, value1, value2, value3);
    
    /* if (zmq_send(resquester, lizard_msg, sizeof(*lizard_msg), 0) == -1) {
        perror("zmq_send");
    } */
}

void sendCockroachResponse(void *resquester, remote_char_t *m, remote_cockroach *cockroach_msg) {
    // Initializes response message to be sent to the machine client
    init_cockroach_msg(cockroach_msg, m->number_cr, cockroach_msg->ids, cockroach_msg->key);
    
    // Send the response message
    if (zmq_send(resquester, cockroach_msg, sizeof(*cockroach_msg), 0) == -1) {
        perror("zmq_send");
    }
}

void sendWaspResponse(void *resquester, remote_char_t *m, remote_wasp *wasp_msg) {
    // Initializes response message to be sent to the machine client
    init_wasp_msg(wasp_msg, m->number_cr, wasp_msg->ids, wasp_msg->key);
    
    // Send the response message
    if (zmq_send(resquester, wasp_msg, sizeof(*wasp_msg), 0) == -1) {
        perror("zmq_send");
    }
}

void sendLizardErrorMessage(remote_lizard *lizard_msg) {
    // Initializes lizard error message
    init_lizard_msg(lizard_msg, -2, -2, -2);

    /* if (zmq_send(resquester, lizard_msg, sizeof(*lizard_msg), 0) == -1) {
        perror("zmq_send");
    } */
}

void sendDisplayMessage(void *publisher, int ch, int pos_x, int pos_y) {
    display_msg msg;

    // Initializes message to be sent to displays
    init_display_msg(&msg, ch, pos_x, pos_y);
    
    if (zmq_send(publisher, &msg, sizeof(msg), 0) == -1) {
        perror("zmq_send");
    }
}



/* void initializeZMQ(char *ip, main_ports *server_ports) {
    char requester_info[40], publisher_info[40];
    sprintf(requester_info, "tcp://%s:%d", ip, server_ports->requester_port);
    sprintf(publisher_info, "tcp://%s:%d", ip, server_ports->publisher_port);

    server_ports->context = zmq_ctx_new();
    server_ports->requester = zmq_socket(server_ports->context, ZMQ_REP);
    server_ports->publisher = zmq_socket(server_ports->context, ZMQ_PUB);

    int rc = zmq_bind(server_ports->requester, requester_info);
    if (zmq_bind(server_ports->publisher, publisher_info) != 0) {
        perror("zmq_bind failed");
        zmq_close(server_ports->publisher);
        zmq_ctx_destroy(server_ports->context);
        exit(EXIT_FAILURE);  // Modify this according to your error handling strategy
    }
    assert(rc == 0);

    // Your existing curses initialization code...
} */

void initializeZMQ(char *ip, main_ports *mports){

    strcpy(mports->router_ip, ip);
    strcpy(mports->dealer_ip, ip);

    char backend_info[40], frontend_info[40], publisher_info[40], insect_req_info[40];
    sprintf(frontend_info, "tcp://%s:%d", ip, mports->frontend_port);
    sprintf(backend_info, "tcp://%s:%d", ip, mports->backend_port);
    sprintf(publisher_info, "tcp://%s:%d", ip, mports->publisher_port);
    sprintf(insect_req_info, "tcp://%s:%d", ip, mports->insect_req_port);


    mports->context = zmq_ctx_new ();
    mports->frontend = zmq_socket(mports->context, ZMQ_ROUTER);
    mports->backend = zmq_socket(mports->context, ZMQ_DEALER);
    mports->publisher = zmq_socket(mports->context, ZMQ_PUB);
    mports->insect_req = zmq_socket(mports->context, ZMQ_REP);
    // tcp for internet based communication

    if(zmq_bind(mports->frontend, frontend_info) != 0 ){
        perror("zmq_bind failed: frontend (router)");
        zmq_close(mports->frontend);
        zmq_ctx_destroy(mports->context);
        exit(EXIT_FAILURE);
    }

    if(zmq_bind(mports->backend, backend_info) != 0){
        perror("zmq_bind failed: backend (dealer)");
        zmq_close(mports->frontend);
        zmq_close(mports->backend);
        zmq_ctx_destroy(mports->context);
        exit(EXIT_FAILURE);
    }

    if(zmq_bind(mports->publisher, publisher_info) != 0){
        perror("zmq_bind failed: publisher");
        zmq_close(mports->publisher);
        zmq_ctx_destroy(mports->context);
        exit(EXIT_FAILURE);
    }

    int rc = zmq_bind(mports->insect_req, insect_req_info);
    assert(rc == 0);
    // ipc for communication in the same computer
    // zmq_bind (mports->frontend, "ipc://fifo-pipe-front-end");
    // zmq_bind (mports->backend, "inproc://back-end");
}