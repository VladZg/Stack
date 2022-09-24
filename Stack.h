#ifndef STACK_H
#define STACK_H

#include "Config.h"

// void create_masks(int masks[])
// {
//     for (int i = 0, num = 1; i < 8; i++)
//     {
//         num <<= 1;
//         masks[i] = num - 1;
//     }
// }
//
// int masks[8] = {};
// create_masks(masks);

const int masks[8] = {1, 2, 4, 8, 16, 32, 64, 128};
const char* errors = {};

#define MASKS     masks
#define INIT_MASK 254
#define DEL_MASK  253

struct StackInfo
{
    char*       name     = UNDEFINED_NAME;
    // char*  function_name;
    const char* filename = __FILE__;
    int         line     = UNDEFINED_LINE;
};

struct Stack
{
    Elem_t*     data;
    size_t      size;
    size_t      capacity;
    StackInfo   info;
    const char* status = N_INIT_STATUS;
};

void   DecodeError  (int error);
void   FillPoisons  (Stack* stack, size_t start_index);
int    StackError   (Stack* stack);
void   StackDump    (Stack* stack);
void   StackCtor_   (Stack* stack);
void   StackDtor    (Stack* stack);
void   StackRealloc (Stack* stack, int direction);
Elem_t StackPop     (Stack* stack, int* err = NULL);
void   StackPush    (Stack* stack, const Elem_t value);

static void StackPushN(Stack* stack, size_t n);
static void FillStack (Stack* stack);
static void StackPopN (Stack* stack, size_t n);
static void CleanStack(Stack* stack);

#endif
