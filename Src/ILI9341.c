/*************************************************
 * AUTHOR(s): Tom Stutz, Joey Shotts
 * 
 * FILE: config.h
 * 
 * PROGRAM: 
 * 
 * DATE: (DD-MM-YYYY) 02-11-2024
 * 
 * 
 * MINIMAL HEADER
 *************************************************/

#include "ILI9341.h"
#include <stdlib.h>
#include <string.h>

static uint16_t ILI_TFTwidth = TFT_WIDTH;
static uint16_t ILI_TFTheight = TFT_HEIGHT;
static uint8_t ILI_Orientation = 0;

/*****************************************************************************/
//                          USER FUNCTION DEFINITIONS
/*****************************************************************************/
/*void SysTick_Init(void)
{
    SysTick->CTRL = 0;
    SysTick->LOAD = 0x00FFFFFF;
    SysTick->VAL = 0;
    SysTick->CTRL |= 5;
}*/

void delayMS(uint16_t n)
{
    SysTick->LOAD = ((n * 16000) - 1);
    SysTick->VAL = 0;
    while ((SysTick->CTRL & 0x00010000) == 0)
    {
    }
}

void delayMicroS(uint16_t n)
{
    SysTick->LOAD = ((n * 16) - 1);
    SysTick->VAL = 0;
    while ((SysTick->CTRL & 0x00010000) == 0)
    {
    }
}

void Draw_String_BG(uint16_t x, uint16_t y, char *str, uint16_t fore_color, uint16_t back_color, const tFont *font)
{
    volatile uint16_t temp_x = x;
    volatile uint16_t temp_y = y;
    volatile uint16_t currWidth = font->chars[(uint8_t)str[0] - 0x20U].image->width;
    volatile uint8_t length = strlen(str);
    Draw_Char_BG(x, y, str[0], fore_color, back_color, font);
    for (uint8_t i = 1; i < length; i++)
    {
        temp_x += currWidth;
        currWidth = font->chars[(uint8_t)str[i] - 0x20U].image->width;
        if ((temp_x + currWidth) >= ILI_TFTwidth)
        {
            temp_y += font->chars[0].image->height;
            temp_x = 0;
        }
        Draw_Char_BG(temp_x, temp_y, str[i], fore_color, back_color, font);
    }
}

void Draw_String_NoBG(uint16_t x, uint16_t y, char *str, uint16_t fore_color, const tFont *font)
{
    uint16_t temp_x = x;
    uint16_t temp_y = y;
    uint16_t currWidth = font->chars[(uint8_t)str[0] - 0x20U].image->width;
    uint8_t length = strlen(str);
    Draw_Char_NoBG(x, y, str[0], fore_color, font);
    for (uint8_t i = 1; i < length; i++)
    {
        temp_x += currWidth;
        currWidth = font->chars[(uint8_t)str[i] - 0x20U].image->width;
        if ((temp_x + currWidth) >= ILI_TFTwidth)
        {
            temp_y += font->chars[0].image->height;
            temp_x = 0;
        }
        Draw_Char_NoBG(temp_x, temp_y, str[i], fore_color, font);
    }
}

void Draw_Char_BG(uint16_t x, uint16_t y, char character, uint16_t fore_color, uint16_t back_color, const tFont *font)
{
    const tImage *img = NULL;

    for (uint8_t i = 0; i < font->length; i++)
    {
        if (font->chars[i].code == character)
        {
            img = font->chars[i].image;
            break;
        }
    }

    /* No image for character found, so return from this function */
    if (img == NULL)
    {
        return;
    }

    uint16_t temp_x = x;
    uint16_t temp_y = y;
    uint16_t width = 0;
    uint16_t height = 0;
    uint8_t mask = 0x80;
    uint8_t bit_counter = 0;
    const uint8_t *img_data_ptr = (const uint8_t *)(img->data);
    uint8_t img_data = 0;

    width = img->width;
    height = img->height;

    // font bitmaps are stored in column major order (scanned from
    // left-to-right, not the conventional top-to-bottom) as font glyphs have
    // heigher height than width, this scanning saves some storage. So, we also
    // render in left-to-right manner.

    // Along x axis (width)
    for (int i = 0; i < width; i++)
    {
        // Along y axis (height)
        for (int j = 0; j < height; j++)
        {

            // load new data only when previous byte (or word, depends on
            // glyph->dataSize) is completely traversed by the mask bit_counter = 0
            // means glyph_data is completely traversed by the mask
            if (bit_counter == 0)
            {
                img_data = *img_data_ptr++;
                bit_counter = img->dataSize;
            }
            // Decrement bit counter
            bit_counter--;

            // If pixel is blank
            if (img_data & mask)
            {
                Draw_Pixel(temp_x, temp_y, back_color);
            }
            else
            {
                Draw_Pixel(temp_x, temp_y, fore_color);
            }

            img_data <<= 1;
            temp_y++;
        }

        // New col starts. So, row is set to initial value and col is increased by
        // one
        temp_y = y;
        temp_x++;

        // Reset the bit counter cause we're moving to next column, so we start
        // with a new byte
        bit_counter = 0;
    }
}

