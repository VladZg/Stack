#ifndef CONFIG_H
#define CONFIG_H

#include "Consts.h"

// #define RELEASE_MODE
// #define NDEBUG
#define NLOGGING
// #define N_HASH_PROTECTION
#define N_CANARY_PROTECTION

#ifdef RELEASE_MODE

    #define  NDEBUG
    #define  NLOGGING
    #define  N_HASH_PROTECTION
    #define  N_CANARY_PROTECTION

#endif

typedef int Elem_t;

#endif
