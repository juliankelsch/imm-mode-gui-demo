#ifndef PROCEDURALWORLD_DEBUGLOG_H
#define PROCEDURALWORLD_DEBUGLOG_H

#include "stdarg.h"
#include "stdio.h"
#include "string.h"

#ifdef _WIN32
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

#ifndef NDEBUG
#define DEBUG 1
#else
#define DEBUG 0
#endif

#if DEBUG
#define Assert(Expression) if(!(Expression)){*(int *)0 = 0;}
#define LogInfo(format, ...) (printf("LOG [%s:%d<%s>] ", __FILENAME__, __LINE__, __func__), printf(format, __VA_ARGS__))
#define LogWarn(format, ...) (fprintf(stdout, "WARN [%s:%d<%s>] -> ", __FILENAME__, __LINE__, __func__), fprintf(stdout, format, __VA_ARGS__))
#define LogError(format, ...) (fprintf(stderr, "WARN [%s:%d<%s>] -> ", __FILENAME__, __LINE__, __func__), fprintf(stderr, format, __VA_ARGS__))
#else
#define Assert(Expression)
#define LogInfo(format, ...)
#define LogWarn(format, ...)
#define LogError(format, ...)
#endif

#endif //PROCEDURALWORLD_DEBUGLOG_H