void Draw_Char_NoBG(uint16_t x, uint16_t y, char character, uint16_t fore_color, const tFont *font)
{
    const tImage *img = NULL;

    for (uint8_t i = 0; i < font->length; i++)
    {
        if (font->chars[i].code == character)
        {
            img = font->chars[i].image;
            break;
        }
    }

    /* No image for character found, so return from this function */
    if (img == NULL)
    {
        return;
    }

    uint16_t width = 0, height = 0;

    uint16_t temp_x = x;
    uint16_t temp_y = y;

    uint8_t mask = 0x80;
    uint8_t bit_counter = 0;

    const uint8_t *img_data_ptr = (const uint8_t *)(img->data);
    uint8_t img_data = 0;

    width = img->width;
    height = img->height;

    // font bitmaps are stored in column major order (scanned from
    // left-to-right, not the conventional top-to-bottom) as font glyphs have
    // heigher height than width, this scanning saves some storage. So, we also
    // render in left-to-right manner.

    // Along x axis (width)
    for (int i = 0; i < width; i++)
    {
        // Along y axis (height)
        for (int j = 0; j < height; j++)
        {

            // load new data only when previous byte (or word, depends on
            // glyph->dataSize) is completely traversed by the mask bit_counter = 0
            // means glyph_data is completely traversed by the mask
            if (bit_counter == 0)
            {
                img_data = *img_data_ptr++;
                bit_counter = img->dataSize;
            }
            // Decrement bit counter
            bit_counter--;

            if (img_data & mask)
            {
            }
            else
            {
                Draw_Pixel(temp_x, temp_y, fore_color);
            }
            img_data <<= 1;
            temp_y++;
        }

        // New col starts. So, row is set to initial value and col is increased by
        // one
        temp_y = y;
        temp_x++;

        // Reset the bit counter cause we're moving to next column, so we start
        // with a new byte
        bit_counter = 0;
    }
}

void Draw_Bitmap(uint16_t x, uint16_t y, const tImage16bit *bitmap)
{
    RESET_LCD_CS;
    uint16_t width = 0, height = 0;
    width = bitmap->width;
    height = bitmap->height;

    uint32_t total_pixels = bitmap->dataSize;

    Set_Address_Window(x, y, x + width - 1, y + height - 1);

    for (uint32_t pixels = 0; pixels < total_pixels; pixels++)
    {
        ILI_8Bit_Data((uint16_t)(bitmap->data[pixels]) >> 8);
        ILI_8Bit_Data((uint8_t)(bitmap->data[pixels]));
    }
    SET_LCD_CS;
}

void Fill_Screen(uint16_t color)
{
    RESET_LCD_CS;
    Set_Address_Window(0, 0, ILI_TFTwidth, ILI_TFTheight);
    Fill_Color(color, (uint32_t)ILI_TFTwidth * (uint32_t)ILI_TFTheight);
    SET_LCD_CS;
}

