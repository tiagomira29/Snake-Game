#include "list.h"

void init_list(list **list){
    *list = malloc(sizeof(list));
    (*list)->head = NULL;
    (*list)->tail = NULL;
}

void remove_list(list *list){
    free(list);
}

node_linked *init_node(node_linked *previous, node_linked *next, size_t data_size){

    node_linked *new_node;

    new_node = malloc(sizeof(node_linked));

    new_node->data = malloc(data_size);
    new_node->next = NULL;
    new_node->previous = previous;

    if(previous != NULL)
        previous->next = new_node;

    return new_node;
}

void remove_node(list *list, node_linked *node_remove){

    if(node_remove->data != NULL) free(node_remove->data);

    if(node_remove == list->head){ /* if head is removed, next node_linked in list becomes head */
        list->head = node_remove->next;
    }

    if(node_remove == list->tail){ /*  if tail is removed, previous node_linked in list becomes tail  */
        list->tail = node_remove->previous;
    }

    if(node_remove->previous != NULL){
        (node_remove->previous)->next = node_remove->next;
    }
    
    if(node_remove->next != NULL){
        (node_remove->next)->previous = node_remove->previous;
    }

    free(node_remove);
}

void remove_list_plus_nodes(list *list){

    while(list->head != NULL){
        remove_node(list, list->head);
    }
    remove_list(list);
}