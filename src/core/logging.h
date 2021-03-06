#ifndef LOGGING_H
#define LOGGING_H

#ifdef __cplusplus
#include <cstdio>
extern "C" {
#else
#include <stdio.h>
#endif /* __cplusplus */

#define LOG_LEVEL_TRACE 0
#define LOG_LEVEL_DEBUG 1
#define LOG_LEVEL_INFO 2
#define LOG_LEVEL_ERROR 3

#ifndef LOGGING_LOG_LEVEL
#define LOGGING_LOG_LEVEL LOG_LEVEL_TRACE
#endif

typedef enum LOG_LEVEL
{
    ERROR,
    INFO,
    DEBUG,
    TRACE
} LOG_LEVEL;

#define LOG_OPTION_NONE 0x00
#define LOG_OPTION_LINE 0x01

#define FUNC_NAME __func__

typedef void(*LOGGER)(LOG_LEVEL log_category, const char *file, const char *func, int line, unsigned int options, const char *format, ...);

/*no logging is useful when time and fprintf are mocked*/
#ifdef NO_LOGGING

#define WRITE_LOG(...)
#define Log_Info(...)
#define Log_Error(...)
#define logging_get_log_function() NULL
#define logging_set_log_function(...)

#elif (defined MINIMAL_Log_Error)

#define WRITE_LOG(...)
#define Log_Trace(...) 
#define Log_Debug(...)
#define Log_Info(...)
#define Log_Error(...) printf("Error %s: line %d\n",__FILE__,__LINE__);
#define logging_get_log_function() NULL
#define logging_set_log_function(...)

#else 
#define WRITE_LOG(log_category, log_options, format, ...) { LOGGER l = logging_get_log_function(); if (l != NULL) l(log_category, __FILE__, FUNC_NAME, __LINE__, log_options, format, ##__VA_ARGS__); } 
#endif

#if (LOGGING_LOG_LEVEL == LOG_LEVEL_TRACE)

#define Log_Trace(FORMAT, ...) do{WRITE_LOG(TRACE, LOG_OPTION_LINE, FORMAT, ##__VA_ARGS__); }while((void)0,0)
#define Log_Debug(FORMAT, ...) do{WRITE_LOG(DEBUG, LOG_OPTION_LINE, FORMAT, ##__VA_ARGS__); }while((void)0,0)
#define Log_Info(FORMAT, ...) do{WRITE_LOG(INFO, LOG_OPTION_LINE, FORMAT, ##__VA_ARGS__); }while((void)0,0)
#define Log_Error(FORMAT, ...) do{ WRITE_LOG(ERROR, LOG_OPTION_LINE, FORMAT, ##__VA_ARGS__); }while((void)0,0)

#elif (LOGGING_LOG_LEVEL == LOG_LEVEL_DEBUG)

#define Log_Trace(...) 
#define Log_Debug(FORMAT, ...) do{WRITE_LOG(DEBUG, LOG_OPTION_LINE, FORMAT, ##__VA_ARGS__); }while((void)0,0)
#define Log_Info(FORMAT, ...) do{WRITE_LOG(INFO, LOG_OPTION_LINE, FORMAT, ##__VA_ARGS__); }while((void)0,0)
#define Log_Error(FORMAT, ...) do{ WRITE_LOG(ERROR, LOG_OPTION_LINE, FORMAT, ##__VA_ARGS__); }while((void)0,0)

#elif (LOGGING_LOG_LEVEL == LOG_LEVEL_INFO)

#define Log_Trace(...) 
#define Log_Debug(...)
#define Log_Info(FORMAT, ...) do{WRITE_LOG(INFO, LOG_OPTION_LINE, FORMAT, ##__VA_ARGS__); }while((void)0,0)
#define Log_Error(FORMAT, ...) do{ WRITE_LOG(ERROR, LOG_OPTION_LINE, FORMAT, ##__VA_ARGS__); }while((void)0,0)

#elif (LOGGING_LOG_LEVEL == LOG_LEVEL_ERROR)

#define Log_Trace(...) 
#define Log_Debug() 
#define Log_Info(...)
#define Log_Error(FORMAT, ...) do{ WRITE_LOG(ERROR, LOG_OPTION_LINE, FORMAT, ##__VA_ARGS__); }while((void)0,0)

#endif

extern void logging_set_log_function(LOGGER log_function);
extern LOGGER logging_get_log_function(void);

#ifdef __cplusplus
}   // extern "C"
#endif /* __cplusplus */

#endif /* LOGGING_H */
