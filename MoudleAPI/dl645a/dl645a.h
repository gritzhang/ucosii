/****************************************Copyright (c)****************************************************
**                            		xxx Co.,LTD.
**
**                                 http://www.xxx.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               calendar.h
** Latest modified Date:    2013-09-05
** Latest Version:          1.00
** Descriptions:            日历时钟数据转换函数
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AndyLau
** Created date:            2013-09-05
** Version:                 1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
**
*********************************************************************************************************/
#ifndef __ANDY_DL645A_H
#define __ANDY_DL645A_H

#include "AllHand.h"

#ifdef __cplusplus
extern "C" {
#endif                                                                  /*  __cplusplus                 */

//#include "typedefine.h"

#define 	MAXFRMLEN		200

#define 	DLENOFFSET		9
#define 	DATAOFFSET		10
#define 	VALUEOFFSET		14

extern uint8_t 	FramePack_645_1997(uint16_t IDSymbol,uint8_t *MeterNo,uint8_t *DataPtr,uint8_t DataLen,uint8_t *SendBuff,uint8_t Ctlw);
extern uint8_t  FramePack_645_2007(uint8_t *MeterNo,uint8_t Ctlw,uint32_t IDSymbol,uint8_t *DataPtr,uint8_t DataLen,uint8_t *SendBuff);
extern uint16_t FrameCheck_645_2007( uint8_t *RcvBuff ,uint8_t *MeterNo,uint32_t IDSymbol);
extern uint16_t FrameCheck_645_1997( uint8_t *RcvBuff ,uint8_t *MeterNo,uint8_t *IDSymbol);

#ifdef __cplusplus
}
#endif                                                                  /*  __cplusplus                 */

#endif                                                                  /*  __ANDY_DL645A_H             */

/*********************************************************************************************************
							END OF FILE
*********************************************************************************************************/
