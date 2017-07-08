/*****************************************************************************
 *  SPI Low Level Module Header
 *
 *
 ****************************************************************************/

#ifndef __SPI_H__
#define __SPI_H__


// Public Prototypes
extern void spiInit(void);
extern UINT8 spiRead(void);
extern void spiWrite(UINT8 yByte);
extern void spiChipSelect(void);
extern void spiChipDeselect(void);

extern void Delay2us(void);

// I/O Port and Pins definitions
#define PIN_SCLK    LATA5                   // SCLK: PortA Bit 5
#define PIN_SDOUT   LATA4                   // SDIO: PortA Bit 4 output
#define PIN_SDIN    RA4                     // SDIO: PortA Bit 4 input
#define TRIS_SDIO   TRISA4                  // TRIS bit for data I/O
#define PIN_CE      LATA3                   // Chip Enable: PortA Bit 3


#endif /* __SPI_H__ */
