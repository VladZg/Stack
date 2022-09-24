#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "Defines.h"
#include "Config.h"
#include "Stack.h"

int main()
{
    Stack stk1 = {};
    Stack stk2 = {};

    // StackDump(&stk1);

    // StackDump(&stk1);
    StackCtor(stk1);
    StackCtor(stk2);
    // StackCtor(stk2);

    // FillStack(&stk1);
    // StackPush(&stk1, 8);
    // StackPush(&stk1, 12);
    // StackDump(&stk1);
    // StackPushN(&stk1, 4);
    // StackDump(&stk1);
    // StackDump(&stk1);
    // StackCtor(stk1);
    // StackDump(&stk1);

//     StackPushN(&stk1, 74);
//     StackDump(&stk1);
//
//     StackPushN(&stk1, 15);
//     StackDump(&stk1);
//
//     StackPopN(&stk1, 45);
//     StackDump(&stk1);

    StackDtor(&stk1);
    StackDtor(&stk2);

    // StackDtor(&stk1);

    // StackPop(&stk1);
    // StackDump(&stk1);
    // StackCtor(stk1);

    // StackDump(&stk1);

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

    if (stack->status == N_INIT_STATUS)
    {
        err |= MASKS[0];
        return err;
    }

    if (stack->status == DELETED_STATUS)
    {
        err |= MASKS[1];
        return err;
    }

    if (stack->status == ALR_INIT_STATUS)
    {
        err |= MASKS[2];
        return err;
    }

    if (stack == NULL)
        err |= MASKS[3];

    if (stack->size > stack->capacity)
        err |= MASKS[4];

    if (stack->capacity < MIN_CAPACITY)
        err |= MASKS[5];

    if (stack->size < 0)
        err |= MASKS[6];

    if (stack->data == nullptr)
        err |= MASKS[7];

    if (err)
        stack->status = ERROR_STATUS;

    return err;
}

void DecodeError(int error)
{
    if (error & MASKS[0])
        fprintf(stderr, KYEL "STACK WASN'T INITIALIZED\n" KNRM);

    else if (error & MASKS[1])
        fprintf(stderr, KYEL "STACK DOESN'T EXIST (DELETED)\n" KNRM);

    else if (error & MASKS[2])
        fprintf(stderr, KYEL "STACK WAS ALREADY INITIALIZED\n" KNRM);

    if (error & MASKS[3])
        fprintf(stderr, KYEL "STACK EQUALS NULL\n" KNRM);

    if (error & MASKS[4])
        fprintf(stderr, KYEL "STACK OVERFLOW\n" KNRM);

    if (error & MASKS[5])
        fprintf(stderr, KYEL "STACK UNDERFLOW (CAPACITY < MIN_CAPACITY)\n" KNRM);

    if (error & MASKS[6])
        fprintf(stderr, KYEL "STACK UNDERFLOW (SIZE < 0)\n" KNRM);

    if (error & MASKS[7])
        fprintf(stderr, KYEL "DATA EQUALS NULL\n" KNRM);
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
        stack->data[i] = POISONED;
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
        stack->data[stack->size - 1] = POISONED;

        if ((stack->capacity / stack->size-- >= 2) && (stack->capacity > MIN_CAPACITY))
            StackRealloc(stack, stack->capacity / 2);
    }

    else if (err != NULL)
        *err = 1;

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
