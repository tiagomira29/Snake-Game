#include "server.h"

int find_min_response_entity(lizard_info_t lizards[], int max_lizards,
                              croach_info_t cockroaches[], int max_insects,
                              wasp_info_t wasps[],
                              int *ch_pos, int *type) {

    *ch_pos = -1;
    time_t min_response_time = 5;  // Initialize with a large value
    *type = -1;

    // Check cockroach entities
    update_min_response_cockroach(max_insects, cockroaches, &min_response_time, ch_pos, type);

    // Check lizard entities
    update_min_response_lizard(max_lizards, lizards, &min_response_time, ch_pos, type);

    // Check wasp entities
    update_min_response_wasp(max_insects, wasps, &min_response_time, ch_pos, type);
    // printf("%ld\n", min_response_time);
    if(min_response_time < 0)min_response_time=0;   
    // Certify there is still space for more cockroaches
    return min_response_time;
}




void findValidPosition(croach_info_t cockroaches[], wasp_info_t wasps[], int max_insects, lizard_info_t lizard[], int max_lizards, int *pos_x, int *pos_y) {
    while (1) {
        *pos_x = random_pos();
        *pos_y = random_pos();
        if ((find_cr_pos(cockroaches, max_insects, *pos_x, *pos_y, FALSE) == -1) &&
            (find_lizard_pos(lizard, max_lizards, *pos_x, *pos_y, FALSE) == -1)&&
            (find_wasp_pos(wasps, max_lizards, *pos_x, *pos_y) == -1)) {
            break;
        }
    }
}

bool noVisibleAnimal(lizard_info_t lizard[], int max_lizards, croach_info_t cockroaches[], int max_insects, int xpos, int ypos, wasp_info_t wasps[]) {
    return (find_lizard_pos(lizard, max_lizards, xpos, ypos, FALSE) == -1) &&
           (no_visible_cr_pos(cockroaches, max_insects, xpos, ypos)) &&
           (!ON_BORDER(xpos, ypos)) && 
           (find_wasp_pos(wasps, max_insects, xpos, ypos) == -1);
}

void new_position(int* x, int *y, direction_t direction){
    switch (direction)
    {
    case UP:
        (*x) --;
        if(*x ==0)
            *x = 2;
        break;
    case DOWN:
        (*x) ++;
        if(*x ==WINDOW_SIZE-1)
            *x = WINDOW_SIZE-3;
        break;
    case LEFT:
        (*y) --;
        if(*y ==0)
            *y = 2;
        break;
    case RIGHT:
        (*y) ++;
        if(*y ==WINDOW_SIZE-1)
            *y = WINDOW_SIZE-3;
        break;
    default:
        break;
    }
}

args_t *init_args(WINDOW *my_win, lizard_info_t *lizard, int *n_lizards, croach_info_t *cockroaches, int *n_insects, main_ports *mports, wasp_info_t *wasps, pthread_mutex_t (*lock_pos)[30]) {
    // Allocate memory for server_info_t
    args_t *args = malloc(sizeof(args_t));

    // Copy parameters to the server structure
    args->my_win = my_win;
    args->lizard = lizard;
    args->n_lizards = n_lizards;
    args->cockroaches = cockroaches;
    args->n_insects = n_insects;
    args->wasps = wasps;
    args->mports = mports; // Assuming server_ports is a value, not a pointer
    args->lock_pos = lock_pos;
    return args;
}

void initializeServer(char *ip, main_ports *mports, lizard_info_t lizard[], int *n_lizards, croach_info_t cockroaches[], int *n_insects, WINDOW **my_win, wasp_info_t wasps[]){
    initializeLizards(lizard, n_lizards);
    initializeCockroaches(cockroaches, n_insects);
    initializeWasps(wasps, n_insects);

    initializeZMQ(ip, mports);
    initializeWindow(my_win);
    
}



