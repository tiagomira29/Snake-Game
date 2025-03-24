#ifndef REMOTE_CHAR_H
#define REMOTE_CHAR_H

#include "zhelpers.h"

typedef enum direction_t {UP, DOWN, LEFT, RIGHT} direction_t;

typedef struct display_msg
{   
    int ch;
    int pos_x, pos_y;
    /* data */
} display_msg;

typedef struct remote_char_t
{
    int id;
    int msg_type; /* 0 joinlizard 1 join cockroach   2 - movelizard 3 move cockroach */
    char ch[10];
    int number_cr;
    direction_t direction ;
    int key;
    /* data */
} remote_char_t;

typedef struct remote_lizard
{
    int value;
    char ch;
    int key;
} remote_lizard;

typedef struct remote_cockroach
{
    int ids[10], number_cr, key;
} remote_cockroach;

typedef struct remote_wasp
{
    int ids[10], number_wasp, key;
} remote_wasp;

typedef struct ports
{
    int requester_port;
    int publisher_port;
    void *requester;
    void *publisher;
    void *context;
} ports;

typedef struct main_ports {
    void *context;
    void *frontend;
    void *backend;
    void *publisher;
    void *insect_req;
    int frontend_port;
    int backend_port;
    int publisher_port;
    int insect_req_port;

    char router_ip[30];
    char dealer_ip[30];
} main_ports;

/* **************************
    FUNCTIONS' PROTOTYPES
************************** */

void init_display_msg(display_msg *msg, int ch, int pos_x, int pos_y);

void init_remote_char_msg(remote_char_t *msg, int id, int msg_type, int ch[10], int number_cr, direction_t direction, int key);

void init_lizard_msg(remote_lizard *msg, int value, char ch, int key);

void init_cockroach_msg(remote_cockroach *msg, int number_cr, int ids[10], int key);

void init_wasp_msg(remote_wasp *msg, int number_wasp, int ids[10], int key);

void sendLizardResponse(remote_lizard *lizard_msg, int value1, int value2, int value3);

void sendCockroachResponse(void *resquester, remote_char_t *m, remote_cockroach *cockroach_msg);

void sendWaspResponse(void *resquester, remote_char_t *m, remote_wasp *wasp_msg);

void sendLizardErrorMessage(remote_lizard *lizard_msg);

void sendDisplayMessage(void *publisher, int ch, int pos_x, int pos_y);

void initializeZMQ(char *ip, main_ports *mports);

#endif /* REMOTE_CHAR_H */