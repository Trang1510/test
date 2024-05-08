#ifndef SWONT_IDE_API_DRAW_H
#define SWONT_IDE_API_DRAW_H
/******************************************************************************
*   Includes                                                                  *
******************************************************************************/
#include "stdint.h"
#include "VGA_io_driver.h"

/******************************************************************************
*   #defines                                                                  *
******************************************************************************/
#define SCREEN_SIZE_HORIZONTAL  (320)
#define SCREEN_SIZE_VERTICAL    (240)
/******************************************************************************
*   Typedefs                                                                  *
******************************************************************************/

/******************************************************************************
*   Globals vars                                                              *
******************************************************************************/

/******************************************************************************
*   Global function prototypes                                                *
******************************************************************************/
int API_draw_text (int x_lup, int y_lup, int color, char *text, char *fontname,
                   int fontsize, int fontstyle, int reserved); // fontsize:1 small, 2 big

int API_draw_line (int x_1, int y_1, int x_2, int y2, int color, int weight, int reserved);

int API_draw_rectangle (int x, int y, int width, int height, int color, int filled,
                        int reserved0, int reserved1);

int API_draw_bitmap (int x_lup, int y_lup, int bm_nr);

int API_clearscreen(uint8_t color);

#endif //SWONT_IDE_API_DRAW_H
