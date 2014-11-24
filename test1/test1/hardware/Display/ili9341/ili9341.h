//*****************************************************************************
//
// Translates a 24-bit RGB color to a display driver-specific color.
//
// \param c is the 24-bit RGB color.  The least-significant byte is the blue
// channel, the next byte is the green channel, and the third byte is the red
// channel.
//
// This macro translates a 24-bit RGB color into a value that can be written
// into the display's frame buffer in order to reproduce that color, or the
// closest possible approximation of that color.
//
// \return Returns the display-driver specific color.
//
// 24-bit format: XXXX XXXX RRRR RRRR GGGG GGGG BBBB BBBB
// 16-bit format: ---- ---- ---- ---- RRRR RGGG GGGB BBBB
//  8-bit format: ---- ---- ---- ---- ---- ---- RRRG GGBB
//
//
//*****************************************************************************

#include "fonts/fonts.h"

#define TFT_CS_LOW      GPIOPinWrite(DISPLAY_CS_PORT, DISPLAY_CS_PIN, 0)
#define TFT_CS_HIGH     GPIOPinWrite(DISPLAY_CS_PORT, DISPLAY_CS_PIN, DISPLAY_CS_PIN)

#define TFT_RST_ON      GPIOPinWrite(DISPLAY_RST_PORT, DISPLAY_RST_PIN, 0)
#define TFT_RST_OFF     GPIOPinWrite(DISPLAY_RST_PORT, DISPLAY_RST_PIN, DISPLAY_RST_PIN)

#define TFT_DC_LOW      GPIOPinWrite(DISPLAY_D_C_PORT, DISPLAY_D_C_PIN, 0)
#define TFT_DC_HIGH     GPIOPinWrite(DISPLAY_D_C_PORT, DISPLAY_D_C_PIN, DISPLAY_D_C_PIN)

#define DELAY_VALUE     300000

#define DPYCOLORTRANSLATE16(c)  ((((c) & 0x00f80000) >> 8) |                  \
                                 (((c) & 0x0000fc00) >> 5) |                  \
                                 (((c) & 0x000000f8) >> 3))
#define DPYCOLORTRANSLATE8(c)   ((((c) & 0x00e00000) >> 16) |                 \
                                 (((c) & 0x0000e000) >> 11) |                 \
                                 (((c) & 0x000000c0) >> 6))
#define DPYCOLORTRANSLATE DPYCOLORTRANSLATE8

// Macro to translate 24 bit RGB to 5-6-5 16 bit RGB
#define COLOR24TO16BIT(rgb)		((((rgb) & 0x00f80000) >> 8) | (((rgb) & 0x0000fc00) >> 5) | (((rgb) & 0x000000f8) >> 3))

/**Macro definitions for char display direction**/
#define ORIENTATION_LEFT2RIGHT  0
#define ORIENTATION_DOWN2UP     1
#define ORIENTATION_RIGHT2LEFT  2
#define ORIENTATION_UP2DOWN     3

#define DISPLAY_WIDTH		    239
#define DISPLAY_HEIGHT			319

//*****************************************************************************
//
// Prototypes for the globals exported by this driver.
//
//*****************************************************************************
extern void InitDisplay(void);
extern void PixelDraw(void *pvDisplayData, int32_t x, int32_t y, uint32_t color);
extern void LineDrawH(void *pvDisplayData, int32_t x1, int32_t x2, int32_t y, uint32_t color);
extern void LineDrawV(void *pvDisplayData, int32_t x, int32_t y1, int32_t y2, uint32_t color);
//extern uint32_t ColorTranslate(void *pvDisplayData, uint32_t ui32Value);
extern const tDisplay psDisplay;

extern void TFT_setOrientation(uint8_t orientation);

extern void DrawTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, int32_t color);
