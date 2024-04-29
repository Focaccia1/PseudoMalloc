#define _GNU_SOURCE     //was having problems with "MAP_ANONYMOUS" declaration so i defined a different standard. -std=c99 does not support MAP_ANONYMOUS definition, soo i used gnu99 
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <sys/mman.h>
#include <errno.h>
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

    size += 4; //I sum the header size (4 bytes)
    //now i check if i have to use buddy or mmap: size <= (1/4)page_size -> buddy | size > (1/4)page_size -> mmap
    if (size <= PAGE_SIZE / 4){
        void *pointer = buddy_allocator_pseudo_malloc(&buddy_allocator, size);
        if (pointer == NULL){
            perror("buddy_allocator_pseudo_malloc failed\n");
            return NULL;
        }
        return pointer;
    }

    //if the size is greater than 1/4 of the page size, i use mmap whith these chosen FLAGS:
    /*
    PROT_READ: This specifies that the memory region should be readable. So processes can read from this memory.

    PROT_WRITE: This specifies that the memory region should be writable. So processes can write to this memory.

    MAP_PRIVATE: This pecifies that modifications to the mapped region are private to the mapping process.
    So changes are not visible to other processes mapping the same file, and are not written back to the underlying file.

    MAP_ANONYMOUS: This specifies that the mapping is not backed by any file. So basically the kernel should create an anonymous memory mapping,
    which is not associated with any file on disk. Perfect for our purpose of allocating memory.
    */
    else{
        void *pointer = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (pointer == MAP_FAILED){
            errno = ENOMEM; //if the mmap fails, i set errno to ENOMEM -> not enough memory
            perror("mmap failed");
            return NULL;
        }
        return pointer;
    }
}

void my_free(void *pointer, size_t size){
    if (pointer == NULL || size <= 0){      //if the pointer is NULL or the size is less than 0, so invalid input, we return
        printf("invalid stuff in my_free input\n");
        return;
    }
    size += 4;  //to free also the header
    // as before, if the size is less than 1/4 of the page size, i now use my buddy_allocator functions
    if (size <= PAGE_SIZE / 4){
        free_buddy_pointer(&buddy_allocator, pointer);  
    }
    //if the dim is >= 1/4, we use the unmap
    else{
        if (munmap(pointer, size) == -1){
            perror("munmap failed");
        }
    }
}


