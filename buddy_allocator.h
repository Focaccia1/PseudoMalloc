#pragma once
#include "bit_map.h"

typedef struct{
    int min_block_size; // dim minima del blocco
    char* memory; // puntatore alla memoria allocata
    int lvl; // livello del blocco
    BitMap* bitmap; // puntatore alla bitmap (vd bit_map.h)
}buddy_allocator_t;

int get_level(int index);    //posso calcolare il floor del log2 per avere l'indice, sapendo ch il mio albero è sicuramente completo

int get_parent(int index);

int get_buddy(int index);     //per trovare index del buddy di un blocco dato(index input)

int get_left_child(int index);//calcolo figlio sx

int get_right_child(int index);//calcolo figlio dx

int get_first_buddy(int index);//calcolo primo buddy

void check_index(buddy_allocator_t * buddy_allocator ,int index);    //controlla se index è nei limiti del buddy [0, 2^(buddy->lvl)]

void buddy_allocator_init(buddy_allocator_t *buddy_allocator, void *buffer, BitMap *bitmap, int lvl, int min);    //inizializza

int search_available_block(BitMap* bitmap, int lvl);     //cerca blocco libero al livello "lvl"
