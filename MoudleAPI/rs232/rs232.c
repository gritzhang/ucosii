/****************************************Copyright (c)****************************************************
**                            Guangzhou ZHIYUAN electronics Co.,LTD.
**
**                                 http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           main.c
** Last modified Date:  2009-05-12
** Last Version:        V1.01
** Descriptions:        The main() function example template
**
**--------------------------------------------------------------------------------------------------------
** Created by:          Chengmingji
** Created date:        2009-07-24
** Version:             V1.00
** Descriptions:        添加用户应用程序
**
**--------------------------------------------------------------------------------------------------------
** Modified by:         Li Baihua
** Modified date:       2009-07-27
** Version:				V1.01
** Descriptions:        编写UART2例程
**
** Rechecked by:        Zhang bin1
*********************************************************************************************************/
#include "rs232.h"

/*********************************************************************************************************
  变量与宏定义
*********************************************************************************************************/
#define UART2_BPS  	2400 												/* 串口通信波特率               */
/*********************************************************************************************************/
#define FEILV			1			//费率
#define READCTW			0x11
#define METERNOLEN	    6			//电表表号长度
#define METDATLEN		12		    //电表数据长度
#define	METERNO			2 		    //电表个数
#define	METERIDNO		3			//抄表数据项个数

/*********************************************************************************************************
  
*********************************************************************************************************/
uint8_t               GucRcvBuf[UARTBUFLEN];                                      /*  串口接收数据缓冲区          */
uint8_t               GucSndBuf[UARTBUFLEN];                                      /*  串口接收数据缓冲区          */
uint32_t              GulNum;                                                     /*  串口接收数据的个数          */
volatile            uint32_t              RcvLen;                                 /*  串口接收数据的个数          */
volatile            uint32_t              SndLen;                                 /*  串口接收数据的个数          */
volatile            uint8_t               GucRcvNew;                              /* 串口接收新数据的标志         */

//extern uint8_t bgntimesave;
/*********************************************************************************************************
  抄表相关数据结构定义 
*********************************************************************************************************/
typedef struct {	
	uint32_t	ID;						//数据标识
	uint8_t	Slen;						//发送数据长度	
	uint8_t	Rlen;						//接收数据长度
	uint8_t	Data[METDATLEN];			//数据内容
}sData;

typedef struct {
	uint8_t 	meterflag;				//电表启用标志
	uint8_t 	meterno[METERNOLEN];	//电表编号
	sData IDdata[METERIDNO];		    //所有抄表数据标识 
}sMeter;

uint8_t ReadFirstFlag;
sMeter Meter[METERNO];				    //电表

uint8_t         MD5[16];                //计费策略的MD5码
uint8_t         Strategy[24*4];         //策略0-23
uint8_t         RecodeNum;             //计费策略的记录数量

uint8_t	MD5_BAK[16];
uint8_t RecodeNum_BAK;
uint8_t Strategy_BAK[24*4];
uint8_t StrategyRev = 0;

//const uint8_t  MeterNoDB[METERNO][METERNOLEN]  = { 0x03,0x50,0x17,0x19,0x11,0x13,
//											  0x03,0x50,0x17,0x19,0x11,0x13,
//											};
//const uint8_t  MeterNoDB[METERNO][METERNOLEN]  = { 0x32,0x00,0x00,0x80,0x13,0x20,
//											  0x32,0x00,0x00,0x80,0x13,0x20,
//											};
const uint8_t  MeterNoDB[METERNO][METERNOLEN]  = { 0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,
											  0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,
											};
											//电压 电流 电量
const uint32_t MeterNoIDDB[METERNO][METERIDNO] = { 0x0201ff00,0x0202ff00,0x00010000,	  	
											  0x0201ff00,0x0202ff00,0x00010000,	
											};
uint32_t RxMeterDtLen(uint32_t id )
{
	uint32_t Rlen=0;

  	switch(id)
	{
		case 0x00010000:	//电量		
			Rlen =4;		
			break;
		case 0x02010100:	//电压		
			Rlen =2;		
			break;
		case 0x02020100:	//电流		
			Rlen =3;		
			break;
		case 0x0201ff00:	//电压块		
			Rlen =2*3;		
			break;
		case 0x0202ff00:	//电流块		
			Rlen =3*3;		
			break;

		default:
		break;	
	
	}

	return Rlen;
}

