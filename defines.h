#ifndef DEFINES_H
#define DEFINES_H

#include "ColourConsts.h"

// #define NDEBUG

typedef int Elem_t;
const char* statuses[] = {
                         KGRN "OK" KNRM,
                         KRED "DELETED" KNRM,
                         KRED "NOT INITIALIZED" KNRM,
                         KRED "ERROR" KNRM
                         };
char undefined_name[]  = KRED "?" KNRM;
char poisoned[]        = KRED "poisoned" KNRM;

#define POISONED          0xBADDED
#define UNDEFINED_NAME    undefined_name
#define UNDEFINED_LINE    -1
#define MAX_NAMESIZE      20
#define MIN_CAPACITY      10
#define OK_STATUS         statuses[0]
#define DELETED_STATUS    statuses[1]
#define N_INIT_STATUS     statuses[2]
#define ERROR_STATUS      statuses[3]
#define MASK_1            1
#define MASK_2            3
#define MASK_3            7
#define MASK_4            15
#define MASK_5            31
#define MASK_6            63
#define MASK_7            127
#define MASK_8            255

# ifndef NDEBUG

    #define PRINT( cmd )                                                                          \
    {                                                                                             \
        fprintf (stderr, "\"%s\" in line %d in file \"%s\" in %s\n",                              \
                 #cmd, __LINE__, __FILE__, __PRETTY_FUNCTION__);                                  \
        cmd;                                                                                      \
    }

    #define  ASSERT( condition )                                                                  \
    {                                                                                             \
        if (!(condition))                                                                         \
            fprintf (stderr, "Error in '%s' in line %d in file \"%s\" in %s\n",                   \
                     #condition, __LINE__, __FILE__, __PRETTY_FUNCTION__);                        \
    }

            // char status[] = DecodeStatus(StackError(stack_ptr))                              \

    #define ASSERT_OK( stack_ptr )                                                                \
    {                                                                                             \
        if (StackError(stack_ptr))                                                                \
        {                                                                                         \
            fprintf (stderr, "\"%s\" at \"%s\"(%d)\n", __PRETTY_FUNCTION__, __FILE__, __LINE__);  \
            stack_ptr->status = ERROR_STATUS;                                                     \
            StackDump(stack_ptr);                                                                 \
        }                                                                                         \
                                                                                                  \
        else                                                                                      \
            stack_ptr->status = OK_STATUS;                                                        \
    }

#else
    #define ASSERT(    condition ) {}
    #define ASSERT_OK( stack )     {}

#endif

#define StackCtor(stack)                                                                          \
        {                                                                                         \
            StackCtor_(&stack);                                                                   \
            stack.info = {(char*) #stack, __FILE__, (size_t) __LINE__};                           \
        }

#endif
