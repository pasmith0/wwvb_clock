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
 *  WWV Module                                                              *
 *  CMax WWVB routines                                                      *
 *                                                                          *
 ****************************************************************************/
#include <htc.h>
#include "types.h"
#include "main.h"
#include "rtc.h"
#include "wwv.h"
#include "glcd.h"


// local functions
static void wwvMakeTimeStruct(TIMESTRUCT *pt, UINT8 yMin, UINT8 yHour,
        UINT16 wDays, UINT8 yYears, UINT8 yLeap, UINT8 yDST, UINT8 yTZ);
static UINT8 wwvGetBit(void);
static void wwvStartFrameTimer(void);
static UINT8 wwvGetSyncBit(void);
static UINT8 wwvRead8BitFrame(UINT8 *pResult);
static UINT8 wwvGetAnyBit(void);
static UINT8 wwvBCDToDec(UINT8 yNum);
static UINT8 wwvDecToBCD(UINT8 yNum);
static UINT8 wwvGetWeekDay(int nMonth, int nDate, int nYear);
static void glcdMakeLastSyncString(UINT8 *pBuffer, UINT8 yMonth, UINT8 yDate,
                                UINT8 yHour, UINT8 yMin);


// End of Month Table - for standard and leap years
const UINT16 wEOM[13][2] = {
                        {0,0},              // Begin
                        {31,31},            // Jan
                        {59,60},            // Feb
                        {90,91},            // Mar
                        {120,121},          // Apr
                        {151,152},          // May
                        {181,182},          // Jun
                        {212,213},          // Jul
                        {243,244},          // Aug
                        {273,274},          // Sep
                        {304,305},          // Oct
                        {334,335},          // Nov
                        {365,366},          // Dec
                        };

const UINT8 cszSync[] = "Syncing Up...";
const UINT8 cszLastSync[] = "Last Sync Times: ";



/****************************************************************************
 * void wwvInit(void)                                                       *
 *                                                                          *
 * DESCRIPTION                                                              *
 * Initializes the I/O and interrupts for the cmax interface. Called only   *
 * once on power-up.                                                        *
 *                                                                          *
 * PARAMETERS                                                               *
 * none.                                                                    *
 *                                                                          *
 * RETURN VALUE                                                             *
 * none.                                                                    *
 *                                                                          *
 ****************************************************************************/
void wwvInit(void)
{
    UINT8 y;

    PIN_TLED = 0;                                   // Turn off LED
    RCONbits.IPEN = 0;                              // no int priorities

    // Set up Timer0 for 25ms period
    T0CONbits.T08BIT = 1;                           // 8-bit mode
    T0CONbits.T0CS = 0;                             // Internal clock source
    T0CONbits.PSA = 0;                              // Prescaler Enabled
    T0CONbits.T0PS = 7;                             // 256:1 Prescale

    INTCONbits.TMR0IE = 0;                          // not interrupt based
    INTCONbits.TMR0IF = 0;                          // clear overflow
    T0CONbits.TMR0ON = 1;                           // Enable Timer

    // Setup Timer1 for 200ms w/interrupts
    T1CONbits.TMR1ON = 0;                           // Disable Timer
    T1CONbits.RD16 = 0;                         // Dual 8-bit write mode
    T1CONbits.T1CKPS = 3;                           // 8:1 Prescale
    T1CONbits.T1OSCEN = 0;                          // No Oscillator
    T1CONbits.TMR1CS = 0;                           // Fosc/4 source
    PIR1bits.TMR1IF = 0;                            // Clear overflow flag
    PIE1bits.TMR1IE = 1;                            // Timer1 interrupt enable

    INTCONbits.GIE = 1;                             // enable global interrupts
    INTCONbits.PEIE = 1;                            // enable peripheral interrupts

} /* End wwvInit */





/****************************************************************************
 * UINT8 wwvGetTime(TIMESTRUCT *pt)                                         *
 *                                                                          *
 * DESCRIPTION                                                              *
 * This is the main function for getting the time from the WWVB module.     *
 * It can take up to two minutes to get the full frame.                     *
 *                                                                          *
 * PARAMETERS                                                               *
 * pt - Pointer to a TIMESTRUCT to receive the new time.                    *
 *                                                                          *
 * RETURN VALUE                                                             *
 * Non-zero if the time was sucessfully received and the passed             *
 * TIMESTRUCT was filled with the current time. Zero if the time could      *
 * not be received.                                                         *
 *                                                                          *
 ****************************************************************************/
