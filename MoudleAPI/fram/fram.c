/****************************************Copyright (c)*********************************************
**                                     惠州龙鼎盛电力科技有限公司
**									       开   发   部
**--------------文件信息---------------------------------------------------------------------------
**文   件   名: FRAM.C
**创   建   人: 邓小颖
**创 建 日 期 ：2009年03月06日
**描        述: FM25H**的铁电驱动程序
**--------------当前版本修订-----------------------------------------------------------------------
** 修改人: 
** 日　期: 
** 描　述: 
**
**************************************************************************************************/
#define IN_FRAM_GLOBALS
#include "fram.h"
#include "FM1702.h"

/**************************************************************************************************
**	函数名称:	FramState()
**	功	能	:	查询FRAM的状态
**	入口参数:	无
**	出口参数:	无
**  返回值  ：  statetemp		:	期间的状态
**	全局变量:	无
**	调用模块:	FMSendByte、FMReadByte
**	作	者	:	邓小颖
**	日	期	:	2009年03月06日
**************************************************************************************************/
uint8_t FramState(void)
{	
	uint8_t statetemp;
	CLR_SSPCSFR() ;
	ddelay(5) ;
	FMSendByte(COMD_RD_STATUS);				//读器件状态
	statetemp	= FMReadByte();
	SET_SSPCSFR() ;
	ddelay(5) ;
	return(statetemp);
}
/**************************************************************************************************
**	函数名称:	FramWrite()
**	功	能	:	向FRAM写数据
**	入口参数:	*dataptr	:	指向所要发送的数据的指针
**				addr		:	FRAM的起始存储地址
**				length		:	数据长度
**	出口参数:	无
**  返回值  ：  无
**	全局变量:	无
**	调用模块:	FMSendByte、FramState
**	作	者	:	邓小颖
**	日	期	:	2009年03月06日
**************************************************************************************************/
void FRAMWrite(uint8_t *dataptr,uint16_t addr,uint16_t length)
{	
	uint16_t i;
	uint8_t	err;
	
//	OSSemPend(FRAMSem,0,&err);
	
	err	= FramState();
	
	if((err & 0x71) == 0)
	{
		CLR_SSPCSFR() ;
		ddelay(5) ;
		FMSendByte(COMD_WR_ENABLE);
		SET_SSPCSFR();
		ddelay(5) ;
		
		while((FramState() & 0x02 )== 0)
        {
            OSTimeDly(OS_TICKS_PER_SEC/10);
        }

		CLR_SSPCSFR() ;
		ddelay(5) ;
		FMSendByte(COMD_WR_MEMORY);
		FMSendByte((addr >> 8) & 0xff);
		FMSendByte(addr & 0xff);
		for (i = 0; i < length; i++)
		{	
			FMSendByte(*dataptr);
			dataptr	+= 1;
		}
		SET_SSPCSFR();
		ddelay(5) ;
	}
}
/**************************************************************************************************
**	函数名称:	FRAMRead()
**	功	能	:	读FRAM
**	入口参数:	*dataptr	:	指向所要存放的数据的指针
**				addr		:	FRAM的起始存储地址
**				length		:	数据长度
**	出口参数:	无
**  返回值  ：  无
**	全局变量:	无
**	调用模块:	FMSendByte、FMReadByte、FramState
**	作	者	:	邓小颖
**	日	期	:	2009年03月06日
**************************************************************************************************/
void FRAMRead(uint8_t *dataptr,uint16_t addr,uint16_t length)
{	
	uint16_t i;
	uint8_t	err;
	
	CLR_SSPCSFR();
	ddelay(5) ;
	FMSendByte(COMD_RD_MEMORY);
	FMSendByte((addr >> 8) & 0xff);
	FMSendByte(addr & 0xff);	
	for (i = 0; i < length; i++)
	{
		*dataptr	= FMReadByte();
		dataptr++;
	}
	SET_SSPCSFR();
	ddelay(5) ;

}
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
uint8_t	FRAMCheck(void)
{	
	uint8_t	i;
	uint8_t	nn;
	uint8_t	err;
	
	err	= 1;

//	OSSemPend(FRAMSem,0,&err);
//	CLR_SSPCSFR() ;
//	ddelay(5) ;
	for(i = 0;i < 50; i++)
	{
		nn	= FramState();
		
		if((nn & 0x71) == 0)
		{
			err	= 0;
			break;
		}
	}
//	SET_SSPCSFR();
//	ddelay(5) ;
//	OSSemPost(FRAMSem);

	return err;
}

/*************************************************************************************************
**                            End Of File
**************************************************************************************************/

