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
  * including but not limited to selling for no monetary gain.              *
 *                                                                          *
 *  18F2420 4 Mhz internal Oscillator                                       *
 *  HiTech C Compiler                                                       *
 *                                                                          *
 *  REV         DATE                DESCRIPTION                             *
 *  ====    ===========         =========================                   *
 *  0.1     17-MAR-2010         Initial Revision                            *
 *                                                                          *
 ****************************************************************************/
#include <htc.h>
#include <string.h>
#include "types.h"
#include "delay.h"
#include "main.h"
#include "rtc.h"
#include "glcd.h"
#include "wwv.h"


/*************** 18F2420 CONFIGURATION BITS ******************************/
// (1) No OSC-Switchover, No Fail-safe clock, internal oscillator
__CONFIG(1, IESODIS & FCMDIS & INTIO);
// (2) No Brown-out, Power-up Timer, No watchdog
__CONFIG(2, BORDIS & PWRTEN & WDTDIS);
// (3) MCLR Enable, Timer-1 Hi power, RC1 Mux, No A/D
__CONFIG(3, MCLREN & LPT1DIS & CCP2RC1 & PBDIGITAL);
// (4) No Ext Instructions, Stack Reset, No LVP
__CONFIG(4, XINSTDIS & STVREN & LVPDIS);
// (5) Unprotected flash + eeprom
__CONFIG(5, UNPROTECT);


// Prototypes
void DisplayTime(TIMESTRUCT *ptt, UINT8 yAll);
void MakeDateString(TIMESTRUCT *ptt, UINT8 *pszBuffer);
void Vdelay_ms(UINT16 wDelay);
void interrupt GlobalInt(void);
void ButtonDown(void);
void DrawTZSetting(UINT8 yTZ);


// Globals
volatile TIMESTRUCT g_ts;                               // last read time
volatile UINT8 g_yUpdate;                               // 1=Update wwvb time
volatile UINT16 g_wFrameCounter;                        // 750ms frame counter
volatile UINT8 g_yTimedOut;                             // 1=time out occured


// Constants
const UINT8 cszDay[7][11] ={"Monday", "Tuesday", "Wednesday", "Thursday",
                            "Friday", "Saturday", "Sunday" };

const UINT8 cszMonth[12][10] =  {"January", "February", "March", "April",
                                "May", "June", "July", "August",
                                "September", "October", "November", "December" };

const UINT8 cszTZChange[] = "Select Time Zone:";
const UINT8 cszTimeZone[7][10] ={"Atlantic", "Eastern", "Central", "Mountain",
                            "Pacific", "Alaskan", "Hawaiian" };
const UINT8 cszSet[] = "-> ";
const UINT8 cszBlank[] = "   ";
const UINT8 cszST[7][4] = {"AST","EST","CST","MST","PST","KST","HST"};
const UINT8 cszDT[7][4] = {"ADT","EDT","CDT","MDT","PDT","KDT","HDT"};


/****************************************************************************
 * void main(void)                                                          *
 *                                                                          *
 * DESCRIPTION                                                              *
 * program start vector.                                                    *
 *                                                                          *
 * PARAMETERS                                                               *
 * none.                                                                    *
 *                                                                          *
 * RETURN VALUE                                                             *
 * none.                                                                    *
 *                                                                          *
 ****************************************************************************/
void main()
{
    TIMESTRUCT tt;
    UINT16 w;
    UINT8 y;

    OSCCONbits.IRCF = 6;                        // 4Mhz internal clock
    OSCCONbits.SCS = 0;                         // primary oscillator

    ADCON0 = 0x00;                              // Disable A/D converter
    ADCON1 = 0x0F;
    CMCON  = 0x07;
    CVRCON = 0x00;
    CCP1CON = 0x00;                             // Disable Capture/Compare/PWM
    CCP2CON = 0x00;

    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    TRISA = 0b11000100;                         // TCO line input
    TRISB = 0b00000100;                         // Push button Switch as input
    TRISC = 0b11111111;                         // LCD Data Bus input
    RBPU = 0;                                   // Weak pullups on PortB

    g_yUpdate = 0;
    rtcInit();                                  // initialize real time clock
    glcdInit();                                 // initialize LCD
    wwvInit();                                  // initialize WWVB module
    glcdClearScreen();

    rtcGetClock(&tt);                           // get current time
    DisplayTime(&tt, 1);                        // display All



    /************************
     *  M A I N   L O O P   *
    /************************/
    while(1)
    {
        rtcGetClock(&tt);                       // get current time

        if(g_ts.sec != tt.sec)                  // seconds time change?
        {
            // Time has changed- update display
            DisplayTime(&tt, 0);                // display it
            memcpy(&g_ts, &tt, sizeof(TIMESTRUCT)); // update for next time

            // update wwvb sync up every 4 hours
            if(tt.sec == 0x01 && tt.min == 0x01)
            {
                if( (rtcBCDToDec(tt.hr) & 0x03) == 0x03)
                    g_yUpdate = 1;              // Set update flag
            }

            // See if wwvb update is needed
            if(g_yUpdate)
            {
                g_yUpdate = 0;                  // reset
                if(wwvGetTime(&tt))
                    rtcSetClock(&tt);           // set new time
                PIN_TLED = 0;
                glcdClearScreen();
                DisplayTime(&tt, 1);            // update display (All)
                memcpy(&g_ts, &tt, sizeof(TIMESTRUCT));

            }
        }

        if(SWITCH_DOWN)                         // Is button down?
        {
            PIN_TLED = 1;
            ButtonDown();                       // Yes, go process
        }
    }


} /* End main */







