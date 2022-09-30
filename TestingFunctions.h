#ifndef TESTINGFUNCTIONS_H
#define TESTINGFUNCTIONS_H

#include "Stack.h"
#include "Config.h"

void StackPushN(Stack* stack, size_t n)
{
    for (size_t i = 1; i <= n; i++)
        StackPush(stack, (Elem_t) NULL);
}

void FillStack(Stack* stack)
{
    StackPushN(stack, stack->capacity - stack->size);
}

void StackPopN(Stack* stack, size_t n)
{
    for (size_t i = 1; i <= n; i++)
        StackPop(stack);
}

void CleanStack(Stack* stack)
{
    StackPopN(stack, stack->size);
}

#endif
