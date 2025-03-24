#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

typedef struct node_linked {
    void *data;
    struct node_linked *next;
    struct node_linked *previous;
} node_linked;

typedef struct {
    node_linked *head;
    node_linked *tail;
} list;

/* ************************
    FUNCTION PROTOTYPES
************************ */

void init_linked_list(list **list);

node_linked *init_node(node_linked *previous, node_linked *next, size_t data_size);

void remove_node(list *list, node_linked *node_remove);

#endif /* LIST_H */