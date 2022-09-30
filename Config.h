#ifndef CONFIG_H
#define CONFIG_H

// #define NDEBUG
#include "ColourConsts.h"
#include "Log.h"

typedef int Elem_t;

static const char* statuses[] = {
                                KGRN "OK"                 KNRM,
                                KRED "DELETED"            KNRM,
                                KRED "NOT INITIALIZED"    KNRM,
                                KRED "ERROR"              KNRM,
                                KRED "ALREDY INITIALIZED" KNRM
                                };

// void create_masks(int* masks)
// {
//     for (int i = 0, num = 1; i < 8; i++)
//     {
//         num <<= 1;
//         masks[i] = num;
//     }
// }
//
// int masks[8] = {};
// create_masks(masks);

const int   masks[8]          = {1, 2, 4, 8, 16, 32, 64, 128};

// enum        errors            = {
//                                  STACK_WASNT_INIT         = 1;
//                                  STACK_DELETED            = 2;
//                                  STACK_ALR_INIT           = 4;
//                                  STACK_OVERFLOW           = 8;
//                                  STACK_UNDERFLOW_CAPACITY = 16;
//                                  STACK_UNDERFLOW_SIZE     = 32;
//                                  STACK_DATA_NULL          = 64;
//                                  WRONG_PUSH_DATA          = 128;
//                                 };

const char* errors[]          = {
                                KYEL "STACK WASN'T INITIALIZED\n"                  KNRM,
                                KYEL "STACK DOESN'T EXIST (DELETED)\n"             KNRM,
                                KYEL "STACK WAS ALREADY INITIALIZED\n"             KNRM,
                                KYEL "STACK EQUALS NULL\n"                         KNRM,
                                KYEL "STACK OVERFLOW\n"                            KNRM,
                                KYEL "STACK UNDERFLOW (CAPACITY < MIN_CAPACITY)\n" KNRM,
                                KYEL "STACK UNDERFLOW (SIZE < 0)\n"                KNRM,
                                KYEL "DATA EQUALS NULL\n"                          KNRM,
                                KYEL "WRONG FORMAT OF PUSHING DATA\n"              KNRM
                                };

char        undefined_name[]    = KRED "?" KNRM;
int         int_poisoned        = 0xBADDED;
char        char_ptr_poisoned[] = "poisoned";
char        char_poisoned       = '\?';
float       float_poisoned      = 13.37;

#define UNDEFINED_NAME    undefined_name
#define UNDEFINED_LINE    -1
#define MIN_CAPACITY      10
#define OK_STATUS         statuses[0]
#define DELETED_STATUS    statuses[1]
#define N_INIT_STATUS     statuses[2]
#define ERROR_STATUS      statuses[3]
#define ALR_INIT_STATUS   statuses[4]
#define INIT_MASK         254
#define DEL_MASK          253

#endif
