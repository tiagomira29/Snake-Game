#include <stdlib.h>

#include "randoms.h"

int random_id() {return rand() % 5 + 1;} // random number between 1 and 5

int random_pos() {return rand()%(WINDOW_SIZE-2) + 1;} // random number between 1 and WINDOW_SIZE - 2

direction_t random_dir() {return rand() %4;} // returns a random direction

int generateRandomKey() {return rand();} // returns a random integer