UINT8 wwvGetTime(TIMESTRUCT *pt)
{
    UINT8   y, yUpper, yLower;
    UINT8   yMin, yHour, yDays1, yDays2, yYears1, yYears2, yLeap, yDST, yTZ;
    UINT16  wDays;
    UINT8   sz[16];

    // Display last sync up time
    glcdClearScreen();
    glcdMoveTo(0, 0);
    glcdWriteString((UINT8*) cszLastSync);      // display last sync

    glcdMakeLastSyncString(sz, rtcReadRam(RAM_R_LASTMONTH2), rtcReadRam(RAM_R_LASTDATE2),
                            rtcReadRam(RAM_R_LASTHOUR2), rtcReadRam(RAM_R_LASTMIN2) );

    glcdMoveTo(0, 1);
    glcdWriteString(sz);                        // display sync

    glcdMakeLastSyncString(sz, rtcReadRam(RAM_R_LASTMONTH), rtcReadRam(RAM_R_LASTDATE),
                            rtcReadRam(RAM_R_LASTHOUR), rtcReadRam(RAM_R_LASTMIN) );

    glcdMoveTo(0, 2);
    glcdWriteString(sz);                    // display it

    glcdMoveTo(0, 4);
    glcdWriteString((UINT8*) cszSync);      // display sync up message
    glcdMoveTo(0, 5);

    wwvStartFrameTimer();                   // start the fail-safe timer

    // Wait for a high on TCO line
    while(!PIN_TCO)
    {
        PIN_TLED = 1;
        if(g_yTimedOut)
            return 0;                       // failed, timed out > 2.5 minutes
    }

    // Need to find 2 Sync bits in a row
    while(1)
    {
        if(g_yTimedOut)
            return 0;                       // failed, out of time

        // Check to see if this bit is a sync bit
        if(wwvGetSyncBit())
        {
            if(wwvGetSyncBit())             // must get 2 in a row
                break;                      // got 2nd sync!
        }
    }
    glcdWriteChar('S');

    // Read in minutes frame
    if(!wwvRead8BitFrame(&yMin))            // get minutes byte
        return 0;

    if(!wwvGetSyncBit())                    // Verify ending sync bit
        return 0;

    glcdWriteChar('1');

    // Read in hours frame
    if(wwvGetAnyBit() != WWV_LOW)           // get (and trash) leading 0
        return 0;

    if(!wwvRead8BitFrame(&yHour))           // get hours byte
        return 0;

    if(!wwvGetSyncBit())                    // Verify ending sync bit
        return 0;

    glcdWriteChar('2');

    // Read in Days1 frame
    if(wwvGetAnyBit() != WWV_LOW)           // get (and trash) leading 0
        return 0;

    if(!wwvRead8BitFrame(&yDays1))          // get first Day byte
        return 0;

    if(!wwvGetSyncBit())                    // Verify ending sync bit
        return 0;

    glcdWriteChar('3');

    // Read in Days2 frame
    if(!wwvRead8BitFrame(&yDays2))          // get second Day byte
        return 0;

    if(wwvGetAnyBit() == WWV_ERROR)         // get (and trash) UT1 ADD sign bit
        return 0;

    if(!wwvGetSyncBit())                    // Verify ending sync bit
        return 0;

    glcdWriteChar('4');

    // Read in Years1 frame
    if(wwvGetAnyBit() == WWV_ERROR)         // Trash UT1 .8 correction bit
        return 0;

    if(!wwvRead8BitFrame(&yYears1))         // get Years-1 byte
        return 0;

    if(!wwvGetSyncBit())                    // Verify ending sync bit
        return 0;

    glcdWriteChar('5');

    // Read in Years2 frame
    if(!wwvRead8BitFrame(&yYears2))         // get Years-2 byte
        return 0;

    yDST = wwvGetAnyBit();                  // get DST (bit 58)
    switch(yDST)
    {
        case WWV_LOW: yDST = 0; break;      // Set to zero
        case WWV_HIGH: yDST = 1; break;     // Set to one
        default: return 0;                  // error
    }
    if(!wwvGetSyncBit())                    // Verify ending sync bit
        return 0;

    // We got the whole stream
    T1CONbits.TMR1ON = 0;                   // Stop the frame timer

    // Convert minutes
    yUpper = (yMin & 0xf0)  >> 1;           // shift MSB down by 1
    yLower = yMin & 0x0f;
    yMin = yUpper | yLower;
    yMin = rtcBCDToDec(yMin);
    if(yMin > 59)
        return 0;                           // must be 59 or less

    // Convert hours
    if(yHour & 0x80)
        return 0;                           // leading bit must be zero

    yUpper = (yHour & 0xf0)  >> 1;          // shift MSB down by 1
    yLower = yHour & 0x0f;
    yHour = yUpper | yLower;
    yHour = rtcBCDToDec(yHour);

    if(yHour > 23)
        return 0;                           // must be 23 or less

    // Convert days
    yUpper = (yDays1 & 0xf0) >> 1;          // shift down
    yLower = yDays1 & 0x0f;
    yDays1 = yUpper | yLower;
    yDays1 = rtcBCDToDec(yDays1);
    wDays = yDays1 * 10;                    // mult by 100

    yLower = yDays2 >> 4;                   // move to LSB
    wDays += yLower;                        // add in
    if(wDays > 366)
        return 0;                           // must be less than 366 days

    // Convert years
    yUpper = yYears1 << 4;                  // move to msb
    yLower = yYears2  >> 4;
    y = rtcBCDToDec(yUpper | yLower);       // combine
    yYears1 = rtcBCDToDec(y);
    yLeap = yYears2 & 0x04;                 // get Leap Year flag
        if(yLeap) yLeap = 1;

    // make DST flag
    // 00 = standard time   10 = dst begins tonight
    // 11 = dst in effect   01 = dst ends tonight
// y = ((yYears2 & 0x01) << 1) | yDST;
// if(y == 3)
// yDST = 1;
// else
// yDST = 0;

// y = ((yYears2 & 0x01) << 1) | yDST;
// if(y == 2)
// yDST = 1;
// else
// yDST = 0;
        
    y = ((yYears2 & 0x01) << 1) | yDST;
    if((y == 2) || (y == 3))
        yDST = 1;
    else
        yDST = 0; 

    // Get time zone offset from UTC
    // Eastern -5 UTC, Central -6 UTC, Mountain -7 UTC, etc.
    yTZ = rtcReadRam(RAM_R_TZONE);

    // Save Daylight Savings Time status bit
    rtcWriteRam(RAM_W_DST, yDST);

    // Now convert this whole mess to a TIMESTRUCT
    wwvMakeTimeStruct(pt, yMin, yHour, wDays, yYears1, yLeap, yDST, yTZ);

    rtcBumpLastSync();
    rtcSaveLastSync(pt, 0);                 // set new sync time
    return 1;                               // successful exit

} /* End wwvGetTime */