void Draw_Line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t width, uint16_t color)
{
    RESET_LCD_CS;

    int16_t dx;
    int16_t dy;
    int8_t yi;
    int8_t xi;

    int16_t D;
    uint16_t y;
    uint16_t x;

    uint8_t pixels_per_point = width * width;

    if (abs(y1 - y0) < abs(x1 - x0))
    {
        if (x0 > x1)
        {
            dx = x0 - x1;
            dy = y0 - y1;
            yi = 1;

            if (dy < 0)
            {
                yi = -1;
                dy = -dy;
            }

            D = 2 * dy - dx;
            y = y1;
            x = x1;

            while (x <= x0)
            {
                Set_Address_Window(x, y, x + width - 1, y + width - 1);
                // Drawing all the pixels of a single point

                for (uint8_t pixel_cnt = 0; pixel_cnt < pixels_per_point; pixel_cnt++)
                {
                    ILI_Push_16Bit(color);
                }
                if (D > 0)
                {
                    y = y + yi;
                    D = D - 2 * dx;
                }
                D = D + 2 * dy;
                x++;
            }
        }
        else
        {
            dx = x1 - x0;
            dy = y1 - y0;
            yi = 1;
            if (dy < 0)
            {
                yi = -1;
                dy = -dy;
            }

            D = 2 * dy - dx;
            y = y0;
            x = x0;

            while (x <= x1)
            {
                Set_Address_Window(x, y, x + width - 1, y + width - 1);
                // Drawing all the pixels of a single point

                for (uint8_t pixel_cnt = 0; pixel_cnt < pixels_per_point; pixel_cnt++)
                {
                    ILI_Push_16Bit(color);
                }
                if (D > 0)
                {
                    y = y + yi;
                    D = D - 2 * dx;
                }
                D = D + 2 * dy;
                x++;
            }
        }
    }

    else
    {
        if (y0 > y1)
        {
            dx = x0 - x1;
            dy = y0 - y1;
            xi = 1;

            if (dx < 0)
            {
                xi = -1;
                dx = -dx;
            }

            D = 2 * dx - dy;
            y = y1;
            x = x1;

            while (y <= y0)
            {
                Set_Address_Window(x, y, x + width - 1, y + width - 1);
                // Drawing all the pixels of a single point

                for (uint8_t pixel_cnt = 0; pixel_cnt < pixels_per_point; pixel_cnt++)
                {
                    ILI_Push_16Bit(color);
                }
                if (D > 0)
                {
                    x = x + xi;
                    D = D - 2 * dy;
                }
                D = D + 2 * dx;
                y++;
            }
        }
        else
        {
            dx = x1 - x0;
            dy = y1 - y0;
            xi = 1;

            if (dx < 0)
            {
                xi = -1;
                dx = -dx;
            }

            D = 2 * dx - dy;
            y = y0;
            x = x0;

            while (y <= y1)
            {
                Set_Address_Window(x, y, x + width - 1, y + width - 1);
                // Drawing all the pixels of a single point

                for (uint8_t pixel_cnt = 0; pixel_cnt < pixels_per_point; pixel_cnt++)
                {
                    ILI_Push_16Bit(color);
                }
                if (D > 0)
                {
                    x = x + xi;
                    D = D - 2 * dy;
                }
                D = D + 2 * dx;
                y++;
            }
        }
    }
    SET_LCD_CS;
}

void Draw_VLine(uint16_t x0, uint16_t length, uint16_t row, uint8_t width, uint16_t color)
{
    RESET_LCD_CS;
    Set_Address_Window(x0, row, x0 + width - 1, row + length - 1);
    Fill_Color(color, (uint32_t)width * (uint32_t)abs((row + length - 1) - row + 1));
    SET_LCD_CS;
}

void Draw_HLine(uint16_t y0, uint16_t length, uint16_t col, uint8_t width, uint16_t color)
{
    RESET_LCD_CS;
    Set_Address_Window(col, y0, col + length - 1, y0 + width - 1);
    Fill_Color(color, (uint32_t)width * (uint32_t)abs((col + length - 1) - col + 1));
    SET_LCD_CS;
}

void Draw_Pixel(uint16_t x, uint16_t y, uint16_t color)
{
    RESET_LCD_CS;
    Set_Address_Window(x, y, x, y);
    ILI_Push_16Bit(color);
    SET_LCD_CS;
}

