#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "Defines.h"
#include "Config.h"
#include "Stack.h"

static void StackPushN(Stack* stack, size_t n);
static void FillStack (Stack* stack);
static void StackPopN (Stack* stack, size_t n);
static void CleanStack(Stack* stack);


int main()
{
    Stack stk1 = {};
    Stack stk2 = {};

    StackCtor(stk1);
    StackCtor(stk2);
    // StackCtor(stk1;

    // stk1.capacity = 0;
    // stk1.size = -1;
    // stk1.data=NULL;
    // StackPushN(&stk1, 13);
    // StackDump(&stk1);
    // FillStack(&stk1);
    // StackDump(&stk1);


    StackDtor(&stk1);
    StackDtor(&stk2);

    // StackPush(&stk1, 78);
    // StackDtor(&stk1);

    return 0;
}


int StackError(Stack* stack)
{
    int err = 0;

    if (stack->status == N_INIT_STATUS)
    {
        err |= masks[0];
        return err;
    }

    if (stack->status == DELETED_STATUS)
    {
        err |= masks[1];
        return err;
    }

    if (stack->status == ALR_INIT_STATUS)
    {
        err |= masks[2];
        return err;
    }

    if (stack == NULL)
        err |= masks[3];

    if (stack->size > stack->capacity)
        err |= masks[4];

    if (stack->capacity < MIN_CAPACITY)
        err |= masks[5];

    if (stack->size < 0)
        err |= masks[6];

    if (stack->data == nullptr)
        err |= masks[7];

    if (err)
        stack->status = ERROR_STATUS;

    return err;
}

void DecodeError(int error)
{
    for (int i = 0; i < 8; i++)
        if (error & masks[i])
            fprintf(stderr, "%s", errors[i]);
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

        if (stack->data[i] != stack->poisoned)
        {
            printf("        *[%d] = " KMAG, i);
            print_stack_elem(stack->data[i]);
            printf(KNRM "\n");
        }

        else
        {
            printf("         [%d] = " KBLU, i);
            print_stack_elem(stack->data[i]);
            printf(" (poison)\n" KNRM);
        }

    printf("    }                                \n");
    printf("}                                    \n\n");
}

void FillPoisons(Stack* stack, size_t start_index)
{
    for (size_t i = start_index; i < stack->capacity; i++)
        stack->data[i] = stack->poisoned;
}

void StackCtor_(Stack* stack)
{
    ASSERT(stack != NULL);

    if (stack->status == OK_STATUS)
        stack->status = ALR_INIT_STATUS;

    int err = StackError(stack);

    if (!(err & INIT_MASK))
    {
        stack->data     = (Elem_t*) calloc(MIN_CAPACITY, sizeof(Elem_t));
        stack->capacity = MIN_CAPACITY;
        stack->size     = (size_t) 0;

        Elem_t poisoned_elem = (Elem_t) NULL;
        poisoned(&poisoned_elem);
        stack->poisoned = poisoned_elem;

        FillPoisons(stack, 0);

        stack->status   = OK_STATUS;
    }

    else if (!(err & DEL_MASK))
        stack->status = N_INIT_STATUS;

    ASSERT_OK(stack);
}

void StackDtor(Stack* stack)
{
    ASSERT(stack != NULL);
    ASSERT_OK(stack);

    if (stack->status != DELETED_STATUS)
    {
        free(stack->data);
        stack->size     = (size_t) NULL;
        stack->capacity = (size_t) NULL;
        stack->status   = DELETED_STATUS;
    }
}

void StackRealloc(Stack* stack, size_t capacity)
{
    ASSERT(stack != NULL);

    stack->capacity = capacity;
    stack->data = (Elem_t*) realloc(stack->data, capacity * sizeof(Elem_t));
}

Elem_t StackPop(Stack* stack, int* err)
{
    ASSERT_OK(stack);

    Elem_t popped = (Elem_t) NULL;

    if (stack->size > 0)
    {
        popped = stack->data[stack->size - 1];
        stack->data[stack->size - 1] = stack->poisoned;

        if ((stack->capacity / stack->size-- >= 2) && (stack->capacity > MIN_CAPACITY))
            StackRealloc(stack, stack->capacity / 2);
    }

    else if (err != NULL)
        *err = 1;

    ASSERT_OK(stack);

    return popped;
}

int CheckElem(Stack* stack, Elem_t value)
{
    return !(sizeof(stack->poisoned) - sizeof(value));
}

void StackPush(Stack* stack, const Elem_t value)
{
    ASSERT_OK(stack);

    if (CheckElem(stack, value))
    {
        if (stack->size >= stack->capacity)
            StackRealloc(stack, stack->capacity * 2);
            FillPoisons(stack, stack->size + 1);

        stack->data[stack->size++] = value;
    }
    else
    {
        fprintf(stderr, "%s", errors[8]);
        abort();
    }

    ASSERT_OK(stack);
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
