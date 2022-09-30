#ifndef DEFINES_H
#define DEFINES_H

#include "Config.h"
#include "Log.h"

# ifndef NDEBUG

    #define PRINT( cmd )                                                       \
    {                                                                          \
        fprintf(stderr, "\"%s\" in line %d in file \"%s\" in %s\n",            \
                #cmd, __LINE__, __FILE__, __PRETTY_FUNCTION__);                \
        cmd;                                                                   \
    }

    #define  ASSERT( condition )                                               \
    {                                                                          \
        if (!(condition))                                                      \
            fprintf(stderr, "Error in '%s' in line %d in file \"%s\" in %s\n", \
                    #condition, __LINE__, __FILE__, __PRETTY_FUNCTION__);      \
    }


    #define ASSERT_OK( stack_ptr )                                             \
    {                                                                          \
        CheckHash(stack_ptr);                                                  \
                                                                               \
        size_t err = StackError(stack_ptr);                                    \
                                                                               \
        if (err)                                                               \
        {                                                                      \
            fprintf(stderr, KRED "ERROR " KNRM "in \"%s\" at \"%s\"(%d)\n",    \
                     __PRETTY_FUNCTION__, __FILE__, __LINE__);                 \
            StackDecodeErrors(err);                                            \
            StackDump(stack_ptr);                                              \
            abort();                                                           \
        }                                                                      \
                                                                               \
        else                                                                   \
            stack_ptr->status = STK_OK_STATUS;                                 \
                                                                               \
        SetStackHash(stack_ptr);                                               \
    }

#else
    #define ASSERT(    condition ) {}
    #define ASSERT_OK( stack )     {}
    #define PRINT(     cmd )       {}

#endif

#define StackCtor(stack)                                                       \
        {                                                                      \
            StackCtor_(&stack);                                                \
            stack.info = {(char*) #stack, __FILE__, (size_t) __LINE__};        \
            SetStackHash(&stack);                                              \
        }

#endif
