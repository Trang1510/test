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
#define SANITIZE_COLOR(colorTypeReturn) ({                                  \
    if (colorTypeReturn == CMD_UNKNOWN_COLOR)                               \
    {                                                                       \
        LOGE("Color unkown");                                               \
        return;                                                             \
    }                                                                       \
})
/******************************************************************************
 *   Typedefs                                                                  *
 ******************************************************************************/

/******************************************************************************
 *   Globals vars                                                              *
 ******************************************************************************/
input_vars Input;

/******************************************************************************
 *   Local vars                                                                *
 ******************************************************************************/
//array for testing
char* ParsedData[MAX_VARS];

/******************************************************************************
 *   Local function prototypes                                                 *
 ******************************************************************************/
/**
 * @brief Converteert een commando string naar een CommandType waarde.
 * @param command De commando string om te converteren.
 * @return CommandType De overeenkomende CommandType waarde.
 */
CommandType GetCommandType(const char* command)
{
    if(strncmp(command, "lijn", strlen("lijn")) == 0)
        return CMD_LINE;
    if(strncmp(command, "tekst", strlen("tekst")) == 0)
        return CMD_TEXT;
    if(strncmp(command, "debug", strlen("debug")) == 0)
        return CMD_DEBUG;
    if(strncmp(command, "bitmap", strlen("bitmap")) == 0)
        return CMD_BITMAP;
    if(strncmp(command, "rechthoek", strlen("rechthoek")) == 0)
        return CMD_RECTANGLE;
    if(strncmp(command, "clearscherm", strlen("clearscherm")) == 0)
        return CMD_CLEAR_SCREEN;
    return CMD_UNKNOWN;
}

ColorType GetColorType(const char* colorString)
{
    if(strncmp(colorString, "wit", strlen("wit")) == 0)
        return VGA_COLOUR_WHITE;
    if(strncmp(colorString, "rood", strlen("rood")) == 0)
        return VGA_COLOUR_RED;
    if(strncmp(colorString, "cyaan", strlen("cyaan")) == 0)
        return VGA_COLOUR_CYAN;
    if(strncmp(colorString, "blauw", strlen("blauw")) == 0)
        return VGA_COLOUR_BLUE;
    if(strncmp(colorString, "geel", strlen("geel")) == 0)
        return VGA_COLOUR_YELLOW;
    if(strncmp(colorString, "zwart", strlen("zwart")) == 0)
        return VGA_COLOUR_BLACK;
    if(strncmp(colorString, "groen", strlen("groen")) == 0)
        return VGA_COLOUR_GREEN;
    if(strncmp(colorString, "bruin", strlen("bruin")) == 0)
        return CMD_UNKNOWN_COLOR;
    if(strncmp(colorString, "grijs", strlen("grijs")) == 0)
        return CMD_UNKNOWN_COLOR;
    if(strncmp(colorString, "magenta", strlen("magenta")) == 0)
        return VGA_COLOUR_MAGENTA;
    if(strncmp(colorString, "lichtrood", strlen("lichtrood")) == 0)
        return CMD_UNKNOWN_COLOR;
    if(strncmp(colorString, "lichtblauw", strlen("lichtblauw")) == 0)
        return CMD_UNKNOWN_COLOR;
    if(strncmp(colorString, "lichtgroen", strlen("lichtgroen")) == 0)
        return CMD_UNKNOWN_COLOR;
    if(strncmp(colorString, "lichtcyaan", strlen("lichtcyaan")) == 0)
        return CMD_UNKNOWN_COLOR;
    if(strncmp(colorString, "lichtmagenta", strlen("lichtmagenta")) == 0)
        return CMD_UNKNOWN_COLOR;
    return CMD_UNKNOWN_COLOR;
}

/******************************************************************************
*   Global functions                                                          *
******************************************************************************/

/**
 * @brief  Separates strings from incoming array to parsed data
 * @param  input.line_rx_buffer used
 * @retval None currently
 */
void LL_Receive(void)
{
    char* data;
    char UART_copy[LINE_BUFLEN] = {0};                //if not init as 0 leads to last parsed data inconsistency

    strncpy(UART_copy, Input.line_rx_buffer, Input.msglen);    //string copy as double buffer for safety

    data = strtok(UART_copy, ",");                // Get the first token

    // Walk through other tokens
    int i = 0;
    while(data != NULL && i < MAX_VARS)
    {
        ParsedData[i++] = data;
        data = strtok(NULL, ",");
    }

    //DEBUG for data inside the parsed array
    for(int j = 0; j < i; j++)
    {
        LOGD("data bit %d: %s", j, ParsedData[j]);
    }
    LL_Execute();                            //after finished parsing call LL_exec
}

/**
 * @brief Voert een commando uit op basis van de opgegeven parameters.
 * @param ParsedData Array van strings met commando en parameters.
 */
void LL_Execute(void)
{
    CommandType commandType = GetCommandType(ParsedData[0]);
    ColorType colorType;
    switch(commandType)
    {
    case CMD_LINE:
        colorType = GetColorType(ParsedData[5]);
        SANITIZE_COLOR(colorType);
        API_draw_line(atoi(ParsedData[1]),
                      atoi(ParsedData[2]),
                      atoi(ParsedData[3]),
                      atoi(ParsedData[4]),
                      colorType,
                      atoi(ParsedData[6]),
                      atoi(ParsedData[7]));
        break;

    case CMD_RECTANGLE:
        colorType = GetColorType(ParsedData[5]);
        SANITIZE_COLOR(colorType);
        API_draw_rectangle(atoi(ParsedData[1]),
                           atoi(ParsedData[2]),
                           atoi(ParsedData[3]),
                           atoi(ParsedData[4]),
                           colorType,
                           atoi(ParsedData[6]),
                           atoi(ParsedData[7]),
                           atoi(ParsedData[8]));
        break;

    case CMD_TEXT:
        colorType = GetColorType(ParsedData[3]);
        SANITIZE_COLOR(colorType);
        API_draw_text(atoi(ParsedData[1]),
                      atoi(ParsedData[2]),
                      colorType, ParsedData[4],
                      (ParsedData[5]),
                      atoi(ParsedData[6]),
                      atoi(ParsedData[7]),
                      atoi(ParsedData[8]));
        break;

    case CMD_BITMAP:
        API_draw_bitmap(atoi(ParsedData[1]),
                        atoi(ParsedData[2]),
                        atoi(ParsedData[3]));
        break;

    case CMD_CLEAR_SCREEN:
        colorType = GetColorType(ParsedData[1]);
        SANITIZE_COLOR(colorType);
        API_clearscreen(colorType);
        break;

    case CMD_DEBUG:
        int logLevel = atoi((ParsedData[1]));
        if(logLevel >= LOG_NONE && logLevel <= LOG_VERBOSE)
        {
            LOG_SetLogLevel(logLevel);
            LOGI("Log level changes: %d", logLevel);
        }
        else
            LOGE("Log level not supported");
        break;

    case CMD_UNKNOWN:

    default:
        // Foutafhandeling voor onbekende commando's
        LOGW("Onbekend commando: %s\n", ParsedData[0]);
        break;
    }
}


