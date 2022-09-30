#ifndef STACK_H
#define STACK_H

#include "Config.h"
#include "Log.h"
#include "Defines.h"

struct StackInfo
{
    char*       name     = UNDEFINED_NAME;
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
    size_t      hash;
};

// void   DecodeError  (int error);
// void   FillPoisons  (Stack* stack, size_t start_index);
// int    StackError   (Stack* stack);
// void   StackDump    (Stack* stack);
// void   StackCtor_   (Stack* stack);
// void   StackDtor    (Stack* stack);
// void   StackRealloc (Stack* stack, int direction);
Elem_t StackPop     (Stack* stack, int* err = NULL);
// int    CheckElem    (Stack* stack, Elem_t value);
// void   StackPush    (Stack* stack, const Elem_t value);
// size_t Hash         (const void* obj, size_t n);
// void   SetStackHash (Stack* stack);
// void   CheckHash    (Stack* stack);


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


// #ifndef NDEBUG

size_t Hash(const void* obj, size_t len)
{
    size_t hash = 0;
    const char* obj_start = (const char*) obj;

// size_t c1 = 0xcc9e2d51;
// size_t c2 = 0x1b873593;
// size_t r1 = 15;
// size_t r2 = 13;
// size_t m  = 5;
// size_t n  = 0xe6546b64;

    char k = (char) NULL;

    for (size_t i = 0; i < len; i++)
    {
//         k = (char) obj_start[i];
//
//         k *= 0xcc9e2d51;
//         k = (k << 15) | (k >> 17);
//         k *= 0x1b873593;
//
//         hash = (hash << 13) | (hash >> 19);
//         hash = hash * 5 + 0xe6546b64;

        hash += (char) obj_start[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
//     k *= 0xcc9e2d51;
//     k = (k << 15) | (k >> 17);
//     k *= 0x1b873593;
//
//     hash ^= n;
//
//     hash ^= (hash >> 16);
//     hash *= 0x85ebca6b;
//     hash &= (hash >> 13);
//     hash *= 0xc2b2ae35;
//     hash &= (hash >> 16);

    return hash;
}

void SetStackHash(Stack* stack)
{
    stack->hash = 0;
    stack->hash = Hash((const void*) stack, sizeof(Stack));
}

void CheckHash(Stack* stack)
{
    size_t hash_temp = stack->hash;

    SetStackHash(stack);

    // printf("%ld VS %ld\n", hash_temp, stack->hash);

    if (stack->hash - hash_temp)
    {
        fprintf(stderr, KRED "ERROR! STACK IS DAMAGED!!!\n" KNRM);
        abort();
    }
}

// #else
//
// size_t Hash(const void* obj, size_t len)
// {
//     return 0;
// }
// void   SetStackHash(Stack* stack);
// void   CheckHash(Stack* stack);
//
// #endif


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
    {
        stack->status = ERROR_STATUS;
        SetStackHash(stack);
    }

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

    Elem_t poison = (Elem_t) NULL;
    poisoned(&poison);

    for (int i = 0; i < stack->capacity; i++)

        if (stack->data[i] != poison)
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
    Elem_t poison = (Elem_t) NULL;
    poisoned(&poison);

    for (size_t i = start_index; i < stack->capacity; i++)
        stack->data[i] = poison;

    SetStackHash(stack);
}

void StackCtor_(Stack* stack)
{
    ASSERT(stack != NULL);

    SetStackHash(stack);

    if (stack->status == OK_STATUS)
        stack->status = ALR_INIT_STATUS;

    int err = StackError(stack);

    if (!(err & INIT_MASK))
    {
        stack->data     = (Elem_t*) calloc(MIN_CAPACITY, sizeof(Elem_t));
        stack->capacity = MIN_CAPACITY;
        stack->size     = (size_t) 0;

        Elem_t poison = (Elem_t) NULL;
        poisoned(&poison);

        FillPoisons(stack, 0);

        stack->status = OK_STATUS;
    }

    else if (!(err & DEL_MASK))
        stack->status = N_INIT_STATUS;

    SetStackHash(stack);

    ASSERT_OK(stack);
}

void StackDtor(Stack* stack)
{
    ASSERT(stack != NULL);
    ASSERT_OK(stack);

    if (stack->status != DELETED_STATUS)
    {
        free(stack->data);
        stack->data     = (Elem_t) NULL;
        stack->size     = (size_t) NULL;
        stack->capacity = (size_t) NULL;
        stack->status   = DELETED_STATUS;
    }

    SetStackHash(stack);
}

void StackRealloc(Stack* stack, size_t capacity)
{
    ASSERT(stack != NULL);

    stack->capacity = capacity;
    stack->data = (Elem_t*) realloc(stack->data, capacity * sizeof(Elem_t));

    SetStackHash(stack);
}

Elem_t StackPop(Stack* stack, int* err)
{
    ASSERT_OK(stack);

    Elem_t popped = (Elem_t) NULL;

    if (stack->size > 0)
    {
        Elem_t poison = (Elem_t) NULL;
        poisoned(&poison);

        popped = stack->data[stack->size - 1];
        stack->data[stack->size - 1] = poison;

        if ((stack->capacity / stack->size-- >= 4) && (stack->capacity / 4 > MIN_CAPACITY))
            StackRealloc(stack, stack->capacity / 4);
    }

    else if (err != NULL)
        *err = 1;

    SetStackHash(stack);

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

    SetStackHash(stack);

    ASSERT_OK(stack);
}


#endif
