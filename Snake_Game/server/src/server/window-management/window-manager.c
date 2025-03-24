#include "server.h"

void initializeWindow(WINDOW **my_win) {
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();

    *my_win = newwin(WINDOW_SIZE, WINDOW_SIZE, 0, 0);
    box(*my_win, 0, 0);
    wrefresh(*my_win);
}

void deleteCockroachOldPosition(WINDOW *my_win, void *publisher, croach_info_t cockroaches[], int max_insects, lizard_info_t lizard[], int max_lizards, int old_x, int old_y) {
    int value, id;

    if ((value = find_cr_pos(cockroaches, max_insects, old_x, old_y, FALSE)) != -1 && is_visible(cockroaches[value])) {
        //se tinha la uma cockroach da print da cockroach
        
        print_to_window(my_win, old_x, old_y, cockroaches[value].ch, TRUE);
        sendDisplayMessage(publisher, cockroaches[value].ch, old_x, old_y);
    } 
    else if ((id = find_point_lizard(lizard, max_lizards, old_x, old_y)) != -1) {
        //se tinha la um ponto da print do ponto
        if (lizard[id].value < 50) {
            print_to_window(my_win, old_x, old_y, '.', FALSE);
            sendDisplayMessage(publisher, '.', old_x, old_y);
        } 
        else {
            print_to_window(my_win, old_x, old_y, '*', FALSE);
            sendDisplayMessage(publisher, '*', old_x, old_y);
        }   
    } 
    else {
        
        //se nao tinha nada apaga
        print_to_window(my_win, old_x, old_y, ' ', TRUE);
        sendDisplayMessage(publisher, ' ', old_x, old_y);
    }
} 

void deleteWaspOldPosition(WINDOW *my_win, void *publisher, lizard_info_t lizard[], int max_lizards, int old_x, int old_y) {
    int value, id;

    if ((id = find_point_lizard(lizard, max_lizards, old_x, old_y)) != -1) {
        //se tinha la um ponto da print do ponto
        if (lizard[id].value < 50) {
            print_to_window(my_win, old_x, old_y, '.', FALSE);
            sendDisplayMessage(publisher, '.', old_x, old_y);
        } 
        else {
            print_to_window(my_win, old_x, old_y, '*', FALSE);
            sendDisplayMessage(publisher, '*', old_x, old_y);
        }   
    } 
    else {
        //se nao tinha nada apaga
        print_to_window(my_win, old_x, old_y, ' ', TRUE);
        sendDisplayMessage(publisher, ' ', old_x, old_y);
    }
}

void deleteDeadLizardOldPosition(WINDOW *my_win, void *publisher, lizard_info_t lizard[], int max_lizards, int old_x, int old_y) {
    int value, id;

    if (((id = find_point_lizard(lizard, max_lizards, old_x, old_y)) != -1) && (lizard[id].value >= 0)) {
        //se tinha la um ponto da print do ponto
        if (lizard[id].value < 50) {
            print_to_window(my_win, old_x, old_y, '.', FALSE);
            sendDisplayMessage(publisher, '.', old_x, old_y);
        } 
        else {
            print_to_window(my_win, old_x, old_y, '*', FALSE);
            sendDisplayMessage(publisher, '*', old_x, old_y);
        }
    }
    else {
        //se nao tinha nada apaga
        print_to_window(my_win, old_x, old_y, ' ', TRUE);
        sendDisplayMessage(publisher, ' ', old_x, old_y);
    }
    
}

void deleteOldPoints(WINDOW *my_win, void *publisher, lizard_info_t lizard[], int max_lizards, croach_info_t cockroaches[], int max_insects, int ch_pos, wasp_info_t wasps[]) {
    for (int i = 0; i < 5; i++) {
        int xpos = lizard[ch_pos].tail.point[i].x;
        int ypos = lizard[ch_pos].tail.point[i].y;

        // nao e critico
        //lock 1
        if (!noVisibleAnimal(lizard, max_lizards, cockroaches, max_insects, xpos, ypos, wasps))
            continue;

        if (point_in_coord(lizard, max_lizards, xpos, ypos) <= 1) {
            //unlock 1

            print_to_window(my_win, xpos, ypos, ' ', TRUE);
            sendDisplayMessage(publisher, ' ', xpos, ypos);
            continue;
        }
        int id = find_point_other_lizard(lizard, max_lizards, xpos, ypos, ch_pos);
        if (lizard[id].value < 50) {
            // unlock 1
            if (lizard[id].value > 0) {
            // unlock 1
                print_to_window(my_win, xpos, ypos, '.', FALSE);
                sendDisplayMessage(publisher, '.', xpos, ypos);
            }
            continue;
        } 
        print_to_window(my_win, xpos, ypos, '*', FALSE);
        sendDisplayMessage(publisher, '*', xpos, ypos);
    }
}

void writeNewPoints(WINDOW *my_win, void *publisher, lizard_info_t lizard[], int max_lizards, croach_info_t cockroaches[], int max_insects, int ch_pos, wasp_info_t wasps[]) {
    for (int i = 0; i < 5; i++) {
        int xpos = lizard[ch_pos].tail.point[i].x;
        int ypos = lizard[ch_pos].tail.point[i].y;

        if (!noVisibleAnimal(lizard, max_lizards, cockroaches, max_insects, xpos, ypos, wasps))
            
            continue;
            
        if (lizard[ch_pos].value < 50) {
            print_to_window(my_win, xpos, ypos, '.', FALSE);
            sendDisplayMessage(publisher, '.', xpos, ypos);
            continue;
        } 
        print_to_window(my_win, xpos, ypos, '*', FALSE);
        sendDisplayMessage(publisher, '*', xpos, ypos);
    }
}

void cleanup(WINDOW *my_win, void *publisher, void *resquester, void *context) {
    // Exit curses mode
    endwin();

    // Additional cleanup for ZMQ
    zmq_close(publisher);
    zmq_close(resquester);
    zmq_ctx_destroy(context);
}