void Invert_Display(void)
{
    static uint8_t inv_flag = 0;
    inv_flag ^= 1;
    RESET_LCD_CS;
    if (inv_flag)
    {
        ILI_8Bit_Command(ILI_INVON);
    }
    else
    {
        ILI_8Bit_Command(ILI_INVOFF);
    }
    SET_LCD_CS;
}

void Rotate_Display(uint8_t rotation)
{
    uint16_t temp_height = 320;
    uint16_t temp_width = 240;
    RESET_LCD_CS;
    switch (rotation)
    {
    /* Portrait */
    case 0:
        ILI_8Bit_Command(ILI_MADCTL); // Memory Access Control
        ILI_8Bit_Data(0x48);          // MX: 1, MY: 0, MV: 0
        ILI_TFTheight = temp_height;
        ILI_TFTwidth = temp_width;
        ILI_Orientation = 0;
        break;

    /* Landscape */
    case 1:
        ILI_8Bit_Command(ILI_MADCTL); // Memory Access Control
        ILI_8Bit_Data(0x28);          // MX: 0, MY: 0, MV: 1
        ILI_TFTheight = temp_width;
        ILI_TFTwidth = temp_height;
        ILI_Orientation = 1;
        break;

    /* Portrait Inverted */
    case 2:
        ILI_8Bit_Command(ILI_MADCTL); // Memory Access Control
        ILI_8Bit_Data(0x88);          // MX: 0, MY: 1, MV: 0
        ILI_TFTheight = temp_height;
        ILI_TFTwidth = temp_width;
        ILI_Orientation = 2;
        break;

    /* Landscape Inverted */
    case 3:
        ILI_8Bit_Command(ILI_MADCTL); // Memory Access Control
        ILI_8Bit_Data(0xE8);          // MX: 1, MY: 1, MV: 1
        ILI_TFTheight = temp_width;
        ILI_TFTwidth = temp_height;
        ILI_Orientation = 3;
        break;

    /* Default to Portrait */
    default:
        ILI_8Bit_Command(ILI_MADCTL); // Memory Access Control
        ILI_8Bit_Data(0x48);          // MX: 1, MY: 0, MV: 0
        ILI_TFTheight = temp_height;
        ILI_TFTwidth = temp_width;
        ILI_Orientation = 0;
        break;
    }
    SET_LCD_CS;
}

