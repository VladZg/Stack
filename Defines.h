#ifndef DEFINES_H
#define DEFINES_H

#include "ColourConsts.h"
#include "Config.h"
#include "Stack.h"


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
        int err = StackError(stack_ptr);                                       \
                                                                               \
        if (err)                                                               \
        {                                                                      \
            fprintf(stderr, KRED "ERROR " KNRM "in \"%s\" at \"%s\"(%d)\n",    \
                     __PRETTY_FUNCTION__, __FILE__, __LINE__);                 \
            DecodeError(err);                                                  \
            StackDump(stack_ptr);                                              \
        }                                                                      \
                                                                               \
        else                                                                   \
            stack_ptr->status = OK_STATUS;                                     \
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
        }

#endif