/****************************************************************************
 * void DisplayTime(TIMESTRUCT *ptt, UINT8 yAll)                            *
 *                                                                          *
 * DESCRIPTION                                                              *
 * Displays the time and date on the LCD from the passed TIMESTRUCT. To     *
 * speed up the routine, only the difference between the passed TIMESTRUCT  *
 * and the global TIMESTRUCT are displayed.                                 *
 *                                                                          *
 * PARAMETERS:                                                              *
 * ptt - Pointer to a TIMESTRUCT containing the current time. This time is  *
 *       compared to the g_ts global time to determine what needs updating. *
 * yAll - If non-zero, the entire page is updated including colons and      *
 *        and all other bitmaps.                                            *
 *                                                                          *
 * RETURN VALUE:                                                            *
 * none.                                                                    *
 *                                                                          *
 ****************************************************************************/
void DisplayTime(TIMESTRUCT *ptt, UINT8 yAll)
{
    UINT8 yUpdateHr, yUpdateMin, yUpdateSec;
    UINT8 y, yHr, yPM, yTZ;
    UINT8 sz[32];

    // Determine what we need to update
    if(yAll)
        yUpdateHr = yUpdateMin = yUpdateSec = 1;    // update all digits
    else
    {
        yUpdateHr  = (ptt->hr != g_ts.hr);
        yUpdateMin = (ptt->min != g_ts.min);
        yUpdateSec = (ptt->sec != g_ts.sec);
    }

    if(yUpdateSec)
    {
        // Update Seconds display
        y = ptt->sec >> 4;                          // move to LSB
        glcdMoveTo(POS_SEC_HI_X, POS_SEC_HI_Y);
        glcdDrawImage(y);                           // Draw upper digit/secs
        glcdMoveTo(POS_SEC_LO_X, POS_SEC_LO_Y);
        glcdDrawImage(ptt->sec & 0x0f);             // Draw lower digit/sec
    }

    if(yUpdateMin)
    {
        // Update Minutes display
        y = ptt->min >> 4;                          // move to LSB
        glcdMoveTo(POS_MIN_HI_X, POS_MIN_HI_Y);
        glcdDrawImage(y);                           // Draw upper digit/minutes
        glcdMoveTo(POS_MIN_LO_X, POS_MIN_LO_Y);
        glcdDrawImage(ptt->min & 0x0f);             // Draw lower digit/minutes
    }

    if(yUpdateHr)
    {
        // Update Hours and AM/PM indicator
        yHr = rtc24to12Hour(ptt->hr, &yPM);         // convert to 12 hour
        y = yHr >> 4;                               // move to LSB
        glcdMoveTo(POS_HR_HI_X, POS_HR_HI_Y);
        if(y)
            glcdDrawImage(y);                       // Draw upper digit/hour
        else
            glcdDrawImage(BMP_BLANK);               // Draw blank, no leading zero

        glcdMoveTo(POS_HR_LO_X, POS_HR_LO_Y);
        glcdDrawImage(yHr & 0x0f);                  // Draw lower digit/hour

        // Draw AM/PM indicator
        glcdMoveTo(POS_AMPM_X, POS_AMPM_Y);
        glcdDrawImage( (yPM?BMP_PM:BMP_AM) );       // Draw AM/PM indicator

        // Update ascii date string
        glcdMoveTo(POS_MMDDYY_X, POS_MMDDYY_Y);     // position for weekday
        strcpy(sz, &(cszDay[(ptt->day) - 1])[0]);   // get day-of-week
        glcdWriteString(sz);                        // display it

        glcdMoveTo(POS_MMDDYY_X, POS_MMDDYY_Y+1);   // position for date
        MakeDateString(ptt, sz);                    // make the string
        glcdWriteString(sz);                        // display it
    }

    if(yAll)
    {
        // Draw colons
        glcdMoveTo(POS_COLON1_X, POS_COLON1_Y);     // position
        glcdDrawImage(BMP_COLON);                   // Draw colon-1
        glcdMoveTo(POS_COLON2_X, POS_COLON2_Y);     // position
        glcdDrawImage(BMP_COLON);                   // Draw colon-2

        // Draw Time Zone string
        y = rtcReadRam(RAM_R_DST);                  // Get DST flag
        yTZ = rtcReadRam(RAM_R_TZONE);              // Get Time Zone
        yTZ = yTZ - TZ_AT;
        glcdMoveTo(POS_ZONE_X, POS_ZONE_Y);         // position for Time Zone
        if(y)
            glcdWriteString((UINT8*)&(cszDT[yTZ][0]));  // display DST
        else
            glcdWriteString((UINT8*)&(cszST[yTZ][0]));  // display ST
    }

} /* End  DisplayTime */






