//--------------------------------------------------------------
// File     : stm32_ub_vga_320x240.c
// CPU      : STM32F4
// IDE      : CooCox CoIDE 1.7.0
// Module   : GPIO, TIM, MISC, DMA
// Function : vgaData_s out by GPIO (320x240 Pixel, 8bit color)
//
// signals  : PB11      = HSync-Signal
//            PB12      = VSync-Signal
//            PE8+PE9   = color Blue
//            PE10-PE12 = color Green
//            PE13-PE15 = color red
//
// uses     : TIM1, TIM2
//            DMA2, Channel6, Stream5
//--------------------------------------------------------------


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "VGA_io_driver.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern DMA_HandleTypeDef hdma_tim1_up;

typedef struct {
    uint16_t    lineCounter;         // Line counter
    uint32_t    startAddressDMA;     // Start Address for the DMA transfer
    uint32_t    dmaConfigRegister;   // Register constant CR-Register
    uint8_t     videoRAM[(VGA_DISPLAY_X + 1) * VGA_DISPLAY_Y];
} VGA_metaData_s;

VGA_metaData_s vgaData_s;

//--------------------------------------------------------------
// Init vgaData_s-Module
//--------------------------------------------------------------
void VGA_Init(void)
{
    vgaData_s.lineCounter = 0;
    vgaData_s.startAddressDMA = 0;
    vgaData_s.dmaConfigRegister = 0;

  GPIOB->BSRR = VGA_VSYNC_Pin;

  // TIM2
  HAL_TIM_Base_Start(&htim2);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
  HAL_TIM_PWM_Start_IT(&htim2, TIM_CHANNEL_3);

  // TIM1
  __HAL_TIM_ENABLE_DMA(&htim1, TIM_DMA_UPDATE);
  __HAL_TIM_ENABLE(&htim1);
  HAL_DMA_Start_IT(&hdma_tim1_up, (uint32_t)&vgaData_s.videoRAM[0], VGA_GPIOE_ODR_ADDRESS, VGA_DISPLAY_X + 1);

  HAL_DMA_Init(&hdma_tim1_up);
  __HAL_DMA_ENABLE_IT(&hdma_tim1_up, DMA_IT_TC);

  //-----------------------
  // Register swap and safe
  //-----------------------
  // content of CR-Register read and save
  vgaData_s.dmaConfigRegister = DMA2_Stream5->CR;
}


//--------------------------------------------------------------
// fill the DMA RAM buffer with one color
//--------------------------------------------------------------
void VGA_FillScreen(uint8_t color)
{
  uint16_t xp,yp;

  for(yp = 0; yp < VGA_DISPLAY_Y; yp++) {
    for(xp = 0; xp < VGA_DISPLAY_X; xp++) {
        VGA_SetPixel(xp, yp, color);
    }
  }
}


//--------------------------------------------------------------
// put one Pixel on the screen with one color
// Important : the last Pixel+1 from every line must be black (don't know why??)
//--------------------------------------------------------------
void VGA_SetPixel(uint16_t xp, uint16_t yp, uint8_t color)
{
  if(xp >= VGA_DISPLAY_X)
    xp = 0;
  if(yp >= VGA_DISPLAY_Y)
    yp = 0;

  // Write pixel to ram
    vgaData_s.videoRAM[(yp * (VGA_DISPLAY_X + 1)) + xp] = color;
}

__inline void VGA_InterruptHsync(void)
{
    vgaData_s.lineCounter++;
    if (vgaData_s.lineCounter >= VGA_VSYNC_PERIODE)
    {
        // Address pointer first dot
        vgaData_s.lineCounter = 0;
        vgaData_s.startAddressDMA = (uint32_t)(&vgaData_s.videoRAM[0]);
    }

    // HSync-Pixel
    GPIOB->BSRR = (vgaData_s.lineCounter < VGA_VSYNC_IMP) ? VGA_VSYNC_Pin<<16u : VGA_VSYNC_Pin;

    // Test for DMA start
    if((vgaData_s.lineCounter >= VGA_VSYNC_BILD_START) && (vgaData_s.lineCounter <= VGA_VSYNC_BILD_STOP))
    {
        // Restart DMA transfer
        DMA2_Stream5->CR = vgaData_s.dmaConfigRegister;
        DMA2_Stream5->M0AR = vgaData_s.startAddressDMA;
        TIM1->CR1 |= TIM_CR1_CEN;
        __HAL_DMA_ENABLE(&hdma_tim1_up);
        // Test Adrespointer for high
        if(vgaData_s.lineCounter & 0x01)
            vgaData_s.startAddressDMA += (VGA_DISPLAY_X + 1); // inc after Hsync
    }
}

__inline void VGA_InterruptDma(void)
{
    // Timer1 stop
    __HAL_TIM_DISABLE(&htim1);
    // switch on black
    GPIOE->BSRR = VGA_GPIO_HINIBBLE<<16u;
}
