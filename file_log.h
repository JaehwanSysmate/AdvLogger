#pragma once

#ifndef FILE_LOG_H
#define FILE_LOG_H

#include <stdio.h>
#include <stdarg.h>

enum { LOG_TRACE, LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR, LOG_FATAL };

#define flog_trace(...) flog_write(LOG_TRACE, __FILE__, __LINE__, __VA_ARGS__);
#define flog_debug(...) flog_write(LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__);
#define flog_info(...) flog_write(LOG_INFO, __FILE__, __LINE__, __VA_ARGS__);
#define flog_warn(...) flog_write(LOG_WARN, __FILE__, __LINE__, __VA_ARGS__);
#define flog_error(...) flog_write(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__);
#define flog_fatal(...) flog_write(LOG_FATAL, __FILE__, __LINE__, __VA_ARGS__);

void flog_set_filepath(const char* AFileDir, const char* APrefix, const char* ASuffix, const char* AExtName);
void flog_set_init();
void flog_set_final();
void flog_set_level(int level);
void flog_set_quiet(int enable);

void flog_write(int level, const char* file, int line, const char* fmt, ...);


#endif
