#ifndef REMOTE_CHAR_H
#define REMOTE_CHAR_H

typedef enum direction_t {UP, DOWN, LEFT, RIGHT} direction_t;
typedef struct display_msg
{   
    int ch;
    int pos_x, pos_y;
    /* data */
}display_msg;
typedef struct remote_char_t
{
    int id;
    int msg_type; /* 0 joinlizard 1 join cockroach   2 - movelizard 3 move cockroach */
    char ch[10];
    int number_cr;
    direction_t direction ;
    int key;
    /* data */
}remote_char_t;

typedef struct remote_lizard
{
    int value;
    char ch;
    int key;
}remote_lizard;

typedef struct remote_cockroach
{
    int ids[10], number_cr;
}remote_cockroach;


/* **************************
    FUNCTIONS' PROTOTYPES
************************** */

void init_display_msg(display_msg *msg, int ch, int pos_x, int pos_y);

void init_remote_char_msg(remote_char_t *msg, int id, int msg_type, int ch, int number_cr, direction_t direction, int key);

void init_lizard_msg(remote_lizard *msg, int value, char ch, int key);

void init_cockroach_msg(remote_cockroach *msg, int number_cr, int ids[10]);

#endif /* REMOTE_CHAR_H */