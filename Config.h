#ifndef CONFIG_H
#define CONFIG_H

// #define NDEBUG
#include "ColourConsts.h"

typedef int Elem_t;

static const char* statuses[] = {
                                KGRN "OK" KNRM,
                                KRED "DELETED" KNRM,
                                KRED "NOT INITIALIZED" KNRM,
                                KRED "ERROR" KNRM,
                                KRED "ALREDY INITIALIZED" KNRM
                                };
char  undefined_name[]         = KRED "?" KNRM;
int   int_poisoned             = 0xBADDED;
char  char_ptr_poisoned[]      = "poisoned";
char  char_poisoned            = '\0';
float float_poisoned           = -13.37;

#define POISONED          int_poisoned
#define UNDEFINED_NAME    undefined_name
#define UNDEFINED_LINE    -1
#define MIN_CAPACITY      10
#define OK_STATUS         statuses[0]
#define DELETED_STATUS    statuses[1]
#define N_INIT_STATUS     statuses[2]
#define ERROR_STATUS      statuses[3]
#define ALR_INIT_STATUS   statuses[4]

#endif
