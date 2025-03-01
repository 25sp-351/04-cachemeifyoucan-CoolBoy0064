#ifndef FIFO_CACHE_H
#define FIFO_CACHE_H

#include "cut_list.h"
typedef struct FIFO_node{
    CutList *cutlist;
    int key;
    struct FIFO_node *next;
} FIFO_node;

#endif
