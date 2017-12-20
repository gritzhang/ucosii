#ifndef _RTC_H_
#define _RTC_H_

#include "AllHand.h"

/*********************************************************************************************************
** Function name:       ExRTCSet
** Descriptions:        设置外部时钟
** input parameters:    无
** Output parameters:   无
** Returned value:      无
*********************************************************************************************************/
extern void ExRTCSet(PCF8563_DATE *pTimeAndDate );

/*********************************************************************************************************
** Function name:       ExRTCInit
** Descriptions:        外部时钟初始化
** input parameters:    无
** Output parameters:   无
** Returned value:      无
*********************************************************************************************************/
extern void ExRTCInit(void);

/*********************************************************************************************************
** Function name:       main_rtc
** Descriptions:        PCF8563时钟
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
extern void main_rtc (void);


#endif

