/******************************************************************************
*   Includes                                                                  *
******************************************************************************/
#include "API_draw.h"
#include "math.h"
/******************************************************************************
*   #defines                                                                  *
******************************************************************************/

/******************************************************************************
*   Typedefs                                                                  *
******************************************************************************/
//------------------------------              ------------------------------
//|              |             |            \ |              |             |
//|     DATA     |     NEXT    |--------------|     DATA     |     NEXT    |
//|              |             |            / |              |             |
//------------------------------              ------------------------------

typedef struct elements
{
    char* element;
    struct elements* nextElement;
} elementsParsed_t;

typedef struct dataParsed
{
    char command[0x40];
    uint8_t totalElements;
    elementsParsed_t* elements;
} dataParsed_t;

// Jack output 5x char*
// Trang input 19x char*

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
    return 1;
}

/**
  * @brief  Sets line on screen
  * @param  x_1 Start location x
  * @param  y_1 Start location y
  * @param  x_2 End location x
  * @param  y2 End location y
  * @param  color Line colour
  * @param  weight Width of the line in pixels
  * @param  reserved Unused
  * @retval Error code
  */
int API_draw_line(int x_1, int y_1, int x_2, int y2, int color, int weight, int reserved)
{
    return 1;
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
    return 1;
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
    return 0;
}

/******************************************************************************
*   Local functions                                                           *
******************************************************************************/
