#ifndef CONNECT_H
#define CONNECT_H
#include <stdlib.h>
#include "server.h"


/* ************************
    FUNCTION PROTOTYPES
************************ */

void generateNewWasp(WINDOW *my_win, void *publisher, lizard_info_t lizard[], croach_info_t cockroaches[], wasp_info_t wasps[], int *n_insects, int max_insects, int max_lizards, remote_char_t *m, remote_wasp *wasp_msg);

void handleCockroachCountError(void *resquester, remote_cockroach *cockroach_msg);

void handleWaspCountError(void *requester, remote_wasp *wasp_msg);

#endif /* CONNECT_H */