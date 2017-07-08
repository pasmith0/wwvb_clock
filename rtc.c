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
 *  RTC Module                                                              *
 *  DS1302 clock chip functions                                             *
 *                                                                          *
 ****************************************************************************/
#include <htc.h>
#include "types.h"
#include "rtc.h"
#include "spi.h"



/****************************************************************************
 * void rtcInit(void)                                                       *
 *                                                                          *
 * DESCRIPTION                                                              *
 * Performs power-up initialization. Called only once on power-up.          *
 *                                                                          *
 * PARAMETERS                                                               *
 * none.                                                                    *
 *                                                                          *
 * RETURN VALUE                                                             *
 * none.                                                                    *
 *                                                                          *
****************************************************************************/
void rtcInit(void)
{
    UINT8 y;
    TIMESTRUCT tt;

    spiInit();                                      // init low level SPI

    // Set Control Register to Write Enable
    rtcWriteRam(RTC_W_CTRL, 0x00);

    // Set trickle charge to 1 diode + 2.2K Resistor
    rtcWriteRam(RTC_W_TRKL, 0xa5);

    // check for signature in RAM
    y = rtcReadRam(RAM_R_SIG);                      // get signature byte
    if(y != RTC_SIGNATURE)
    {
        // the clock has lost power!
        rtcWriteRam( RTC_W_SEC, 0x00);              // Clear Halt (CH) flag
        rtcWriteRam( RAM_W_SIG, RTC_SIGNATURE);     // write signature
        rtcWriteRam( RAM_W_TZONE, TZ_ET);           // default is Eastern Time

        // default time is Friday, January 1, 2010
        tt.sec=  0x00;                              // 0x00 - 0x59
        tt.min=  0x00;                              // 0x00 - 0x59
        tt.hr=   0x00;                              // 0x00 - 0x23
        tt.date= 0x01;                              // 0x01 - 0x31
        tt.month=0x01;                              // 0x01 - 0x12
        tt.day=  0x05;                              // 1-7 (1=Monday)
        tt.year= 0x10;                              // 2000 + value
        rtcWriteRam(RAM_W_DST, 0x00);               // Standard Time
        rtcSetClock(&tt);
        rtcSaveLastSync(&tt, 0);                    // default last sync time
        rtcSaveLastSync(&tt, 1);
    }

} /* End rtcInit */





/****************************************************************************
 * UINT8 rtcReadRam( UINT8 yAddress)                                        *
 *                                                                          *
 * DESCRIPTION                                                              *
 * Reads a byte from the DS1302 battery-backed up RAM.                      *
 *                                                                          *
 * PARAMETERS                                                               *
 * yAddress - Memory Address to read.                                       *
 *                                                                          *
 * RETURN VALUE                                                             *
 * The byte at the passed address.                                          *
 *                                                                          *
 ****************************************************************************/
UINT8 rtcReadRam( UINT8 yAddress)
{
    UINT8 y;

    spiChipSelect();                            // select it

    spiWrite(yAddress);                         // write address
    y = spiRead();                              // read back the data

    spiChipDeselect();                          // CS back to high

    return y;

}/* End rtcReadRam */






/****************************************************************************
 * void rtcWriteRam( UINT8 yAddress, UINT8 yData)                           *
 *                                                                          *
 * DESCRIPTION                                                              *
 * Write a byte to the DS1302 battery-backed up RAM.                        *
 *                                                                          *
 * PARAMETERS                                                               *
 * yAddress - Memory Address.                                               *
 * yData - data to write.                                                   *
 *                                                                          *
 * RETURN VALUE                                                             *
 * none.                                                                    *
 *                                                                          *
 ****************************************************************************/
void rtcWriteRam( UINT8 yAddress, UINT8 yData)
{

    spiChipSelect();                            // select it
    spiWrite(yAddress);                         // write address

    spiWrite(yData);                            // write the byte

    spiChipDeselect();                          // CS back to high

} /* End rtcWriteRam */







