#include "wasp.h"

int find_wasp_info(wasp_info_t wasp[], int max_insects, int id){

    for (int i = 0 ; i < max_insects; i++){
        if(wasp[i].key == -2) continue;
        if(id == wasp[i].id){
            return i;
        }
    }
    return -1;
}

int find_wasp_free_index(wasp_info_t wasp[], int max_insects){

    for(int i=0; i<max_insects; i++){
        if(wasp[i].key == -2) return i;
    }

    return -1;
}

int verify_wasp_info(wasp_info_t wasp[], int max_insects, int received_id, int received_key){

    for (int i = 0 ; i < max_insects; i++){
        if(wasp[i].key == -2) continue;
        if(received_id == wasp[i].id && wasp[i].key == received_key){//se tiver o mesmo id e chave vai estar certo
            return i;
        }
    }
    return -1;
}

int find_wasp_pos(wasp_info_t wasp[], int max_cr, int pos_x, int pos_y){
    for (int i = 0 ; i < max_cr; i++){
        if(wasp[i].key == -2) continue;
        if(pos_x == wasp[i].pos_x && pos_y == wasp[i].pos_y){
            return i;
        }
    }
    return -1;
}

void init_wasp(wasp_info_t *wasp, int ch, int x, int y, int id,int key){
    if(ch != -1) wasp->ch = ch;
    if(x != -1) wasp->pos_x = x;
    if(y != -1) wasp->pos_y = y;
    if(id != -1) wasp->id = id;
    if(key != -1) wasp->key = key;
    wasp->last_response = time(NULL) + 60;
}

int adjustWaspCount(int *number_wasps, int *n_insects, int max_insects) {
    // Calculate the available space for new wasps
    int availableSpace = max_insects - *n_insects;
    // Adjust the number of wasps if needed
    if (*number_wasps + *n_insects > max_insects) {
        *number_wasps = availableSpace;
    }
    // Return the adjusted number of wasps
    return *number_wasps;
}

void initializeWasps(wasp_info_t wasps[], int *n_insects) {
    for (int i = 0; i < 300; i++) {
        wasps[i].id = i + 1;
        wasps[i].key = -2;
    }
    *n_insects = 0;
}

void update_min_response_wasp(int max_insects, wasp_info_t wasps[], time_t *min_response_time, int *ch_pos, int *type) {
        
    for (int i = 0; i < max_insects; i++) {
        if ((wasps[i].key != -2) && ((wasps[i].last_response - time(NULL)) < *min_response_time)) {
            *min_response_time = wasps[i].last_response - time(NULL);
            *ch_pos = i;
            *type = 2; // 2 represents wasp
        }
    }
}