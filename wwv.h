/*****************************************************************************
 *  WWVB Header
 *
 *
 ****************************************************************************/

#ifndef __WWV_H__
#define __WWV_H__

//#define TMR0_RELOAD 13            // TMR0 reload value (25ms @ 10Mhz)
//#define TMR1_RELOAD 3035          // TMR1 reload value (200ms @ 10Mhz)
#define TMR0_RELOAD 159             // TMR0 reload value (25ms @ 4Mhz)
#define TMR1_RELOAD 40535           // TMR1 reload value (200ms @ 4Mhz)

#define WWV_LOW     0x01            // Bit values returned
#define WWV_HIGH    0x02
#define WWV_SYNC    0x04
#define WWV_ERROR   0x08

#define isleapyear(year) ((!(year % 4) && (year % 100)) || (!(year % 400) && (year % 1000)))

// Public Prototypes
extern void wwvInit(void);
extern UINT8 wwvGetTime(TIMESTRUCT *pt);

// Port and Pins definitions
#define PIN_TCO     RA2             // CMax input pin
#define PIN_TLED    LATA0           // LED
#define PIN_SWITCH  RB2             // Input Switch

#define SWITCH_UP   (PIN_SWITCH)
#define SWITCH_DOWN (!PIN_SWITCH)

#endif /* __WWV_H__ */
