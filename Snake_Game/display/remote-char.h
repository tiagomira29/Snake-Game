
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
    char ch; 
    direction_t direction ;
    /* data */
}remote_char_t;

#define FIFO_NAME "/tmp/fifo_snail"
