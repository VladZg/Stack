#include <stdio.h>
#include <stdlib.h>

#include "Config.h"
#include "Log.h"
#include "Defines.h"
#include "Stack.h"

#include "TestingValues.h"
#include "TestingFunctions.h"

int main()
{
    Stack stk1 = {};
    Stack stk2 = {};

    // StackDump(&stk1);
    // StackDump(&stk2);

    StackCtor(stk2);
    StackCtor(stk1);

    // StackPushN(&stk1, 19);
    // StackDump(&stk1);
    // stk1.size = 1;
    // stk1.data[4] = 888;
    // StackPush(&stk1, TEST_CHAR_PTR);
    // StackPush(&stk1, TEST_CHAR_PTR);

    StackDump(&stk1);

    // stk1.size = 3;
    // StackPopN(&stk1, 56);
    // StackPush(&stk1, 89);
    // StackDump(&stk1);

    StackDtor(&stk1);
    StackDtor(&stk2);

    // StackDump(&stk1);
    // StackDump(&stk2);

    // StackPush(&stk1, 2);

    return 0;
}