void MeterParaInit(void )
{
	uint32_t i,j;
	for(i=0;i<METERNO;i++)
	{
		if(i==0)
			Meter[i].meterflag = 1;
		else
			Meter[i].meterflag = 0;	
		memcpy((uint8_t *)&Meter[i].meterno[0],(uint8_t *)&MeterNoDB[i][0],METERNOLEN);
		for(j=0;j<METERIDNO;j++)
		{
			Meter[i].IDdata[j].ID = MeterNoIDDB[i][j];
			Meter[i].IDdata[j].Slen = 4;
			Meter[i].IDdata[j].Rlen = 0;
			memset(Meter[i].IDdata[j].Data,0,METDATLEN);
		}	
	}	
}

/*********************************************************************************************************
** Function name:   	delayNS 
** Descriptions:	    延时函数
** input parameters:    ulDly:   值越大，延时时间越长
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void delayNS (uint32_t ulDly)
{
    uint32_t i;
    
    for (; ulDly > 0; ulDly--) {
        for (i = 0; i < 50000; i++);
    }
}

uint8_t     strategy_kwh[24*2];     //计费策略中的电量

/*********************************************************************************************************
** Function name:	    DLT645_2007CRC
** Descriptions:	    CRC 计算
** input parameters:    pDLT645_2007CF ：指向要计算的协议缓存区
** output parameters:   无
** Returned value:      CRC
*********************************************************************************************************/
void ChargeStrategyMeterKwh(uint32_t meter_cur_kwh)
{
	static uint8_t recode_cnt = 0;
	static uint8_t cs_time[96] = {0};
	uint8_t md5[15] = {0};
	uint16_t cur_time, cs_start, cs_stop, i = 0,j = 0;
	static uint16_t cs_kwh[24];
	static uint8_t csp = 0;
	static uint32_t cskwbg = 0;	//某时间段开始电流
	static uint16_t cskwlast = 0; // 上次保存的电量
	uint8_t cspb;
    uint32_t kwh_buf;
	
	if (!ReadFirstFlag)
	{
		ReadFirstFlag = 1;
			
		recode_cnt = 0;
		memset(md5, 0, sizeof(md5));
		memset(cs_time, 0, sizeof(cs_time));

        OS_ENTER_CRITICAL();
        FRAMRead(md5, MD5_ADD ,16);
        FRAMRead(&recode_cnt, CHARGE_STRATEGY_RECODE_NUM_ADD ,1);
        FRAMRead(cs_time, CHARGE_STRATEGY_ADD , 24*4);
        OS_EXIT_CRITICAL();	

        if(recode_cnt == 0 || (cs_time[0] == 0 && cs_time[1] == 0 && cs_time[2] == 0 && cs_time[3] == 0))
        {
            OSTimeDly(OS_TICKS_PER_SEC/20);	
            OS_ENTER_CRITICAL();
            FRAMRead(md5, MD5_BAK_ADD ,16);
            FRAMRead(&recode_cnt, CHARGE_STRATEGY_RECODE_NUM_BAK_ADD ,1);
            FRAMRead(cs_time, CHARGE_STRATEGY_BAK_ADD , 24*4);  //从备份器读取最新的计费策略
            OS_EXIT_CRITICAL();
					
            if(recode_cnt == 0 || (cs_time[0] == 0 && cs_time[1] == 0 && cs_time[2] == 0 && cs_time[3] == 0))
            {
                if(RecodeNum_BAK && StrategyRev && (Strategy_BAK[0] != 0 || Strategy_BAK[1] != 0 || Strategy_BAK[2] != 0 || Strategy_BAK[3] != 0))
                {
                    memcpy(md5,MD5_BAK,16);
                    recode_cnt = RecodeNum_BAK;
                    memcpy(cs_time,Strategy_BAK,96);
                }
                else
                {
                    recode_cnt = 1;
                    memset(cs_time,0,96);
                    cs_time[0] =  0;
                    cs_time[1] =  0;
                    cs_time[2] =  23;
                    cs_time[3] =  59;
                }
            }						
        }

        RecodeNum = recode_cnt;

        memcpy(MD5, md5, 16);
        memcpy(Strategy, cs_time, 96);

        csp = 0;
	}

	cur_time = ((uint16_t)RTCtimedate.hour << 8) | RTCtimedate.minute;
	
	if(recode_cnt)
	{
		for(i = 0; i < recode_cnt; i++)
		{
			cs_start = ((uint16_t)cs_time[i*4] << 8) | cs_time[i*4+1];
			cs_stop = ((uint16_t)cs_time[i*4+2] << 8) | cs_time[i*4+3];
			//时间段计算
			if(cs_start <= cs_stop)
			{
				if(cur_time >= cs_start && cur_time <= cs_stop)
				{
					cspb = i+1;
					if(csp == cspb)
					{
						if(meter_cur_kwh >= cskwbg)
						{
							cs_kwh[i] = (uint16_t)(meter_cur_kwh - cskwbg) + cskwlast;
						}	
						else
						{
							cs_kwh[i] = cskwlast;
						}
					}	
					else
					{						
						cskwbg = meter_cur_kwh;
						kwh_buf = 0;
						for(j = 0;j < recode_cnt;j ++)
						{							
							if(j != i) kwh_buf += cs_kwh[j];							
						}
						if(meter_cur_kwh >= kwh_buf)							
						{
							cskwlast = (meter_cur_kwh - kwh_buf);
						}
						else
						{
							cskwlast = cs_kwh[i];	
						}						
						csp = cspb;
					}
					strategy_kwh[i*2] = cs_kwh[i] & 0xFF; 
					strategy_kwh[i*2+1] = (cs_kwh[i] >> 8) & 0xFF;
					break;
				}								
			}	
			else if(cs_start > cs_stop)
			{
				if(cur_time >= cs_start || cur_time <= cs_stop)
				{
					cspb = i+1;
					if(csp == cspb)
					{
						if(meter_cur_kwh >= cskwbg)
						{
							cs_kwh[i] = (uint16_t)(meter_cur_kwh - cskwbg) + cskwlast;
						}	
						else
						{
							cs_kwh[i] = cskwlast;
						}
					}	
					else
					{						
						cskwbg = meter_cur_kwh;
						kwh_buf = 0;
						for(j = 0;j < recode_cnt;j ++)
						{							
							if(j != i) kwh_buf += cs_kwh[j];							
						}
						if(meter_cur_kwh >= kwh_buf)							
						{
							cskwlast = (meter_cur_kwh - kwh_buf);
						}
						else
						{
							cskwlast = cs_kwh[i];	
						}	
						csp = cspb;
					}
					strategy_kwh[i*2] = cs_kwh[i] & 0xFF; 
					strategy_kwh[i*2+1] = (cs_kwh[i] >> 8) & 0xFF;
					break;
				}
			}
		}
	}
	else
	{
		strategy_kwh[0] = meter_cur_kwh & 0xFF;
		strategy_kwh[1] = (meter_cur_kwh >> 8) & 0xFF; 
	}
	
}

