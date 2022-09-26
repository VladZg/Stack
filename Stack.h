#ifndef STACK_H
#define STACK_H

#include "Config.h"

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
    Elem_t      poisoned;
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
int    CheckElem    (Stack* stack, Elem_t value);
void   StackPush    (Stack* stack, const Elem_t value);


void print_stack_elem(int elem)
{
    printf("%d", elem);
}

void print_stack_elem(float elem)
{
    printf("%f", elem);
}

void print_stack_elem(char elem)
{
    printf("%c", elem);
}

void print_stack_elem(char* elem)
{
    printf("%s", elem);
}


void poisoned(int* elem)
{
    *elem = int_poisoned;
}

void poisoned(float* elem)
{
    *elem = float_poisoned;
}

void poisoned(char* elem)
{
    *elem = char_poisoned;
}

void poisoned(char** elem)
{
    *elem = char_ptr_poisoned;
}


#endif
