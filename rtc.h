/*****************************************************************************
 *  Real Time Clock Module header
 *
 *
 ****************************************************************************/

#ifndef __RTC_H__
#define __RTC_H__

#include "types.h"

// DS1302 Register Definitions
#define RTC_R_CTRL      (UINT8)0x8f         // Control Register Read
#define RTC_W_CTRL      (UINT8)0x8E         // Control Register Write
#define RTC_R_TRKL      (UINT8)0x91         // Trickle Charge Register Read
#define RTC_W_TRKL      (UINT8)0x90         // Trickle Charge Register Write
#define RTC_R_BURST     (UINT8)0xBF         // Burst mode Read
#define RTC_W_BURST     (UINT8)0xBE         // Burst mode Write
#define RTC_R_SEC       (UINT8)0x81         // clock time locations
#define RTC_W_SEC       (UINT8)0x80
#define RTC_R_MIN       (UINT8)0x83
#define RTC_W_MIN       (UINT8)0x82
#define RTC_R_HOUR      (UINT8)0x85
#define RTC_W_HOUR      (UINT8)0x84
#define RTC_R_DATE      (UINT8)0x87
#define RTC_W_DATE      (UINT8)0x86
#define RTC_R_MONTH     (UINT8)0x89
#define RTC_W_MONTH     (UINT8)0x88
#define RTC_R_DAY       (UINT8)0x8B
#define RTC_W_DAY       (UINT8)0x8A
#define RTC_R_YEAR      (UINT8)0x8D
#define RTC_W_YEAR      (UINT8)0x8C


// DS1302 Battery RAM Addresses
#define RAM_W_SIG       (UINT8)0xC0
#define RAM_R_SIG       (UINT8)0xC1         // signature byte
#define RAM_W_TZONE     (UINT8)0xC2
#define RAM_R_TZONE     (UINT8)0xC3         // Time Zone flag
#define RAM_W_LASTMONTH (UINT8)0xC4
#define RAM_R_LASTMONTH (UINT8)0xC5         // Last sync Month
#define RAM_W_LASTDATE  (UINT8)0xC6
#define RAM_R_LASTDATE  (UINT8)0xC7         // Last sync Date
#define RAM_W_LASTHOUR  (UINT8)0xC8
#define RAM_R_LASTHOUR  (UINT8)0xC9         // Last sync Hour
#define RAM_W_LASTMIN   (UINT8)0xCA
#define RAM_R_LASTMIN   (UINT8)0xCB         // Last sync Minute
#define RAM_W_DST       (UINT8)0xCC
#define RAM_R_DST       (UINT8)0xCD         // DST Flag
#define RAM_W_LASTMONTH2 (UINT8)0xCE
#define RAM_R_LASTMONTH2 (UINT8)0xCF        // Last sync Month2
#define RAM_W_LASTDATE2  (UINT8)0xD0
#define RAM_R_LASTDATE2  (UINT8)0xD1        // Last sync Date2
#define RAM_W_LASTHOUR2  (UINT8)0xD2
#define RAM_R_LASTHOUR2  (UINT8)0xD3        // Last sync Hour2
#define RAM_W_LASTMIN2   (UINT8)0xD4
#define RAM_R_LASTMIN2   (UINT8)0xD5        // Last sync Minute2


// Time Zone flags stored in RR_TZone (negative time offset from UTC)
#define TZ_AT           4                   // Atlantic Time
#define TZ_ET           5                   // Eastern Time
#define TZ_CT           6                   // Central Time
#define TZ_MT           7                   // Mountain Time
#define TZ_PT           8                   // Pacific Time
#define TZ_KT           9                   // Alaskan Time
#define TZ_HT           10                  // Hawaiian Time

#define RTC_SIGNATURE   (UINT8)0x95         // Ram signature check byte value

// The TIMESTRUCT struct consolidates the time readings. All values
// are in BCD format
typedef struct _TIMESTRUCT
{
    UINT8 sec;                          // 0x00 - 0x59 seconds
    UINT8 min;                          // 0x00 - 0x59 minutes
    UINT8 hr;                           // 0x00 - 0x23 hours
    UINT8 date;                         // 0x01 - 0x31 date of month
    UINT8 month;                        // 0x01 - 0x12 Month (1=January)
    UINT8 day;                          // day of week 1-7 (1=Monday)
    UINT8 year;                         // 0x00-0x99 (2000 - 2099)
} TIMESTRUCT;


// Public Prototypes
extern void  rtcInit(void);
extern UINT8 rtcReadRam( UINT8 yAddress);
extern void  rtcWriteRam( UINT8 yAddress, UINT8 yData);
extern void  rtcSetClock(TIMESTRUCT *pTime);
extern void  rtcGetClock(TIMESTRUCT *pTime);
extern UINT8 rtcBCDToDec(UINT8 yNum);
extern UINT8 rtcDecToBCD(UINT8 yNum);
extern UINT8 rtc24to12Hour(UINT8 yHour, UINT8 *pPM);
extern void  rtcSaveLastSync(TIMESTRUCT *pt, UINT8 ySet);
extern void  rtcBumpLastSync(void);

#endif /* __RTC_H__ */
