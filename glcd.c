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
 *  GLCD Module                                                             *
 *  High level lcd graphics routines                                        *
 *                                                                          *
 ****************************************************************************/
#include <htc.h>
#include "types.h"
#include "glcd.h"
#include "bitmaps.h"


volatile UINT8 s_yLastSeg;                      // Current segment(0-1)
volatile UINT8 s_yLastCol;                      // Current Column (0-63)
volatile UINT8 s_yLastPage;                     // Current Page (row) (0-7)




/****************************************************************************
 * void glcdInit(void)                                                      *
 *                                                                          *
 * DESCRIPTION                                                              *
 * Initializes the glcd interface. Called only once on power-up.            *
 *                                                                          *
 * PARAMETERS                                                               *
 * none.                                                                    *
 *                                                                          *
 * RETURN VALUE                                                             *
 * none.                                                                    *
 *                                                                          *
 ****************************************************************************/
void glcdInit(void)
{
    UINT8 y;

    glbaseInit();                           // init low level ports
    s_yLastSeg = 0;                         // init to zero position
    s_yLastCol = 0;
    s_yLastPage = 0;

    // Turn on display
    for(y = 0; y < 2; y++)
    {
        glbaseWriteData((DISPLAY_ON_CMD | ON), 0, y);   // Display ON
        glbaseWriteData((DISPLAY_START_LINE), 0, y);    // Zero Start Line
    }
} /* End glcdInit */





/****************************************************************************
 * void glcdClearScreen(void)                                               *
 *                                                                          *
 * DESCRIPTION                                                              *
 * Clears the screen.                                                       *
 *                                                                          *
 * PARAMETERS                                                               *
 * none.                                                                    *
 *                                                                          *
 * RETURN VALUE                                                             *
 * none.                                                                    *
 *                                                                          *
 ****************************************************************************/
void glcdClearScreen(void)
{
    UINT8 yX, yY, ySeg;

    for(ySeg = 0; ySeg < 2; ySeg++)                 // both segments to clear
    {

        for(yX=0; yX < 8; yX++)
        {
            glcdSetPage(yX, ySeg);                  // set the page (x-address)
            glcdSetCol(0, ySeg);                    // set the y address to 0

            for (yY = 0; yY < 64; yY++)
            {
                glbaseWriteData(0, 1, ySeg);            // Write zeroes

            }
        }
    }

} /* End glcdClearScreen */






/****************************************************************************
 * void glcdSetPage(UINT8 yPage, UINT8 ySeg)                                *
 *                                                                          *
 * DESCRIPTION                                                              *
 * Sets the Page (vertical row) of the display where the next data write    *
 * will occur.                                                              *
 *                                                                          *
 * PARAMETERS                                                               *
 * yPage - The page number (0 - 7)                                          *
 * ySeg -  The segment, left (0) or right(1)                                *
 *                                                                          *
 * RETURN VALUE                                                             *
 * none.                                                                    *
 *                                                                          *
 ****************************************************************************/
void glcdSetPage(UINT8 yPage, UINT8 ySeg)
{
    glbaseWaitForNonBusy(ySeg);                         // wait for non-busy
    glbaseWriteData(DISPLAY_SET_X | yPage, 0, ySeg);    // set lcd X-address
}





/****************************************************************************
 * void glcdSetCol(UINT8 yCol, UINT8 ySeg)                                  *
 *                                                                          *
 * DESCRIPTION                                                              *
 * Sets the Column (horizontal) of the display where the next data write    *
 * will occur.                                                              *
 *                                                                          *
 * PARAMETERS                                                               *
 * yCol - The column number (0 - 63)                                        *
 * ySeg -  The segment, left=0 right=1                                      *
 *                                                                          *
 * RETURN VALUE                                                             *
 * none.                                                                    *
 *                                                                          *
 *****************************************************************************/
void glcdSetCol(UINT8 yCol, UINT8 ySeg)
{
    glbaseWaitForNonBusy(ySeg);                         // wait for non-busy
    glbaseWriteData(DISPLAY_SET_Y | yCol, 0, ySeg);     // set lcd Y-address
}





