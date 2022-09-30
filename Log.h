#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include "Config.h"
#include "time.h"

#ifndef LOG_FILE
#define LOG_FILE "Log.txt"
#endif

#ifndef NDEBUG

FILE* log_file  = NULL;

void OpenLog()
{
    log_file = fopen(LOG_FILE, "a");

    time_t now        = time(0);
    tm     *real_time = localtime(&now);

    fprintf(log_file,
            "\\===========================================================\\"
            "\nSession started (%d:%d:%d  %d.%d.%d)\n\n",
            real_time->tm_hour, real_time->tm_min, real_time->tm_sec,
            real_time->tm_mday, 1 + real_time->tm_mon, 1900 + real_time->tm_year);
}

// void WriteLog(void (*cmd))

void CloseLog()
{
    time_t now        = time(0);
    tm     *real_time = localtime(&now);

    fprintf(log_file,
            "\n\nSession finished (%d:%d:%d  %d.%d.%d)\n"
            "\\===========================================================\\\n",
            real_time->tm_hour, real_time->tm_min, real_time->tm_sec,
            real_time->tm_mday, 1 + real_time->tm_mon, 1900 + real_time->tm_year);

    fclose(log_file);
}

#else

void OpenLog();
void CloseLog();

#endif

#endif
