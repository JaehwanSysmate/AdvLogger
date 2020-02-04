// dllmain.cpp : DLL 애플리케이션의 진입점을 정의합니다.
#include "pch.h"
#include "file_log.h"

#include <sys/stat.h>
#include <time.h>



const char* DefaultExtName = ".log";
const int DefaultLogLevel = 2;                      // INFO

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        flog_set_init();
        flog_set_quiet(1);
        flog_set_level(DefaultLogLevel);
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        flog_set_final();
        break;
    }
    return TRUE;
}


extern "C" __declspec(dllexport) bool SetLogFilePath(char* AFileDir, char* APrefix, char* ASuffix, char* AExtName)
{
    struct stat sb;

    if (AFileDir && stat(AFileDir, &sb) == 0 && (sb.st_mode & S_IFDIR)) {

        if (AExtName) {
            flog_set_filepath(AFileDir, APrefix, ASuffix, AExtName);
        }
        else {
            flog_set_filepath(AFileDir, APrefix, ASuffix, DefaultExtName);
        }

        return true;
    }
    else {
        return false;
    }
}

extern "C" __declspec(dllexport) bool SetLogLevel(int ALevel)
{
    if (ALevel >= 0 && ALevel <= 10) {
        flog_set_level(ALevel);
        return true;
    }
    else {
        return false;
    }
}


extern "C" __declspec(dllexport) bool WriteLog(int ALevel, char* AFileName, int ALine, char* ALogStr)
{
    flog_write(ALevel, AFileName, ALine, ALogStr);
    return true;
}