// #include <assert.h>
// #include "bit_map.h"
// #include <stdio.h>
// #include <stdlib.h>
// #include <stdint.h>

// //#include "../include/allocator.h"
// //#include "../include/buddy_allocator.h"
// #include "../include/bit_map.h"

// int BitMap_getBytes(int bits){  //calcolo i byte necessari per memorizzare un numero x di bit(preso in input)
//   return bits/8 + (bits%8)!=0;  //bits/8 mi da il numero di byte e poi controllo se serve aggiungere un altro byte extra epr bit avanzati
// }

// void BitMap_init(BitMap* bit_map, int num_bits, uint8_t* buffer){       //inizializzo la bitmap con un array
//   bit_map->buffer=buffer;
//   bit_map->num_bits=num_bits;
//   bit_map->buffer_size=BitMap_getBytes(num_bits);
// }


// void BitMap_setBit(BitMap* bit_map, int bit_num, int status){   //setto un bit specifico nella bitmap, calcolo l byte che contiene il bit in questione
//                                                                 //e poi lo setto in base allo status in unput

//   int byte_num=bit_num>>3;  //come se dividessi per 2^3, cioè 8, per ottenere il numero di byte
//   assert(byte_num<bit_map->buffer_size);    //controllo che il byte sia minore del numero di byte della bitmap
//   int bit_in_byte=byte_num&0x03;    //AND bit a bit per ottenere il bit in questione
//   if (status) {                    //se lo status è 1 allora setto il bit
//     bit_map->buffer[byte_num] |= (1<<bit_in_byte);
//   } else {                       //altrimenti lo resetto (eseguo not bitwise)
//     bit_map->buffer[byte_num] &= ~(1<<bit_in_byte);
//   }
// }

// // inspects the status of the bit bit_num
// int BitMap_bit(const BitMap* bit_map, int bit_num){
//   int byte_num=bit_num>>3; //trovo numero del byte
//   assert(byte_num<bit_map->buffer_size);    //controllo che il byte sia minore del numero di byte della bitmap
//   int bit_in_byte=byte_num&0x03;    //AND bit a bit per ottenere il bit in questione
//   return (bit_map->buffer[byte_num] & (1<<bit_in_byte))!=0; //ritorno il bit in questione(1 acceso, 0 spento)
// }


#include "bit_map.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>


void BitMap_init(BitMap * bm, uint8_t * buff ,int dim_buffer){
    bm->buffer = buff; // pointer to buffer
    bm->buffer_size = dim_buffer; // buffer dimension
    bm->num_bits = dim_buffer * 8; // bits number
}

void BitMap_setBit(BitMap * bm, int pos, int boolean){

    if(pos >= bm->num_bits){ // if the position is greater than the bits number
        errno = EINVAL; // set errno as invalid argument
        perror("error bit_map_set"); // stampo l'errore
    }

    int byte = pos / 8; // calculate byte index that contains the bit to set
    int bit = pos % 8; // define the offset inside the byte

    if(boolean == 0){ // 0 -> busy
        bm->buffer[byte] &= ~(1 << bit); // set the bit to 0 
    }
    else{
        bm->buffer[byte] |= 1 << bit; // setto il bit a 1 
    }
}

int BitMap_bit(BitMap * bm, int pos){

    if(pos >= bm->buffer_size*8){ 
        printf("max bit number exceeded  %d\n", bm->buffer_size*8);
        perror("error bit_map_get");
        
    }

    int byte = pos / 8; //  calculate byte index that contains the bit to get
    int bit = pos % 8; // define the offset inside the byte

    return (bm->buffer[byte] >> bit) & 1; // return the bit in position pos, the bit-bit and gives me just the less significant bit
}