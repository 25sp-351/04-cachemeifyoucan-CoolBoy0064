#include <stdio.h>
#include <stdlib.h>
 
#include "cut_list.h"
#include "LRU_cache.h"

CutList *(*cached_function)(Vec, PieceLength);

typedef struct QNode
{
    struct QNode *prev, *next;
    unsigned pageNumber;  
    CutList *cutlist;
} QNode;
 
typedef struct Queue
{
    unsigned count;  
    unsigned numberOfFrames; 
    QNode *front, *rear;
} Queue;
 
typedef struct Hash
{
    int capacity; 
    QNode* *array;
} Hash;
 
QNode* newQNode( unsigned pageNumber )
{
    QNode* temp = (QNode *)malloc( sizeof( QNode ) );
    temp->pageNumber = pageNumber;
    temp->prev = temp->next = NULL;
    return temp;
}
 
Queue* createQueue( int numberOfFrames )
{
    Queue* queue = (Queue *)malloc( sizeof( Queue ) ); 
    queue->count = 0;
    queue->front = queue->rear = NULL;
    queue->numberOfFrames = numberOfFrames;
 
    return queue;
}
 

Hash* createHash( int capacity )
{
    Hash* hash = (Hash *) malloc( sizeof( Hash ) );
    hash->capacity = capacity;
    hash->array = (QNode **) malloc( hash->capacity * sizeof( QNode* ) );
    int i;
    for( i = 0; i < hash->capacity; ++i )
        hash->array[i] = NULL;
 
    return hash;
}
 
int AreAllFramesFull( Queue* queue )
{
    return queue->count == queue->numberOfFrames;
}
 

int isQueueEmpty( Queue* queue )
{
    return queue->rear == NULL;
}
 
void deQueue( Queue* queue )
{
    if( isQueueEmpty( queue ) )
        return;
 
    if (queue->front == queue->rear)
        queue->front = NULL;
 
    QNode* temp = queue->rear;
    queue->rear = queue->rear->prev;
 
    if (queue->rear)
        queue->rear->next = NULL;
 
    free( temp );

    queue->count--;
}
 
void Enqueue( Queue* queue, Hash* hash, unsigned pageNumber )
{

    if ( AreAllFramesFull ( queue ) )
    {
        hash->array[ queue->rear->pageNumber ] = NULL;
        deQueue( queue );
    }
 
    QNode* temp = newQNode( pageNumber );
    temp->next = queue->front;
 
    if ( isQueueEmpty( queue ) )
        queue->rear = queue->front = temp;
    else 
    {
        queue->front->prev = temp;
        queue->front = temp;
    }
 
    hash->array[ pageNumber ] = temp;
 
 
    queue->count++;
}
 

void ReferencePage( Queue* queue, Hash* hash, unsigned pageNumber )
{
    QNode* reqPage = hash->array[ pageNumber ];
 
    if ( reqPage == NULL )
        Enqueue( queue, hash, pageNumber );
 
    else if (reqPage != queue->front)
    {
        reqPage->prev->next = reqPage->next;
        if (reqPage->next)
           reqPage->next->prev = reqPage->prev;
         if (reqPage == queue->rear)
        {
           queue->rear = reqPage->prev;
           queue->rear->next = NULL;
        }
        reqPage->next = queue->front;
        reqPage->prev = NULL;
        reqPage->next->prev = reqPage;
 
        queue->front = reqPage;
    }
}

CutList *cached_optimal_cutlist_for(Vec pv, PieceLength total_length, Queue* queue, Hash* hash){
    QNode *cache_hit = hash->array[ total_length ];
    if(cache_hit != NULL){
        printf("Cache hit\n");
        if(cache_hit != queue->front){
            cache_hit->prev->next = cache_hit->next;
            if (cache_hit->next)
                cache_hit->next->prev = cache_hit->prev;
            if (cache_hit == queue->rear)
            {
                queue->rear = cache_hit->prev;
                queue->rear->next = NULL;
            }
            cache_hit->next = queue->front;
            cache_hit->prev = NULL;
            cache_hit->next->prev = cache_hit;
            queue->front = cache_hit;
        }
        return cache_hit -> cutlist;
    }
    else{
        printf("Cache miss\n");
        CutList *cache_hit = cached_function(pv, total_length);
        Enqueue( queue, hash, total_length);
        return cache_hit;
    }
}
 
int main()
{
    Queue* q = createQueue( 4 );
 
    Hash* hash = createHash( 10 );

    cached_function = &optimal_cutlist_for;
 
    Vec value_list = read_piece_values();
    int rod_length = 0;
    printf("Enter rod length: ");
    fscanf(stdin, "%d", &rod_length);
    CutList *cl = cached_optimal_cutlist_for(value_list, rod_length, q, hash);
    printf("Optimal cut list for rod of length %d:\n", rod_length);
    cutlist_print(cl);
    cutlist_free(cl);
    CutList *cache_test = cached_optimal_cutlist_for(value_list, rod_length, q, hash);
    printf("Optimal cut list for rod of length %d:\n", rod_length);
    cutlist_print(cache_test);
    cutlist_free(cache_test);

 
    return 0;
}