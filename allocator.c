#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <sys/mman.h>
#include <string.h>

#include "buddy_allocator.h"
#include "bit_map.h"
#include "allocator.h"

char memory[MAX_MEM_SIZE];
uint8_t buffer[((1 << (BUDDY_LVLS)) - 1)];

buddy_allocator_t buddy_allocator;
BitMap bitmap;

void allocator_init(){
    buddy_allocator_init(&buddy_allocator, memory, BUDDY_LVLS, MIN_SIZE, buffer);   //the bitmap is initialized in buddy_allocator_init
}

void* my_malloc(size_t size){
    if (size <= 0){
        printf("invalid size in my_malloc\n");
        return NULL;
    }

    size += 4; 
    //now i check if i can use the buddy allocator, the size must be not greater than 1/4 of the page size
    if (size <= PAGE_SIZE / 4){
        return buddy_allocator_pseudo_malloc(&buddy_allocator, size);
    }

    //if the size is greater than 1/4 of the page size, i use mmap
    else{
        void *pointer = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (pointer == MAP_FAILED){
            perror("mmap failed");
            return NULL;
        }
        return pointer;
    }
}

void my_free(void *pointer, size_t size){
    if (pointer == NULL || size <= 0){
        printf("invalid stuff in my_free input\n");
        return;
    }
    size += 4;
    // as before if the size is less than 1/4 of the page size, i use buddy_allocator
    if (size <= PAGE_SIZE / 4){
        free_buddy_pointer(&buddy_allocator, pointer);
    }
    //else we use the unmap
    else{
        if (munmap(pointer, size) == -1){
            perror("munmap failed");
        }
    }
}


