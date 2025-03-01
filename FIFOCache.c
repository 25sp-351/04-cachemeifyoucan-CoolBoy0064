#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "cut_list.h"
#include "FIFOCache.h"

CutList *(*cached_function)(Vec pv, PieceLength total_length);


const int MAX_CACHE_SIZE = 10;

static FIFO_node *head;
static FIFO_node *tail;
static int current_size = 0;


void  create_cache(){
    head -> next = tail;
    tail -> next = NULL;
    return;
}

void add_node(FIFO_node *head, int key, CutList *output_val){
    FIFO_node *new_head = (FIFO_node*)malloc(sizeof(FIFO_node));
    new_head -> key = key;
    new_head -> cutlist = output_val;
    new_head -> next = head;
    head = new_head;
    return;
}

void evict(FIFO_node *head, FIFO_node *tail){
    FIFO_node *current = head;
    while(current != NULL){
        if(current -> next == tail){
            FIFO_node *old_head = head;
            head = head -> next;
            free(old_head);
        }
    }
    return;
}

FIFO_node *get_key(FIFO_node *head, int key){
    FIFO_node *current = head;
    while(current != NULL){
        if (current -> key == key){
            return current;
        }
        current = current -> next;
    }
    return NULL; // if we did not find the key in the list
}


CutList *cached_optimal_cutlist_for(Vec pv, PieceLength total_length){
    FIFO_node *cache_hit = get_key(head, total_length);
    if(cache_hit != NULL){
        printf("Cache hit\n");
        return cache_hit -> cutlist;
    }
    else{
        printf("Cache miss\n");
        CutList *output_val = optimal_cutlist_for(pv, total_length);
        add_node(head, total_length, output_val);
        current_size++;
        if(current_size >= MAX_CACHE_SIZE){
            evict(head, tail);
        }
        return output_val;
    }
}

int main(){
    head = (FIFO_node*)malloc(sizeof(FIFO_node));
    tail = (FIFO_node*)malloc(sizeof(FIFO_node));
    create_cache();

    cached_function = &cached_optimal_cutlist_for;
    Vec value_list = read_piece_values();
    int rod_length = 0;
    printf("Enter rod length: ");
    fscanf(stdin, "%d", &rod_length);
    CutList *cl = cached_optimal_cutlist_for(value_list, rod_length);
    printf("Optimal cut list for rod of length %d:\n", rod_length);
    cutlist_print(cl);
    cutlist_free(cl);
    CutList *cache_test = cached_optimal_cutlist_for(value_list, rod_length);
    printf("Optimal cut list for rod of length %d:\n", rod_length);
    cutlist_print(cl);
    cutlist_free(cl);
    return 0;
}