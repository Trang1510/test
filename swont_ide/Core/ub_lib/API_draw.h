#ifndef SWONT_IDE_API_DRAW_H
#define SWONT_IDE_API_DRAW_H
/**
  ******************************************************************************
  * @file    API_draw.c
  * @brief   This file provides code for the drawing of anything more complex
  *          on a vga screen then a single pixel
  ******************************************************************************
  */
/******************************************************************************
*   Includes                                                                  *
******************************************************************************/
#include "stdint.h"
#include "VGA_io_driver.h"

/******************************************************************************
*   #defines                                                                  *
******************************************************************************/
#define SCREEN_SIZE_HORIZONTAL  (VGA_DISPLAY_X)
#define SCREEN_SIZE_VERTICAL    (VGA_DISPLAY_Y)
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

int API_draw_line (int x_1, int y1, int x2, int y2, int color, int weight, int reserved);

int API_draw_rectangle (int x, int y, int width, int height, int color, int filled,
                        int reserved0, int reserved1);

int API_draw_bitmap (int x_lup, int y_lup, int bm_nr);

int API_clearscreen(uint8_t color);

#endif //SWONT_IDE_API_DRAW_H