void messageHandlingLoop(WINDOW *my_win, lizard_info_t *lizard, int *n_lizards, croach_info_t *cockroaches, int *n_insects, main_ports *server_ports, wasp_info_t wasps[]){
/*     struct timeout_t timeout;
    find_min_response_entity(lizard, MAX_LIZARDS, cockroaches, MAX_INSECTS, wasps, &timeout.ch_pos, &timeout.type);
    timeout.time = 10000;
    zmq_setsockopt(server_ports->requester, ZMQ_RCVTIMEO, &timeout.time, sizeof(timeout.time));*/
    pthread_mutex_t lock_pos[30][30];

    // Initialize mutexes
    for (int i = 0; i < 30; ++i) {
        for (int j = 0; j < 30; ++j) {
            pthread_mutex_init(&lock_pos[i][j], NULL);
        }
    }

    // Your code here

    // Destroy mutexes when done
    for (int i = 0; i < 30; ++i) {
        for (int j = 0; j < 30; ++j) {
            pthread_mutex_destroy(&lock_pos[i][j]);
        }
    }

    args_t *arguments = init_args(my_win, lizard, n_lizards, cockroaches, n_insects, server_ports, wasps, lock_pos);



    int received;


    // Criacao da thread para lidar com mensagens de baratas
    pthread_t cockroach_thread;
    pthread_create(&cockroach_thread, NULL, cockroachWaspThread, arguments);

    // Espera que todas as threads de lagartos terminem
    for (int n_lizard_thread = 0; n_lizard_thread < MAX_LIZARD_THREADS; n_lizard_thread++) {
        pthread_t handlelizard;
        pthread_create( &handlelizard, NULL, LizardThread, (void *) arguments);
    }

    pthread_t clockouter;
    pthread_create(&clockouter, NULL, timeOutThread, arguments);
    
    zmq_proxy (server_ports->frontend, server_ports->backend, NULL);
    // Espera que a thread de baratas termine
    /* pthread_join(cockroach_thread, NULL);
 */
    // Restante do código ou limpeza, se necessário

    return;



    /* while (1) {
        timeout.time = 1000*find_min_response_entity(lizard, MAX_LIZARDS, cockroaches, MAX_INSECTS, wasps, &timeout.ch_pos, &timeout.type);
        if(timeout.time == 0)received = -1;
        else{
            zmq_setsockopt(server_ports->requester, ZMQ_RCVTIMEO, &timeout.time, sizeof(timeout.time));
            received = zmq_recv(server_ports->requester, &m, sizeof(m), 0);
        }

        // Handle time outs
        if (received == -1) {
            // Check if the receive operation timed out
            if (zmq_errno() == EAGAIN) {
             if (1) {  
                // Handle timeout logic here
                // Check who timedout
                if (timeout.type == 0 || timeout.type == 2) {
                    // Handle both cockroaches and wasps in the same thread
                } else if (timeout.type == 1) {
                    // Handle lizards in a separate thread
                    // ...
                } else {
                    // Handle unknown timeout type
                }
                wrefresh(my_win);
                continue;
            }
            else {
                fprintf(stderr, "Error receiving message: %s (errno: %d)\n", zmq_strerror(errno), errno);
                // Handle other errors here
                 perror("zmq_recv"); 
                // Handle other errors here
            }
            
        }
            // Create a cockroach/wasp thread
            args_t current_args = init_args(my_win, lizard, n_lizards, max_lizards, cockroaches, n_insects, max_insects, mports, wasps, received_msg)
            zmq_send(pusher,);
        } else {
            continue;
        }

        

        // Draw mark on new position and refresh window
        wrefresh(my_win);cockroaches, MAX_INSECTS);
        if (m.msg_type == 0 || m.msg_type == 2 || m.msg_type == 4) {
            // Create a lizard thread
            
        } else if (m.msg_type == 1 || m.msg_type == 3 || m.msg_type == 5 || m.msg_type == 6 || m.msg_type == 7 || m.msg_type == 8) 
        {
            // Create a cockroach/wasp thread
            args_t current_args = init_args(my_win, lizard, n_lizards, max_lizards, cockroaches, n_insects, max_insects, mports, wasps, received_msg)
            zmq_send(pusher,);
        } else {
            continue;
        }

        

        // Draw mark on new position and refresh window
        wrefresh(my_win);
    }

    // Cleanup
    endwin();
    zmq_close(server_ports->publisher);
    zmq_close(server_ports->requester);
    zmq_ctx_destroy(server_ports->context); */
}