#include "../include/allocator.h"
#include "../include/buddy_allocator.h"
#include "../include/bit_map.h"
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
#define DIVIDER ("---------------------------------------------------------------------------------------------------------------\n")

int main(){
    printf("tests started\n");
    int buddy_tests = 0, mmap_tests = 0;
    int max_buddy_tests = 4;
    int max_mmap_tests = 3;
    allocator_init();
    printf("Allocator it's been initialized\n");

    //now i'm gonna test the my_malloc funciton with different sizes
    printf("TESTING my_malloc allocation with buddy allocator (dim > 1/4 of page_dim)\n");

    printf("Test 1: size = 4+4 bytes\n");
    char *my_pointer1 = (char *)my_malloc(4);   //8 bytes < 1/4 of the page size, so it should use buddy
    size_t size = 4;
    if (my_pointer1 == NULL){
        printf("Test 1 failed\n");
        return -1;
    }
    printf("Test 1 passed, %ld bytes have been allocated\n", size);
    buddy_tests ++;

    printf(DIVIDER);

    printf("Test 2: size = 32+4bytes\n");
    char *my_pointer2 = (char *)my_malloc(32);  //32 bytes < 1/4 of the page size, so it should use buddy
    size = 32;
    if (my_pointer2 == NULL){
        printf("Test 2 failed\n");
        return -1;
    }
    printf("Test 2 passed, %ld bytes have been allocated\n", size);
    buddy_tests ++;

    printf(DIVIDER);

    printf("Test 3: size = 256+4 bytes\n");
    char *my_pointer3 = (char *)my_malloc(256); //256 bytes < 1/4 of the page size, so it should use buddy
    size = 256;
    if (my_pointer3 == NULL){
        printf("Test 3 failed\n");
        return -1;
    }
    printf("Test 3 passed, %ld bytes have been allocated\n", size);
    buddy_tests ++;

    printf(DIVIDER);

    printf("Test 4: size = 1020+4 bytes\n");
    char *my_pointer4 = (char *)my_malloc(1020);    //1020 bytes < 1/4 of the page size, so it should use buddy
    size = 1020;
    if (my_pointer4 == NULL){
        printf("Test 4 failed\n");
        return -1;
    }
    printf("Test 4 passed, %ld bytes have been allocated\n", size);
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
    printf("Test 1 passed, %ld bytes have been allocated\n", size);
    mmap_tests ++;

    printf(DIVIDER);

    printf("Test 2: size = 2048+4 bytes\n");
    char *my_pointer6 = (char *)my_malloc(2048);    //2048 bytes > 1/4 of the page size, so it should use mmap
    size = 2048;
    if (my_pointer6 == NULL){
        printf("Test 2 failed\n");
        return -1;
    }
    printf("Test 2 passed, %ld bytes have been allocated\n", size);
    mmap_tests ++;

    printf(DIVIDER);

    printf("Test 3: size = 4096+4 bytes\n");
    char *my_pointer7 = (char *)my_malloc(4096);   //4096 bytes > 1/4 of the page size, so it should use mmap
    size = 4096;
    if (my_pointer7 == NULL){
        printf("Test 3 failed\n");
        return -1;
    }
    printf("Test 3 passed, %ld bytes have been allocated\n", size);
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


/*
#include "../include/allocator.h"
#include "../include/buddy_allocator.h"
#include "../include/bit_map.h"
#include <stdio.h>

#define SEPARATOR "----------------------------------------\n"

void test_my_malloc()
{
    void *ptr;
    for (int size = 1; size <= MAX_MEM_SIZE * 5; size <<= 1)
    {
        printf("Testing my_malloc() with size: %d\n", size);
        ptr = my_malloc(size);
        printf("Allocated ptr: %p\n", ptr);
        printf("Testing my_free() with ptr: %p\n", ptr);
        my_free(ptr, size);
        printf("Freed ptr: %p\n", ptr);
        printf(SEPARATOR);
    }
}

void test_my_malloc_mmap()
{
    int NUM_TESTS = 10;
    int SIZE = 1 << 30;
    void *ptr[NUM_TESTS];

    printf("Testing my_malloc() with %d pointers of fixed size: %d\n", NUM_TESTS, SIZE);
    printf("All pointers should be allocated from mmap()\n");
    for (int i = 0; i < NUM_TESTS; i++)
    {
        ptr[i] = my_malloc(SIZE);
        printf("Allocated ptr %d : %p\n", i, ptr[i]);
    }
    printf(SEPARATOR);

    printf("Freeing pointers\n");
    for (int i = 0; i < NUM_TESTS; i++)
    {
        printf("Freeing ptr %d : %p\n", i, ptr[i]);
        my_free(ptr[i],SIZE);
    }
    printf(SEPARATOR);
}

void test_my_malloc_buddy_allocator()
{
    int NUM_TESTS = 10;
    int SIZE = PAGE_SIZE / 4 - 1;
    void *ptr[NUM_TESTS];

    printf("Testing my_malloc() with %d pointers of fixed size: %d\n", NUM_TESTS, SIZE);
    printf("All pointers should be allocated from BuddyAllocator\n");
    for (int i = 0; i < NUM_TESTS; i++)
    {
        ptr[i] = my_malloc(SIZE);
        printf("Allocated ptr %d : %p\n", i, ptr[i]);
    }
    printf(SEPARATOR);

    printf("Freeing pointers\n");
    for (int i = 0; i < NUM_TESTS; i++)
    {
        printf("Freeing ptr %d : %p\n", i, ptr[i]);
        my_free(ptr[i],SIZE);
    }
    printf(SEPARATOR);
}
int main()
{
    printf(SEPARATOR);
    printf("Testing my_malloc.c\n");
    printf("Page size: %d\n", PAGE_SIZE);
    printf("Requests with size < %d will be handled by BuddyAllocator\n", PAGE_SIZE / 4);
    printf("Requests with size >= %d will be handled by mmap()\n", PAGE_SIZE / 4);
    printf(SEPARATOR);
    allocator_init();

    test_my_malloc();
    test_my_malloc_mmap();
    test_my_malloc_buddy_allocator();

    return 0;
}
*/