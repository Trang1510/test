/******************************************************************************
*   Includes                                                                  *
******************************************************************************/
#include "API_draw.h"

/******************************************************************************
*   #defines                                                                  *
******************************************************************************/

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

/******************************************************************************
*   Global functions                                                          *
******************************************************************************/
int API_draw_text(int x_lup, int y_lup, int color, char* text, char* fontname,
                  int fontsize, int fontstyle, int reserved)
{
    return 1;
}

int API_draw_line(int x_1, int y_1, int x_2, int y2, int color, int weight, int reserved)
{
    if(weight <= 0)
        return -1;
    if((x_1 < 0) || (x_2 < 0) || (y_1 < 0) || (y2 < 0))
        return 1;
    if(x_1 > x_2)
    {
        int x_1Holder = x_1;
        x_1 = x_2;
        x_2 = x_1Holder;
    }
    if(y_1 > y2)
    {
        int y_1Holder = y_1;
        y_1 = y2;
        y2 = y_1Holder;
    }
    uint16_t currentX = x_1;
    uint16_t currentY = y_1;
    int errorX = x_2 - currentX;
    int errorY = y2 - currentY;
    uint32_t error = errorY / errorX;
    do
    {
        VGA_SetPixel(currentX, currentY, color);
        if(error < 1)
        {
            currentY++;
        }
        else if(error)
        {
            currentY++;
        }
        else
        {
            currentX++;
            currentY++;
        }
        errorX = x_2 - currentX;
        errorY = y2 - currentY;
    }while(currentx != x_2 && current != y2);
    return 0;
}


int API_draw_rectangle(int x, int y, int width, int height, int color, int filled,
                       int reserved0, int reserved1)
{
    return 1;
}

int API_draw_bitmap(int x_lup, int y_lup, int bm_nr)
{
    return 1;
}

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
    return 0;
}

/******************************************************************************
*   Local functions                                                           *
******************************************************************************/
