#ifndef LOG_META_H
#define LOG_META_H
/******************************************************************************
*                                                                             *
*   log_meta.h                                                                *
*   Copyright (c) 2023  Marijn Kagchelland                                    *
*   All Rights Reserved.                                                      *
*                                                                             *
*   Author:                                                                   *
*     Marijn Kagchelland                                                      *
*                                                                             *
*   Revision information:                                                     *
*     @version $Revision:: 1.0.1                                            $ *
*     @author    $Author:: Marijn Kagchelland                               $ *
*     @date        $Date:: 15/05/2024                                       $ *
*                                                                             *
******************************************************************************/

/******************************************************************************
*   Includes                                                                  *
******************************************************************************/

/******************************************************************************
*   #defines                                                                  *
******************************************************************************/
#define LOG_COLOR_ENABLE            (1)
#define LOG_USE_FREERTOS            (0)             // only  supports esp32 version
#define LOG_SYSTEM_TIMER_MS         HAL_GetTick()
#define LOG_NAME_FUNCTION_THREAD    __func__

#define LOG_COLOR_BLACK             "30"
#define LOG_COLOR_RED               "31"
#define LOG_COLOR_GREEN             "32"
#define LOG_COLOR_BROWN             "33"
#define LOG_COLOR_BLUE              "34"
#define LOG_COLOR_PURPLE            "35"
#define LOG_COLOR_CYAN              "36"
#define LOG_COLOR(COLOR)            "\033[0;" COLOR "m"
#define LOG_BOLD(COLOR)             "\033[1;" COLOR "m"
#define LOG_RESET_COLOR             "\033[0m"
#define LOG_COLOR_E                 LOG_COLOR(LOG_COLOR_RED)
#define LOG_COLOR_W                 LOG_COLOR(LOG_COLOR_BROWN)
#define LOG_COLOR_I                 LOG_COLOR(LOG_COLOR_GREEN)
#define LOG_COLOR_D
#define LOG_COLOR_V
#define LOG_COLOR_H		            LOG_COLOR(LOG_COLOR_CYAN)

/******************************************************************************
*   Typedefs                                                                  *
******************************************************************************/
typedef enum
{
    LOG_NONE,       /*!< No log output */
    LOG_ERROR,      /*!< Critical errors, software module can not recover on its own */
    LOG_WARN,       /*!< Error conditions from which recovery measures have been taken */
    LOG_INFO,       /*!< Information messages which describe normal flow of events */
    LOG_DEBUG,      /*!< Extra information which is not necessary for normal use (values, pointers, sizes, etc). */
    LOG_VERBOSE     /*!< Bigger chunks of debugging information, or frequent messages which can potentially flood the output. */
} LogLevel_e;

/******************************************************************************
*   Globals vars                                                              *
******************************************************************************/

/******************************************************************************
*   Global function prototypes                                                *
******************************************************************************/

#endif // End LOG_META_H
