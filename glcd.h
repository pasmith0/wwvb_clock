/*****************************************************************************
 *  Graphics LCD Module Header
 *
 *
 ****************************************************************************/

#ifndef __glcd__H__
#define __glcd__H__

#include <htc.h>
#include "types.h"


#define KS0108_SCREEN_WIDTH     128
#define KS0108_SCREEN_HEIGHT    64

#define DISPLAY_SET_Y       0x40
#define DISPLAY_SET_X       0xB8
#define DISPLAY_START_LINE  0xC0
#define DISPLAY_ON_CMD      0x3E
#define ON  0x01
#define OFF 0x00
#define DISPLAY_STATUS_BUSY 0x90            // BUSY + RESET bits

// Bitmap Definitions
#define BMP_0       0
#define BMP_1       1
#define BMP_2       2
#define BMP_3       3
#define BMP_4       4
#define BMP_5       5
#define BMP_6       6
#define BMP_7       7
#define BMP_8       8
#define BMP_9       9
#define BMP_BLANK   10
#define BMP_COLON   11
#define BMP_AM      12
#define BMP_PM      13


// glcd Prototypes
extern void glcdInit(void);
extern void glcdClearScreen(void);
extern void glcdSetPage(UINT8 yPage, UINT8 ySeg);
extern void glcdSetCol(UINT8 yCol, UINT8 ySeg);
extern void glcdMoveTo(UINT8 yX, UINT8 yY);
extern void glcdWriteChar(UINT8 yChar);
extern void glcdWriteString(char * pString);
extern void glcdDrawBitmap(UINT8 *pBMP, UINT8 yWidth, UINT8 yHeight);
extern void glcdDrawImage(UINT8 yNum);

// glcdbase Prototypes
extern void glbaseInit(void);
extern void glbaseEnableController(UINT8 yCS);
extern void glbaseDisableController(UINT8 yCS);
extern void glbaseWaitForNonBusy(UINT8 yCS);
extern UINT8 glbaseReadData(UINT8 yCS);
extern void glbaseWriteData(UINT8 yData, UINT8 yDFlag, UINT8 yCS);

extern void DelayShort(void);
extern void Vdelay_ms(UINT16 wDelay);


// Port and Pins definitions for the LCD display
#define PIN_RS      LATB3                   // RS - Register Select
#define PIN_RW      LATB4                   // RW - Read/Write
#define PIN_E       LATB5                   // E - Enable
#define PIN_CS1     LATB0                   // CS1 - Chip Select 1
#define PIN_CS2     LATB1                   // CS2 - Chip Select 2
#define PIN_RST     LATA1                   // Reset pin
#define GLREAD      PORTC                   // 8-bit Data Bus Input
#define GLWRITE     LATC                    // 8-bit Data Bus output
#define GLTRIS      TRISC                   // 8-bit Data Bus direction

#define PORT_OUTPUT 0x00
#define PORT_INPUT  0xff

// LCD bitmap positions/coordinates (X in pixels, Y in bytes)
#define POS_HR_HI_X     0                   // Hour Upper Digit-X
#define POS_HR_HI_Y     1                   // Hour Upper Digit-Y
#define POS_HR_LO_X     14                  // Hour Lower Digit-X
#define POS_HR_LO_Y     1                   // Hour Lower Digit-Y

#define POS_COLON1_X    28                  // 1st colon-X
#define POS_COLON1_Y    1                   // 1st colon-Y

#define POS_MIN_HI_X    35                  // Minutes Upper Digit-X
#define POS_MIN_HI_Y    1                   // Minutes Upper Digit-Y
#define POS_MIN_LO_X    49                  // Minutes Lower Digit-X
#define POS_MIN_LO_Y    1                   // Minutes Lower Digit-Y

#define POS_COLON2_X    63                  // 2nd colon-X
#define POS_COLON2_Y    1                   // 2nd colon-Y

#define POS_SEC_HI_X    70                  // Seconds Upper Digit-X
#define POS_SEC_HI_Y    1                   // Seconds Upper Digit-Y
#define POS_SEC_LO_X    84                  // Seconds Lower Digit-X
#define POS_SEC_LO_Y    1                   // Seconds Lower Digit-Y

#define POS_AMPM_X      100                 // AM/PM-X
#define POS_AMPM_Y      2                   // AM/PM-Y

#define POS_ZONE_X      109                 // TimeZone-X
#define POS_ZONE_Y      0                   // TimeZone-Y

#define POS_MMDDYY_X    8                   // Date String-X
#define POS_MMDDYY_Y    5                   // Date String-Y


#endif /* __glcd__H__ */
