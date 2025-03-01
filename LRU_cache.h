#ifndef LRU_CACHE_H
#define LRU_CACHE_H

#include "cut_list.h"
CutList *(*cached_function)(Vec, PieceLength);

#endif