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

int get_buddy(index){
    if (index == 0)
        return -1; //root non ha buddy
    else if (index % 2 == 0) //se è pari -> il buddy è index -1
        return index + 1; //ritorno buddy
    else
        return index - 1; //altrimenti è dispari -> buddy è index +1
}

get_left_child(int index){
    return (int) (2*index + 1); //calcolo figlio sinistro
}

get_right_child(int index){
    return (int) (2*index + 2); //calcolo figlio destro
}

get_first_buddy(int level){
    return (int) (1 << level) - 1; //calcolo primo buddy (bitwise -> 2^index)
}

void check_index(buddy_allocator_t * buddy_allocator, int index){
  if(index < 0 || index > (1 << buddy_allocator->lvl) - 1){
    perror("error: invalid index");
  }
}