/****************************************************************************
 * void wwvMakeTimeStruct(TIMESTRUCT *pt, UINT8 yMin, UINT8 yHour,          *
 *      UINT16 wDays, UINT8 yYears, UINT8 yLeap, UINT8 yDST, UINT8 yTZ)     *
 *                                                                          *
 * DESCRIPTION                                                              *
 * Converts the passed UTC time to a TIMESTRUCT, correcting for local       *
 * time and daylight savings time.                                          *
 *                                                                          *
 * PARAMETERS                                                               *
 * pt - Pointer to a TIMESTRUCT to be filled in with the current time.      *
 * yMin - Minutes (0-59)                                                    *
 * yHours - Hours (0-23)                                                    *
 * wDays - Day of Year (1 - 366)                                            *
 * wYears - (0 - 99, i.e., 2000-2099)                                       *
 * yLeap - 1= Leap Year                                                     *
 * yDST - 1=Daylight Savings Time                                           *
 * yTZ - time zone negative offset from UTC (5=ET, 6=CT, 7=MT, 8=PT)        *
 *                                                                          *
 *                                                                          *
 * RETURN VALUE                                                             *
 * none.                                                                    *
 *                                                                          *
 ****************************************************************************/
static void wwvMakeTimeStruct(TIMESTRUCT *pt, UINT8 yMin, UINT8 yHour,
        UINT16 wDays, UINT8 yYears, UINT8 yLeap, UINT8 yDST, UINT8 yTZ)
{
    UINT16 nEndMonth, wDD;
    long lTime;


    // Convert UTC to match Time Zone
    if(yHour < (yTZ - yDST))
    {
        // oops- TZ adjustment moves us back one day
        yHour = ((yHour + yDST + 24) - yTZ) % 24;       // Move to previous day
        if(wDays == 1)
        {
            // oops- TZ moves us back to previous year
            wDays = 366;
            if(yYears) yYears--;                        // can't go before 2000
        }
        else
            wDays--;
    }
    else
        yHour -= (yTZ - yDST);


    // Add one minute to UTC time (Frame receive time compensation)
    if(yMin == 59)
    {
        yMin = 0;                                       // need to bump hour
        if(yHour == 23)
        {
            yHour = 0;                                  // need to bump day
            if(wDays == 366)
            {
                wDays = 1;                              // need to bump year
                ++yYears;
            }
            else
                ++wDays;
        }
        else
            ++yHour;
    }
    else
        ++yMin;


    // Calculate Month and Day from year-days
        nEndMonth = 0;
        while(wDays > wEOM[nEndMonth++][yLeap])
            ;
        --nEndMonth;

        wDD = wDays - wEOM[nEndMonth-1][yLeap];

    // Fill in the TIMESTRUCT
    pt->sec   = 0;
    pt->min   = rtcDecToBCD(yMin);
    pt->hr    = rtcDecToBCD(yHour);
    pt->day   = wwvGetWeekDay(nEndMonth, (UINT8)(wDD), (UINT16)yYears + 2000);
    pt->date  = rtcDecToBCD((UINT8)wDD);
    pt->month = rtcDecToBCD(nEndMonth);
    pt->year  = rtcDecToBCD(yYears);


} /* End wwvMakeTimeStruct */







