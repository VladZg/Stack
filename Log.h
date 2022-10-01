#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdlib.h>
#include "Config.h"
#include "time.h"

FILE* OpenLog  ();
void  WriteLog (const char* data);
void  CloseLog ();

#endif
