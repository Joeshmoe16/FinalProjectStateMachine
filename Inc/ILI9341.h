/*************************************************
 * AUTHOR(s): Tom Stutz, Joey Shotts
 * 
 * FILE: ILI9341.h
 * 
 * PROGRAM: 
 * 
 * DATE: (DD-MM-YYYY) 02-11-2024
 * 
 * 
 * MINIMAL HEADER
 *************************************************/


#ifndef __ILI9341__
#define __ILI9341__

#include "bitmap_typedefs.h"
#include "spi.h"
#include "stm32f4xx.h"
#include "stdio.h"
#include "stdint.h"
#include "primary.h"

//#define BIT(x) (1<<(x))

#define TFT_WIDTH 320
#define TFT_HEIGHT 240


//Pin Controls, dependent on what pins you choose
#define LCD_RST_PORT GPIOA
#define LCD_CS_PORT  GPIOA
#define LCD_DCX_PORT GPIOA

//#define MODER 0
#define LCD_RST 9
#define LCD_CS  8
#define LCD_DCX  10

//Commands to turn and off respective pins
#define RESET_LCD_CS   GPIOA -> ODR &= ~(BIT(LCD_CS));
#define SET_LCD_CS     GPIOA -> ODR |= BIT(LCD_CS);

#define SET_LCD_RST    GPIOA -> ODR |= (BIT(LCD_RST));
#define RESET_LCD_RST  GPIOA -> ODR &= ~(BIT(LCD_RST));
 
#define RESET_LCD_DCX  GPIOA -> ODR &= ~(BIT(LCD_DCX));
#define SET_LCD_DCX    GPIOA -> ODR |= (BIT(LCD_DCX));

//Commands
#define ILI_NOP     0x00
#define ILI_SWRESET 0x01
#define ILI_RDDID   0xD3
#define ILI_RDDST   0x09

#define ILI_SLPIN  0x10
#define ILI_SLPOUT 0x11
#define ILI_PTLON  0x12
#define ILI_NORON  0x13

#define ILI_RDMODE     0x0A
#define ILI_RDMADCTL   0x0B
#define ILI_RDPIXFMT   0x0C
#define ILI_RDIMGFMT   0x0D
#define ILI_RDSELFDIAG 0x0F

#define ILI_INVOFF   0x20
#define ILI_INVON    0x21
#define ILI_GAMMASET 0x26
#define ILI_DISPOFF  0x28
#define ILI_DISPON   0x29

#define ILI_CASET 0x2A
#define ILI_PASET 0x2B
#define ILI_RAMWR 0x2C
#define ILI_RAMRD 0x2E

#define ILI_PTLAR  0x30
#define ILI_MADCTL 0x36
#define ILI_PIXFMT 0x3A

#define ILI_FRMCTR1 0xB1
#define ILI_FRMCTR2 0xB2
#define ILI_FRMCTR3 0xB3
#define ILI_INVCTR  0xB4
#define ILI_DFUNCTR 0xB6

#define ILI_PWCTR1 0xC0
#define ILI_PWCTR2 0xC1
#define ILI_PWCTR3 0xC2
#define ILI_PWCTR4 0xC3
#define ILI_PWCTR5 0xC4
#define ILI_VMCTR1 0xC5
#define ILI_VMCTR2 0xC7

#define ILI_RDID1 0xDA
#define ILI_RDID2 0xDB
#define ILI_RDID3 0xDC
#define ILI_RDID4 0xDD

#define ILI_GMCTRP1 0xE0
#define ILI_GMCTRN1 0xE1