/*********************************************************************************************************
** Function name:	    uart2Init
** Descriptions:	    UART2初始化，设置为8位数据位，1位停止位，偶校验，波特率为UART2_BPS
** input parameters:    ulDly:   值越大，延时时间越长
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void uart2Init (void)
{
    uint32_t Fdiv;
	LPC_SC->PCONP |= 0x01000000;
	LPC_IOCON->P0_10 &= ~0x07;
	LPC_IOCON->P0_10 |= 1;                                      /* P0.10为U2_TXD                */
	LPC_IOCON->P0_11 &= ~0x07;
	LPC_IOCON->P0_11 |= 1;                                      /* P0.11为U2_RXD                */

	LPC_UART2->LCR = 0x9b;        
	Fdiv = ( PeripheralClock / 16 ) / (UART2_BPS*2) ;           /*求除数值                      */
	LPC_UART2->DLM = Fdiv / 256;
	LPC_UART2->DLL = Fdiv % 256;
	LPC_UART2->LCR = 0x1b;                                     	/* 锁定除数访问                 */
	LPC_UART2->FCR = 0x07 | 0x80;                              	/* 使能并复位FIFO               */

	NVIC_EnableIRQ(UART2_IRQn);

	LPC_UART2->IER = IER_RBR | IER_RLS;

	ENRD485();																									
}
/*********************************************************************************************************
** Function name:	    Uart2RxbufClear
** Descriptions:	    清除接收缓存
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void Uart2RxbufClear(void )
{
    GucRcvNew = 0;                                              		/* 清除标志*/
    RcvLen = 0;
    memset(GucRcvBuf,0,sizeof(GucRcvBuf));
}
/*********************************************************************************************************
** Function name:	    uart2SendByte
** Descriptions:	    向串口发送子节数据，并等待数据发送完成，使用查询方式
** input parameters:    ucDat:   要发送的数据
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void uart2SendByte (uint8_t ucDat)
{
    LPC_UART2->THR = ucDat;                                           /* 写入数据                     */
    while ((LPC_UART2->LSR & 0x20) == 0);                             /* 等待数据发送完毕             */
}
/*********************************************************************************************************
** Function name:	    uart2SendStr
** Descriptions:    	向串口发送字符串
** input parameters:    pucStr:  要发送的字符串指针
**                      ulNum:   要发送的数据个数
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void uart2SendStr (uint8_t const *pucStr, uint32_t ulNum)
{
    uint32_t i;
	
    ENWR485();
	OSTimeDly(3);
	Uart2RxbufClear();	
    for (i = 0; i < ulNum; i++) {                                       /* 发送指定个字节数据           */
        uart2SendByte (*pucStr++);
    }
	OSTimeDly(3);
	ENRD485();
}

