#include <stdio.h>

#include "pch.h"
#include "file_log.h"
#include "ExtTimeFormat.h"


CRITICAL_SECTION cs;

static struct {
    FILE* logFile;
    UINT16 wYear;
    UINT16 wMon;
    UINT16 wDay;
    int level;
    int quiet;
} LogInfo;

static const char* level_names[] = {
  "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"
};

#ifdef LOG_USE_COLOR
static const char* level_colors[] = {
  "\x1b[94m", "\x1b[36m", "\x1b[32m", "\x1b[33m", "\x1b[31m", "\x1b[35m"
};
#endif


char PATH_PRE[MAX_PATH];
char PATH_SUF[MAX_PATH];

static void lock(void) 
{
    /*if (LogInfo.lock) {
        LogInfo.lock(LogInfo.udata, 1);
    }*/
    EnterCriticalSection(&cs);
    
}

static void unlock(void) 
{
    /*if (LogInfo.lock) {
        LogInfo.lock(LogInfo.udata, 0);
    }*/
    LeaveCriticalSection(&cs);
}


void flog_set_init()
{
    InitializeCriticalSection(&cs);
}

void flog_set_final()
{
    if (LogInfo.logFile) fclose(LogInfo.logFile);
    LeaveCriticalSection(&cs);
}

void flog_set_filepath(const char* AFileDir, const char* APrefix, const char* ASuffix, const char* AExtName)
{
    lock();

    strcpy(PATH_PRE, AFileDir);
    if (PATH_PRE[strlen(PATH_PRE)] != '\\')
        strcat(PATH_PRE, "\\");
    if (APrefix)
        strcat(PATH_PRE, APrefix);

    if (ASuffix) {
        strcpy(PATH_SUF, ASuffix);
        strcat(PATH_SUF, AExtName);
    }
    else {
        strcpy(PATH_SUF, AExtName);
    }

    // 파일이 열려있으면 닫아준다.
    if (LogInfo.logFile) {
        fclose(LogInfo.logFile);
        LogInfo.logFile = NULL;
    }

    unlock();
}

void flog_set_level(int level)
{
    LogInfo.level = level;
}

void flog_set_quiet(int enable)
{
    LogInfo.quiet = enable ? 1 : 0;
}


void check_log_file(const SYSTEMTIME* ATime)
{
    if ((!LogInfo.logFile) || (LogInfo.wYear != ATime->wYear) || (LogInfo.wMon != ATime->wMonth) || (LogInfo.wDay != ATime->wDay)) {
        
        // 파일이 열려있으면 닫아준다.
        if (LogInfo.logFile) fclose(LogInfo.logFile);

        char strLogFilePath[MAX_PATH];
        strcpy(strLogFilePath, PATH_PRE);

        // 날짜
        char buf[10];
        sprintf_s(buf, sizeof(buf), "%04d%02d%02d", ATime->wYear, ATime->wMonth, ATime->wDay);
        strcat(strLogFilePath, buf);

        strcat(strLogFilePath, PATH_SUF);

        LogInfo.logFile = fopen(strLogFilePath, "a");
    }
}


void flog_write(int level, const char* file, int line, const char* fmt, ...)
{
    if (level < LogInfo.level) return;

    // Acquire Lock
    lock();

    // Get Current Time
    SYSTEMTIME sysTime;
    GetLocalTime(&sysTime);

    // File Check
    check_log_file(&sysTime);

    if (!LogInfo.quiet) {
        va_list args;
        char buf[32];
        buf[strfstime(buf, sizeof(buf), "%H:%M:%S.%s", &sysTime)] = '\0';
#ifdef LOG_USE_COLOR
        fprintf(stderr
            , "%s %s%-5s\x1b[0m \x1b[90m%s[%d]\x1b[0m: "
            , buf
            , level_colors[level]
            , level_names[level]
            , file
            , line);
#else
        fprintf(stderr
            , "%s[%-5s]%s[%d]: "
            , buf
            , level_names[level]
            , file
            , line);
#endif
        va_start(args, fmt);
        vfprintf(stderr, fmt, args);
        va_end(args);
        fprintf(stderr, "\n");
        fflush(stderr);
    }

    if (LogInfo.logFile) {
        va_list args;
        char buf[32];
        buf[strfstime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S.%s", &sysTime)] = '\0';
        fprintf(LogInfo.logFile
            , "[%s][%-5s]%s(%d): "
            , buf
            , level_names[level]
            , file
            , line);
        va_start(args, fmt);
        vfprintf(LogInfo.logFile, fmt, args);
        va_end(args);
        fprintf(LogInfo.logFile, "\n");
        fflush(LogInfo.logFile);
    }


    // Release Lock
    unlock();
}

