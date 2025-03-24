#include "server.h"

void *timeOutThread(void *arguments){
    
    args_t *args = (args_t *) arguments;
    struct timeout_t timeout;
    timeout.time = find_min_response_entity(args->lizard, MAX_LIZARDS, args->cockroaches, MAX_INSECTS, args->wasps, &timeout.ch_pos, &timeout.type);
    
    while(1){
        
        sleep(timeout.time);
        timeout.time = find_min_response_entity(args->lizard, MAX_LIZARDS, args->cockroaches, MAX_INSECTS, args->wasps, &timeout.ch_pos, &timeout.type);
        if(timeout.time!=0)continue;
        
        switch (timeout.type) {
            case 0:
                removeCockroach(args->my_win, args->mports->publisher, args->lizard, args->cockroaches, args->n_insects, MAX_INSECTS, MAX_LIZARDS, args->wasps, timeout.ch_pos, args->lock_pos);
                break;
            case 1:
                removeLizard(args->my_win, *args->mports, args->lizard, args->n_lizards, timeout.ch_pos, args->cockroaches, MAX_INSECTS, args->wasps, args->lock_pos);
                break;
            case 2:
                removeWasp(args->my_win, args->mports->publisher, args->lizard, args->n_insects, &args->wasps[timeout.ch_pos], args->lock_pos);
                break;
            }
        wrefresh(args->my_win);
    
        
        
    }
}

void *LizardThread(void *arguments){
    
    args_t *args = (args_t *) arguments;

    remote_char_t message;
    remote_lizard l_msg;

    char responder_info[40];
    void *responder = zmq_socket (args->mports->context, ZMQ_REP);
    sprintf(responder_info, "tcp://%s:%d", args->mports->router_ip, args->mports->backend_port);
    int rc = zmq_connect(responder, responder_info);
    assert(rc == 0);

    while(1){
        zmq_recv(responder, &message, sizeof(remote_char_t), 0); // int flags??? ultimo campo da funcao
        
        switch (message.msg_type)
        {
        case 0:
            handleConnectLizardRequest(args->my_win, &message, args->lizard, args->n_lizards, MAX_LIZARDS, args->mports, args->cockroaches, MAX_INSECTS, args->wasps, &l_msg);
            break;
        case 2:
            handleMoveLizardRequest(args->my_win, &message, args->lizard, *(args->n_lizards), MAX_LIZARDS, args->cockroaches, MAX_INSECTS, *(args->mports), args->wasps, &l_msg, args->lock_pos);
            break;
        case 4:
            break;
            handleRemoveLizardRequest(args->my_win, &message, args->lizard, args->n_lizards, MAX_LIZARDS, args->cockroaches, MAX_INSECTS, *(args->mports), args->wasps, &l_msg, args->lock_pos);
        default:
            break;
        }
        zmq_send(responder, &l_msg, sizeof(remote_lizard), 0);
        wrefresh(args->my_win);
    }
    free(args);
}

void *cockroachWaspThread(void *arguments) {
    args_t *args = (args_t *)arguments;

    remote_char_t msg;

    int rc;
    while(1){
        do{
            timeout(1000);
            rc = zmq_recv(args->mports->insect_req, &msg, sizeof(remote_char_t), 0);
            check_respawn_all_cr(args->cockroaches,MAX_INSECTS);
        } while(rc==-1);
        
        switch (msg.msg_type) {
            
            case 1:
                handleConnectCockroachRequest(args->my_win, &msg, args->cockroaches, args->n_insects, MAX_INSECTS, args->lizard, MAX_LIZARDS, *(args->mports), args->wasps);
                break;
            case 3:
                handleMoveCockroachRequest(args->my_win, &msg, args->cockroaches, MAX_INSECTS, args->lizard, MAX_LIZARDS, *(args->mports), args->wasps, args->lock_pos);
                break;
            case 7:
                handleRemoveCockroachRequest(args->my_win, &msg, args->cockroaches, args->n_insects, MAX_INSECTS, args->lizard, MAX_LIZARDS, *(args->mports), args->wasps, args->lock_pos);
                break;
            case 5:
                handleConnectWaspRequest(args->my_win, &msg, args->cockroaches, args->wasps, args->n_insects, MAX_INSECTS, args->lizard, MAX_LIZARDS, *(args->mports));
                break;
            case 6:
                handleMoveWaspRequest(args->my_win, &msg, args->cockroaches, MAX_INSECTS, args->lizard, MAX_LIZARDS, *(args->mports), args->wasps, args->lock_pos);
                break;
            case 8:
                handleRemoveWaspRequest(args->my_win, &msg, args->cockroaches, args->n_insects, MAX_INSECTS, args->lizard, MAX_LIZARDS, *(args->mports), args->wasps, args->lock_pos);
                break;
            default:
                // Handle unknown message type for cockroaches and wasps
                break;
        }
       wrefresh(args->my_win);
       // zmq_send(args->mports->insect_req, &msg, sizeof(remote_char_t), 0);
    }
    // Free the allocated memory for arguments
    free(args);

    return NULL;
}