/****************************************************************************
 * void rtcSetClock(TIMESTRUCT *pTime)                                      *
 *                                                                          *
 * DESCRIPTION                                                              *
 * Sets the current time from the passed TIMESTRUCT.                        *
 *                                                                          *
 * PARAMETERS                                                               *
 * pTime - Pointer to the TIMESTRUCT.                                       *
 *                                                                          *
 * RETURN VALUE                                                             *
 * none.                                                                    *
 *                                                                          *
 ****************************************************************************/
void rtcSetClock(TIMESTRUCT *pTime)
{

    rtcWriteRam( RTC_W_SEC, pTime->sec);
    rtcWriteRam( RTC_W_MIN, pTime->min);
    rtcWriteRam( RTC_W_HOUR, pTime->hr);
    rtcWriteRam( RTC_W_DATE, pTime->date);
    rtcWriteRam( RTC_W_MONTH, pTime->month);
    rtcWriteRam( RTC_W_DAY, pTime->day);
    rtcWriteRam( RTC_W_YEAR, pTime->year);

} /* End rtcSetClock */







/****************************************************************************
 * void rtcGetClock(TIMESTRUCT *pTime)                                      *
 *                                                                          *
 * DESCRIPTION                                                              *
 * Reads the current time from the DS1302 chip into the passed TIMESTRUCT.  *
 *                                                                          *
 * PARAMETERS                                                               *
 * pTime - Pointer to the TIMESTRUCT.                                       *
 *                                                                          *
 * RETURN VALUE                                                             *
 * none.                                                                    *
 *                                                                          *
 ****************************************************************************/
void rtcGetClock(TIMESTRUCT *pTime)
{
    spiChipSelect();                            // select it

    spiWrite(RTC_R_BURST);                      // initiate burst mode read

    pTime->sec = spiRead();
    pTime->min = spiRead();
    pTime->hr = spiRead();
    pTime->date = spiRead();
    pTime->month = spiRead();
    pTime->day = spiRead();
    pTime->year = spiRead();

    spiChipDeselect();                          // CS back to high

} /* End rtcGetClock */





/****************************************************************************
 * UINT8 rtcBCDToDec(UINT8 yNum)                                            *
 *                                                                          *
 * DESCRIPTION                                                              *
 * Converts the passed BCD encoded byte to its binary equivalent.           *
 *                                                                          *
 * PARAMETERS                                                               *
 * yNum The number in BCD to convert.                                       *
 *                                                                          *
 * RETURN VALUE                                                             *
 * The binary equivalent of yNum                                            *
 *                                                                          *
 ****************************************************************************/
UINT8 rtcBCDToDec(UINT8 yNum)
{
    UINT8 yBin;

    yBin = ((yNum & (UINT8)0xf0) >> (UINT8)4) * (UINT8)10;
    yBin += (yNum & (UINT8)0x0f);
    return yBin;

} /* End rtcBCDToDec */






/****************************************************************************
 * UINT8 rtcDecToBCD(UINT8 yNum)                                            *
 *                                                                          *
 * DESCRIPTION                                                              *
 * Converts the passed decimal number to BCD.                               *
 *                                                                          *
 * PARAMETERS                                                               *
 * yNum -  The decimal number to convert.                                   *
 *                                                                          *
 * RETURN VALUE                                                             *
 * The BCD equivalent of yNum                                               *
 *                                                                          *
 ****************************************************************************/
UINT8 rtcDecToBCD(UINT8 yNum)
{
    return ((yNum/(UINT8)10)<<(UINT8)4) + (yNum % (UINT8)10);

} /* End wwvDecToBCD */





