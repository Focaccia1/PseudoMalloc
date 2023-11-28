#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "buddy_allocator.h"
#include "bit_map.h"
#include <string.h>
#include <math.h>

int get_level(int index){   //posso calcolare il floor del log2 per avere l'indice, sapendo ch il mio albero è sicuramente completo
  return (int)floor(log2(index));
}

int get_parent(int index){
    if (index == 0)
        return -1;  //root non ha genitori
    else
        return floor((index-1)/2); //calcolo il padre
}

int get_buddy(index){  //per trovare index del buddy di un blocco dato(index input)
    if (index == 0)
        return -1; //root non ha buddy
    else if (index % 2 == 0) //se è pari -> il buddy è index -1
        return index + 1; //ritorno buddy
    else
        return index - 1; //altrimenti è dispari -> buddy è index +1
}

int get_left_child(int index){
    return (int) (2*index + 1); //calcolo figlio sinistro
}

int get_right_child(int index){
    return (int) (2*index + 2); //calcolo figlio destro
}

int get_first_buddy(int level){
    return (int) (1 << level) - 1; //calcolo primo buddy (bitwise -> 2^index)
}

void check_index(buddy_allocator_t * buddy_allocator, int index){  //controlla se index è nei limiti del buddy [0, 2^(buddy->lvl)]
  if(index < 0 || index > (1 << buddy_allocator->lvl) - 1){ 
    perror("error: invalid index");
  }
}

void buddy_allocator_init(buddy_allocator_t *buddy_allocator, void *buffer, BitMap *bitmap, int lvl, int min){    //inizializza allocator
  buddy_allocator->min_block_size = min;  //dim minima del blocco
  buddy_allocator->memory = buffer;  //ptr memoria allocata
  buddy_allocator->bitmap = bitmap;  //ptr bitmap (vd. bitmap.h)
  buddy_allocator->lvl = lvl;
}

int search_available_block(BitMap* bitmap, int lvl){  //cerca blocco libero al livello "lvl"
  printf("cerco blocco libero al livello %d\n",lvl);
  int start_index = first_index_from_level(level);
  int end_index = first_index_from_level(level + 1);
  printf("\t indice di partenza %d\n",start_index);
  printf("\t indice finale &d\n",end_index);

  // devo cercare un blocco disponibile, itero finchè non lo trovo
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
  return -1;  //se non trovo blocchi disponibili ritorno -1
}
