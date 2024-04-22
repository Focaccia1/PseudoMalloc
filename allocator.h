#pragma once

#define MEM_SIZE 4096   //most common atm
#define BUDDY_LVLS 9    //10 cause there is 0
#define MIN_SIZE (MEM_SIZE >>(BUDDY_LVLS - 1))
#define N_BUDDIES ((1 << BUDDY_LVLS) - 1)
#define BITSET_SIZE ((N_BUDDIES + 7) << 3)