void Display_Init(void)
{
    SysTick_Init();
    GPIO_PinMode_Setup();
    SPI_Init();

    SET_LCD_RST;
    delayMS(50);
    RESET_LCD_RST;
    delayMS(50);
    SET_LCD_RST;

    RESET_LCD_CS;
    ILI_8Bit_Command(0xEF);
    delayMicroS(100);
    ILI_8Bit_Data(0x03);
    delayMicroS(100);
    ILI_8Bit_Data(0x80);
    delayMicroS(100);
    ILI_8Bit_Data(0x02);
    delayMicroS(100);

    ILI_8Bit_Command(0xCF);
    delayMicroS(100);
    ILI_8Bit_Data(0x00);
    delayMicroS(100);
    ILI_8Bit_Data(0XC1);
    delayMicroS(100);
    ILI_8Bit_Data(0X30);
    delayMicroS(100);

    ILI_8Bit_Command(0xED);
    delayMicroS(100);
    ILI_8Bit_Data(0x64);
    delayMicroS(100);
    ILI_8Bit_Data(0x03);
    delayMicroS(100);
    ILI_8Bit_Data(0X12);
    delayMicroS(100);
    ILI_8Bit_Data(0X81);
    delayMicroS(100);

    ILI_8Bit_Command(0xE8);
    delayMicroS(100);
    ILI_8Bit_Data(0x85);
    delayMicroS(100);
    ILI_8Bit_Data(0x00);
    delayMicroS(100);
    ILI_8Bit_Data(0x78);
    delayMicroS(100);

    ILI_8Bit_Command(0xCB);
    delayMicroS(100);
    ILI_8Bit_Data(0x39);
    delayMicroS(100);
    ILI_8Bit_Data(0x2C);
    delayMicroS(100);
    ILI_8Bit_Data(0x00);
    delayMicroS(100);
    ILI_8Bit_Data(0x34);
    delayMicroS(100);
    ILI_8Bit_Data(0x02);
    delayMicroS(100);

    ILI_8Bit_Command(0xF7);
    delayMicroS(100);
    ILI_8Bit_Data(0x20);
    delayMicroS(100);

    ILI_8Bit_Command(0xEA);
    delayMicroS(100);
    ILI_8Bit_Data(0x00);
    delayMicroS(100);
    ILI_8Bit_Data(0x00);
    delayMicroS(100);

    ILI_8Bit_Command(ILI_PWCTR1); // Power control
    delayMicroS(100);
    ILI_8Bit_Data(0x23); // VRH[5:0]
    delayMicroS(100);

    ILI_8Bit_Command(ILI_PWCTR2); // Power control
    ILI_8Bit_Data(0x10);          // SAP[2:0];BT[3:0]

    ILI_8Bit_Command(ILI_VMCTR1); // VCM control
    delayMicroS(100);
    ILI_8Bit_Data(0x3e);
    delayMicroS(100);
    ILI_8Bit_Data(0x28);
    delayMicroS(100);

    ILI_8Bit_Command(ILI_VMCTR2); // VCM control2
    delayMicroS(100);
    ILI_8Bit_Data(0x86); //--
    delayMicroS(100);

    ILI_8Bit_Command(ILI_MADCTL); // Memory Access Control
    delayMicroS(100);
    ILI_8Bit_Data(0x48); // i.e. Rotate_Display(0);
    delayMicroS(100);

    ILI_8Bit_Command(ILI_PIXFMT);
    delayMicroS(100);
    ILI_8Bit_Data(0x55);
    delayMicroS(100);

    ILI_8Bit_Command(ILI_FRMCTR1);
    delayMicroS(100);
    ILI_8Bit_Data(0x00);
    delayMicroS(100);
    ILI_8Bit_Data(0x13); // 0x18 79Hz, 0x1B default 70Hz, 0x13 100Hz
    delayMicroS(100);

    ILI_8Bit_Command(ILI_DFUNCTR); // Display Function Control
    delayMicroS(100);
    ILI_8Bit_Data(0x08);
    delayMicroS(100);
    ILI_8Bit_Data(0x82);
    delayMicroS(100);
    ILI_8Bit_Data(0x27);
    delayMicroS(100);

    ILI_8Bit_Command(0xF2); // 3Gamma Function Disable
    delayMicroS(100);
    ILI_8Bit_Data(0x00);
    delayMicroS(100);

    ILI_8Bit_Command(ILI_GAMMASET); // Gamma curve selected
    delayMicroS(100);
    ILI_8Bit_Data(0x01);
    delayMS(10);

    ILI_8Bit_Command(ILI_GMCTRP1); // Set Gamma Positive
    delayMicroS(100);
    ILI_8Bit_Data(0x0F);
    delayMicroS(100);
    ILI_8Bit_Data(0x31);
    delayMicroS(100);
    ILI_8Bit_Data(0x2B);
    delayMicroS(100);
    ILI_8Bit_Data(0x0C);
    delayMicroS(100);
    ILI_8Bit_Data(0x0E);
    delayMicroS(100);
    ILI_8Bit_Data(0x08);
    delayMicroS(100);
    ILI_8Bit_Data(0x4E);
    delayMicroS(100);
    ILI_8Bit_Data(0xF1);
    delayMicroS(100);
    ILI_8Bit_Data(0x37);
    delayMicroS(100);
    ILI_8Bit_Data(0x07);
    delayMicroS(100);
    ILI_8Bit_Data(0x10);
    delayMicroS(100);
    ILI_8Bit_Data(0x03);
    delayMicroS(100);
    ILI_8Bit_Data(0x0E);
    delayMicroS(100);
    ILI_8Bit_Data(0x09);
    delayMicroS(100);
    ILI_8Bit_Data(0x00);
    delayMicroS(100);

    ILI_8Bit_Command(ILI_GMCTRN1); // Set Gamma Negative
    delayMicroS(100);
    ILI_8Bit_Data(0x00);
    delayMicroS(100);
    ILI_8Bit_Data(0x0E);
    delayMicroS(100);
    ILI_8Bit_Data(0x14);
    delayMicroS(100);
    ILI_8Bit_Data(0x03);
    delayMicroS(100);
    ILI_8Bit_Data(0x11);
    delayMicroS(100);
    ILI_8Bit_Data(0x07);
    delayMicroS(100);
    ILI_8Bit_Data(0x31);
    delayMicroS(100);
    ILI_8Bit_Data(0xC1);
    delayMicroS(100);
    ILI_8Bit_Data(0x48);
    delayMicroS(100);
    ILI_8Bit_Data(0x08);
    delayMicroS(100);
    ILI_8Bit_Data(0x0F);
    delayMicroS(100);
    ILI_8Bit_Data(0x0C);
    delayMicroS(100);
    ILI_8Bit_Data(0x31);
    delayMicroS(100);
    ILI_8Bit_Data(0x36);
    delayMicroS(100);
    ILI_8Bit_Data(0x0F);
    delayMicroS(100);

    ILI_8Bit_Command(ILI_SLPOUT); // Wake Up Display
    delayMS(150);
    ILI_8Bit_Command(ILI_DISPON); // Display On
    delayMS(150);
    SET_LCD_CS;

    Fill_Screen(WHITE); // Begin white screen, Portrait mode
    Rotate_Display(2);
}