/*********************************************************************************************************
* Function Name  : UART2_IRQHandler
* Description    : UART2 中断处理函数
* Input          : 无
* Output         : 无
* Return         : 无
*********************************************************************************************************/
void UART2_IRQHandler(void)
{ 
    uint8_t uiIIRValue, uiLSRValue;
    uint8_t uiDummy = uiDummy;

  	uiIIRValue = LPC_UART2->IIR;

    uiIIRValue >>= 1;                                                     /* 中断标志寄存器值右移1位*/
    uiIIRValue &= 0x07;

    if ( uiIIRValue == IIR_RLS ) {                                        /* 接收线状态中 */
        uiLSRValue = LPC_UART2->LSR;
        /* 
         * 溢出中断、奇偶中断、帧中断、UART RBR中包含错误数据和帧间隔中断处理
         */
        if ( uiLSRValue & (LSR_OE|LSR_PE|LSR_FE|LSR_RXFE|LSR_BI) ) {
            uiDummy = LPC_UART2->RBR;
            return;
        }
        if ( uiLSRValue & LSR_RDR ) {                                     /* 接收FIFO不为空中断 */
            for (GulNum = 0; GulNum < 8; GulNum++){                      /*  连续接收8个字节*/
                if ( GulNum+RcvLen >= BUFSIZE ) {
                    Uart2RxbufClear();
                }
                GucRcvBuf[GulNum+RcvLen] = LPC_UART2->RBR;
            }
            RcvLen+=GulNum;
            GucRcvNew = 1; 					
        }
    }
    else if ( uiIIRValue == IIR_RDA ) {                                   /* 接收数据可用中断*/
        for (GulNum = 0; GulNum < 8; GulNum++){                      /*  连续接收8个字节 */
            if ( GulNum+RcvLen >= BUFSIZE ) {
                Uart2RxbufClear();
            }
            GucRcvBuf[GulNum+RcvLen] = LPC_UART2->RBR;
        }
        RcvLen+=GulNum;
        GucRcvNew = 1; 						
    }
    else if ( uiIIRValue == IIR_CTI ) {                 /* 字符超时中断               */
        while ((LPC_UART2->LSR & 0x01) == 0x01){  
            if(RcvLen >= BUFSIZE){
                Uart2RxbufClear();
            }                       					/*  判断数据是否接收完毕      */ 
            GucRcvBuf[RcvLen] = LPC_UART2->RBR;
            RcvLen++;
        }
        GucRcvNew = 1; 									/* 	接收到完整数据帧 		*/
    }
    else if ( uiIIRValue == IIR_THRE ) {                /* 发送保持寄存器空中断       */
        uiLSRValue = LPC_UART2->LSR;
        if ( uiLSRValue & LSR_THRE ) {
            //UART2TxEmpty = 1;
        } else {
            //UART2TxEmpty = 0;
        }
    }
}

