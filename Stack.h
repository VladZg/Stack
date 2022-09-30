#ifndef STACK_H
#define STACK_H

#include "Config.h"
#include "Log.h"
#include "HashCounters.h"
#include "Defines.h"

struct StackInfo
{
    char*       name     = UNDEFINED_NAME;
    const char* filename = __FILE__;
    int         line     = UNDEFINED_LINE;
};

struct Stack
{
    // size_t      left_canary;
    Elem_t*   data;
    size_t    size;
    size_t    capacity;
    StackInfo info;
    size_t    status = STK_N_INIT_STATUS;
    uint32_t  hash;
    // size_t      right_canary;
};


size_t StackError        (Stack* stack);
void   StackPrintError   (size_t error, size_t* mask, const char* error_msg);
void   StackDecodeErrors (size_t error);
void   StackDecodeStatus (size_t status);
void   StackDump         (Stack* stack);
Elem_t StackPop          (Stack* stack, int* err = NULL);


#ifndef N_HASH_PROTECTION

void SetStackHash(Stack* stack)
{
    ASSERT(stack != NULL);

    stack->hash = 0;
    stack->hash = HashCounter((const void*) stack, (uint32_t) sizeof(Stack), HASH_SEED);
}

void CheckHash(Stack* stack)
{
    ASSERT(stack != NULL);

    if (stack->status == STK_N_INIT_STATUS)
        SetStackHash(stack);

    uint32_t hash_temp = stack->hash;

    SetStackHash(stack);

    if (stack->hash - hash_temp)
    {
        stack->status = STK_DAMAGED_STATUS;

        printf(KYEL "STACK IS DAMAGED\n" KNRM);
        StackDecodeErrors(StackError(stack));
        StackDump(stack);

        abort();
    }
}

#else
void SetStackHash(Stack* stack) {}
void CheckHash(Stack* stack) {}

#endif

#ifndef N_CANARY_PROTECTION

size_t CheckCanaries(Stack* stack)
{
    return !((stack->left_canary - LEFT_CANARY)  | (stack->right_canary - RIGHT_CANARY))
}

#else

size_t CheckCanaries(Stack* stack)
{

    return 1;
}

#endif

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
    int tmp_int_poisoned = (int) int_poisoned;
    *elem = tmp_int_poisoned;
}

void poisoned(float* elem)
{
    float tmp_float_poisoned = (float) float_poisoned;
    *elem = tmp_float_poisoned;
}

void poisoned(char* elem)
{
    char tmp_char_poisoned = (char) char_poisoned;
    *elem = tmp_char_poisoned;
}

void poisoned(char** elem)
{
    char* tmp_char_ptr_poisoned = (char*) char_ptr_poisoned;
    *elem = tmp_char_ptr_poisoned;
}


size_t StackError(Stack* stack)
{
    size_t err = 0;

    if (stack->status == STK_N_INIT_STATUS)
    {
        err |= STK_N_INIT_ERR;
        return err;
    }

    if (stack->status == STK_DEL_STATUS)
    {
        err |= STK_N_EXIST_ERR;
        return err;
    }

    if (stack->status == STK_ALR_INIT_STATUS)
    {
        err |= STK_ALR_INIT_ERR;
        return err;
    }

    if (stack == NULL)
        err |= STK_IS_NULL_ERR;

    if (stack->size > stack->capacity)
        err |= STK_OVERFLOW_ERR;

    if (stack->capacity < MIN_CAPACITY)
        err |= STK_UNDERFLOW_ERR;

    if (!CheckCanaries(stack))
        err |= CHANGED_CANARY_ERR;

    if (stack->data == nullptr)
        err |= STK_DATA_IS_NULL_ERR;

    if (err)
    {
        stack->status = STK_ERR_STATUS;
        SetStackHash(stack);
    }

    return err;
}

void StackPrintError(size_t error, size_t* mask, const char* error_msg)
{
    if (error & *mask)
        printf("%s", error_msg);

    *mask <<= 1;
}