/****************************************************************************
 * UINT8 wwvGetBit(void)                                                    *
 *                                                                          *
 * DESCRIPTION                                                              *
 * Gets a bit from the wwvb module. This routine should be called           *
 * immediately after a negative transition of the TCO line.                 *
 *                                                                          *
 * PARAMETERS                                                               *
 * none.                                                                    *
 *                                                                          *
 * RETURN VALUE                                                             *
 * WWV_HIGH - Logic 1 was read.                                             *
 * WWV_LOW  - Logic 0 was read.                                             *
 * WWV_SYNC - Sync Bit was detected.                                        *
 * WWV_ERROR- Bad bit detected.                                             *
 *                                                                          *
 ****************************************************************************/
static UINT8 wwvGetBit(void)
{
    UINT8 y25Count;                         // 25ms passed increments

    const UINT8 yLowMin = 7;                // Logic 0, 175ms minimum
    const UINT8 yLowMax = 9;                // Logic 0, 225ms maximum
    const UINT8 yHighMin = 19;              // Logic 1, 475ms minimum
    const UINT8 yHighMax = 21;              // Logic 1, 525ms maximum
    const UINT8 ySyncMin = 31;              // Sync bit, 775ms minimum
    const UINT8 ySyncMax = 33;              // Sunc bit, 825 ms maximum

    y25Count = 0;
    while(1)
    {
        if(PIN_TCO)                         // signal ended?
        {
            PIN_TLED = 1;
            // End Signal Transition occured
            if( (y25Count >= yLowMin) && (y25Count <= yLowMax) )
                return WWV_LOW;             // It's a Logic 0 bit
            if( (y25Count >= yHighMin) && (y25Count <= yHighMax) )
                return WWV_HIGH;            // It's a Logic 1 bit
            if( (y25Count >= ySyncMin) && (y25Count <= ySyncMax) )
                return WWV_SYNC;            // It's a Sync bit
            return WWV_ERROR;               // all others is an error
        }


        PIN_TLED = 0;
        INTCONbits.TMR0IF = 0;              // clear timer overflow flag
        TMR0 = TMR0_RELOAD;                 // restart the 25ms timer
        while(!INTCONbits.TMR0IF)           // wait for expiration
            ;

        if(y25Count++ > ySyncMax)           // check for way over due
            return WWV_ERROR;               // hmm.. no data is coming in
    }
    return WWV_ERROR;

} /* End wwvGetBit */






