/******************************************************************************
*   Includes                                                                  *
******************************************************************************/
#include "Logic_Layer.h"
#include "API_draw.h"
#include <string.h>
#include <stdlib.h>
/******************************************************************************
*   #defines                                                                  *
******************************************************************************/
#define SANITIZE_COLOR(colorTypeReturn) ({                                 \
    if (colorTypeReturn == CMD_UNKNOWN_COLOR)                              \
    {                                                                   \
        LOGE("Color unkown");                                             \
        return;                                                           \
    }                                                                   \
})
/******************************************************************************
*   Typedefs                                                                  *
******************************************************************************/

/******************************************************************************
*   Globals vars                                                              *
******************************************************************************/
input_vars input;

/******************************************************************************
*   Local vars                                                                *
******************************************************************************/
//array for testing
char command[] = "rechthoek,10,10,150,100,licht blauw,0";
char* ParsedData[MAX_VARS];

/******************************************************************************
*   Local function prototypes                                                 *
******************************************************************************/
/**
 * @brief Converteert een commando string naar een CommandType waarde.
 * @param command De commando string om te converteren.
 * @return CommandType De overeenkomende CommandType waarde.
 */
CommandType get_command_type(const char* command)
{
	if (strncmp(command, "lijn", strlen("lijn")) == 0) return CMD_LINE;
    if (strncmp(command, "rechthoek", strlen("rechthoek")) == 0) return CMD_RECTANGLE;
    if (strncmp(command, "tekst", strlen("tekst")) == 0) return CMD_TEXT;
    if (strncmp(command, "bitmap", strlen("bitmap")) == 0) return CMD_BITMAP;
    if (strncmp(command, "clearscherm", strlen("clearscherm")) == 0) return CMD_CLEAR_SCREEN;
    return CMD_UNKNOWN;
}

ColorType get_color_type(const char* command)
{
	if (strncmp(command, "rood", strlen("rood")) == 0) return VGA_COLOUR_RED;
    if (strncmp(command, "blauw", strlen("blauw")) == 0) return VGA_COLOUR_BLUE;
    if (strncmp(command, "lichtblauw", strlen("lichtblauw")) == 0) return CMD_UNKNOWN_COLOR;
    if (strncmp(command, "groen", strlen("groen")) == 0) return VGA_COLOUR_GREEN;
    if (strncmp(command, "lichtgroen", strlen("lichtgroen")) == 0) return CMD_UNKNOWN_COLOR;
	if (strncmp(command, "cyaan", strlen("cyaan")) == 0) return VGA_COLOUR_CYAN;
    if (strncmp(command, "lichtcyaan", strlen("lichtcyaan")) == 0) return CMD_UNKNOWN_COLOR;
    if (strncmp(command, "zwart", strlen("zwart")) == 0) return VGA_COLOUR_BLACK;
    if (strncmp(command, "lichtrood", strlen("lichtrood")) == 0) return CMD_UNKNOWN_COLOR;
    if (strncmp(command, "magenta", strlen("magenta")) == 0) return VGA_COLOUR_MAGENTA;
	if (strncmp(command, "lichtmagenta", strlen("lichtmagenta")) == 0) return CMD_UNKNOWN_COLOR;
    if (strncmp(command, "bruin", strlen("bruin")) == 0) return CMD_UNKNOWN_COLOR;
    if (strncmp(command, "geel", strlen("geel")) == 0) return VGA_COLOUR_YELLOW;
    if (strncmp(command, "grijs", strlen("grijs")) == 0) return CMD_UNKNOWN_COLOR;
    if (strncmp(command, "wit", strlen("wit")) == 0) return VGA_COLOUR_WHITE;
    return CMD_UNKNOWN_COLOR;
}

/******************************************************************************
*   Global functions                                                          *
******************************************************************************/

/**
  * @brief  Separates values from incoming array
  * @param  input.line_rx_buffer used
  * @retval None currently
  */
void LL_receive(void)
{
	char* data;
	char temp_buffer[LINE_BUFLEN]; // Adjust size as necessary

#ifdef DEBUG
	strncpy(temp_buffer, command, input.msglen);
#else
	strncpy(temp_buffer, input.line_rx_buffer, input.msglen);
#endif

	// Get the first token
	data = strtok(temp_buffer, ",");

	// Walk through other tokens
	int i = 0;
	while (data != NULL && i < MAX_VARS)
	{
		ParsedData[i++] = data;
		data = strtok(NULL, ",");
	}

	//DEBUG for data inside the parsed array
	for (int j = 0; j < i; j++)
	{
		LOGD("data bit %d: %s", j, ParsedData[j]);
	}

	LL_exec();
	return;
}

/**
 * @brief Voert een commando uit op basis van de opgegeven parameters.
 * @param ParsedData Array van strings met commando en parameters.
 */
void LL_exec(void)
{
	CommandType commandType = get_command_type(ParsedData[0]);
	ColorType colorType;
	    switch (commandType)
	    {
	        case CMD_LINE:
	        	colorType = get_color_type(ParsedData[5]);
	        	SANITIZE_COLOR(colorType);
			API_draw_line(atoi(ParsedData[1]), atoi(ParsedData[2]), atoi(ParsedData[3]), atoi(ParsedData[4]), colorType, atoi(ParsedData[6]), atoi(ParsedData[7]));
			break;
	        case CMD_RECTANGLE:
	        	colorType = get_color_type(ParsedData[5]);
	        	SANITIZE_COLOR(colorType);
			API_draw_rectangle(atoi(ParsedData[1]), atoi(ParsedData[2]), atoi(ParsedData[3]), atoi(ParsedData[4]), colorType, atoi(ParsedData[6]), atoi(ParsedData[7]), atoi(ParsedData[8]));
			break;
	        case CMD_TEXT:
	        	colorType = get_color_type(ParsedData[3]);
	        	SANITIZE_COLOR(colorType);
	        	API_draw_text(atoi(ParsedData[1]), atoi(ParsedData[2]), colorType, ParsedData[4],(ParsedData[5]), atoi(ParsedData[6]), atoi(ParsedData[7]), atoi(ParsedData[8]));
	            	break;
	        case CMD_BITMAP:
			API_draw_bitmap(atoi(ParsedData[1]), atoi(ParsedData[2]), atoi(ParsedData[3]));
			break;
	        case CMD_CLEAR_SCREEN:
	        	colorType = get_color_type(ParsedData[1]);
	        	SANITIZE_COLOR(colorType);
	            	API_clearscreen(colorType);
	            	break;
	        case CMD_UNKNOWN:
	            // Foutafhandeling voor onbekende commando's
	            LOGW("Onbekend commando: %s\n", ParsedData[0]);
	            break;
		}

}


