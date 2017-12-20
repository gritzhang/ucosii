#ifndef __FRAM__H 
#define __FRAM__H

#include "AllHand.h"
#include "stdint.h"


//#define 	SSP0CSFR		(1 << 21)	              			//p1.21               
//#define 	SET_SSPCSFR()	LPC_GPIO1->DIR |= SSP0CSFR;LPC_GPIO1->SET |= SSP0CSFR	                            
//#define 	CLR_SSPCSFR()	LPC_GPIO1->DIR |= SSP0CSFR;LPC_GPIO1->CLR |= SSP0CSFR	


/******************************************
*命令操作码 SPI MODE0 OR SPI MODE3 
*******************************************/
#define  COMD_WR_ENABLE				0x06
#define  COMD_WR_DISABLE			0x04
#define  COMD_RD_STATUS				0x05
#define  COMD_WR_STATUS				0x01
#define  COMD_RD_MEMORY				0x03
#define  COMD_WR_MEMORY				0x02

/**************************************************************************************************
**	函数名称:	FramSendByte()
**	功	能	:	向FRAM发送一字节数据
**	入口参数:	dat				:	所要发送的数据
**	出口参数:	无
**  返回值  ：  无
**	全局变量:	无
**	调用模块:	ddelay();
**	作	者	:	邓小颖
**	日	期	:	2009年03月06日
**************************************************************************************************/
extern void FramSendByte(uint8_t dat);
/**************************************************************************************************
**	函数名称:	FramReadByte()
**	功	能	:	从FRAM读出一字节数据
**	入口参数:	无
**	出口参数:	无
**  返回值  ：  data			:	读出的数据
**	全局变量:	无
**	调用模块:	ddelay();
**	作	者	:	邓小颖
**	日	期	:	2009年03月06日
**************************************************************************************************/
extern uint16_t FramReadByte(void );
/**************************************************************************************************
**	函数名称:	FramState()
**	功	能	:	查询FRAM的状态
**	入口参数:	无
**	出口参数:	无
**  返回值  ：  statetemp		:	期间的状态
**	全局变量:	无
**	调用模块:	FramSendByte、FramReadByte
**	作	者	:	邓小颖
**	日	期	:	2009年03月06日
**************************************************************************************************/
extern uint8_t FramState(void);
/**************************************************************************************************
**	函数名称:	FramWrite()
**	功	能	:	向FRAM写数据
**	入口参数:	*dataptr	:	指向所要发送的数据的指针
**				addr		:	FRAM的起始存储地址
**				length		:	数据长度
**	出口参数:	无
**  返回值  ：  无
**	全局变量:	无
**	调用模块:	FramSendByte、FramState
**	作	者	:	邓小颖
**	日	期	:	2009年03月06日
**************************************************************************************************/
extern void FRAMWrite(uint8_t *dataptr,uint16_t addr,uint16_t length);
/**************************************************************************************************
**	函数名称:	FRAMRead()
**	功	能	:	读FRAM
**	入口参数:	*dataptr	:	指向所要存放的数据的指针
**				addr		:	FRAM的起始存储地址
**				length		:	数据长度
**	出口参数:	无
**  返回值  ：  无
**	全局变量:	无
**	调用模块:	FramSendByte、FramReadByte、FramState
**	作	者	:	邓小颖
**	日	期	:	2009年03月06日
**************************************************************************************************/
extern void	FRAMRead(uint8_t *dataptr,uint16_t addr,uint16_t length);
/**************************************************************************************************
**	函数名称:	FRAMCheck()
**	功	能	:	FRAM检查，用于上电检查FRAM故障
**	入口参数:	无储地址
**	出口参数:	无
**  返回值  ：  =0 成功	= 1 失败
**	全局变量:	无
**	调用模块:	
**	作	者	:	邓小颖
**	日	期	:	2009年03月06日
**************************************************************************************************/
extern uint8_t	FRAMCheck(void);

#endif


/*************************************************************************************************
**                            End Of File
**************************************************************************************************/