/****************************************************************************
 * void MakeDateString(TIMESTRUCT *ptt, UINT8 *pszBuffer)                   *
 *                                                                          *
 * DESCRIPTION                                                              *
 * Fills the passed buffer with a string containing the day of week, month, *
 * and year. The string has the format of:                                  *
 *                  "January 15, 2010"                                      *
 *                                                                          *
 * PARAMETERS                                                               *
 * ptt - Pointer to a TIMESTRUCT containing the time/date.                  *
 * pszBuffer - Pointer to a buffer to receive the string. The buffer must   *
 *             be at least 22 bytes long.                                   *
 *                                                                          *
 * RETURN VALUE                                                             *
 * none.                                                                    *
 *                                                                          *
 ****************************************************************************/
void MakeDateString(TIMESTRUCT *ptt, UINT8 *pszBuffer)
{
    UINT8 sz[8];
    UINT8 yPos, yChar;
    UINT16 wYear;

    strcpy(pszBuffer, &(cszMonth[rtcBCDToDec(ptt->month) - 1][0])); // do month string

    // Make ascii day-of-month and add a comma/space
    yPos = 0;
    sz[yPos++] = ' ';
    yChar = (ptt->date & 0xf0) >> 4;                    // get upper digit
    if(yChar)
        sz[yPos++] = yChar | 0x30;                      // convert to ascii

    sz[yPos++] = (ptt->date & 0x0f) | 0x30;
    sz[yPos++] = ',';
    sz[yPos++] = ' ';
    sz[yPos] = '\0';
    strcat(pszBuffer, sz);                              // append date

    // Make ASCII year
    sz[0] = '2';                                        // 20xx
    sz[1] = '0';
    sz[2] = ((ptt->year & 0xf0) >> 4) | 0x30;
    sz[3] = (ptt->year & 0x0f) | 0x30;
    sz[4] = ' ';
    sz[5] = '\0';

    strcat(pszBuffer, sz);                              // append year

} /* End MakeDateString */






/****************************************************************************
 * void Vdelay_ms(UINT16 wDelay)                                            *
 *                                                                          *
 * DESCRIPTION                                                              *
 * Dummy delay. The function returns when the passed milliseconds           *
 * have expired.                                                            *
 *                                                                          *
 * PARAMETERS:                                                              *
 * wDelay - Number of milliseconds to delay. Max value is 6,553. Accurate   *
 * to the nearest 100 microseconds.                                         *
 *                                                                          *
 * RETURN VALUE:                                                            *
 * none.                                                                    *
 *                                                                          *
 ****************************************************************************/
void Vdelay_ms(UINT16 wDelay)
{
    UINT16 wCount;

    for(wCount = 0; wCount < (wDelay * 10); wCount++)
    {
        DelayUs(100);                       // 100us delay increments
    }

} /* End Vdelay_ms */





/****************************************************************************
 * void DelayShort(void)                                                    *
 *                                                                          *
 * DESCRIPTION                                                              *
 * Small delay for rtc and LCD graphics use.                                *
 *                                                                          *
 * PARAMETERS                                                               *
 * none.                                                                    *
 *                                                                          *
 * RETURN VALUE                                                             *
 * none.                                                                    *
 *                                                                          *
 ****************************************************************************/