/****************************************************************************
 * void wwvStartFrameTimer(void)                                            *
 *                                                                          *
 * DESCRIPTION                                                              *
 * Starts up the frame timer. This is used to expire the WWVB               *
 * trasmission if it takes too long to complete. It can take up to 59       *
 * seconds to get to the beginning of the frame, followed by another        *
 * 60 seconds to read the frame. The timer is set to expire in 2.5 minutes  *
 * which gives plenty of time to complete the whole operation. The          *
 * expiration is indicated by g_yTimedOut global var becoming non-zero.     *
 *                                                                          *
 * PARAMETERS                                                               *
 * none.                                                                    *
 *                                                                          *
 * RETURN VALUE                                                             *
 * none.                                                                    *
 *                                                                          *
 ****************************************************************************/
static void wwvStartFrameTimer(void)
{
    PIR1bits.TMR1IF = 0;                            // Clear overflow flag

    TMR1L = LOBYTE(TMR1_RELOAD);                // Set Timer value (200ms @ 10Mhz)
    TMR1H = HIBYTE(TMR1_RELOAD);
    g_yTimedOut = 0;                            // clear timed out flag
    g_wFrameCounter = 0;                        // clear 200ms counter
    T1CONbits.TMR1ON = 1;                           // Enable Timer

} /* End wwvStartFrameTimer */







/****************************************************************************
 * UINT8 wwvGetSyncBit(void)                                                *
 *                                                                          *
 * DESCRIPTION                                                              *
 * Reads the stream expecting a sync bit.                                   *
 *                                                                          *
 * PARAMETERS                                                               *
 * none.                                                                    *
 *                                                                          *
 * RETURN VALUE                                                             *
 * Non-zero if a sync bit was found. Zero if any other bit was read or      *
 * a Time out occured.                                                      *
 *                                                                          *
 ****************************************************************************/
static UINT8 wwvGetSyncBit(void)
{
    while(PIN_TCO)                      // wait for low going
    {
        PIN_TLED = 1;
        if(g_yTimedOut)
            return 0;                   // failed
    }
    PIN_TLED = 0;

    if(wwvGetBit() == WWV_SYNC)
        return 1;                       // got a sync bit

    return 0;

} /* End wwvGetSyncBit */






/****************************************************************************
 * UINT8 wwvRead8BitFrame(UINT8 *pResult)                                   *
 *                                                                          *
 * DESCRIPTION                                                              *
 * Reads in 8-bits from the stream and returns the byte value.              *
 *                                                                          *
 * PARAMETERS                                                               *
 * pResult - pointer to a byte to receive the 8 bits read.                  *
 *                                                                          *
 * RETURN VALUE                                                             *
 * None-zero if the frame was sucessfully read. Zero if there were bit      *
 * errors or a time out occurred.                                           *
 *                                                                          *
 ****************************************************************************/
static UINT8 wwvRead8BitFrame(UINT8 *pResult)
{
    UINT8 y, yCode;

    *pResult = 0;                           // zero out caller's byte

    for(y = 0x80; y; y >>= 1)
    {
        while(PIN_TCO)                      // wait for low going
        {
            PIN_TLED = 1;
            if(g_yTimedOut)
                return 0;                   // failed
        }
        PIN_TLED = 0;
        yCode = wwvGetBit();
        if(yCode & (WWV_SYNC | WWV_ERROR) )
            return 0;                       // must be Logic 1 or 0
        if(yCode & WWV_HIGH)
            *pResult |= y;
    }
    return 1;

} /* End wwvRead8BitFrame */






/****************************************************************************
 * UINT8 wwvGetAnyBit(void)                                                 *
 *                                                                          *
 * DESCRIPTION                                                              *
 * Reads the next bit from the stream.                                      *
 *                                                                          *
 * PARAMETERS                                                               *
 * none.                                                                    *
 *                                                                          *
 * RETURN VALUE                                                             *
 * The bit read. WWV_ERROR is returned for an invalid bit or a              *
 * time out occured.                                                        *
 *                                                                          *
 ****************************************************************************/