void StackDecodeErrors(size_t error)
{
    size_t mask = 1;

    StackPrintError(error, &mask, KYEL "STACK WASN'T INITIALIZED\n"                  KNRM);
    StackPrintError(error, &mask, KYEL "STACK DOESN'T EXIST (DELETED)\n"             KNRM);
    StackPrintError(error, &mask, KYEL "STACK WAS ALREADY INITIALIZED\n"             KNRM);
    StackPrintError(error, &mask, KYEL "STACK EQUALS NULL\n"                         KNRM);
    StackPrintError(error, &mask, KYEL "STACK OVERFLOW (SIZE > CAPACITY)\n"          KNRM);
    StackPrintError(error, &mask, KYEL "STACK UNDERFLOW (CAPACITY < MIN_CAPACITY)\n" KNRM);
    StackPrintError(error, &mask, KYEL "ONE OF CANARIES WAS CHANGED\n"               KNRM);
    StackPrintError(error, &mask, KYEL "DATA EQUALS NULL\n"                          KNRM);
}

void StackDecodeStatus(size_t status)
{
    if(status == STK_OK_STATUS)
        printf("%s", KGRN "OK"                 KNRM);

    else if(status == STK_DEL_STATUS)
        printf("%s", KGRN "DELETED"            KNRM);

    else if(status == STK_N_INIT_STATUS)
        printf("%s", KGRN "NOT INITIALIZED"    KNRM);

    else if(status == STK_ERR_STATUS)
        printf("%s", KGRN "ERROR"              KNRM);

    else if(status == STK_ALR_INIT_STATUS)
        printf("%s", KGRN "ALREDY INITIALIZED" KNRM);

    else if(status == STK_DAMAGED_STATUS)
        printf("%s", KGRN "DAMAGED"            KNRM);
}

void StackDump(Stack* stack)
{
    ASSERT(stack != NULL);

    printf("Stack[%p] (", (void *) stack);
    StackDecodeStatus(stack->status);
    printf(") \"%s\" at \"%s\"(%d)               \n", stack->info.name, stack->info.filename, stack->info.line);
    printf("{                                    \n");
    printf("    size     = %lu                   \n", stack->size);
    printf("    capacity = %lu                   \n", stack->capacity);
    printf("    data[%p]                         \n", (void *) &(stack->data));
    printf("    {                                \n");

    Elem_t poison = (Elem_t) NULL;
    poisoned(&poison);

    for (size_t i = 0; i < stack->capacity; i++)

        if (stack->data[i] != poison)
        {
            printf("        *[%ld] = " KMAG, i);
            print_stack_elem(stack->data[i]);
            printf(KNRM "\n");
        }

        else
        {
            printf("         [%ld] = " KBLU, i);
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

    if (stack->status == STK_OK_STATUS)
        stack->status = STK_ALR_INIT_STATUS;

    size_t err = StackError(stack);

    if (!(err & STK_INIT_MASK))
    {
        stack->data     = (Elem_t*) calloc(MIN_CAPACITY, sizeof(Elem_t));
        stack->capacity = MIN_CAPACITY;
        stack->size     = (size_t) 0;

        Elem_t poison = (Elem_t) NULL;
        poisoned(&poison);

        FillPoisons(stack, 0);

        stack->status = STK_OK_STATUS;
    }

    else if (!(err & STK_DEL_MASK))
        stack->status = STK_N_INIT_STATUS;

    SetStackHash(stack);

    ASSERT_OK(stack);
}

void StackDtor(Stack* stack)
{
    ASSERT(stack != NULL);
    ASSERT_OK(stack);

    if (stack->status != STK_DEL_STATUS)
    {
        free(stack->data);
        stack->data     = (Elem_t*) NULL;
        stack->size     = 0;
        stack->capacity = 0;
        stack->status   = STK_DEL_STATUS;
    }

    SetStackHash(stack);
}

void StackRealloc(Stack* stack, size_t capacity)
{
    ASSERT(stack != NULL);

    stack->capacity = capacity;
    stack->data = (Elem_t*) realloc(stack->data, capacity * sizeof(Elem_t));

    ASSERT(stack->data != NULL);

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
    {
        StackRealloc(stack, stack->capacity * 2);
        FillPoisons(stack, stack->size + 1);
    }

    stack->data[stack->size++] = value;

    SetStackHash(stack);

    ASSERT_OK(stack);
}


#endif
