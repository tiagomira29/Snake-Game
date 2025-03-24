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

void init_lizard_msg(remote_lizard *msg, int value, char ch){
    msg->value = value;
    msg->ch = ch;
}

void init_cockroach_msg(remote_cockroach *msg, int number_cr, int ids[10], int key){
    msg->number_cr = number_cr;
    for(int i=0; i<10; i++){
        msg->ids[i] = ids[i];
    }
    msg->key = key;
}