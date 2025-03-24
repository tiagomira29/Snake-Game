#include "server.h"

int stoi(char *s){
    int n=0;
    for(int i=0; s[i]!='\000'; i++){
        if(s[i] >= '0' && s[i] <= '9'){
            n *= 10;
            n += (int)(s[i] - '0');
        } else {
            return -1;
        }
    }
    return n;
}

direction_t inv_direction(direction_t direction){
    switch (direction){
        case UP:
            return DOWN;
            break;
        case DOWN:
            return UP;
            break;
        case LEFT:
            return RIGHT;
            break;
        case RIGHT:
            return LEFT;
            break;
        default:
            return UP;
    }
}