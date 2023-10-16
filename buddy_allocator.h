#pragma once
#include "bit_map.h"

typedef struct{
    int min_block_size; // dim minima del blocco
    char* memory; // puntatore alla memoria allocata
    int lvl; // livello del blocco
    BitMap* bitmap; // puntatore alla bitmap (vd bit_map.h)
}buddy_allocator_t;