/****************************************************************************
 * void glcdMoveTo(UINT8 yX, UINT8 yY)                                      *
 *                                                                          *
 * DESCRIPTION                                                              *
 * Moves the cursor to the position specificed in the passed x-y coords.    *
 *                                                                          *
 * PARAMETERS                                                               *
 * yX - New X position (0 - 127)                                            *
 * yY - New Y position (0 - 7). NOTE: The KS0108 controller can only be     *
 * positioned in Y direction on 8-bit boundaries, so specifying 0-7 yields: *
 * 0=pixel Pos #0, 1=pixel Pos #8, 2=pixel Pos #16, etc.                    *
 *                                                                          *
 * RETURN VALUE                                                             *
 * none.                                                                    *
 *                                                                          *
 ****************************************************************************/
void glcdMoveTo(UINT8 yX, UINT8 yY)
{

    // Set new static Segment and Column
    s_yLastPage = yY;
    if(yX > 63)                                     // in 2nd segment?
    {
        s_yLastSeg = 1;
        s_yLastCol = yX - 64;
    }
    else
    {
        s_yLastSeg = 0;                             // in first segment
        s_yLastCol = yX;
    }

    glcdSetCol(yX, s_yLastSeg);                     // set it
    glcdSetPage(yY, s_yLastSeg);

} /* End glcdMoveTo */






/****************************************************************************
 * void glcdWriteChar(UINT8 yChar)                                          *
 *                                                                          *
 * DESCRIPTION                                                              *
 * Write a font character to the display at the current position.           *
 *                                                                          *
 * PARAMETERS                                                               *
 * yChar - The ASCII character to write                                     *
 *                                                                          *
 * RETURN VALUE                                                             *
 * none.                                                                    *
 *                                                                          *
 ****************************************************************************/
void glcdWriteChar(UINT8 yChar)
{
    UINT16 i;
    char *pChar;

    yChar -= 32;                                // subtract to index into table

    // Write each of the 5 columns
    for(i = 0; i < 6; i++)
    {
        pChar = ((UINT8 *)yFont6x8 + (6 * yChar) + i);
        glbaseWriteData(*pChar, 1, s_yLastSeg);
        if(++s_yLastCol > 63)
        {
            // Column is at the end of the segment
            s_yLastCol = 0;                     // reset to 1st column
            if(s_yLastSeg)                      // In segment 1? (right)
            {
                // We're at the right end of pixels
                s_yLastSeg = 0;                 // wrap-around to left segment
                if(++s_yLastPage > 7)
                    s_yLastPage = 0;            // wrap to top
            }
            else
                s_yLastSeg = 1;                 // continue in 2nd column

            glcdSetCol(0, s_yLastSeg);
            glcdSetPage(s_yLastPage, s_yLastSeg);
        }
    }

} /* End glcdWriteChar */






/****************************************************************************
 * void glcdWriteString(char * pString)                                     *
 *                                                                          *
 * DESCRIPTION                                                              *
 * Writes a string of characters to the lcd display.                        *
 *                                                                          *
 * PARAMETERS                                                               *
 * pString - Pointer to a zero-terminated string to write.                  *
 *                                                                          *
 * RETURN VALUE                                                             *
 * none.                                                                    *
 *                                                                          *
 ****************************************************************************/
void glcdWriteString(char * pString)
{
    while(*pString)
        glcdWriteChar(*pString++);

} /* End glcdWriteString */





/****************************************************************************
 * void glcdDrawBitmap(UINT8 *pBMP, UINT8 yWidth, UINT8 yHeight)            *
 *                                                                          *
 * DESCRIPTION                                                              *
 * Draws a bitmap image on the lcd display.                                 *
 *                                                                          *
 * PARAMETERS                                                               *
 * pBMP - Pointer to the bitmap image.                                      *
 * yWidth - The width of the bitmap in PIXELS. (1 - 128)                    *
 * yHeight - The height of the bitmap in BYTES. (1 - 7)                     *
 *                                                                          *
 * RETURN VALUE                                                             *
 * none.                                                                    *
 *                                                                          *
 ****************************************************************************/