uint8_t CheckSum0904(uint8_t *pdat,uint16_t dlen)
{	
	uint8_t  cs=0;
	uint16_t i,j,len;
		
	for(i=0;i<dlen-MINFLEN0904;i++)
	{
	 	if(	*(pdat+i)!=0x68 || *(pdat+i+7)!=0x68)
			continue;
		len = *(pdat+i+9) + *(pdat+i+10) * 256;
		if(len>MAXFLEN0904-13-i)
			break;
		for(j=0;j<len+11;j++)
		  	cs += *(pdat+i+j);		
		if(cs != *(pdat+i+j))
			break;
		if(0x16 != *(pdat+i+j+1))
			break;
		return 1;
	}
	return 0;
}

void ClearDataBuffer(void)
{
    sPTL_104_I_100.CDSCDY = 0;
    sPTL_104_I_100.CDSCDL = 0;
    sPTL_104_I_100.YGZDD  = 0;

    ReadFirstFlag = 0;
    if(sPTL_104_I_UP_ZXSJ.PQSZ != 0)
    {
        memset(&sPTL_104_I_UP_ZXSJ,0,sizeof(sPTL_104_I_UP_ZXSJ));
    }
    
//  bgntimesave = 0;
}

uint32_t ndbcdstr2u32(uint8_t const *pointer,uint16_t len)//倒序高位在后
{
    uint8_t i,j,temp;
    uint32_t ret=0;
    
    for(i= 0;i<len;i++)
    {
        temp = (*(pointer+(len-1-i))>>4)*10+(*(pointer+(len-1-i))&0x0f);
        ret = ret*100+temp;
    }
    return ret;
}

