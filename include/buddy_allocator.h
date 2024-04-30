#pragma once
#include "bit_map.h"

typedef struct{
    int min_block_size; // dim minima del blocco
    char* memory; // puntatore alla memoria allocata
    int lvl; // livello del blocco
    BitMap* bitmap; // puntatore alla bitmap (vd bit_map.h)
}buddy_allocator_t;

    //DICHIARAZIONI DI FUNZIONI

int get_level(int index);    //posso calcolare il floor del log2 per avere l'indice, sapendo ch il mio albero è sicuramente completo

int get_parent(int index);

int get_buddy(int index);     //per trovare index del buddy di un blocco dato(index input)

int get_left_child(int index);//calcolo figlio sx

int get_right_child(int index);//calcolo figlio dx

int get_first_buddy(int index);//calcolo primo buddy

int first_index_from_level(int level); //calcolo primo indice da livello

void check_index(buddy_allocator_t * buddy_allocator ,int index);    //controlla se index è nei limiti del buddy [0, 2^(buddy->lvl)]

void buddy_allocator_init(buddy_allocator_t *buddy_allocator, char *buffer, int lvl, int min, uint8_t *buffer_dim);    //initialize allocator

int search_available_block(BitMap* bitmap, int lvl);     //cerca blocco libero al livello "lvl"

int search_available_level(buddy_allocator_t * buddy_allocator, int dim); //check for the fittest available level

int get_available_buddy_index(buddy_allocator_t *buddy_allocator, int lvl); //recursive function to find a free index

void *get_buddy_allocator_address(buddy_allocator_t* buddy_allocator, int lvl, int index); //get the address of the buddy allocator

void *buddy_allocator_pseudo_malloc(buddy_allocator_t *buddy_allocator, int dim); //pseudo malloc function

void free_buddy(buddy_allocator_t* buddy_allocator, int index);   //free blocks

void free_buddy_pointer(buddy_allocator_t* buddy_allocator, void* mem); //free pointer