void glcdDrawBitmap(UINT8 *pBMP, UINT8 yWidth, UINT8 yHeight)
{
    UINT8 yXAxis, yYAxis;
    UINT8 yBaseSeg, yBaseCol;

    s_yLastPage--;                                  // pre-decrement for loop
    yBaseCol = s_yLastCol;
    yBaseSeg = s_yLastSeg;


    // Walk through each page (Y Axis)
    for(yYAxis = 0; yYAxis < yHeight; yYAxis++)
    {
        // Set the Page/Column/Segment position for this row (page)
        if(++s_yLastPage > 7)
        {
            s_yLastPage = 7;
            return;                                 // column overflow, just quit
        }
        glcdSetPage(s_yLastPage, yBaseSeg);         // set page in lcd
        glcdSetCol(yBaseCol, yBaseSeg);             // set column back to beginning
        s_yLastCol = yBaseCol;                      // reset back to base
        s_yLastSeg = yBaseSeg;

        // Walk through each Column (X Axis)
        yXAxis = 0;
        while(yXAxis < yWidth)
        {
            glbaseWriteData(*pBMP++, 1, s_yLastSeg);    // write current byte

            if(++s_yLastCol > 63)
            {
                // Column is at the end of the segment
                if(!s_yLastSeg)                         // In segment 1? (left)
                {
                    // Yes, move to next segment
                    s_yLastCol = 0;                     // reset to column #0
                    s_yLastSeg = 1;                     // now in 2nd(right) seg
                    glcdSetCol(s_yLastCol, s_yLastSeg); // update in lcd
                    glcdSetPage(s_yLastPage, s_yLastSeg);
                }
                else
                {
                    // We hit the far right column of the display
                    return;                             // no wrap around allowed
                }
            }
            yXAxis++;

        }
    }

} /* End glcdDrawBitmap */






/****************************************************************************
 * void glcdDrawImage(UINT8 yNum)                                           *
 *                                                                          *
 * DESCRIPTION                                                              *
 * Draws an image defined as a IMAGE_XXXX constant.                         *
 *                                                                          *
 * PARAMETERS                                                               *
 * yNum - The image number. One of the IMAGE_xxx constants.                 *
 *                                                                          *
 * RETURN VALUE                                                             *
 * none.                                                                    *
 *                                                                          *
 ****************************************************************************/
void glcdDrawImage(UINT8 yNum)
{
    const UINT8 *py;
    UINT8 yW, yH;

    switch(yNum)
    {
        // Make height, width, and image pointers
        // Width is in pixels, Height is in bytes.
        case BMP_0:     yW = 13; yH = 3; py = dig0;     break;
        case BMP_1:     yW = 13; yH = 3; py = dig1;     break;
        case BMP_2:     yW = 13; yH = 3; py = dig2;     break;
        case BMP_3:     yW = 13; yH = 3; py = dig3;     break;
        case BMP_4:     yW = 13; yH = 3; py = dig4;     break;
        case BMP_5:     yW = 13; yH = 3; py = dig5;     break;
        case BMP_6:     yW = 13; yH = 3; py = dig6;     break;
        case BMP_7:     yW = 13; yH = 3; py = dig7;     break;
        case BMP_8:     yW = 13; yH = 3; py = dig8;     break;
        case BMP_9:     yW = 13; yH = 3; py = dig9;     break;
        case BMP_BLANK: yW = 13; yH = 3; py = digBlank; break;
        case BMP_COLON: yW =  6; yH = 3; py = digColon; break;
        case BMP_AM:    yW = 23; yH = 2; py = digAM;    break;
        case BMP_PM:    yW = 23; yH = 2; py = digPM;    break;
        default: return;
    }

    glcdDrawBitmap((UINT8*)py, yW, yH);         // go draw the bitmap

} /* End glcdDrawBigDigit */



