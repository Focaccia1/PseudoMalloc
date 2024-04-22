#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
//#include <sys/mman.h>
#include <string.h>

#include "buddy_allocator.h"
#include "bit_map.h"
#include "allocator.h"

char memory[MEM_SIZE];
uint8_t bitset[BITSET_SIZE];

buddy_allocator_t buddy_allocator;
BitMap bitmap;

void allocator_init(){
    BitMap_init(&bitmap, N_BUDDIES, bitset);
    BitMap_setBit(&bitmap, 0, 1);   //1->buddy free
    buddy_allocator_init(&buddy_allocator, memory, BUDDY_LVLS, MIN_SIZE, bitset);

}