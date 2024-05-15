/******************************************************************************
*                                                                             *
*   log.c                                                                     *
*   Copyright (c) 2023  Marijn Kagchelland                                    *
*   All Rights Reserved.                                                      *
*                                                                             *
*   Author:                                                                   *
*     Marijn Kagchelland                                                      *
*                                                                             *
*   Revision information:                                                     *
*     @version $Revision:: 1.12.1                                           $ *
*     @author    $Author:: Marijn Kagchelland                               $ *
*     @date        $Date:: 15/05/2024                                       $ *
*     to do list:                                                             *
*           - make the task watchdog universal for multiple platforms         *
******************************************************************************/

/******************************************************************************
*   Includes                                                                  *
******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>

#include "log.h"
#include "usart.h"

//#include "esp_task_wdt.h"
//#include "esp_log.h"


/******************************************************************************
*   #defines                                                                  *
******************************************************************************/
//#define NO_ASYNC_LOGGING

#ifdef NO_ASYNC_LOGGING
#warning Async logging is disabled
#endif

// MAX_MESSAGE_LENGTH + LOG_BUFFER_SIZE cannot exceed 65535
#define MAX_MESSAGE_LENGTH (256)  // max number of characters in a single message
#define LOG_BUFFER_SIZE    (8 * 1024) // size of the log buffer in bytes


#define BUFFER_SIZE_TOTAL (LOG_BUFFER_SIZE + MAX_MESSAGE_LENGTH + 1)
#define DROP_COUNT_FORMAT "[DROP %u] "

#define MIN(a, b) ((a) < (b) ? (a) : (b))
/******************************************************************************
*   Typedefs                                                                  *
******************************************************************************/

/******************************************************************************
*   Globals vars                                                              *
******************************************************************************/

/******************************************************************************
*   Local vars                                                                *
******************************************************************************/
static LogLevel_e CurrentLogLevel = LOG_VERBOSE;
static char logBuffer[LOG_BUFFER_SIZE + MAX_MESSAGE_LENGTH + 1];
static volatile uint16_t logBufferLength = LOG_BUFFER_SIZE;
static uint16_t logBufferInPtr = 0;
static volatile uint16_t logBufferOutPtr = 0;

#if LOG_USE_FREERTOS == 1
static SemaphoreHandle_t logWriteSemaphore = NULL;
static StaticSemaphore_t logWriteSemaphoreBuffer;

TaskHandle_t logTaskHandle = NULL;
TaskHandle_t waitingTaskHandle = NULL;

static bool stopping = false;
static uint32_t numDroppedMessages = 0;
#else
static char txDmaBuffer[MAX_MESSAGE_LENGTH] = {0};
static int numTotalMessages = 0;
static volatile bool Sending = false;
#endif

/******************************************************************************
*   Local function prototypes                                                 *
******************************************************************************/
#if LOG_USE_FREERTOS == 1
static void Main(void);
#endif
static int Log(const char* logMessageFormat, va_list logMessageArguments);

/******************************************************************************
*   Global functions                                                          *
******************************************************************************/
__inline void LOG_SetLogLevel(LogLevel_e logLevel)
{
	CurrentLogLevel = logLevel;
}

__inline LogLevel_e LOG_GetLogLevel(void)
{
	return CurrentLogLevel;
}

void LOG_Log(LogLevel_e logLevel, const char* logMessageFormat, ...)
{
	if (logLevel <= CurrentLogLevel) {
		va_list logMessageArguments;
		va_start(logMessageArguments, logMessageFormat);
		Log(logMessageFormat, logMessageArguments);
		va_end(logMessageArguments);
	}
}

#if LOG_USE_FREERTOS == 1
void LOG_Task(void* taskParameters)
{
	esp_task_wdt_add(NULL);
	logWriteSemaphore = xSemaphoreCreateMutexStatic(&logWriteSemaphoreBuffer);
	logTaskHandle = xTaskGetCurrentTaskHandle();

	logBuffer[BUFFER_SIZE_TOTAL - 1] = '\0';

	// redirect ESP log to our own function
	esp_log_set_vprintf(&Log);

	LOGI("Logger initialized");

	Main();
}

void LOG_StopAndFlush(void)
{
	if (stopping) return;

	LOGC(LOG_COLOR_CYAN, "Logs stopping...\n");
	stopping = true;
	waitingTaskHandle = xTaskGetCurrentTaskHandle();
	xTaskNotify(logTaskHandle, 0, eNoAction);
	vTaskSuspend(NULL);
}

void LOG_Start(void)
{
	if (!stopping) return;

	stopping = false;
	vTaskResume(logTaskHandle);
	vTaskDelay(1);
	LOGC(LOG_COLOR_CYAN, "Logs starting...");
}
#else
void LOG_SendNextLog(void)
{
    if (numTotalMessages > 0) {
        uint32_t i = 0;
        do {
            txDmaBuffer[i++] = logBuffer[logBufferOutPtr++];
            if (logBufferOutPtr == LOG_BUFFER_SIZE)
                logBufferOutPtr = 0;
        } while(logBuffer[logBufferOutPtr] != 0);
        numTotalMessages--;
        txDmaBuffer[i++] = 0;
        HAL_UART_Transmit_DMA(&huart2, (uint8_t*) txDmaBuffer, i);
        Sending = true;
    } else
        Sending = false;
}
#endif

