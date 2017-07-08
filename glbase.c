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
 *  GLCDBase Module                                                         *
 *  Low Level, nuts-and-bolts functions for the 128x64 LCD                  *
 *                                                                          *
 *                                                                          *
 ****************************************************************************/
#include <htc.h>
#include "delay.h"
#include "glcd.h"




/****************************************************************************
 * void glbaseInit(void)                                                    *
 *                                                                          *
 * DESCRIPTION                                                              *
 * Initialize I/O Ports and reset the lcd.                                  *
 *                                                                          *
 * PARAMETERS                                                               *
 * none.                                                                    *
 *                                                                          *
 * RETURN VALUE                                                             *
 * none.                                                                    *
 *                                                                          *
 ****************************************************************************/
extern void glbaseInit(void)
{
    UINT16 wDelay;

    // Initialize I/O Ports
    PIN_RST = 0;                            // starting default values
    PIN_E = 0;
    PIN_RS = 0;
    PIN_RW = 1;
    PIN_CS1 = 0;
    PIN_CS2 = 0;

    Vdelay_ms(5);                           // wait for reset
    PIN_RST = 1;
    Vdelay_ms(5);

} /* End glbaseInit */








/****************************************************************************
 * void glbaseEnableController(UINT8 yCS)                                   *
 *                                                                          *
 * DESCRIPTION                                                              *
 * Enables one of the controllers via the CS line.                          *
 *                                                                          *
 * PARAMETERS                                                               *
 * yCS - The controller to enable (0=Left or 1=Right)                       *
 *                                                                          *
 * RETURN VALUE                                                             *
 * none.                                                                    *
 *                                                                          *
 ****************************************************************************/
void glbaseEnableController(UINT8 yCS)
{
    switch(yCS)
    {
        case 0 : PIN_CS1 = 1; break;
        case 1 : PIN_CS2 = 1; break;
    }

} /* End glbaseEnableController */






/****************************************************************************
 * void glbaseDisableController(UINT8 yCS)                                  *
 *                                                                          *
 * DESCRIPTION                                                              *
 * Disables one of the controllers via the CS line.                         *
 *                                                                          *
 * PARAMETERS                                                               *
 * yCS - The controller to disable (0=Left or 1=Right)                      *
 *                                                                          *
 * RETURN VALUE                                                             *
 * none.                                                                    *
 *                                                                          *
 ****************************************************************************/
void glbaseDisableController(UINT8 yCS)
{
    switch(yCS)
    {
        case 0 : PIN_CS1 = 0; break;
        case 1 : PIN_CS2 = 0; break;
    }

} /* glbaseDisableController */






/****************************************************************************
 * void glbaseWaitForNonBusy(UINT8 yCS)                                     *
 *                                                                          *
 * DESCRIPTION                                                              *
 * Check the controller status and returns when it is ready to receive new  *
 * commands or data.                                                        *
 *                                                                          *
 * PARAMETERS                                                               *
 * yCS - The controller to check (0=left screen, or 1=right screen).        *
 *                                                                          *
 * RETURN VALUE                                                             *
 * none. This function doesn't return until the busy bit is clear.          *
 *                                                                          *
 ****************************************************************************/
void glbaseWaitForNonBusy(UINT8 yCS)
{
    UINT8 yStatus;

    do
    {
        GLTRIS = PORT_INPUT;                        // data bus to inputs
        PIN_RS = 0;
        PIN_RW = 1;
        glbaseEnableController(yCS);                // Chip Select the controller
        PIN_E = 1;                                  // Enable high
        DelayShort();                               // wait a bit
        yStatus = GLREAD;                           // read the data
        PIN_E = 0;
        DelayShort();
        glbaseDisableController(yCS);               // Chip Select off
    } while (yStatus & DISPLAY_STATUS_BUSY);

} /* End glbaseWaitForNonBusy */







/****************************************************************************
 * UINT8 glbaseReadData(UINT8 yCS)                                          *
 *                                                                          *
 * DESCRIPTION                                                              *
 * Reads a byte of data from the lcd.                                       *
 *                                                                          *
 * PARAMETERS                                                               *
 * yCS - The controller to use (0=left or 1=right).                         *
 *                                                                          *
 * RETURN VALUE                                                             *
 * The byte read. The byte comes from the last page/column set.             *
 *                                                                          *
 ****************************************************************************/
UINT8 glbaseReadData(UINT8 yCS)
{
    UINT8 yByte;

    glbaseWaitForNonBusy(yCS);                      // wait for non-busy

    GLTRIS = PORT_INPUT;                            // data bus to inputs
    PIN_RW = 1;
    PIN_RS = 1;
    glbaseEnableController(yCS);                    // enable controller
    PIN_E = 1;
    DelayShort();                                   // wait for it
    yByte = GLREAD;
    PIN_E = 0;
    DelayShort();
    glbaseDisableController(yCS);                   // disable controller
    return yByte;

} /* End glbaseReadData */






/****************************************************************************
 * void glbaseWriteData(UINT8 yData, UINT8 yDFlag, UINT8 yCS)               *
 *                                                                          *
 * DESCRIPTION                                                              *
 * Writes a byte of data to the lcd.                                        *
 *                                                                          *
 * PARAMETERS                                                               *
 * yData  - The data byte to write.                                         *
 * yDFlag - Data/Command flag (1=data, 0=command)                           *
 * yCS    - The controller to write to (0=left or 1=right).                 *
 *                                                                          *
 * RETURN VALUE                                                             *
 * none.                                                                    *
 *                                                                          *
 *****************************************************************************/
void glbaseWriteData(UINT8 yData, UINT8 yDFlag, UINT8 yCS)
{

    glbaseWaitForNonBusy(yCS);                      // wait for non-busy

    GLTRIS = PORT_OUTPUT;                           // data bus to outputs
    PIN_RW = 0;
    PIN_RS = yDFlag;
    glbaseEnableController(yCS);                    // enable controller
    GLWRITE = yData;                                // write the data
    PIN_E = 1;
    DelayShort();                                   // wait for receive
    PIN_E = 0;
    DelayShort();
    glbaseDisableController(yCS);


} /* End glbaseWriteData */







