#ifndef LOG_H
#define LOG_H
/******************************************************************************
*                                                                             *
*   log.h                                                                     *
*   Copyright (c) 2023  Marijn Kagchelland                                    *
*   All Rights Reserved.                                                      *
*                                                                             *
*   Author:                                                                   *
*     Marijn Kagchelland                                                      *
*                                                                             *
*   Revision information:                                                     *
*     @version $Revision:: 1.3.1                                            $ *
*     @author    $Author:: Marijn Kagchelland                               $ *
*     @date        $Date:: 15/05/2024                                       $ *
*                                                                             *
******************************************************************************/

/******************************************************************************
*   Includes                                                                  *
******************************************************************************/
#include <stdio.h>
#include "log_config.h"

/******************************************************************************
*   #defines                                                                  *
******************************************************************************/
#if LOG_COLOR_ENABLE == 1
#define LOG_COLOR_DEV               LOG_COLOR(LOG_COLOR_PURPLE)
#define LOG_COLOR_NONE              ""

#define FILENAME_ (__builtin_strrchr(__FILE__, '/') ? __builtin_strrchr(__FILE__, '/') + 1 : __FILE__)

#define LOGH(format, ...)        LOG_(LOG_ERROR,   LOG_COLOR_H,      FILENAME_, __LINE__, H, format, ##__VA_ARGS__)     // @brief Hardware Info
#define LOGU(format, ...)        LOG_(LOG_ERROR,   LOG_COLOR_E,      FILENAME_, __LINE__, U, format, ##__VA_ARGS__)     // @brief Urgent. System will not work. Think Hard fault. I2C Device not detected.
#define LOGE(format, ...)        LOG_(LOG_ERROR,   LOG_COLOR_E,      FILENAME_, __LINE__, E, format, ##__VA_ARGS__)     // @brief Error. System can not handle this fault. Thick out of bounce. Failed malloc. Can not be directly handled
#define LOGW(format, ...)        LOG_(LOG_WARN,    LOG_COLOR_W,      FILENAME_, __LINE__, W, format, ##__VA_ARGS__)     // @brief Warning. System can go on with this fault like normal. It can be fixed if handled.
#define LOGI(format, ...)        LOG_(LOG_INFO,    LOG_COLOR_I,      FILENAME_, __LINE__, I, format, ##__VA_ARGS__)     // @brief Info. Short piece of information for the user of status, what the system is doing, ect.
#define LOGD(format, ...)        LOG_(LOG_DEBUG,   LOG_COLOR_NONE,   FILENAME_, __LINE__, D, format, ##__VA_ARGS__)     // @brief Debug. Used in production to debug faults in the code.
#define LOGV(format, ...)        LOG_(LOG_VERBOSE, LOG_COLOR_NONE,   FILENAME_, __LINE__, V, format, ##__VA_ARGS__)     // @brief Verbose. Indepth information for the user of status, what the system is doing, ect.
#define DEVLOG(format, ...)      LOG_(LOG_DEBUG,   LOG_COLOR_DEV,    FILENAME_, __LINE__, D, format, ##__VA_ARGS__)     // @brief Developer. Must only be used in development. Should never be pushed to a Version Software Control. Example GitHub


#define LOG_(level, color, filename, line, prefix, format, ...) \
    LOG_Log(level, "%s" #prefix " (%lu) [%s] %s %d: " format "\033[0m\r\n", color, LOG_SYSTEM_TIMER_MS, LOG_NAME_FUNCTION_THREAD, filename, line, ##__VA_ARGS__)
#else
#define LOG_COLOR_NONE              ""

#define FILENAME_ (__builtin_strrchr(__FILE__, '/') ? __builtin_strrchr(__FILE__, '/') + 1 : __FILE__)

#define LOGH(format, ...)        LOG_(LOG_ERROR,   FILENAME_, __LINE__, H, format, ##__VA_ARGS__)                       // @brief Hardware Info.
#define LOGU(format, ...)        LOG_(LOG_ERROR,   FILENAME_, __LINE__, U, format, ##__VA_ARGS__)                       // @brief Urgent. System will not work. Think Hard fault. I2C Device not detected.
#define LOGE(format, ...)        LOG_(LOG_ERROR,   FILENAME_, __LINE__, E, format, ##__VA_ARGS__)                       // @brief Error. System can not handle this fault. Thick out of bounce. Failed malloc. Can not be directly handled
#define LOGW(format, ...)        LOG_(LOG_WARN,    FILENAME_, __LINE__, W, format, ##__VA_ARGS__)                       // @brief Warning. System can go on with this fault like normal. It can be fixed if handled.
#define LOGI(format, ...)        LOG_(LOG_INFO,    FILENAME_, __LINE__, I, format, ##__VA_ARGS__)                       // @brief Info. Short piece of information for the user of status, what the system is doing, ect.
#define LOGD(format, ...)        LOG_(LOG_DEBUG,   FILENAME_, __LINE__, D, format, ##__VA_ARGS__)                       // @brief Debug. Used in production to debug faults in the code.
#define LOGV(format, ...)        LOG_(LOG_VERBOSE, FILENAME_, __LINE__, V, format, ##__VA_ARGS__)                       // @brief Verbose. Indepth information for the user of status, what the system is doing, ect.
#define DEVLOG(format, ...)      LOG_(LOG_DEBUG,   FILENAME_, __LINE__, D, format, ##__VA_ARGS__)                       // @brief Developer. Must only be used in development. Should never be pushed to a Version Software Control. Example GitHub

#define LOG_(level, filename, line, prefix, format, ...) \
    LOG_Log(level, #prefix " (%lu) [%s] %s %d: " format "\r\n", LOG_SYSTEM_TIMER_MS, LOG_NAME_FUNCTION_THREAD, filename, line, ##__VA_ARGS__)
#endif

/******************************************************************************
*   Typedefs                                                                  *
******************************************************************************/

/******************************************************************************
*   Globals vars                                                              *
******************************************************************************/

/******************************************************************************
*   Global function prototypes                                                *
******************************************************************************/
#if LOG_USE_FREERTOS == 1
void LOG_Task(void *taskParameters);

void LOG_StopAndFlush(void);

void LOG_Start(void);
#else
void LOG_SendNextLog(void);
#endif

void LOG_Log(LogLevel_e logLevel, const char *logMessageFormat, ...) __attribute__ ((format (printf, 2, 3)));

void LOG_SetLogLevel(LogLevel_e logLevel);

LogLevel_e LOG_GetLogLevel(void);


#endif  // end LOG_H
