#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "defines.h"

struct StackInfo
{
    char*        name    = UNDEFINED_NAME;
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

// int    DecodeError  (int error);
void   FillPoisons  (Stack* stack, size_t start_index);
int    StackError   (Stack* stack);
void   StackDump    (Stack* stack);
void   StackCtor_   (Stack* stack);
void   StackDtor    (Stack* stack);
void   StackRealloc (Stack* stack, int direction);
Elem_t StackPop     (Stack* stack);
void   StackPush    (Stack* stack, const Elem_t value);

static void StackPushN(Stack* stack, size_t n);
static void FillStack (Stack* stack);
static void StackPopN (Stack* stack, size_t n);
static void CleanStack(Stack* stack);


int main()
{
    Stack stk1 = {};
    Stack stk2 = {};

    // StackDump(&stk1);

    StackCtor(stk1);
    StackCtor(stk2);

    StackPushN(&stk1, 74);
    StackDump(&stk1);

    StackPopN(&stk1, 45);
    StackDump(&stk1);

    StackDtor(&stk1);
    StackDtor(&stk2);

//     StackDump(&stk1);
//     StackDump(&stk2);

    return 0;
}


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


int StackError(Stack* stack)
{
    int err = 0;

    if (stack == NULL)
        err |= MASK_1;

    if (stack->size > stack->capacity)
        err |= MASK_2;

    if (stack->capacity < 0)
        err |= MASK_3;

    if (stack->size < 0)
        err |= MASK_4;

    // if (stack->status == N_INIT_STATUS)
    //     err |= MASK_5;

    if (stack->status == DELETED_STATUS)
        err |= MASK_6;

    if (stack->data == nullptr)
        err |= MASK_7;

    return err;
}

void StackDump(Stack* stack)
{
    ASSERT(stack != NULL);

    printf("Stack[%p] (%s) \"%s\" at \"%s\"(%d)  \n", stack, stack->status, stack->info.name, stack->info.filename, stack->info.line);
    printf("{                                    \n");
    printf("    size     = %lu                   \n", stack->size);
    printf("    capacity = %lu                   \n", stack->capacity);
    printf("    data[%p]                         \n", &(stack->data));
    printf("    {                                \n");

    for (int i = 0; i < stack->capacity; i++)

        if (stack->data[i] != POISONED)
        {
            printf("        *[%d] = ", i);
            print_stack_elem(stack->data[i]);
            printf("\n");
        }

        else
        {
            printf("         [%d] = ", i);
            print_stack_elem(stack->data[i]);
            printf(" (poison)\n");
        }

    printf("    }                                \n");
    printf("}                                    \n\n");
}

void FillPoisons(Stack* stack, size_t start_index)
{
    for (size_t i = start_index; i < stack->capacity; i++)
        stack->data[i] = POISONED;
}

void StackCtor_(Stack* stack)
{
    ASSERT(stack != NULL);

    stack->data = (Elem_t*) calloc(MIN_CAPACITY, sizeof(Elem_t));
    stack->capacity = MIN_CAPACITY;
    stack->size = (size_t) 0;

    FillPoisons(stack, 0);

    if (!StackError(stack))
        stack->status = OK_STATUS;
    else
        stack->status = N_INIT_STATUS;
    // stack->status = OK_STATUS;
}

void StackDtor(Stack* stack)
{
    ASSERT(stack != NULL);

    free(stack->data);
    stack->size     = (size_t) NULL;
    stack->capacity = (size_t) NULL;
    stack->status   = DELETED_STATUS;
}

void StackRealloc(Stack* stack, size_t capacity)
{
    ASSERT(stack != NULL);

    stack->capacity = capacity;
    stack->data = (Elem_t*) realloc(stack->data, capacity * sizeof(Elem_t));
}

Elem_t StackPop(Stack* stack)
{
    ASSERT_OK(stack);

    Elem_t popped = (Elem_t) NULL;

    if (stack->size > 0)
    {
        popped = stack->data[stack->size - 1];
        stack->data[stack->size - 1] = POISONED;

        if ((stack->capacity / stack->size-- >= 2) && (stack->capacity > MIN_CAPACITY))
            StackRealloc(stack, stack->capacity / 2);
    }

    ASSERT_OK(stack);

    return popped;
}

void StackPush(Stack* stack, const Elem_t value)
{
    ASSERT_OK(stack);

    if (stack->size >= stack->capacity)
        StackRealloc(stack, stack->capacity * 2);
        FillPoisons(stack, stack->size + 1);

    stack->data[stack->size++] = value;

    ASSERT_OK(stack);
}


static void StackPushN(Stack* stack, size_t n)
{
    for (size_t i = 1; i <= n; i++)
    {
        StackPush(stack, rand());
    }
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
