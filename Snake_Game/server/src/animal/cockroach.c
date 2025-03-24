#include "cockroach.h"

int find_cr_info(croach_info_t cockroach[], int max_cr, int id){

    for (int i = 0 ; i < max_cr; i++){
        if(cockroach[i].key == -2) continue;
        if(id == cockroach[i].id){
            return i;
        }
    }
    return -1;
}

int find_cockroach_free_index(croach_info_t cockroach[], int max_cockroachs){

    for(int i=0; i<max_cockroachs; i++){
        if(cockroach[i].key == -2) return i;
    }

    return -1;
}

int verify_cockroach_info(croach_info_t cockroach[], int max_insects, int received_id, int received_key){
    for (int i = 0 ; i < max_insects; i++){
        
        if(cockroach[i].key == -2) continue;
        if((received_id == cockroach[i].id) && (cockroach[i].key == received_key)){//se tiver o mesmo id e chave vai estar certo
            return i;
        }
    }
    return -1;
}

int find_cr_pos(croach_info_t cockroach[], int max_cr, int pos_x, int pos_y, bool food){

    for (int i = 0 ; i < max_cr; i++){
        if(cockroach[i].key == -2) continue;
        if(pos_x == cockroach[i].pos_x && pos_y == cockroach[i].pos_y){
            if(food) return cockroach[i].ch; else return i;
        }
    }
    return -1;
}

bool is_visible(croach_info_t cockroach){
    return cockroach.visible;
}

void init_cockroach(croach_info_t *cr, int ch, int x, int y, int id, int visibility, int key){
    if(ch != -1) cr->ch = ch;
    if(x != -1) cr->pos_x = x;
    if(y != -1) cr->pos_y = y;
    if(id != -1) cr->id = id;
    if(visibility == 0 || visibility == 1) cr->visible = (bool) visibility;
    if(key != -1) cr->key = key;
    cr->last_response = time(NULL) + 60;
}

void kill_cockroach(croach_info_t *cockroach) {
    // Set the respawn time to current time + 10 seconds
    cockroach->respawnTime = time(NULL) + 10;
    cockroach->visible = 0; // Make the cockroach invisible
}

void respawn(croach_info_t *cockroach) {
    // Set the respawn time to current time + 10 seconds
    cockroach->respawnTime = -1;
    cockroach->visible = 1; // Make the cockroach invisible
    
}

void check_respawn_all_cr(croach_info_t cockroach[], int max_cr) {
    // Set the respawn time to current time + 10 seconds
    for(int i=0; i<max_cr; i++){
        if(cockroach[i].key == -2) continue;
        if(cockroach[i].respawnTime != -1){
            if(time(NULL) >= cockroach[i].respawnTime){//se ja tiver passado o tempo da respawn
                respawn(&(cockroach[i]));
            }
        }
    }
}

int adjustCockroachCount(int *number_cr, int *n_insects, int max_insects) {
    // Calculate the available space for new cockroaches
    int availableSpace = max_insects - *n_insects;
    // Adjust the number of cockroaches if needed
    if (*number_cr + *n_insects > max_insects) {
        *number_cr = availableSpace;
    }
    // Return the adjusted number of cockroaches
    return *number_cr;
}

bool no_visible_cr_pos(croach_info_t cockroaches[], int max_insects, int xpos, int ypos) {
    int value = find_cr_pos(cockroaches, max_insects, xpos, ypos, FALSE);
    return (value == -1) || (!is_visible(cockroaches[value]));
}

void initializeCockroaches(croach_info_t cockroaches[], int *n_insects) {
    for (int i = 0; i < 300; i++) {
        cockroaches[i].id = i + 1;
        cockroaches[i].key = -2;
    }
    *n_insects = 0;
}

void update_min_response_cockroach(int max_insects, croach_info_t cockroaches[], time_t *min_response_time, int *ch_pos, int *type) {
    for (int i = 0; i < max_insects; i++) {
        if ((cockroaches[i].key != -2) && ((cockroaches[i].last_response - time(NULL)) < *min_response_time)) {
            *min_response_time = cockroaches[i].last_response - time(NULL);
            *ch_pos = i;
            *type = 0; // 0 represents cockroach
        }
    }
}

