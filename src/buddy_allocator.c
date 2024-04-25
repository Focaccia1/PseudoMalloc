#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "buddy_allocator.h"
#include "bit_map.h"
#include <string.h>
#include <math.h>

int get_level(int index){   //i can calculate floor of log2 to find index, knowing that my tree is certainly complete
  return (int)floor(log2(index));
}

int get_parent(int index){
    if (index == 0)
        return -1;  //root has no parents
    else
        return floor((index-1)/2); //father
}

int get_buddy(index){  //to find an index of a block(index input)
    if (index == 0)
        return -1; //root non ha buddy
    else if (index % 2 == 0) //if is even number -> buddy = index -1
        return index + 1; //ritorno buddy
    else
        return index - 1; //if is odd number -> buddy = index +1
}

int get_left_child(int index){
    return (int) (2*index + 1); //calcolo sx child
}

int get_right_child(int index){
    return (int) (2*index + 2); //calcolo dx child
}

int get_first_buddy(int level){
    return (int) (1 << level) - 1; //calculate first buddy (bitwise -> 2^index)
}

void check_index(buddy_allocator_t * buddy_allocator, int index){  //check if index is within limits [0, 2^(buddy->lvl)]
  if(index < 0 || index > (1 << buddy_allocator->lvl) - 1){ 
    perror("error: invalid index");
  }
}

void buddy_allocator_init(buddy_allocator_t *buddy_allocator, char *buffer, int lvl, int min, uint8_t *buffer_dim){    //initialize allocator
  buddy_allocator->min_block_size = min;  //min dim
  buddy_allocator->memory = buffer;  //ptr to allocated mem
  //buddy_allocator->bitmap = bitmap;  //ptr bitmap (vd. bitmap.h)
  buddy_allocator->lvl = lvl;

  //inizializzo la bitmap per inizializzare il buddy allocator
  int bits = (1 << lvl) - 1;  //calculate necessary bits to represent the tree (2^lvl - 1, -1 cuz tree starts from 0)
  bitmap_init(buddy_allocator->bitmap, bits, buffer_dim);  //initialize bitmap

  //now i set root(bit bitmap) at 1 if available, 0 otherwise
  BitMap_setBit(buddy_allocator->bitmap,0, 1);

  printf("INITIALIZATION COMPLETED\n");
  printf("levels: %d\n", buddy_allocator->lvl);
  printf("minimum dimension available: %d\n", buddy_allocator->min_block_size);
  printf("total memory: %d\n", ((1 << lvl) - 1) * buddy_allocator->min_block_size);
}



int search_available_block(BitMap* bitmap, int lvl){  //search for a free block at level "lvl"
  if (lvl < 0){
    perror("error: invalid level");
    return -1;
  }
  printf("cerco blocco libero al livello %d\n",lvl);
  int start_index = first_index_from_level(lvl);
  int end_index = first_index_from_level(lvl + 1);
  printf("\t indice di partenza %d\n",start_index);
  printf("\t indice finale &d\n",end_index);

  // search till i find an available block
  int i = start_index;
  while (i <=  end_index){
    printf("posizione: %d\n", i);
    printf("bit del bitmap spento/acceso(0/1): %d\n", Bit_Map_bit(bitmap, i) );
    if (Bit_Map_bit(bitmap, i) != 0){
      printf("blocco libero trovato in pos = %d\n", i);
      return i;
    }
    i++;
  }
  return -1;  //if no found, return -1
}

int search_available_level(buddy_allocator_t * buddy_allocator, int dim){ //check for the fittest available level
  printf("dim del search_available_lvl: %d\t", dim);
  int lvl = buddy_allocator->lvl - 1;
  int start_dim = buddy_allocator->min_block_size;
  while(lvl >= 0){
    printf("\t actual lvl: %d\t, dim of the lvl: %d\t, requested dim: %d\t", lvl, start_dim, dim);
    if(dim <= start_dim){
      printf("lvl found: %d\n", lvl);
      return lvl;
    }
    lvl--;
    start_dim *= 2;
  }
  return lvl; //if dim it's always bigger than the start dim, the tree is probably empty so i give the root (when lvl == 0)
}

