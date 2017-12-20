/****************************************Copyright (c)****************************************************
**                            Guangzhou ZHIYUAN electronics Co.,LTD.
**
**                                 http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:                  PCF8563.h
** Latest modified Date:       2009-6-8
** Latest Version:
** Descriptions:
**
**--------------------------------------------------------------------------------------------------------
** Created by:                 Zhangrong
** Created date:               2009-6-8
** Version:
** Descriptions:
**
**--------------------------------------------------------------------------------------------------------
** Modified by:                
** Modified date:              2009-6-8
** Version:
** Descriptions:
**
*********************************************************************************************************/
#ifndef __PCF8563_H
#define __PCF8563_H
#include "stdint.h"
#include "ucos_ii.h"
#include "hardware_interface.h"



typedef struct _PCF8563_DATE{
   uint8_t	second;
   uint8_t	minute;
   uint8_t	hour;
   uint8_t	day;
   uint8_t	week;
   uint8_t	month;
   uint16_t	year;
}PCF8563_DATE;

extern 	uint32_t rtcInit (PCF8563_DATE *pTimeAndDate );
extern 	void ExRTCSet(PCF8563_DATE *pTimeAndDate );

#define PCF8563ADR		0xa2
/*
 *  定义:秒/分/时/日/星期/月/年
 */
#define Second			0x02
#define Minute			0x03
#define Hour			0x04
#define	Day				0x05
#define Week			0x06
#define Month			0x07
#define Year			0x08
/*
 *  报警设置
 */
#define AlarmEn			0x00
#define AlarmDs			0x80
/*
 *  ClkOut输出设置
 */
#define ClkOutEn		0x80
#define ClkOutDs		0x00
#define F32768Hz		0x00
#define F1024Hz			0x01
#define F32Hz			0x02
#define F1Hz			0x03
/*
 *  开关设置
 */
#define Start	1
#define Stop	0
/*
 *  定时器设置
 */
#define TimerOn			0x80
#define TimerOff		0x00
#define TimerClk4096Hz  0x00
#define TimerClk64Hz	0x01
#define TimerClk1Hz		0x02
#define TimerClk1_64Hz	0x03
/*
 *  中断设置
 */
#define SetINT			0x80
#define ClearINT		0x40
#define TITP			0x10
#define AIE				0x02
#define TIE				0x01
/*
 *  数据转换
 */
#define	BCD_to_HEC(b)	(((b>>4)*10)+(b&0x0f))
#define	HEC_to_BCD(h)	(((h/10)<<4)|(h%10))

extern PCF8563_DATE    	TimeAndDate;
extern PCF8563_DATE		RTCtimedate;
//extern PCF8563_DATE		BakTimeDate;

extern uint8_t PCF8563_Set(PCF8563_DATE *tp);
extern uint8_t PCF8563_Read(PCF8563_DATE *tp);
extern uint8_t PCF8563_SS(uint8_t cmd);
extern uint8_t PCF8563_Set_TD(uint8_t cmd, uint16_t TD);
extern uint16_t PCF8563_Get_TD(uint8_t cmd);
extern uint8_t PCF8563_Set_ClkOut(uint8_t cmd);
extern uint8_t PCF8563_Set_Alarm(uint8_t cmd, uint8_t tm);
extern uint8_t PCF8563_Set_Timer(uint8_t cmd, uint8_t cnt);
extern uint8_t PCF8563_INT_State(uint8_t cmd);

#endif


/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
