#ifndef UB_LIB_LOGIC_LAYER_H_
#define UB_LIB_LOGIC_LAYER_H_

/**
  ******************************************************************************
  * @file    Logic_Layer.c
  * @brief   This file provides code for the processing of data received
  *          and calls to the API functions for executing of commands
  ******************************************************************************
  */
/******************************************************************************
*   Includes                                                                  *
******************************************************************************/
#include "main.h"
#include "log.h"

/******************************************************************************
*   #defines                                                                  *
******************************************************************************/
#define MAX_VARS 10
#define BYTE_BUFLEN 	 1
#define LINE_BUFLEN 	 1024
#define CARRIAGE_RETURN  13 /* carriage return char \r */
#define LINE_FEED 		 10 /* linefeed char \n		   */

/******************************************************************************
*   Typedefs                                                                  *
******************************************************************************/
typedef struct
{
    uint8_t byte_buffer_rx[BYTE_BUFLEN];	// Store the rx byte from the USART2
    char line_rx_buffer[LINE_BUFLEN];		// Buffer to hold all the bytes from rx USART2
    int msglen;
    volatile int char_counter;				// Counter for line_rx_buffer
    char command_execute_flag;				/* Set = whole transmission is received, ready for processing \
											   Reset = still receiving*/
}input_vars;

/**
 * @enum CommandType
 * @brief Enumeratie van beschikbare commando types.
 */
typedef enum
{
	CMD_LINE,         /// Commando om een lijn te tekenen.
	CMD_RECTANGLE,    /// Commando om een rechthoek te tekenen.
	CMD_TEXT,         /// Commando om tekst te tekenen.
	CMD_BITMAP,       /// Commando om een bitmap te tonen.
	CMD_CLEAR_SCREEN, /// Commando om het scherm te wissen.
	CMD_UNKNOWN       /// Onbekend commando.
} CommandType;

/**
 * @enum CommandType
 * @brief Enumeratie van beschikbare commando types.
 */
typedef enum
{
	VGA_COLOUR_BLACK,
	VGA_COLOUR_BLUE,
	VGA_COLOUR_GREEN,
	VGA_COLOUR_RED,
	VGA_COLOUR_WHITE,
	VGA_COLOUR_CYAN,
	VGA_COLOUR_MAGENTA,
	VGA_COLOUR_YELLOW,
	CMD_UNKNOWN_COLOR
} ColorType;

/******************************************************************************
*   Globals vars                                                              *
******************************************************************************/
extern input_vars input;
/******************************************************************************
*   Global function prototypes                                                *
******************************************************************************/
void LL_receive(void);

void LL_exec(void);

#endif /* UB_LIB_LOGIC_LAYER_H_ */