int get_available_buddy_index(buddy_allocator_t *buddy_allocator, int lvl){ //recursive function to find a free index

  //base
  if (lvl < 0){
    return -1;  //if lvl invalid
  }

  int index = search_available_block(buddy_allocator->bitmap, lvl);  //search for a free block at level "lvl"
  if (index != -1){ //u found a free block at current lvl, so set the bitmap
    BitMap_setBit(buddy_allocator->bitmap, index, 0);  //set the bit to 0
    return index;
  }
  printf("no available boddy found, recursive call\n");
  int parent_index = get_available_buddy_index(buddy_allocator, lvl - 1);  //recursive call to find a free block at the previous level(parent check)

  if (parent_index == -1){
    return -1;  //if no parent found, return -1
  }

  printf("availability found at lvl: %d\n", lvl);

  int right_index = get_right_child(parent_index);  //calculate right child
  check_index(buddy_allocator, right_index);  //check if index is within limits [0, 2^(buddy->lvl)]

  int left_index = get_left_child(parent_index);  //calculate left child
  check_index(buddy_allocator, left_index);  //check if index is within limits [0, 2^(buddy->lvl)]

  BitMap_setBit(buddy_allocator->bitmap, right_index, 1);  //set right child to 1 (so set it as free)
  return left_index;  //return left child (first in the tree hierarchy)
}

void *get_buddy_allocator_address(buddy_allocator_t* buddy_allocator, int lvl, int index){
  int level_index = index - ((1 << lvl) -1);
  int shift = level_index * (buddy_allocator->min_block_size * 1 << (buddy_allocator->lvl - lvl -1)); //offset calculation
  return (void *)buddy_allocator->memory + shift;
}

void *buddy_allocator_pseudo_malloc(buddy_allocator_t *buddy_allocator, int dim){

  int level = search_available_level(buddy_allocator, dim + sizeof(int));  //search for the fittest available level for page with dim bytes
  if(level == -1){
    printf("didn't find a big enough block\n");
    perror("error buddy_allocator_alloc: no memory available");
    return NULL;
  }

  int index = get_available_buddy_index(buddy_allocator, level);  //get the index of the available block
  if (index == -1){
    perror("error buddy_allocator_alloc: no memory available");
    return NULL;
  }

  //if i get here i found a block, i have to allocate it
  int *address = (int *)get_buddy_allocator_address(buddy_allocator, level, index);  //get the address of the block
  *address = level;  //now save it

  return (void *)(address + 1);  //return the address of the block
}

//now the two funcions that handle the free
void free_buddy(buddy_allocator_t* buddy_allocator, int index){   //free blocks
  //check index
  check_index(buddy_allocator, index);

  BitMap_setBit(buddy_allocator->bitmap, index, 1);  //set the bit to 1

  //now check if the brother is free as well, so you can merge and free the father too
  while(index != 0){
                        // indexes                                                                   checks
    int parent_idx = get_parent(index);                   /*parent index*/        check_index(buddy_allocator, parent_idx);
    int left_child_idx = get_left_child(parent_idx);      /*left child index*/    check_index(buddy_allocator, left_child_idx);
    int right_child_idx = get_right_child(parent_idx);    /*right child index*/   check_index(buddy_allocator, right_child_idx);

    //now check if I can merge, i have to check if both children are free. If so, merge them into parent
    if(BitMap_bit(buddy_allocator->bitmap, left_child_idx) == 1 && BitMap_bit(buddy_allocator->bitmap, right_child_idx) == 1){  // 1-> free, 0-> busy
      BitMap_setBit(buddy_allocator->bitmap, parent_idx, 1);  //set the parent to 1
      BitMap_setBit(buddy_allocator->bitmap, left_child_idx, 0);  //set the left child to 0
      BitMap_setBit(buddy_allocator->bitmap, right_child_idx, 0);  //set the right child to 0
    } else {
      break;  //if i can't merge, break. (One of the children is busy)
    }
    index = parent_idx;  //set the index to the parent
  }
}

void free_buddy_pointer(buddy_allocator_t* buddy_allocator, void* mem){
  int index = *((int *) mem - 1);  //get the index of the block
  check_index(buddy_allocator, index);  //check index
  free_buddy(buddy_allocator, index);  //free
}