/*********************************************************************************************************
** Function name:       main_rs232
** Descriptions:        UART2抄表
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void main_rs232 (void)
{
    uint32_t 	i = 0,j = 0,k,tmp;
    uint32_t 	dval,pbgn,pend,mny;
    uint32_t 	timdiv,head;
    uint32_t 	rlen,cur;
    uint8_t 	*pdat;
    uint8_t 	tmpbuf[6],dtmpbuf[3];

    OSTimeDly(OS_TICKS_PER_SEC); 																			//延时以启动其他任务

    uart2Init();                                   												            //串口初始化   	
    MeterParaInit();																						//初始化抄表参数
    Uart2RxbufClear();	

    while(1){
        OSTimeDly(OS_TICKS_PER_SEC/2);
        if((STATION_Charge != Station_State) && (STATION_ChargeEND != Station_State))
        {
            k=0;
            ClearDataBuffer();
            continue;
        }
        if((Charge_Flag&CHANGE_ING_BIT)&&(Station_State == STATION_Charge))
        {                                               
            for(j = 0;j<3;j++)                      //电表的电量电流电压都是BCD码
            {
                SndLen = FramePack_645_2007(Meter[i].meterno,READCTW,Meter[i].IDdata[j].ID,(uint8_t *)&Meter[i].IDdata[j].Data,4,GucSndBuf);
                uart2SendStr(GucSndBuf,SndLen);
                for(timdiv=0;timdiv<4;timdiv++){							   
                    OSTimeDly(OS_TICKS_PER_SEC/2);
                    if(RcvLen < MINFLEN0904)
                        continue;
                    head = FrameCheck_645_2007(GucRcvBuf,Meter[i].meterno,Meter[i].IDdata[j].ID);            

                    if(head < MAXFRMLEN-VALUEOFFSET){
                        k=0;						
                        rlen = GucRcvBuf[head+9]-4;																//数据长度
                        pdat = GucRcvBuf+VALUEOFFSET+head;							   					        //数据指针
                        memcpy((uint8_t *)&Meter[i].IDdata[j].Data[0],pdat,rlen);
                        if(Meter[i].IDdata[j].ID==0x00010000){ 	                  //电量										        //正向有功总电能			
        					dval = ndbcdstr2u32(GucRcvBuf+VALUEOFFSET+head,4);				                    //转换当前电量						
                            if(dval != 0)
                            {	
                                pbgn = sPTL_104_I_UP_ZXSJ.PQSZ;
                                if(pbgn==0)
                                {
                                    sPTL_104_I_UP_ZXSJ.PQSZ = dval;													//????????,????
                                    sPTL_104_I_UP_LXSJ.PQSZ = dval;													//????????,????
                                    sPTL_104_I_UP_ZXSJ.PZSZ = dval;							 						//平止示值，两位小数
                                    sPTL_104_I_UP_LXSJ.PZSZ = dval;
                                }
                                else
                                {
                                    if(dval < pbgn)
                                    {
                                        sPTL_104_I_UP_ZXSJ.PQSZ = dval;													//在线数据平起示值，两位小数
                                        sPTL_104_I_UP_LXSJ.PQSZ = dval;													//离线数据平起示值，两位小数
                                        sPTL_104_I_UP_ZXSJ.PZSZ = dval;							 						//平止示值，两位小数
                                        sPTL_104_I_UP_LXSJ.PZSZ = dval;							 						//平止示值，两位小数
                                    }
                                    else
                                    {
                                        sPTL_104_I_UP_ZXSJ.PZSZ = dval;							 						//平止示值，两位小数
                                        sPTL_104_I_UP_LXSJ.PZSZ = dval;							 						//平止示值，两位小数
                                        pend = dval - sPTL_104_I_UP_ZXSJ.PQSZ;					                //通道电量																	
                                        sPTL_104_I_UP_ZXSJ.ZDL = pend;													//总电量，两位小数
                                        sPTL_104_I_UP_LXSJ.ZDL = pend;							   					    //总电量，两位小数
                                        sPTL_104_I_UP_ZXSJ.PDL = pend;													//平电量，两位小数
                                        sPTL_104_I_UP_LXSJ.PDL = pend;														//总电量，一位小数																									
                                        sPTL_104_I_100.YGZDD = pend;
                                                                
                                        mny = pend*FEILV;		        //计费策略用电量
                                        ChargeStrategyMeterKwh(mny);
                                    }
                                }
                            }
                        }
                        else if(Meter[i].IDdata[j].ID==0x0201ff00){		//电压	 	
                            tmp =  Meter[i].IDdata[j].Data[0];
                            tmp += (Meter[i].IDdata[j].Data[1]<<8);
                            sPTL_104_I_100.CDSCDY = tmp;            //保存实时充电电压
                            if(rlen>2)
                            {
                                if (Meter[i].IDdata[j].Data[2] != 0xFF && Meter[i].IDdata[j].Data[3] != 0xFF && Meter[i].IDdata[j].Data[4] != 0xFF &&
                                                Meter[i].IDdata[j].Data[5] != 0xFF)
                                {
                                }
                                else
                                {
                                }	
                            }
                        }
                        else if(Meter[i].IDdata[j].ID==0x0202ff00){				//电流							
                            Meter[i].IDdata[j].Data[2] &= 0x7f;					//屏蔽符号位
                            Meter[i].IDdata[j].Data[5] &= 0x7f;
                            Meter[i].IDdata[j].Data[8] &= 0x7f;
                            cur =  Meter[i].IDdata[j].Data[0];
                            cur += (Meter[i].IDdata[j].Data[1]<<8);
                            cur += (Meter[i].IDdata[j].Data[2]<<16);  
                            sPTL_104_I_100.CDSCDL = cur >> 8;
                            if(rlen>3)
                            {
                                if (Meter[i].IDdata[j].Data[3] != 0xFF && Meter[i].IDdata[j].Data[4] != 0xFF && Meter[i].IDdata[j].Data[5] != 0xFF &&
                                        Meter[i].IDdata[j].Data[6] != 0xFF && Meter[i].IDdata[j].Data[7] != 0xFF && Meter[i].IDdata[j].Data[8] != 0xFF)
                                {

                                }
                                else
                                {

                                }
                                cur =  dtmpbuf[2];
                                cur += (dtmpbuf[1]<<8);
                                cur += (dtmpbuf[0]<<16);                                
                            }						
                        }
                        break;
                    } 
                }
            }                
        }
    }
}


/*********************************************************************************************************
  End Of File
*********************************************************************************************************/
