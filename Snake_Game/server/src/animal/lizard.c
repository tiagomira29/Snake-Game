#include "lizard.h"

int find_lizard_info(lizard_info_t lizard[], int max_lizards, int ch){

    for (int i = 0 ; i < max_lizards; i++){
        if(lizard[i].key == -2) continue;
        if(ch == lizard[i].ch){
            return i;
        }
    }
    return -1;
}

int verify_lizard_info(lizard_info_t lizard[], int max_lizards, int received_id, int received_key){

    for (int i = 0 ; i < max_lizards; i++){
        if(lizard[i].key == -2) continue;
        if(received_id == lizard[i].ch && lizard[i].key == received_key){//se tiver o mesmo id e chave vai estar certo
            return i;
        }
    }
    return -1;
}



int find_lizard_pos(lizard_info_t lizard[], int max_lizards, int pos_x, int pos_y, bool head_but){

    for (int i = 0 ; i < max_lizards; i++){
        if(lizard[i].key == -2) continue;
        if(pos_x == lizard[i].pos_x && pos_y == lizard[i].pos_y){
            if(head_but) return lizard[i].value; else return i;
        }
    }
    return -1;
}

int find_lizard_free_index(lizard_info_t lizard[], int max_lizards, int *id) {
    for (int i = 0; i < max_lizards; i++) {
        if (lizard[i].key == -2) {
            *id = i;  // Update the id variable
            return 0;  // Found a free index
        }
    }
    
    return -1;  // No free index found
}

// finds point's lizard
int find_point_lizard(lizard_info_t lizard[], int max_lizards, int pos_x, int pos_y){

    for(int i=0; i<max_lizards; i++){
        if((lizard[i].key == -2) || (lizard[i].value < 0)) continue;
        for(int j=0; j<5; j++){
            if(lizard[i].tail.point[j].x == pos_x && lizard[i].tail.point[j].y == pos_y){
                return i;
            }
        }
    }
    return -1;
}

int find_point_other_lizard(lizard_info_t lizard[], int max_lizards, int pos_x, int pos_y, int ch_pos){

    for(int i=0; i<max_lizards; i++){
        if(lizard[i].key == -2) continue; if(i == ch_pos) continue;
        for(int j=0; j<5; j++){
            if(lizard[i].tail.point[j].x == pos_x && lizard[i].tail.point[j].y == pos_y){
                return i;
            }
        }
    }
}

void atualize_tail(lizard_info_t *lizard){
    for(int i=0; i<5; i++){
        switch (lizard->direction)
        {
        case UP:
            lizard->tail.point[i].x = lizard->pos_x + i+1;
            lizard->tail.point[i].y = lizard->pos_y;
            break;
        case DOWN:
            lizard->tail.point[i].x = lizard->pos_x - i-1;
            lizard->tail.point[i].y = lizard->pos_y;
            break;
        case LEFT:
            lizard->tail.point[i].x = lizard->pos_x;
            lizard->tail.point[i].y = lizard->pos_y + i+1;
            break;
        case RIGHT:
            lizard->tail.point[i].x = lizard->pos_x;
            lizard->tail.point[i].y = lizard->pos_y - i-1;
            break;
        
        default:
            break;
        }
        ;
    }
}

void init_lizard(lizard_info_t *lizard, int ch, int pos_x, int pos_y, int value, int direction, int key){
    if(ch != -1) lizard->ch = ch;
    if(pos_x != -1) lizard->pos_x = pos_x;
    if(pos_y != -1) lizard->pos_y = pos_y;
    if(value != -1) lizard->value = value;
    // 0 - UP, 1 - DOWN, 2 - LEFT, 3 - RIGHT
    if(direction == 0 || direction == 1 || direction == 2 || direction == 3) lizard->direction = (direction_t)direction;
    atualize_tail(lizard);
    if(key != -1) lizard->key = key;
    lizard->last_response = time(NULL) + 5;
}

int point_in_coord(lizard_info_t *lizards, int max_lizards, int x, int y){

    int count = 0;

    for(int i=0; i<max_lizards; i++){
        if((lizards[i].key == -2) || (lizards[i].value<0)) continue;
        for(int j=0; j<5; j++){
            if(lizards[i].tail.point[j].x == x && lizards[i].tail.point[j].y == y){
                count++;
                break;
            }
        }
    }

    return count;
}

void initializeLizards(lizard_info_t lizard[], int *n_lizards) {
    for (int i = 0; i < 26; i++) {
        lizard[i].ch = 'a' + i;
        lizard[i].key = -2;
    }
    *n_lizards = 0;
}

void update_min_response_lizard(int max_lizards, lizard_info_t lizards[], time_t *min_response_time, int *ch_pos, int *type) {
    
    for (int i = 0; i < 26; i++) {
        /* int a;
        if (lizards[i].key != -2){
            a = lizards[i].last_response-time(NULL);
            printf("%ld\n", a);


        } */
        
        if ((lizards[i].key != -2) && ((lizards[i].last_response - time(NULL)) < *min_response_time)) {
            *min_response_time = lizards[i].last_response - time(NULL);
            *ch_pos = i;
            *type = 1; // 1 represents lizard
        }
    }
}