void GPIO_PinMode_Setup(void)
{
    RCC->AHB1ENR |= 1 | 2 | 4 | 8;

    /* Clear pin modes */
    LCD_RST_PORT->MODER &= ~(3 << (LCD_RST * 2));
    LCD_CS_PORT->MODER &= ~(3 << (LCD_CS * 2));
    LCD_DCX_PORT->MODER &= ~(3 << (LCD_DCX * 2));

    /* Set pin modes to output */
    LCD_RST_PORT->MODER |= (1 << (LCD_RST * 2));
    LCD_CS_PORT->MODER |= (1 << (LCD_CS * 2));
    LCD_DCX_PORT->MODER |= (1 << (LCD_DCX * 2));

    /* Pull control pins to HIGH (their idle state) */
    SET_LCD_CS;
}

void ILI_Push_16Bit(uint16_t parameter)
{
    ILI_8Bit_Data((uint8_t)(parameter >> 8));
    ILI_8Bit_Data((uint8_t)parameter);
}

void ILI_8Bit_Command(uint8_t command)
{
    RESET_LCD_DCX;

    RESET_LCD_CS;
    SPI_Transmit(&command, 1);
    SET_LCD_CS;
}

void ILI_8Bit_Data(uint8_t data)
{
    SET_LCD_DCX;

    RESET_LCD_CS;
    SPI_Transmit(&data, 1);
    SET_LCD_CS;
}

void Fill_Rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
    /* Error checking for Rectangle */
    if ((x >= ILI_TFTwidth) || (y >= ILI_TFTheight) || (w == 0) || (h == 0))
        return;

    /* Cropping if Off Screen */
    if (x + w - 1 >= ILI_TFTwidth)
        w = ILI_TFTwidth - x;
    if (y + h - 1 >= ILI_TFTheight)
        h = ILI_TFTheight - y;

    RESET_LCD_CS;
    Set_Address_Window(x, y, x + w - 1, y + h - 1);
    Fill_Color(color, (uint32_t)w * (uint32_t)h);
    SET_LCD_CS;
}

void Set_Address_Window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    ILI_8Bit_Command(ILI_CASET);
    ILI_Push_16Bit(x0);
    ILI_Push_16Bit(x1);

    ILI_8Bit_Command(ILI_PASET);
    ILI_Push_16Bit(y0);
    ILI_Push_16Bit(y1);

    ILI_8Bit_Command(ILI_RAMWR);
}

