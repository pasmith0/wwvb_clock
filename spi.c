/****************************************************************************
 *  WWVB Atomic Clock                                                       *
 *                                                                          *
 *  (c)2010 Joe Jaworski                                                    *
 *  All Rights Reserved.                                                    *
 *  Author: Joe Jaworski jj@joejaworski.com                                 *
 *                                                                          *
 *  For non-commercial and academic use, you are granted                    *
 *  a royalty free license to use, modify, and compile this code,           *
 *  and install it on any number of microcontrollers. The resultant         *
 *  product that contains this code may not be sold in any fashion,         *
 *  including but not limited to selling for no monetary gain.              *
 *                                                                          *
 *                                                                          *
 *  SPI Module                                                              *
 *  Low level, Serial-Peripheral-Interface functions                        *
 *                                                                          *
 ****************************************************************************/
#include <htc.h>
#include "types.h"
#include "spi.h"
#include "main.h"





/****************************************************************************
* void spiInit(void)                                                        *
*                                                                           *
* DESCRIPTION                                                               *
* Performs SPI initialization. Called only once on power-up.                *
*                                                                           *
* PARAMETERS                                                                *
* none.                                                                     *
*                                                                           *
* RETURN VALUE                                                              *
* none.                                                                     *
*                                                                           *
*****************************************************************************/
void spiInit(void)
{
    // init I/O Pin levels
    PIN_SCLK = 0;
    TRIS_SDIO = 0;                              // SPI I/O line to output
    PIN_SDOUT = 0;
    PIN_CE = 0;

} /* End spiInit */





/****************************************************************************
* UINT8 spiRead(void)                                                       *
*                                                                           *
* DESCRIPTION                                                               *
* Reads a byte from the SPI port.                                           *
*                                                                           *
* PARAMETERS                                                                *
* none.                                                                     *
*                                                                           *
* RETURN VALUE                                                              *
* The byte read.                                                            *
*                                                                           *
*****************************************************************************/
UINT8 spiRead(void)
{
    UINT8 yShift;
    UINT8 yData = 0;

    TRIS_SDIO = 1;                                  // SPI I/O line to input

    for(yShift = 0x01; yShift; yShift <<= 1)
    {
        PIN_SCLK = 1;                               // Clock transition high

        if(PIN_SDIN)
            yData |= yShift;                        // set the bit if 1

        DelayShort();
        PIN_SCLK = 0;                               // Clock transition
        DelayShort();
    }

    return yData;

} /* End spiRead */






/****************************************************************************
* void spiWrite(UINT8 yByte)                                                *
*                                                                           *
* DESCRIPTION                                                               *
* Writes a byte of data to the SPI port.                                    *
*                                                                           *
* PARAMETERS                                                                *
* yByte - The byte value to write.                                          *
*                                                                           *
* RETURN VALUE                                                              *
* none.                                                                     *
*                                                                           *
*****************************************************************************/
void spiWrite(UINT8 yByte)
{
    UINT8 yShift;

    TRIS_SDIO = 0;                                      // SPI I/O to output

    for(yShift = 0x01; yShift; yShift <<= 1)
    {
        if(yByte & yShift)
        {
            PIN_SDOUT = 1;                              // set data bit HIGH
        }
        else
        {
            PIN_SDOUT = 0;                              // set data bit LOW
        }

        PIN_SCLK = 1;                                   // toggle clock
        DelayShort();
        PIN_SCLK = 0;
        DelayShort();
    }


} /* End spiWrite */






/****************************************************************************
* void spiChipSelect(void)                                                  *
*                                                                           *
* DESCRIPTION                                                               *
* Selects the DS1302 chip by raising the CE line                            *
*                                                                           *
* PARAMETERS                                                                *
* none.                                                                     *
*                                                                           *
* RETURN VALUE                                                              *
* none.                                                                     *
*                                                                           *
*****************************************************************************/
void spiChipSelect(void)
{
    PIN_CE = 1;                                 // RTC CE high (enable)
    DelayShort();
}





/****************************************************************************
* void spiChipDeselect(void)                                                *
*                                                                           *
* DESCRIPTION                                                               *
* Disables the DS1302 chip by lowering the CE line                          *
*                                                                           *
* PARAMETERS                                                                *
* none.                                                                     *
*                                                                           *
* RETURN VALUE                                                              *
* none.                                                                     *
*                                                                           *
*****************************************************************************/
void spiChipDeselect(void)
{
    PIN_CE = 0;                                 // RTC CE low (disable)
    DelayShort();
}
