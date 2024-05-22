/******************************************************************************
*   Includes                                                                  *
******************************************************************************/
#include "API_draw.h"
#include "math.h"
/******************************************************************************
*   #defines                                                                  *
******************************************************************************/
#define SHIFTING_MULTIPLY_FLOATLESS_CALC (8)
/******************************************************************************
*   Typedefs                                                                  *
******************************************************************************/

/******************************************************************************
*   Globals vars                                                              *
******************************************************************************/

/******************************************************************************
*   Local vars                                                                *
******************************************************************************/

/******************************************************************************
*   Local function prototypes                                                 *
******************************************************************************/
int DrawLine(int x1, int x2, int y1, int y2, int color);

/******************************************************************************
*   Global functions                                                          *
******************************************************************************/
/**
  * @brief  Sets text on screen in the wanted colour and font
  * @param  x_lup Start location of the text x-axis
  * @param  y_lup Start location of the text y-axis
  * @param  color String to be displayed
  * @param  fontname Font name
  * @param  fontsize Size of characters
  * @param  fontstyle
  * @param  reserved Unused
  * @retval Error code
  */
int API_draw_text(int x_lup, int y_lup, int color, char* text, char* fontname,
                  int fontsize, int fontstyle, int reserved)
{
    LOGW("Not implemented yet");
    return 1;
}

/**
  * @brief  Sets line on screen
  * @param  x1 Start location x
  * @param  y1 Start location y
  * @param  x2 End location x
  * @param  y2 End location y
  * @param  color Line colour
  * @param  weight Width of the line in pixels
  * @param  reserved Unused
  * @retval Error code
  */
int API_draw_line(int x1, int y1, int x2, int y2, int color, int weight, int reserved)
{
    if(weight <= 0)
    {
        LOGE("Can not draw a line of 0 or less pixels");
        return -1;
    }
    if((x1 < 0) || (x2 < 0) || (y1 < 0) || (y2 < 0) ||
            (x1 >= VGA_DISPLAY_X) || (x2 >= VGA_DISPLAY_X) ||
            (y1 >= VGA_DISPLAY_Y) || (y2 >= VGA_DISPLAY_Y))
    {
        LOGE("Line start and end point must be on screen");
        return -1;
    }
    if(x1 > x2) // will be optimised out
    {
        int xHolder = x1;
        x1 = x2;
        x2 = xHolder;
    }
    if(y1 > y2) // will be optimised out
    {
        int yHolder = y1;
        y1 = y2;
        y2 = yHolder;
    }
    int stepX = 0;
    int stepY = 0;
    int deltaX = x2 - x1;
    int deltaY = y2 - y1;
    if(deltaX >= deltaY)
    {
        stepX++;
    }
    else
    {
        stepY++;
    }
    for(int i = 0; i < weight; i++)
    {
        DrawLine(x1 + (stepX * i), x2 + (stepX * i), y1 + (stepY * i), y2 + (stepY * i), color);
    }
    LOGI("Line from {%d;%d} to {%d;%d} of %d thick", x1, y1, x2, y2, weight);
    return 0;
}

/**
  * @brief  Sets rectangle on screen
  * @param  x Start location x
  * @param  y Start location y
  * @param  width Width of rectangle
  * @param  height Height of rectangle
  * @param  color Colour of rectangle
  * @param  filled Fill rectangle
  * @param  reserved0 Unused
  * @param  reserved1 Unused
  * @retval Error code
  */
int API_draw_rectangle(int x, int y, int width, int height, int color, int filled,
                       int reserved0, int reserved1)
{
    if((x < 0) || (y < 0) || (width < 0) || (height < 0) || (x >= VGA_DISPLAY_X) || (y >= VGA_DISPLAY_Y) ||
            (x + width >= VGA_DISPLAY_X) || (y + height >= VGA_DISPLAY_Y))
    {
        LOGE("Out of bounce");
        return -1;
    }

    if(filled)
    {
        for(int i = y; i < (y + height); i++)
        {
        	DrawLine(x, i, x + width, i, color);
        }
    }
    else
    {
        DrawLine(x, y, x + width, y, color);
        DrawLine(x, y, x, y + height, color);
        DrawLine(x + width, y, x + width, y + height, color);
        DrawLine(x, y + height, x + width, y + height, color);
    }
    LOGI("Rectangle draw with width/height: {%d; %d} from {%d; %d} with color: 0x%x", width, height, x, y, color);
    return 0;
}

/**
  * @brief  Sets text on screen in the wanted colour and font
  * @param  x_lup Start location x
  * @param  y_lup Start location y
  * @param  bm_nr Bitmap number
  * @retval Error code
  */
int API_draw_bitmap(int x_lup, int y_lup, int bm_nr)
{
    LOGW("Not implemented yet");
    return 1;
}

/**
  * @brief  Fill screen with a the wanted colour
  * @param  color Colour of screen
  * @retval Error code
  */
int API_clearscreen(uint8_t color)
{
    uint16_t xp, yp;
    for(yp = 0; yp < VGA_DISPLAY_Y; yp++)
    {
        for(xp = 0; xp < VGA_DISPLAY_X; xp++)
        {
            VGA_SetPixel(xp, yp, color);
        }
    }
    LOGI("Screen filled with color: 0x%2x", color);
    return 0;
}

/******************************************************************************
*   Local functions                                                           *
******************************************************************************/
int DrawLine(int x1, int x2, int y1, int y2, int color)
{
    if(x1 > x2) // will be optimised out
    {
        int xHolder = x1;
        x1 = x2;
        x2 = xHolder;
    }
    if(y1 > y2) // will be optimised out
    {
        int yHolder = y1;
        y1 = y2;
        y2 = yHolder;
    }
    int deltaX = x2 - x1;
    int deltaY = y2 - y1;
    int errorXY = 0;
    if(deltaX == 0)
    {
        errorXY = INT32_MIN;
    }
    else if(deltaY == 0)
    {
        errorXY = INT32_MAX;
    }
    else
    {
        errorXY = (deltaX<<SHIFTING_MULTIPLY_FLOATLESS_CALC) / deltaY;
    }
    int workingError = errorXY;
    while((x1 != x2) || (y1 != y2))
    {
        VGA_SetPixel(x1, y1, color);
        if(workingError == INT32_MAX)
        {
            x1++;
            continue;
        }
        else if(workingError == INT32_MIN)
        {
            y1++;
            continue;
        }
        if(workingError == (1<<SHIFTING_MULTIPLY_FLOATLESS_CALC))
        {
            x1++;
            y1++;
            workingError -= (1<<SHIFTING_MULTIPLY_FLOATLESS_CALC);
        }
        else if(workingError > (1<<SHIFTING_MULTIPLY_FLOATLESS_CALC))
        {
            x1++;
            workingError -= (1<<SHIFTING_MULTIPLY_FLOATLESS_CALC);
            continue;
        }
        else
        {
            y1++;
        }
        workingError += errorXY;
    }
    return 0;
}
