#include <stdio.h>
#include <stdlib.h>
#include "Config.h"
#include "time.h"

#define LOG_FILE_NAME_DEFAULT "Log.txt"

#ifndef LOG_FILE_NAME
#define LOG_FILE_NAME LOG_FILE_NAME_DEFAULT
#endif

#ifndef NLOGGING

FILE* OpenLog()
{
    FILE* log_file = fopen(LOG_FILE_NAME, "a");

    if (!log_file)
        log_file = fopen(LOG_FILE_NAME_DEFAULT, "a");

    time_t now        = time(0);
    tm     *real_time = localtime(&now);

    fprintf(log_file,
            "\\===========================================================\\"
            "\nSession started (%d:%d:%d  %d.%d.%d)\n\n",
            real_time->tm_hour, real_time->tm_min, real_time->tm_sec,
            real_time->tm_mday, 1 + real_time->tm_mon, 1900 + real_time->tm_year);

    return log_file;
}

extern FILE* LOG_FILE;
FILE* LOG_FILE  = OpenLog();

void WriteLog(const char* data)
{
    fprintf(LOG_FILE, "%s", data);
}

void CloseLog()
{
    time_t now        = time(0);
    tm     *real_time = localtime(&now);

    fprintf(LOG_FILE,
            "\n\nSession finished (%d:%d:%d  %d.%d.%d)\n"
            "\\===========================================================\\\n",
            real_time->tm_hour, real_time->tm_min, real_time->tm_sec,
            real_time->tm_mday, 1 + real_time->tm_mon, 1900 + real_time->tm_year);

    fclose(LOG_FILE);
}

extern int LOG_EXIT;
int LOG_EXIT = atexit(CloseLog);

#else

void OpenLog() {}

void WriteLog(const char* data)
{
    fputs(data, stderr);
}

void CloseLog() {}

#endif