void DelayShort(void)
{
    NOP();

} /* End DelayShort */





/****************************************************************************
 * void ButtonDown(UINT8 yBtnNum)                                           *
 *                                                                          *
 * Handles button presses. If the button is released within 5 seconds,      *
 * a wwvb update is forced. Otherwise, the time zone setting is handled.    *
 *                                                                          *
 * PARAMETERS:                                                              *
 * none.                                                                    *
 *                                                                          *
 * RETURN VALUE:                                                            *
 * none.                                                                    *
 *                                                                          *
 ****************************************************************************/
void ButtonDown()
{
    UINT8 y, yZone;

    Vdelay_ms(50);                                  // debounce delay

    for(y = 0; y < 50; y++)                         // 50*100ms = 5 secs
    {
        Vdelay_ms(100);                             // sample every 100ms
        if(SWITCH_UP)
        {
            // button was released
            g_yUpdate = 1;                          // force an update
            PIN_TLED = 0;                           // Turn off LED
            return;
        }
    }

    // button is still down after 5 seconds
    PIN_TLED = 0;
    glcdClearScreen();
    glcdMoveTo(0, 0);
    glcdWriteString((UINT8*) cszTZChange);          // Time zone lead-in

    yZone = rtcReadRam( RAM_R_TZONE);               // get current TZ setting

    // Draw time zone strings
    for(y = 0; y < 7; y++)
    {
        glcdMoveTo(19, y+1);                        // position for next
        glcdWriteString((UINT8*)&(cszTimeZone[y][0])); // write this one
    }
    while(SWITCH_DOWN)                              // wait for button release
        ;
    Vdelay_ms(50);                                  // debounce delay

    DrawTZSetting(yZone);                           // set current setting

    while(1)
    {
        Vdelay_ms(50);                              // debounce delay
        while(SWITCH_UP)
            ;                                       // wait for button press
        Vdelay_ms(50);                              // debounce delay

        // Time the button release
        for(y = 0; y < 50; y++)                     // 50*100ms = 5 secs
        {
            Vdelay_ms(100);                         // sample every 100ms
            if(SWITCH_UP)
            {
                // button was released, increment to next time zone field
                if(++yZone > TZ_HT)
                    yZone = TZ_AT;                  // wrap-around
                DrawTZSetting(yZone);               // set current setting
                break;
            }
        }

        // Button press exceeded 5 seconds?
        if(y >= 50)
            break;
    }

    rtcWriteRam( RAM_W_TZONE, yZone);               // set new time zone
    g_yUpdate = 1;                                  // mark for update

} /* End ButtonDown */





/****************************************************************************
 * void DrawTZSetting(UINT8 yTZ)                                            *
 *                                                                          *
 * Draws the time zone indicator on the screen. Called during the           *
 * on-screen time zone setup.                                               *
 *                                                                          *
 * PARAMETERS:                                                              *
 * yTZ - Current time zone (TZ_XX format).                                  *
 *                                                                          *
 * RETURN VALUE:                                                            *
 * none.                                                                    *
 *                                                                          *
 ****************************************************************************/
void DrawTZSetting(UINT8 yTZ)
{
    UINT8 y;

    for(y = 0; y < 7; y++)
    {
        glcdMoveTo(0, y+1);

        if(y == (yTZ - TZ_AT))
            glcdWriteString((UINT8*) cszSet);       // draw screen pointer
        else
            glcdWriteString((UINT8*) cszBlank);     // draw blank
    }

} /* End DrawTZSetting */







/////////////////////////////////////////////////////////////////////////////
// void interrupt(void)
//
// DESCRIPTION
// Interrupt handler for all interrupts. We use Timer1 overflows.
//
//
// PARAMETERS
// none
//
// RETURN VALUE
// none.
//
/////////////////////////////////////////////////////////////////////////////
void interrupt GlobalInt(void)
{
    // Check to see if Timer1 interupt overflow occured
    if(PIR1bits.TMR1IF)
    {
        T1CONbits.TMR1ON = 0;                       // Disable Timer
        PIR1bits.TMR1IF = 0;                        // clear overflow flag
        TMR1L = LOBYTE(TMR1_RELOAD);                // Reload Timer value
        TMR1H = HIBYTE(TMR1_RELOAD);

        if(++g_wFrameCounter > 750)                 // 750 = 2.5 minutes
            g_yTimedOut = 1;                        // indicate a time out
        else
            T1CONbits.TMR1ON = 1;                   // ReEnable Timer
    }
}