void Fill_Color(uint16_t color, uint32_t len)
{
    /* This draws using 8x8 squares of the image at a time */
    uint16_t blocks = (uint16_t)(len / 64); // 64 pixels/block
    uint8_t pass_count;
    uint8_t color_high = color >> 8;
    uint8_t color_low = color;

    // Write first pixel
    ILI_Push_16Bit(color);
    len--;

    // If High Color and Low Color are the same,
    // Pulse Write pin to send the previous data
    // if (color_high == color_low)
    // {
    //   while (blocks--)
    //   {
    //     // pass count = number of blocks / pixels per pass = 64 / 4
    //     pass_count = 16;
    //     while (pass_count--)
    //     {
    //       RESET_LCD_WR;
    //       SET_LCD_WR;
    //       RESET_LCD_WR;
    //       SET_LCD_WR;
    //       RESET_LCD_WR;
    //       SET_LCD_WR;
    //       RESET_LCD_WR;
    //       SET_LCD_WR;
    //       RESET_LCD_WR;
    //       SET_LCD_WR;
    //       RESET_LCD_WR;
    //       SET_LCD_WR;
    //       RESET_LCD_WR;
    //       SET_LCD_WR;
    //       RESET_LCD_WR;
    //       SET_LCD_WR;
    //     }
    //   }
    //   // Fill any remaining pixels (1 to 64)
    //   pass_count = len & 63;
    //   while (pass_count--)
    //   {
    //     RESET_LCD_WR;
    //     SET_LCD_WR;

    //     RESET_LCD_WR;
    //     SET_LCD_WR;
    //   }
    // }
    // else
    // {
    while (blocks--)
    {
        pass_count = 16;
        while (pass_count--)
        {
            /* Send the 4 pixels per Pass */
            ILI_Push_16Bit(color);
            ILI_Push_16Bit(color);
            ILI_Push_16Bit(color);
            ILI_Push_16Bit(color);
        }
    }
    pass_count = len & 63;
    while (pass_count--)
    {
        // write here the remaining data
        ILI_8Bit_Data(color_high);
        ILI_8Bit_Data(color_low);
    }
    // }
}

void doFontTest(const tFont *font)
{
    Fill_Screen(BLACK);
    Draw_String_NoBG(0, 0,
                     "!\"#$%&'()*+,-./"
                     "0123456789:;<=>?ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`"
                     "abcdefghijklmnopqrstuvwxyz{|}~",
                     WHITE, font);
}

void DrawPaletteImage(uint16_t x, uint16_t y, const tPaletteImage *imageData)
{
	RESET_LCD_CS;
	uint16_t width = 0, height = 0;
	width = imageData->width;
	height = imageData->height;

	uint32_t dataSize = imageData->dataSize;

	Set_Address_Window(x, y, x + width - 1, y + height - 1);

	uint16_t color = 0;
	uint8_t colorVal = 0;
    uint8_t bitSize = imageData->bitSize;
	uint32_t curPxInt = 0;
    uint8_t desiredBits = 0xFF;
    uint8_t start = 0;
    uint8_t end = 0;

    if(bitSize == 8){
        desiredBits = 0xFF;
        end = 4;
    }
     else if(bitSize == 4){
        desiredBits = 0x0F;
        end = 8;
    }
    else if(bitSize == 2){
        desiredBits = 0x03;
        end = 16;
    }
    else if(bitSize == 1){
        desiredBits = 0x01;
        end = 32;
    }

	for (uint32_t pxInt = 0; pxInt < dataSize; pxInt++)
	{
        curPxInt = imageData->imageColorData[pxInt];
        for(int i=0; i<end; i++){
            colorVal = (curPxInt & (desiredBits<<i*bitSize)) >> i*bitSize;

            color = imageData->paletteData[colorVal];

            ILI_8Bit_Data((uint16_t)(color) >> 8);
            ILI_8Bit_Data((uint8_t)(color));
        }
	}
	SET_LCD_CS;
}

