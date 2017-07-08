/*****************************************************************************
 *  Main Module header
 *
 *
 ****************************************************************************/

#ifndef __MAIN_H__
#define __MAIN_H__

#include <htc.h>
#include "types.h"
#include "rtc.h"

/* Useful Macros */
#define MAKEWORD(h, l)      (((UINT16)((UINT8)(h))) << (UINT8)8) | (UINT16)( ((UINT8)(l)) )
#define LOBYTE(w)           ((UINT8)(w))
#define HIBYTE(w)           ((UINT8)(((UINT16)(w) >> (UINT8)8) & 0xff))
#define LOWORD(l)           ((UINT16)(l))
#define HIWORD(l)           ((UINT16)(((UINT32)(l) >> (UINT8)16) & (UINT16)0xFFFF))
#define MAKEUINT32(h, l)    ((UINT32)(((UINT16)(l)) | ((UINT32)((UINT16)(h))) << (UINT8)16))

extern volatile UINT16 g_wFrameCounter;                 // 750ms counter
extern volatile  UINT8 g_yTimedOut;                     // 1=A time out occured

extern void DelayShort(void);

#endif /* __MAIN_H__ */