#define BLACK       0x0000      /*   0,   0,   0 */
#define NAVY        0x000F      /*   0,   0, 128 */
#define DARKGREEN   0x03E0      /*   0, 128,   0 */
#define DARKCYAN    0x03EF      /*   0, 128, 128 */
#define MAROON      0x7800      /* 128,   0,   0 */
#define PURPLE      0x780F      /* 128,   0, 128 */
#define OLIVE       0x7BE0      /* 128, 128,   0 */
#define LIGHTGREY   0xC618      /* 192, 192, 192 */
#define DARKGREY    0x7BEF      /* 128, 128, 128 */
#define BLUE        0x001F      /*   0,   0, 255 */
#define GREEN       0x07E0      /*   0, 255,   0 */
#define CYAN        0x07FF      /*   0, 255, 255 */
#define RED         0xF800      /* 255,   0,   0 */
#define MAGENTA     0xF81F      /* 255,   0, 255 */
#define YELLOW      0xFFE0      /* 255, 255,   0 */
#define WHITE       0xFFFF      /* 255, 255, 255 */
#define ORANGE      0xFD20      /* 255, 165,   0 */
#define GREENYELLOW 0xAFE5      /* 173, 255,  47 */
#define PINK        0xF81F

#define LANDSCAPE 1
#define PORTRAIT 0
#define LANDSCAPE_INV 3
#define PORTRAIT_INV 2




/*****************************************************************************/
//                           USER FUNCTION PROTOTYPE
/*****************************************************************************/
#ifndef __SYSTICK__
#define __SYSTICK__
/// @brief Initializes SysTick timer for delays.
// void SysTick_Init(void);

/// @brief Delays a given number of milliseconds.
/// @param n is number of milliseconds to delay.
void delayMS(uint16_t n);

/// @brief Delays a given number of microseconds.
/// @param n is number of microseconds to delay.
void delayMicroS(uint16_t n);
#endif

/// @brief Set an area for drawing on the display.
/// @param x1 is start column address.
/// @param y1 is start row address.
/// @param x2 is end column address.
/// @param y2 is end row address.
void Set_Address_Window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

/// @brief Fills number of pixels with a color.
/// @brief Note: Call Set_Address_Window() before calling this function.
/// @param color is 16-bit RGB565 color value.
/// @param len is 32-bit number of pixels.
void Fill_Color(uint16_t color, uint32_t len);

/// @brief Draw a line from (x0,y0) to (x1,y1).
/// @param x0 is start column address.
/// @param y0 is start row address.
/// @param x1 is end column address.
/// @param y1 is end row address.
/// @param width is thickness of the line.
/// @param color is 16-bit RGB565 color of the line.
void Draw_Line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t width, uint16_t color);

/// @brief Draw a vertical line starting top to bottom.
/// @param x0 is start column address.
/// @param length is length of line.
/// @param row is start row address.
/// @param width is width of line.
/// @param color is color of line.
void Draw_VLine(uint16_t x0, uint16_t length, uint16_t row, uint8_t width, uint16_t color);

/// @brief Draw a horizontal line starting left to right.
/// @param y0 is start row address
/// @param length is length of line.
/// @param col is start column address.
/// @param width is width of line.
/// @param color is color of line.
void Draw_HLine(uint16_t y0, uint16_t length, uint16_t col, uint8_t width, uint16_t color);

/// @brief Draw a rectangle without filling it.
/// @param x is start column address.
/// @param y is start row address.
/// @param w is width of rectangle.
/// @param h is height of rectangle.
/// @param color is color of box.
void Draw_Box(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t thickness, uint16_t color);

/// @brief Inverts the colors on the display.
void Invert_Display(void);

/// @brief Rotate display to an orientation. 0: Portrait. 1: Landscape.
/// @brief 2: Portrait Inverted. 3: Landscape Inverted.
/// @param rotation Values 0, 1, 2, 3. Else, default to Portrait.
void Rotate_Display(uint8_t rotation);

/// @brief Initialize the display driver.
void Display_Init(void);

/// @brief Sets up the GPIO pin modes of the display pins.
void GPIO_PinMode_Setup(void);

/// @brief Sends an 8-bit parameter to the display.
/// @param parameter is the parameter to send.
void ILI_Push_16Bit(uint16_t parameter);

