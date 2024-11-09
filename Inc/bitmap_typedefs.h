/*  Author: Jacob Clarey
 *  Date: 10/7/2023
 *  Description: This is a header file for use with the ILI9341 driver.
 *  It is used to define bitmap structures for fonts and images.
 *  Notes:
 */

#ifndef _BITMAP_TYPEDEFS_H_
#define _BITMAP_TYPEDEFS_H_

#include <stdint.h>

/// @brief A structure for an 8-bit mapped glyph.
/// @param data is pointer to glyph data.
/// @param width is width of glyph.
/// @param height is height of glyph.
/// @param dataSize is size of glyph data element. typ. 8-bit
typedef struct
{
    const uint8_t *data;
    uint16_t width;
    uint16_t height;
    uint8_t dataSize;
} tImage;

/// @brief A structure for a bitmap image.
/// @param data is pointer to the image.
/// @param width is width of image.
/// @param height is height of image.
/// @param dataSize is size of bitmap array.
typedef struct
{
    const uint16_t *data;
    uint16_t width;
    uint16_t height;
    uint32_t dataSize;
} tImage16bit;

/// @brief A structure for a font character.
/// @param code is ASCII code of character.
/// @param image is pointer to bitmap image of character.
typedef struct
{
    long int code;
    const tImage *image;
} tChar;

/// @brief A structure for a font.
/// @param length is number of characters in font
/// @param chars is characters in font.
typedef struct
{
    int length;
    const tChar *chars;
} tFont;

/// @brief A structure for a Image With Color Palette
/// @param dataSize is the number of pxInts
/// @param bitSize is the number of bits per pixel
typedef struct
{
		const uint32_t *imageColorData;
	  const uint16_t *paletteData;
    uint16_t width;
		uint16_t height;
    uint16_t dataSize;
    uint8_t  bitSize;
} tPaletteImage;

/// @brief A structure for a Gif With Color Palette
/// @param dataSize is the number of pxInts
/// @param bitSize is the number of bits per pixel
/// @param loopDelay is the delay between frames. 
/// This is intended to be accessed in the main.
typedef struct
{
		const uint32_t *imageColorData;
	  const uint16_t *paletteData;
    uint16_t width;
		uint16_t height;
    uint32_t dataSize;
    uint8_t numFrames;
    uint16_t loopDelay;
    uint8_t  bitSize;
} tPaletteGif;

#endif // _BITMAP_TYPEDEFS_H_

/* EOF */