/******************************************************************************
*   Local functions                                                           *
******************************************************************************/
#if LOG_USE_FREERTOS == 1
static void Main()
{
	while (true) {
		// print every character in the buffer individually
		while (logBufferOutPtr != logBufferInPtr) {
			char nextCharacter = logBuffer[logBufferOutPtr];
			putchar(nextCharacter);

			logBufferOutPtr++;
			if (logBufferOutPtr == logBufferLength) {
				logBufferOutPtr = 0;
			}

			// we want to yield after every full log message
			if (nextCharacter == '\0') {
				esp_task_wdt_reset();
				taskYIELD();
			}
		}
		esp_task_wdt_reset();
		if (stopping) {
			vTaskDelay(pdMS_TO_TICKS(10));
			if (waitingTaskHandle != NULL) vTaskResume(waitingTaskHandle);
			esp_task_wdt_delete(NULL);
			vTaskSuspend(NULL);
			esp_task_wdt_add(NULL);
		}

		// if there's nothing to log, suspend the task
		esp_task_wdt_delete(NULL);
		xTaskNotifyWait(0, ULONG_MAX, NULL, pdMS_TO_TICKS(100));
		esp_task_wdt_add(NULL);
	}
}
#endif


static int Log(const char* logMessageFormat, va_list logMessageArguments)
{
#if LOG_USE_FREERTOS == 1
	if (stopping) return 0;

#ifdef NO_ASYNC_LOGGING
	return vprintf(logMessageFormat, logMessageArguments);
#else

	// we can only start logging once the semaphore is initialized
	if (logWriteSemaphore != NULL) {
		uint16_t numPrintedCharacters = 0;

		if (xSemaphoreTake(logWriteSemaphore, pdMS_TO_TICKS(100))) {
			// printf("L");

			// calculate the remaining space in the buffer
			uint16_t remainingSpace;
			if (logBufferInPtr >= logBufferOutPtr) {
				remainingSpace = BUFFER_SIZE_TOTAL - logBufferInPtr - 1;
			} else {
				remainingSpace = MIN(logBufferOutPtr, LOG_BUFFER_SIZE) - logBufferInPtr;
			}

			// if messages were dropped, print the drop count
			if (numDroppedMessages > 0) {
				numPrintedCharacters = snprintf(&logBuffer[logBufferInPtr], remainingSpace,
												DROP_COUNT_FORMAT, numDroppedMessages);
			}

			// if the buffer didn't overflow, print the log message
			if (numPrintedCharacters < remainingSpace) {
				numPrintedCharacters += vsnprintf(&logBuffer[logBufferInPtr + numPrintedCharacters],
												  remainingSpace - numPrintedCharacters,
												  logMessageFormat, logMessageArguments);
			}

			// if the buffer didn't overflow, notify the log task to send the message
			if (numPrintedCharacters < remainingSpace) {
				numDroppedMessages = 0;

				logBufferInPtr += numPrintedCharacters + 1;
				if (logBufferInPtr > LOG_BUFFER_SIZE) {
					logBufferLength = logBufferInPtr - 1;
					logBufferInPtr = 0;
				}

				xTaskNotify(logTaskHandle, 0, eNoAction);
			} else {
				numDroppedMessages++;
			}

			xSemaphoreGive(logWriteSemaphore);
		} else {
			numDroppedMessages++;
		}

		return numPrintedCharacters;
	} else {
		// if the semaphore isn't initialized, drop the message
		numDroppedMessages++;
		return 0;
	}
#endif
#else
	numTotalMessages++;
	uint16_t remainingSpace;
	uint16_t numPrintedCharacters = 0;
	if (logBufferInPtr >= logBufferOutPtr) {
		remainingSpace = BUFFER_SIZE_TOTAL - logBufferInPtr - 1;
	} else {
		remainingSpace = MIN(logBufferOutPtr, LOG_BUFFER_SIZE) - logBufferInPtr;
	}
	
	numPrintedCharacters += vsnprintf(&logBuffer[logBufferInPtr],
									  remainingSpace,
									  logMessageFormat, logMessageArguments);
	if (numPrintedCharacters > remainingSpace) {
		logBufferInPtr = 0;
        logBufferOutPtr = 0;
		remainingSpace = BUFFER_SIZE_TOTAL - 1;
		numPrintedCharacters = snprintf(&logBuffer[logBufferInPtr], remainingSpace,
										DROP_COUNT_FORMAT, numTotalMessages);
		numTotalMessages = 1;
	}
	
	logBufferInPtr += numPrintedCharacters + 1;
	if (logBufferInPtr > LOG_BUFFER_SIZE) {
		logBufferLength = logBufferInPtr - 1;
		logBufferInPtr = 0;
	}
    if (!Sending)
        LOG_SendNextLog();
	return numTotalMessages;
#endif
}