/****************************************************************************
 * UINT8 rtc24to12Hour(UINT8 yHour, UINT8 *pPM)                             *
 *                                                                          *
 * DESCRIPTION                                                              *
 * Converts a 24 hour time format (in BCD) to 12 hour format.               *
 *                                                                          *
 * PARAMETERS                                                               *
 * yHour            The 24hr time in BCD (0x00 to 0x23).                    *
 * pPM              Pointer to a byte to receive the AM/PM indicator.       *
 *                  1 if yHour represents a PM time. 0 if AM.               *
 *                                                                          *
 * RETURN VALUE                                                             *
 * The hour in 12-hour time format, BCD (0x00 - 0x12). The data at pPM is   *
 * changed to reflect AM/PM status.                                         *
 *                                                                          *
 ****************************************************************************/
UINT8 rtc24to12Hour(UINT8 yHour, UINT8 *pPM)
{
    UINT8 yHr;
    UINT8 yBin;

    yBin = rtcBCDToDec(yHour);
    // 00 hr to 12 AM
    if(!yBin)
    {
        yHr = 12;
        *pPM = 0;
    }

    // 01-11 hr to AM
    else if(yBin < (UINT8)12)
    {
        yHr = yBin;
        *pPM = 0;
    }

    // 12 hr to PM
    else if(yBin == 12)
    {
        yHr = yBin;
        *pPM = 1;
    }

    // 13-23 hr, -12 and PM
    else
    {
        yHr = yBin - (UINT8)12;
        *pPM = 1;
    }

    return rtcDecToBCD(yHr);

} /* End rtc24to12Hour */





/****************************************************************************
 * void rtcSaveLastSync(TIMESTRUCT *pt, UINT8 ySet)                         *
 *                                                                          *
 * DESCRIPTION                                                              *
 * Saves the time in the passed TIMESTRUCT to the last sync time in the     *
 * DS1302 battery backed up RAM.                                            *
 *                                                                          *
 * PARAMETERS                                                               *
 * pt - Pointer to a TIMESTRUCT containg the date/time to save.             *
 * ySet - 0 = Save to set#0, 1= Save to set #1.                             *
 *                                                                          *
 * RETURN VALUE                                                             *
 * none.                                                                    *
 *                                                                          *
 ****************************************************************************/
void rtcSaveLastSync(TIMESTRUCT *pt, UINT8 ySet)
{
    if(ySet)
    {
        rtcWriteRam(RAM_W_LASTMONTH2, pt->month);
        rtcWriteRam(RAM_W_LASTDATE2, pt->date);
        rtcWriteRam(RAM_W_LASTHOUR2, pt->hr);
        rtcWriteRam(RAM_W_LASTMIN2, pt->min);
    }
    else
    {
        rtcWriteRam(RAM_W_LASTMONTH, pt->month);
        rtcWriteRam(RAM_W_LASTDATE, pt->date);
        rtcWriteRam(RAM_W_LASTHOUR, pt->hr);
        rtcWriteRam(RAM_W_LASTMIN, pt->min);
    }

} /* End rtcSaveLastSync */



/****************************************************************************
 * void rtcBumpLastSync(void)                                               *
 *                                                                          *
 * DESCRIPTION                                                              *
 * Moves the last sync2 values to sync1, overwriting the previous values.   *
 *                                                                          *
 * PARAMETERS                                                               *
 * none.                                                                    *
 *                                                                          *
 * RETURN VALUE                                                             *
 * none.                                                                    *
 *                                                                          *
 ****************************************************************************/
void rtcBumpLastSync(void)
{
    UINT8 yMonth, yDate, yHr, yMin;

    yMonth= rtcReadRam(RAM_R_LASTMONTH);
    yDate = rtcReadRam(RAM_R_LASTDATE);
    yHr   = rtcReadRam(RAM_R_LASTHOUR);
    yMin  = rtcReadRam(RAM_R_LASTMIN);

    rtcWriteRam(RAM_W_LASTMONTH2, yMonth);
    rtcWriteRam(RAM_W_LASTDATE2, yDate);
    rtcWriteRam(RAM_W_LASTHOUR2, yHr);
    rtcWriteRam(RAM_W_LASTMIN2, yMin);


} /* End rtcSaveLastSync */