static UINT8 wwvGetAnyBit(void)
{
    UINT8 yCode;

    while(PIN_TCO)                      // wait for low going
    {
        PIN_TLED = 1;
        if(g_yTimedOut)
            return WWV_ERROR;           // failed
    }
    PIN_TLED = 0;
    return wwvGetBit();                 // return read result

} /* End wwvGetAnyBit */





/****************************************************************************
 * UINT8 wwvGetWeekDay(UINT8 yMonth, UINT8 yDate)                           *
 *                                                                          *
 * DESCRIPTION                                                              *
 * Returns the day of week for the given month and date.                    *
 *                                                                          *
 * PARAMETERS                                                               *
 * yMonth - The month number (1-12)                                         *
 * yDate - The date (1-31)                                                  *
 * yYear - The Year (20xx)                                                  *
 *                                                                          *
 * RETURN VALUE                                                             *
 * The day of week, 1-7, with 1=Monday.                                     *
 *                                                                          *
 ****************************************************************************/
static UINT8 wwvGetWeekDay(int nMonth, int nDate, int nYear)
{
     int r;
     // Julian Calendar only
     if (nMonth < 3)
     {
           nMonth = nMonth + 12;
           nYear = nYear - 1;
     }
     r = (nDate + (2 * nMonth) + (6 * (nMonth + 1) / 10) + nYear + (nYear / 4) - (nYear / 100) + (nYear / 400)) % 7;
     return r+1;

} /* End  wwvGetWeekDay */






/****************************************************************************
 * void glcdMakeLastSyncString(UINT8 *pBuffer, UINT8 yMonth, UINT8 yDate,   *
 *                              UINT8 yHour, UINT8 yMin)                    *
 *                                                                          *
 * DESCRIPTION                                                              *
 * Returns a string that indicates the last time that the clock was         *
 * updated via the wwvb signal.                                             *
 *                                                                          *
 * PARAMETERS                                                               *
 * pBuffer - Pointer to a buffer to receive the string.                     *
 * yMonth - The month in BCD (0x01- 0x12)                                   *
 * yDate  - The date in BCD (0x01 - 0x31)                                   *
 * yHour  - The hour in BCD (0x00 - 0x23)                                   *
 * yMin   - The minute in BCD (0x00 - 0x59)                                 *
 *                                                                          *
 * RETURN VALUE                                                             *
 * none.                                                                    *
 *                                                                          *
 ****************************************************************************/
static void glcdMakeLastSyncString(UINT8 *pBuffer, UINT8 yMonth, UINT8 yDate,
                                UINT8 yHour, UINT8 yMin)
{
    UINT8 yPos, yChar;

    // Month
    yPos = 0;
    yChar = (yMonth & 0xf0) >> 4;                       // get upper digit
    if(yChar)
        pBuffer[yPos++] = yChar | 0x30;                 // convert to ascii
    pBuffer[yPos++] = (yMonth & 0x0f) | 0x30;
    pBuffer[yPos++] = '/';                              // add slash char

    // Date
    yChar = (yDate & 0xf0) >> 4;                        // get upper digit
    if(yChar)
        pBuffer[yPos++] = yChar | 0x30;                 // if not zero
    pBuffer[yPos++] = (yDate & 0x0f) | 0x30;
    pBuffer[yPos++] = ' ';                              // add spaces

    // Time- Hours
    yChar = (yHour & 0xf0) >> 4;                        // get upper digit
    pBuffer[yPos++] = yChar | 0x30;
    pBuffer[yPos++] = (yHour & 0x0f) | 0x30;
    pBuffer[yPos++] = ':';                              // add colon

    // Time- Minutes
    yChar = (yMin & 0xf0) >> 4;                         // get upper digit
    pBuffer[yPos++] = yChar | 0x30;
    pBuffer[yPos++] = (yMin & 0x0f) | 0x30;
    pBuffer[yPos] = '\0';                               // null terminator


} /* End glcdMakeLastSyncString */


