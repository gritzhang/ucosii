/****************************************Copyright (c)****************************************************
**                            ShangHai YUNWANG *** Co.,LTD.
**
**                                 http://www.***.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:                  PTL104.h
** Latest modified Date:       2014-1-6
** Latest Version:
** Descriptions:
**
**--------------------------------------------------------------------------------------------------------
** Created by:                 AndyLau
** Created date:               2014-1-6
** Version:
** Descriptions:
**
**--------------------------------------------------------------------------------------------------------
** Modified by:                
** Modified date:              2014-1-6
** Version:
** Descriptions:
**
*********************************************************************************************************/

#ifndef __PTL104_H_
#define __PTL104_H_

#include "AllHand.h"

#define 	FIFODEEP		6
#define 	BUFSIZE			255

//104规约I帧结构体
typedef struct 
{
	unsigned char  Head;		//帧头
	unsigned char  Flen;		//帧长度=总帧长-2
	unsigned short Sdsn;		//发送序号
	unsigned short Rxsn;		//接收序号
	unsigned char  Type;		//类型标识
	unsigned char  Lmtd;		//可变结构限定词
	unsigned short Rson;		//传送原因
	unsigned short Apdu;		//应用服务数据单元
	unsigned char  Dobj[243];	//信息对象1...N
}sPtl_104_i;

extern 	uint8_t F104_INF_PACK(uint8_t *sBuf,sPtl_104_i * pPtl104);
extern  uint8_t F104_RC_PACK(uint8_t *rcBuf,sPtl_104_i * rcPtl104);
extern  uint8_t F104_ZZHSJ_PACK(sPtl_104_i * pPtl104);


//表5-34充电过程实时监测数据
typedef struct 
{
	unsigned short  CDSCDY;		//充电输出电压
	unsigned short 	CDSCDL;		//充电输出电流
	unsigned char   SCJDQZT;	//输出继电器状态
	unsigned char   LJQRKGZT;	//连接确认开关状态
	unsigned int    YGZDD;		//有功总电度
	unsigned char   CDZBH[8];	//充电桩编号
	unsigned short  SFLJDC;		//是否连接电池
	unsigned short  GZZT;		//工作状态
	
}sPtl_104_i_100;
//表5-34充电过程实时监测数据
typedef struct 
{
	unsigned short  MS;		
	unsigned short 	MIN;		
	unsigned char   DAY;	
	unsigned char   WEEK;	
	unsigned char   MON;		
	unsigned char   YEAR;		
}sPtl_104_i_tim;
//系统参数结构体
typedef struct 
{
	unsigned char  Head;		//帧头
	unsigned char  Flen;		//帧长度=总帧长-2
	unsigned short Sdsn;		//发送序号
	unsigned short Rxsn;		//接收序号
	unsigned char  Type;		//类型标识
	unsigned char  Lmtd;		//可变结构限定词
	unsigned short Rson;		//传送原因
	unsigned short Apdu;		//应用服务数据单元
	unsigned char  Dobj[243];	//信息对象1...N
	//82 01 03 00 00 00 01 01 00 00
}sSys_para;
//串口发送缓存
typedef struct
{
	unsigned int		len ; 				//帧长度
	unsigned char		data[BUFSIZE] ;		//帧数据

}UART_SEND_FIFO;
//串口接收缓存
typedef struct
{
	unsigned int		len ; 				//帧长度
	unsigned char		data[BUFSIZE] ;		//帧数据

}UART_RECV_FIFO;

//待确认序号
typedef struct
{
	unsigned char		flag ; 				//有效标志
	unsigned short		Sidx;				//待确认的上行帧发送序号（+2）
	unsigned char		Fdat[255];			//待确认上行帧
}sFor_Ack_Idx;							   	

//串口收发延时
typedef struct 
{
	unsigned short  	AckTimS;			//待确认超时
	
	unsigned short  	SndDlyS;			//发送时间间隔
	unsigned short  	SndTimS;			//发送秒计时	
	unsigned short 		RcvDlyS;			//接收时间间隔	
	unsigned short  	RcvTimS;			//发送秒计时
	unsigned short  	GucHeartF1;         /* 	心跳计时			        */
	unsigned short		GucHeartT1;         /* 	重连计数			        */
	uint8_t					StopFlag;			//停止发送标志
}sUart_DlyTim;

extern uint8_t sPTL_104_COMOUT_COUNT;

extern BALANCEDOWN_PARA	    sPTL_104_I_DW_KKXX;
extern CHGONLINE_PARA		sPTL_104_I_UP_ZXSJ;
extern CHGOFFLINE_PARA		sPTL_104_I_UP_LXSJ;
extern sPtl_104_i_100		sPTL_104_I_100;
extern sPtl_104_i_100		sPTL_104_I_100_BAK;
extern CHECKUP_PARA 		sPTL_104_I_UP_JQSJ;
extern CHECKDOWN_PARA		sPTL_104_I_DW_JQSJ;

extern sFor_Ack_Idx			ForAckIdx[FIFODEEP];
extern sPtl_104_i 			sPTL_104_I_SND;
extern sPtl_104_i 			sPTL_104_I_RCV;
extern sUart_DlyTim			UartDlyTim;
 
extern uint8_t Frame_Active_Packed(uint8_t *sBuf);
extern uint8_t Frame_Test_Packed(uint8_t *sBuf);
extern uint8_t Frame_Stop_Packed(uint8_t *sBuf);
extern uint8_t Frame_Ack_Packed(uint8_t *sBuf,uint16_t rSn);
extern uint8_t Frame_Inf_Packed(uint8_t *sBuf,sPtl_104_i * pPtl104);
extern uint8_t F104_ACK_STAT( void );
extern void F104_JQZ_PACK(sPtl_104_i * pPtl104);
extern void F104_ZXSJ_PACK(sPtl_104_i * pPtl104);
extern void F104_LXSJ_PACK(sPtl_104_i * pPtl104);

extern uint8_t Is_F104_UnACK(void);
extern void F104_LXSJ_Send(sPtl_104_i * pPtl104);
extern uint8_t F104_LOGIN_PACK(uint8_t *sBuf,uint8_t *pNo,uint16_t add);
extern void F104_JFCLCDQKSC_PACK(sPtl_104_i * pPtl104);
extern void SET_CHG_JSSJ(void );
extern void SET_CHG_KSSJ(void );

extern void PTL104_JQXX_unpack(sPtl_104_i * pPtl104);
extern void PTL104_JQXXWithOrder_unpack(sPtl_104_i * pPtl104);
extern void PTL104_JQXXWithOrderAndLogicNumber_unpack(sPtl_104_i * pPtl104);

extern void SET_JFCLSJ(CHASTRATEGY_UP_PARA * strategy_up);
extern uint8_t F104_JFCLCDQK_UP_PACK(sPtl_104_i *pPtl104);
extern void SET_ZXTZSJ(void);
extern uint8_t F104_ZXSJSJ_PACK(sPtl_104_i *pPtl104);
extern void PTL104_KKXX_With_LogicNumber_unpack(sPtl_104_i * pPtl104);
#endif


/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