void DrawPaletteGif(uint8_t f, uint16_t x, uint16_t y, const tPaletteGif *imageData)
{
    RESET_LCD_CS;
	uint16_t width = 0, height = 0;
	width = imageData->width;
	height = imageData->height;

    Set_Address_Window(x, y, x + width - 1, y + height - 1);

	uint32_t dataSize = imageData->dataSize;
    uint32_t dataStart = dataSize * f;
    uint32_t dataEnd = dataSize + dataStart;

	uint16_t color = 0;
	uint8_t colorVal = 0;
    uint8_t bitSize = imageData->bitSize;
	uint32_t curPxInt = 0;
    uint8_t desiredBits = 0xFF;
    uint8_t end = 0;

    if(bitSize == 8){
        desiredBits = 0xFF;
        end = 4;
    }
     else if(bitSize == 4){
        desiredBits = 0x0F;
        end = 8;
    }
    else if(bitSize == 2){
        desiredBits = 0x03;
        end = 16;
    }
    else if(bitSize == 1){
        desiredBits = 0x01;
        end = 32;
    }

	for (uint32_t pxInt = dataStart; pxInt < dataEnd; pxInt++)
	{
        curPxInt = imageData->imageColorData[pxInt];
        for(int i=0; i<end; i++){
            colorVal = (curPxInt & (desiredBits<<i*bitSize)) >> i*bitSize;

            color = imageData->paletteData[colorVal];

            ILI_8Bit_Data((uint16_t)(color) >> 8);
            ILI_8Bit_Data((uint8_t)(color));
        }
	}
	SET_LCD_CS;
}

void Draw_Box(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t thickness, uint16_t color)
{
    Draw_HLine(y,w+thickness,x,thickness,color);
    Draw_HLine(y+h,w+thickness,x,thickness,color);
    Draw_VLine(x,h+thickness,y,thickness,color);
    Draw_VLine(x+w,h+thickness,y,thickness,color);


}

void drawCircle(uint16_t xc, uint16_t yc, uint16_t x, uint16_t y, uint16_t color)
{

    Draw_Pixel(xc+x, yc+y, color);
    Draw_Pixel(xc-x, yc+y, color);
    Draw_Pixel(xc+x, yc-y, color);
    Draw_Pixel(xc-x, yc-y, color);
    Draw_Pixel(xc+y, yc+x, color);
    Draw_Pixel(xc-y, yc+x, color);
    Draw_Pixel(xc+y, yc-x, color);
    Draw_Pixel(xc-y, yc-x, color);
}

void drawCirclePerimeterCircle(uint16_t xc, uint16_t yc, uint16_t x, uint16_t y, uint16_t r, uint16_t thk, uint16_t color)
{

    circleBres(xc+x, yc+y, r, thk, color);
    circleBres(xc-x, yc+y, r, thk ,color);
    circleBres(xc+x, yc-y, r, thk, color);
    circleBres(xc-x, yc-y, r, thk, color);
    circleBres(xc+y, yc+x, r, thk ,color);
    circleBres(xc-y, yc+x, r, thk, color);
    circleBres(xc+y, yc-x, r, thk, color);
    circleBres(xc-y, yc-x, r, thk, color);

}


// Function for circle-generation
// using Bresenham's algorithm
void circleBres(uint16_t xc, uint16_t yc, uint16_t r,uint16_t thk, uint16_t color)
{
    for(size_t i = 0; i < thk; i++)
    {
        int x = 0, y = (r+i);
        int d = 3 - 2 * (r+i);
        drawCircle(xc, yc, x, y, color);
        while (y >= x){
        
            // check for decision parameter
            // and correspondingly 
            // update d, y
            if (d > 0) {
                y--; 
                d = d + 4 * (x - y) + 10;
            }
            else
                d = d + 4 * x + 6;

            // Increment x after updating decision parameter
            x++;
            
            // Draw the circle using the new coordinates
            drawCircle(xc, yc, x, y, color);
            
        }
    }
}

void perimeterCircle(uint16_t xc, uint16_t yc, uint16_t r,uint16_t thk, uint16_t color)
{
    
        int x = 0, y = (r);
        int d = 3 - 2 * (r);
        int i = 0;
        drawCirclePerimeterCircle(xc, yc, x, y,r,thk, color);
        while (y >= x)
        {

            
                // check for decision parameter
                // and correspondingly 
                // update d, y
                if (d > 0) {
                    y--; 
                    d = d + 4 * (x - y) + 10;
                }
                else
                    d = d + 4 * x + 6;

                // Increment x after updating decision parameter
                x++;
                
                // Draw the circle using the new coordinates
            if(!(i%12))
            {
                drawCirclePerimeterCircle(xc, yc, x, y,r,thk, color);
                i++;
            }
            else
            {
                i++;
            }
            
        }
    
}



/*****************************************************************************/
/*****************************************************************************/

/* EOF */
