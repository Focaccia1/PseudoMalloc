#pragma once
#include <stdint.h>

#define MAX_MEM_SIZE (1 << 20)   //1024*1024 as the example showed in classes
#define BUDDY_LVLS 9    //10 cause there is 0
#define MIN_SIZE (MAX_MEM_SIZE >>(BUDDY_LVLS - 1))     //It's like dividing the max size by 2^9, that is the number of levels. So >I obtain the minimum size
#define N_BUDDIES ((1 << BUDDY_LVLS) - 1)   //2^9 - 1 = 511
#define PAGE_SIZE 4096  //as it's the most common page size
