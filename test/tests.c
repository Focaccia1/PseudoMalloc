#include "../src/allocator.h"
#include "../src/buddy_allocator.h"
#include "../src/bit_map.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>

#define MAX_MEM_SIZE (1 << 20)   //1024*1024 as the example showed in classes
#define BUDDY_LVLS 9    //10 cause there is 0
#define MIN_SIZE (MAX_MEM_SIZE >>(BUDDY_LVLS - 1))     //It's like dividing the max size by 2^9, that is the number of levels. So >I obtain the minimum size
#define N_BUDDIES ((1 << BUDDY_LVLS) - 1)   //2^9 - 1 = 511
#define PAGE_SIZE 4096  //as it's the most common page size
#define BITSET_SIZE (N_BUDDIES + 7) >> 3
#define DIVIDER ("---------------------------------------------------------------------------------------------------------------\n")

int main(){
    int buddy_tests, mmap_tests = 0;
    int max_buddy_tests = 4;
    int max_mmap_tests = 3;
    allocator_init();
    printf("Allocator it's been initialized\n");

    //now i'm gonna test the my_malloc funciton with different sizes
    printf("TESTING my_malloc allocation with buddy allocator (dim > 1/4 of page_dim)\n");

    printf("Test 1: size = 8 bytes\n");
    char *my_pointer1 = (char *)my_malloc(8);   //8 bytes < 1/4 of the page size, so it should use buddy
    size_t size = 8;
    if (my_pointer1 == NULL){
        printf("Test 1 failed\n");
        return -1;
    }
    printf("Test 1 passed, %d bytes have been allocated\n", size);
    buddy_tests ++;

    printf(DIVIDER);

    printf("Test 2: size = 32+4bytes\n");
    char *my_pointer2 = (char *)my_malloc(32);  //32 bytes < 1/4 of the page size, so it should use buddy
    size = 32;
    if (my_pointer2 == NULL){
        printf("Test 2 failed\n");
        return -1;
    }
    printf("Test 2 passed, %d bytes have been allocated\n", size);
    buddy_tests ++;

    printf(DIVIDER);

    printf("Test 3: size = 256+4 bytes\n");
    char *my_pointer3 = (char *)my_malloc(256); //256 bytes < 1/4 of the page size, so it should use buddy
    size = 256;
    if (my_pointer3 == NULL){
        printf("Test 3 failed\n");
        return -1;
    }
    printf("Test 3 passed, %d bytes have been allocated\n", size);
    buddy_tests ++;

    printf(DIVIDER);

    printf("Test 4: size = 1020+4 bytes\n");
    char *my_pointer4 = (char *)my_malloc(1020);    //1020 bytes < 1/4 of the page size, so it should use buddy
    size = 1020;
    if (my_pointer4 == NULL){
        printf("Test 4 failed\n");
        return -1;
    }
    printf("Test 4 passed, %d bytes have been allocated\n", size);
    buddy_tests ++;

    printf(DIVIDER);

    printf("now i get to free the memory allocated\n");
    my_free(my_pointer1, 8);
    my_free(my_pointer2, 32);
    my_free(my_pointer3, 256);
    my_free(my_pointer4, 1020);
    printf("All the memory allocated has now been freed\n");

    printf(DIVIDER);

    printf("TESTING my_malloc allocation with mmap (dim > 1/4 of page_dim)\n");

    printf("Test 1: size = 1024+4 bytes\n");
    char *my_pointer5 = (char *)my_malloc(1024);    //1024 bytes > 1/4 of the page size, so it should use mmap
    size = 1024;
    if (my_pointer5 == NULL){
        printf("Test 1 failed\n");
        return -1;
    }
    printf("Test 1 passed, %d bytes have been allocated\n", size);
    mmap_tests ++;

    printf(DIVIDER);

    printf("Test 2: size = 2048+4 bytes\n");
    char *my_pointer6 = (char *)my_malloc(2048);    //2048 bytes > 1/4 of the page size, so it should use mmap
    size = 2048;
    if (my_pointer6 == NULL){
        printf("Test 2 failed\n");
        return -1;
    }
    printf("Test 2 passed, %d bytes have been allocated\n", size);
    mmap_tests ++;

    printf(DIVIDER);

    printf("Test 3: size = 4096+4 bytes\n");
    char *my_pointer7 = (char *)my_malloc(4096);   //4096 bytes > 1/4 of the page size, so it should use mmap
    size = 4096;
    if (my_pointer7 == NULL){
        printf("Test 3 failed\n");
        return -1;
    }
    printf("Test 3 passed, %d bytes have been allocated\n", size);
    mmap_tests ++;

    printf(DIVIDER);

    printf("now i get to free the memory allocated\n");

    my_free(my_pointer5, 1024);
    my_free(my_pointer6, 2048);
    my_free(my_pointer7, 4096);
    printf("All the memory allocated has now been freed\n");

    printf(DIVIDER);

    printf("Buddy allocator tests passed: %d/%d\n", buddy_tests, max_buddy_tests);
    printf("Mmap tests passed: %d/%d\n", mmap_tests, max_mmap_tests);
    printf("Tests passed : %d/%d\n", buddy_tests + mmap_tests, max_buddy_tests + max_mmap_tests);
    int total_tests = buddy_tests + mmap_tests;
    int total_max_tests = max_buddy_tests + max_mmap_tests;
    printf("Tests failed : %d\n", total_max_tests - total_tests);



}
