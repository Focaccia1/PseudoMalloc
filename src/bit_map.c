#include <assert.h>
#include "bit_map.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int BitMap_getBytes(int bits){  //calcolo i byte necessari per memorizzare un numero x di bit(preso in input)
  return bits/8 + (bits%8)!=0;  //bits/8 mi da il numero di byte e poi controllo se serve aggiungere un altro byte extra epr bit avanzati
}

void BitMap_init(BitMap* bit_map, int num_bits, uint8_t* buffer){       //inizializzo la bitmap con un array
  bit_map->buffer=buffer;
  bit_map->num_bits=num_bits;
  bit_map->buffer_size=BitMap_getBytes(num_bits);
}


void BitMap_setBit(BitMap* bit_map, int bit_num, int status){   //setto un bit specifico nella bitmap, calcolo l byte che contiene il bit in questione
                                                                //e poi lo setto in base allo status in unput

  int byte_num=bit_num>>3;  //come se dividessi per 2^3, cioè 8, per ottenere il numero di byte
  assert(byte_num<bit_map->buffer_size);    //controllo che il byte sia minore del numero di byte della bitmap
  int bit_in_byte=byte_num&0x03;    //AND bit a bit per ottenere il bit in questione
  if (status) {                    //se lo status è 1 allora setto il bit
    bit_map->buffer[byte_num] |= (1<<bit_in_byte);
  } else {                       //altrimenti lo resetto (eseguo not bitwise)
    bit_map->buffer[byte_num] &= ~(1<<bit_in_byte);
  }
}

// inspects the status of the bit bit_num
int BitMap_bit(const BitMap* bit_map, int bit_num){
  int byte_num=bit_num>>3; //trovo numero del byte
  assert(byte_num<bit_map->buffer_size);    //controllo che il byte sia minore del numero di byte della bitmap
  int bit_in_byte=byte_num&0x03;    //AND bit a bit per ottenere il bit in questione
  return (bit_map->buffer[byte_num] & (1<<bit_in_byte))!=0; //ritorno il bit in questione(1 acceso, 0 spento)
}