/// @brief Sends an 8-bit command to the display.
/// @param command is the command to send.
void ILI_8Bit_Command(uint8_t command);

/// @brief Sends an 8-bit data to the display.
/// @param data is the data to send.
void ILI_8Bit_Data(uint8_t data);

/// @brief Fills a rectangular area with color.
/// @param x is start col address.
/// @param y is start row address.
/// @param w is width of rectangle.
/// @param h is height of rectangle.
/// @param color is 16-bit RGB565 color.
void Fill_Rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);

/// @brief Fills the entire display with color.
/// @param color is 16-bit RGB565 color.
void Fill_Screen(uint16_t color);

/// @brief Writes a string on the display. Select Background color.
/// @param x is top left col address.
/// @param y is top left row address.
/// @param str is pointer to the string to be drawn.
/// @param fore_color is 16-bit RGB565 foreground color.
/// @param back_color is 16-bit RGB565 background color.
/// @param font is pointer to the font of the string.
void Draw_String_BG(uint16_t x, uint16_t y, char *str, uint16_t fore_color, uint16_t back_color, const tFont *font);

/// @brief Writes a string on the display. No Background color.
/// @param x is top left col address.
/// @param y is top left row address.
/// @param str is pointer to the string to be drawn.
/// @param fore_color is 16-bit RGB565 foreground color.
/// @param font is pointer to the font of the string.
void Draw_String_NoBG(uint16_t x, uint16_t y, char *str, uint16_t fore_color, const tFont *font);

/// @brief Write a character at a given position. Select Background color.
/// @param x is top left col address.
/// @param y is top left row address.
/// @param character is the ASCII character to be drawn.
/// @param fore_color is foreground color.
/// @param back_color is background color.
/// @param font is pointer to the font of the character.
void Draw_Char_BG(uint16_t x, uint16_t y, char character, uint16_t fore_color, uint16_t back_color, const tFont *font);

/// @brief Write a character at a given position. No Background color.
/// @param x is top left col address.
/// @param y is top left row address.
/// @param character is the ASCII character to be drawn.
/// @param fore_color is foreground color.
/// @param font is pointer to the font of the character.
void Draw_Char_NoBG(uint16_t x, uint16_t y, char character, uint16_t fore_color, const tFont *font);

/// @brief Display a bitmap image on the screen.
/// @param x is top left col address.
/// @param y is top left row address.
/// @param bitmap is pointer to the image data to be drawn.
void Draw_Bitmap(uint16_t x, uint16_t y, const tImage16bit *bitmap);

/// @brief Draw a pixel at a coord x,y with color.
/// @param x is col address.
/// @param y is row address.
void Draw_Pixel(uint16_t x, uint16_t y, uint16_t color);

/// @brief Write a character at a given position of selected size
/// @param x is top left col address.
/// @param y is top left row address.
/// @param char is the ASCII character to be drawn.
/// @param textColor is foreground color.
/// @param bgColor is the background color
/// @param size is the font size
void DrawCharS(int16_t x, int16_t y, char c, int16_t textColor, int16_t bgColor, uint8_t size);


void doFontTest(const tFont *font);

void drawCircle(uint16_t xc, uint16_t yc, uint16_t x, uint16_t y, uint16_t color);

void circleBres(uint16_t xc, uint16_t yc, uint16_t r, uint16_t thk, uint16_t color);

void perimeterCircle(uint16_t xc, uint16_t yc, uint16_t r,uint16_t thk, uint16_t color);

void drawCirclePerimeterCircle(uint16_t xc, uint16_t yc, uint16_t x, uint16_t y, uint16_t r, uint16_t thk, uint16_t color);

void DrawPaletteImage(uint16_t x, uint16_t y, const tPaletteImage *imageData);

void DrawPaletteGif(uint8_t f, uint16_t x, uint16_t y, const tPaletteGif *imageData);

/*****************************************************************************/
/*****************************************************************************/

#endif // _ILI9341_H_

/* EOF */
