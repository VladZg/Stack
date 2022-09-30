#include <stdio.h>
#include <sys/stat.h>
#include <cstring>
#include <stdlib.h>
#include "Config.h"
#include "Log.h"
#include "Defines.h"
#include "Stack.h"

static void StackPushN(Stack* stack, size_t n);
static void FillStack (Stack* stack);
static void StackPopN (Stack* stack, size_t n);
static void CleanStack(Stack* stack);


int main()
{
    // OpenLog();

     // char a[] = "oops";
    // printf("%ld\n", Hash((const void*) a, strlen(a)));
    // a[1] = 'e';
    // printf("%ld\n", Hash((const void*) a, strlen(a)));
    // stringf(a);
    // printf("%ld\n", Hash((const void*) a, strlen(a)));

    Stack stk1 = {};
    Stack stk2 = {};

    // StackDump(&stk1);
    // StackDump(&stk2);

    StackCtor(stk2);
    StackCtor(stk1);

    // StackPushN(&stk1, 41);
    // StackDump(&stk1);
    // StackPush(&stk1, 3);
    // StackDump(&stk1);
    // StackPopN(&stk1, 56);
    // StackPush(&stk1, 89);
    // StackDump(&stk1);

    StackDtor(&stk1);
    StackDtor(&stk2);

    // StackDump(&stk1);
    // StackDump(&stk2);

    // StackPush(&stk1, 2);

    // CloseLog();

    return 0;
}


static void StackPushN(Stack* stack, size_t n)
{
    for (size_t i = 1; i <= n; i++)
        StackPush(stack, (Elem_t) NULL);
}

static void FillStack(Stack* stack)
{
    StackPushN(stack, stack->capacity - stack->size);
}

static void StackPopN(Stack* stack, size_t n)
{
    for (size_t i = 1; i <= n; i++)
        StackPop(stack);
}

static void CleanStack(Stack* stack)
{
    StackPopN(stack, stack->size);
}
